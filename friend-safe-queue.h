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
  SafeQueue(void) : leftq(), rightq(), leftm(), rightm(), leftc(), rightc() {}
  ~SafeQueue(void) = default;

  // Add an element to the queue.
  void enqueue_left_channel(const T sample) {
    std::lock_guard<std::mutex> lock(leftm);
    leftq.push(sample);
    leftc.notify_one();
  }

  void enqueue_right_channel(const T sample) {
    std::lock_guard<std::mutex> lock(rightm);
    rightq.push(sample);
    rightc.notify_one();
  }

  const T dequeue_left_channel() {
    std::unique_lock<std::mutex> lock(leftm);
    while (leftq.empty()) {
      // release lock as long as the wait and reaquire it afterwards.
      leftc.wait(lock);
    }
    const T val = leftq.front();
    leftq.pop();
    return val;
  }

  const T dequeue_right_channel() {
    std::unique_lock<std::mutex> lock(rightm);
    while (rightq.empty()) {
      // release lock as long as the wait and reaquire it afterwards.
      rightc.wait(lock);
    }
    const T val = rightq.front();
    rightq.pop();
    return val;
  }

 private:
  std::queue<T> leftq, rightq;
  mutable std::mutex leftm, rightm;
  std::condition_variable leftc, rightc;
};

}  // namespace Friend

#endif  // __FRIEND_SAFE_QUEUE_H
