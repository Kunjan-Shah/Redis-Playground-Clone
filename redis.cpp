#include <bits/stdc++.h>

using namespace std;

class Value {
private:
    string value;
    long long ttl;
public:
    Value(string value, int ttl) {
        this->value = value;
        this->ttl = ttl;
    }
    Value(string value) {
        this->value = value;
        ttl = -1;
    }
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

void printKVStore(RedisCache &cache) {
    unordered_map<string, Value> kvStore = cache.getKVStore();
    cout<<"----------------- REDIS CACHE -----------------\n";
    cout<<"Key                           Value\n";
    for(auto it : kvStore) {
        cout<<it.first;
        int rem = 30 - it.first.size();
        for(int space = 0; space < rem; space++) cout<<" ";
        cout<<it.second<<"\n";
    }
    cout<<"\n";
}

void setEntry(RedisCache &cache, string key, string value) {
    unordered_map<string, Value> kvStore = cache.getKVStore();
    kvStore[key] = value;
    cache.setKVStore(kvStore);
}

string getEntry(RedisCache &cache, string key) {
    unordered_map<string, Value> kvStore = cache.getKVStore();
    if(kvStore.find(key) != kvStore.end()) return kvStore[key];
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
        else {
            errorFlag = true;
        }
        
        if(errorFlag) {
            cout<<"Invalid command\n";
        }
    }
    return 0;
}