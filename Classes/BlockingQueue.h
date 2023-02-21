//
//  BlockingQueue.h
//  miners
//
//  Created by 이동현 on 2023/02/01.
//

#ifndef BlockingQueue_h
#define BlockingQueue_h

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class BlockingQueue {
  std::condition_variable_any _cvCanPop;
  std::mutex _sync;
  std::queue<T> _qu;

public:
  void Push(const T& item)
  {
      std::unique_lock<std::mutex> lock(_sync);
      _qu.push(item);
      _cvCanPop.notify_one();
  }

  T Pop() {
    std::unique_lock<std::mutex> lock(_sync);
    while(_qu.empty()) {
      _cvCanPop.wait(lock);
    }
      
    auto item = _qu.front();
    _qu.pop();
    return item;
  }
};

#endif /* BlockingQueue_h */
