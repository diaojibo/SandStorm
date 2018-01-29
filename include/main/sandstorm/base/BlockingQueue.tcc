#pragma once

namespace sandstorm {
namespace base {

template <class T>
BlockingQueue<T>::BlockingQueue() {
}

template <class T>
BlockingQueue<T>::~BlockingQueue() {
}

template <class T>
void BlockingQueue<T>::Push(const T& element) {
    std::unique_lock<std::mutex> locker(_mutex);

    _queue.push(element);
    _emptyCv.notify_one();
}

template <class T>
bool BlockingQueue<T>::Pop(T& element, int32_t milliseconds) {
    std::unique_lock<std::mutex> locker(_mutex);

    if ( !_queue.size() ) {
        if ( milliseconds == 0 ) {
            _emptyCv.wait(locker);
        }
        else {
            _emptyCv.wait_for(locker, std::chrono::milliseconds(milliseconds));
            if ( !_queue.size() ) {
                return false;
            }
        }
    }

    element = _queue.front();
    _queue.pop();

    return true;
}

template <class T>
bool BlockingQueue<T>::Peek(T& element) const {
    std::unique_lock<std::mutex> locker(_mutex);

    if ( !_queue.size() ) {
        return false;
    }

    element = _queue.front();
    return true;
}

template <class T>
bool BlockingQueue<T>::Empty() const {
    std::unique_lock<std::mutex> locker(_mutex);

    return _queue.size() == 0;
}

}
}
