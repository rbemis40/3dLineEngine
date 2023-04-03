#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <vector>
#include <mutex>
#include "object3d.h"
#include "plane.h"

struct RenderJob {
    std::vector<Plane>* planes;
    int start;
    int end;
    Uint32 frameTime;
};

class RenThreadPool {
public:
    RenThreadPool(int numThreads);
    ~RenThreadPool();
    
    void addJob(RenderJob job);
    void waitForEmptyQueue();

    void quit();
private:
    void checkQueue(int tmp);

    std::vector<std::thread> _threads;
    std::vector<RenderJob> _jobQueue;

    bool _hasQuit;

    std::mutex _queueLock;
    
};

#endif