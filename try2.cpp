#include<bits/stdc++.h>
#include<iostream>
#include<thread>
#include<chrono>
#include<algorithm>
#include<future>
#include<windows.h>
using namespace std;
using namespace std::chrono;

int main() {
    int n = 3;
    auto fun1 = []() {
        Sleep(1000);
    };
    async(std::launch::deferred, fun1);
    Sleep(10000);
    cout<<n<<"\n";
}