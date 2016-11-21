#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

#include "option.h"

template <class T>
class Queue
{
public:
  Queue(void)
    : q()
    , m()
    , c()
  {}

  ~Queue(void)
  {}

  void enqueue(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  T dequeue(void)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty())
    {
      c.wait(lock);
    }
    T val = q.front();
    q.pop();
    return val;
  }

  Option<T> * try_dequeue(int64_t timeout_ms)
  {
    std::unique_lock<std::mutex> lock(m);

    if(!q.empty()) {
      T value = q.front();
      q.pop();
      return new Some<T>(value);
    }

    std::cv_status status = c.wait_for(lock, std::chrono::milliseconds(timeout_ms));
    if (status == std::cv_status::no_timeout) {
      if(!q.empty()) {
        T value = q.front();
        q.pop();
        return new Some<T>(value);
      } else {
        return new None<T>();
      }
    } else {
      return new None<T>();
    }
  }

  bool is_empty()
  {
    std::unique_lock<std::mutex> lock(m);
    return q.empty();
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};
#endif
