#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

class Timer {
public:
    Timer(size_t time, const std::function<void(int,int)>& f, int n, int m) : time{std::chrono::milliseconds{time}}, f{f}, n{n}, m{m} {}
    ~Timer() { wait_thread.join(); }

private:
    void wait_then_call(int n, int m)
    {
        std::unique_lock<std::mutex> lck{mtx};
        for(int i{10}; i > 0; --i) {
            // std::cout << "Thread " << wait_thread.get_id() << " countdown at: " << '\t' << i << '\n';
            cv.wait_for(lck, time / 10);
        }
        f(n, m);
    }
    std::mutex mtx;
    std::condition_variable cv{};
    std::chrono::milliseconds time;
    std::function <void(int,int)> f;
    std::thread wait_thread{[this]() {wait_then_call(n, m); }};
    int n;
    int m;
};
int ttl = 3;
void func(int n, int m) {
    std::cout << "---------------- I waited to print! ----------------"<<ttl<<"\n"; 
    ttl--;
    if(ttl > 0) Timer t{1000, func, 10, 20};
}
int main()
{
    // auto f = []() {
        
    // };
    // Timer t1{3000,f};
    // Timer t2{10'000,f};
    Timer t3{1000,func, 10, 20};
    cout<<"Hello\n";
    cout<<"Hi\n";
    cout<<"How are you\n";
    Sleep(15000);
    cout<<"Coming to the end\n";
    // Timer t4{1000,f};
}