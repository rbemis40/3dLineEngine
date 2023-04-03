#include "threadpool.h"


RenThreadPool::RenThreadPool(int numThreads) {
    for (int i = 0; i < numThreads; i++) {
        _threads.push_back(std::thread(&RenThreadPool::checkQueue, this, i));
    }
}

RenThreadPool::~RenThreadPool() {
    quit();
}

void RenThreadPool::addJob(RenderJob job) {
    _queueLock.lock();
    _jobQueue.insert(_jobQueue.begin(), job);
    _queueLock.unlock();
}

void RenThreadPool::quit() {
    _queueLock.lock();
    _hasQuit = true;
    _queueLock.unlock();

    for (auto& curThread : _threads) {
        if (curThread.joinable()) {
            curThread.join();
        }
    }
}

void RenThreadPool::waitForEmptyQueue() {
    while (true) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        _queueLock.lock();
        if (_jobQueue.size() == 0) {
            _queueLock.unlock();
            return;
        }
        _queueLock.unlock();
    }
}

void RenThreadPool::checkQueue(int tmp) {
    while (true) {
        _queueLock.lock();

        if (_hasQuit) {
            _queueLock.unlock();
            return;
        }
        else if (_jobQueue.size() >= 1) {
            RenderJob newJob = _jobQueue.back();
            _jobQueue.pop_back();

            _queueLock.unlock();

            for (int i = newJob.start; i < newJob.end; i++) {
                newJob.planes->at(i).update(newJob.frameTime);
            }
        }
        else {
            _queueLock.unlock();
        }
    }
}