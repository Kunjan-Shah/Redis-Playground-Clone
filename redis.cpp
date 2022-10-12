#include <bits/stdc++.h>

using namespace std;

class Value {
private:
    string value;
    long long ttl;
public:
    string getValue() { return this->value; }
    void setValue(string value) { this->value = value; }

    long long getTTL() { return this->ttl; }
    void setTTL(long long ttl) { this->ttl = ttl; }
};

class RedisCache {
private:
    unordered_map<string, Value> kvStore;
public:
    RedisCache() {
        kvStore.clear();
    }
    
    unordered_map<string, Value> getKVStore() { return kvStore;}
    void setKVStore(unordered_map<string, Value> kvStore) { this->kvStore = kvStore;}
};

class Timer {
public:
    Timer(size_t time, const std::function<void(RedisCache &, string)>& f, RedisCache &cache, string key) : time{std::chrono::milliseconds{time}}, f{f}, cache{cache}, key(key) {}
    ~Timer() { wait_thread.join(); }

private:
    void wait_then_call(RedisCache &cache, string key)
    {
        std::unique_lock<std::mutex> lck{mtx};
        for(int i{10}; i > 0; --i) {
            cv.wait_for(lck, time / 10);
        }
        f(cache, key);
    }
    std::mutex mtx;
    std::condition_variable cv{};
    std::chrono::milliseconds time;
    std::function <void(RedisCache &, string)> f;
    std::thread wait_thread{[this]() {wait_then_call(cache, key); }};
    RedisCache cache;
    string key;
};

void func(RedisCache &cache, string key) {
    unordered_map<string, Value> kvStore = cache.getKVStore();
    int ttl = kvStore[key].getTTL();
    string value = kvStore[key].getValue();
    ttl--;
    if(ttl > 0) {
        kvStore[key].setTTL(ttl);
        cache.setKVStore(kvStore);
        Timer t{1000, func, cache, key};
    }
}

void printKVStore(RedisCache &cache) {
    unordered_map<string, Value> kvStore = cache.getKVStore();
    cout<<"----------------- REDIS CACHE -----------------\n";
    cout<<"Key                           Value\n";
    for(auto it : kvStore) {
        cout<<it.first;
        int rem = 30 - it.first.size();
        for(int space = 0; space < rem; space++) cout<<" ";
        cout<<it.second.getValue()<<"\n";
    }
    cout<<"\n";
}

void setEntry(RedisCache &cache, string key, string value) {
    unordered_map<string, Value> kvStore = cache.getKVStore();
    Value v;
    v.setValue(value);
    v.setTTL(-1);
    kvStore[key] = v;
    cache.setKVStore(kvStore);
}

string getEntry(RedisCache &cache, string key) {
    unordered_map<string, Value> kvStore = cache.getKVStore();
    if(kvStore.find(key) != kvStore.end()) return kvStore[key].getValue();
    return "(nil)";
}

string deleteEntry(RedisCache &cache, string key) {
    unordered_map<string, Value> kvStore = cache.getKVStore();
    if(kvStore.find(key) != kvStore.end()) {
        kvStore.erase(key);
        cache.setKVStore(kvStore);
        return "1";
    }
    return "0";
}

int main() {
    RedisCache cache;
    bool errorFlag = false;
    while(1) {
        errorFlag = false;
        cout<<"> ";

        // get user input
        string command;
        getline(cin, command, '\n');
        vector<string> commandCompents;
        string component;
        stringstream strstream(command);
        while(getline(strstream, component, ' ')) {
            commandCompents.push_back(component);
        }
        string commandName = commandCompents[0];
        transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper);
        // commands
        if(commandName == "SET") {
            if(commandCompents.size() == 3) {
                string key = commandCompents[1];
                string value = commandCompents[2];
                setEntry(cache, key, value);
                cout<<"OK\n";
            }
            else {
                errorFlag = true;
            }
        }
        else if(commandName == "GET") {
            if(commandCompents.size() == 2) {
                string key = commandCompents[1];
                string value = getEntry(cache, key);
                cout<<value<<"\n";
            }
            else {
                errorFlag = true;
            }
        }
        else if(commandName == "EXISTS") {
            if(commandCompents.size() == 2) {
                string key = commandCompents[1];
                bool exists = getEntry(cache, key) ==  "(nil)" ? false : true;
                cout<<exists<<"\n";
            }
            else {
                errorFlag = true;
            }
        }
        else if(commandName == "GETCACHESIZE") {
            if(commandCompents.size() == 1) {
                cout<<cache.getKVStore().size()<<"\n";
            }
            else {
                errorFlag = true;
            }
        }
        else if(commandName == "PRINT") {
            if(commandCompents.size() == 1) {
                printKVStore(cache);
            }
            else {
                errorFlag = true;
            }
        }
        else if(commandName == "DEL") {
            if(commandCompents.size() == 2) {
                string key = commandCompents[1];
                cout<<deleteEntry(cache, key)<<"\n";
            }
            else {
                errorFlag = true;
            }
        }
        else if(commandName == "EXPIRE") {
            if(commandCompents.size() == 3) {
                string key = commandCompents[1];
                int expiry_time = stoi(commandCompents[2]);

                unordered_map<string, Value> kvStore = cache.getKVStore();
                kvStore[key].setTTL(expiry_time);
                cache.setKVStore(kvStore);
                // Timer t{10000,func, cache, key};
                func(cache, key);
            }
            else {
                errorFlag = true;
            }
        }
        else {
            errorFlag = true;
        }
        
        if(errorFlag) {
            cout<<"Invalid command\n";
        }
    }
    return 0;
}