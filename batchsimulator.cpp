#include "batchscheduler/BatchScheduler.h"

#include <iostream>

int main() {
    std::cout << "Generating batches (call invoked to trigger compile time algorithm)" << std::endl;
    BatchSimulator::BatchScheduler::getInstance().generateBatches();
    std::cout << "Running model bootstrap function" << std::endl;
    BatchSimulator::BatchScheduler::getInstance().warmUp();
    std::cout << "Starting simulation" << std::endl;
    BatchSimulator::BatchScheduler::getInstance().run();
    std::cout << "Shutting down" << std:: endl;
    BatchSimulator::BatchScheduler::getInstance().shutdown();
    std::cout << "Done." << std::endl;
    return 0;
}
