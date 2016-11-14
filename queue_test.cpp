#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include "queue.h"

using namespace std;

void random_wait() {
  this_thread::sleep_for(chrono::milliseconds(rand() % 10));
}

void producer(Queue<string>& q) {
  for(int i = 0; i < 1000; i++) {
    stringstream ss;
    ss << "producer is at " << i;
    string s = ss.str();
    q.enqueue(s);
    random_wait();
  }
  string end = "end";
  q.enqueue(end);
}

void consumer(Queue<string>& q) {
  while(true) {
    string message = q.dequeue();
    if(message == "end") {
      break;
    }
    cout << "received: " << message << endl;
    random_wait();
  }
}

void threads() {
    Queue<string> q;

    std::thread t1(consumer, ref(q));
    std::thread t2(producer, ref(q));

    t2.join();
    t1.join();
    cout << "threads ended" << endl;
}

int main() {
    threads();
}
