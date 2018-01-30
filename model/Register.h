#pragma once

namespace ProofOfConcept {
    size_t sum = 0;
}

#include "Event.h"
#include "IncrementEvent.hpp"
#include "SetEvent.hpp"
#include "BatchScheduler.h"

#include <random>

constexpr  size_t NUM_ITERATIONS = 1'000'000;

namespace ProofOfConcept {

    void bootstrap() {

        std::mt19937_64 generator(0);
        std::uniform_int_distribution<size_t> distribution(0, 1);
        BatchSimulator::Event* e;

        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            bool inc = (distribution(generator) == 0);
            if (inc) {
                e = new IncrementEvent(i);
            } else {
                e = new SetEvent(i);
            }
            BatchSimulator::BatchScheduler::getInstance().schedule(e);
        }
    }

    void shutdown() {}
}

namespace BatchSimulator {
    namespace Register {

        constexpr size_t MAX_EVENT_COUNT = NUM_ITERATIONS + 1;
        constexpr void (*eventHandlers[]) (Event*) = {ProofOfConcept::IncrementEvent::run, ProofOfConcept::SetEvent::run};
        constexpr void (*bootstrap) () = ProofOfConcept::bootstrap;
        constexpr void (*shutdown) () = ProofOfConcept::shutdown;

    }
}
