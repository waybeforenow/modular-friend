#ifndef __FRIEND_SAFE_QUEUE_H
#define __FRIEND_SAFE_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

namespace Friend {

// A threadsafe-queue.
template <class T>
class SafeQueue {
 public:
  SafeQueue(void) : q(), m(), c() {}

  ~SafeQueue(void) {}

  // Add an element to the queue.
  void enqueue(T t) {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  // Get the "front"-element.
  // If the queue is empty, wait till a element is avaiable.
  T dequeue(void) {
    std::unique_lock<std::mutex> lock(m);
    while (q.empty()) {
      // release lock as long as the wait and reaquire it afterwards.
      c.wait(lock);
    }
    T val = q.front();
    q.pop();
    return val;
  }

 private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};

}  // namespace Friend

#endif  // __FRIEND_SAFE_QUEUE_H