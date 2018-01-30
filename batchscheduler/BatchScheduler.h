#pragma once

#include "Event.h"
#include "EventHeap.hpp"

namespace BatchSimulator {

    class Event;

    class BatchScheduler {

    private:
        BatchScheduler() = default;

        EventHeap fel;

        double currentTime;

    public:

        ~BatchScheduler() = default;
        BatchScheduler(const BatchScheduler &b) = delete;
        BatchScheduler(BatchScheduler &&b) = delete;

        static BatchScheduler& getInstance();

        void generateBatches();

        void warmUp();

        void shutdown();

        void schedule(Event* event);

        void run();

        static double getCurrentTime() {
            return BatchScheduler::getInstance().currentTime;
        }
    };

}