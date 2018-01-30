#pragma once

#include "Event.h"

namespace ProofOfConcept {

    class IncrementEvent : public BatchSimulator::Event {

    public:
        IncrementEvent(double time) : BatchSimulator::Event(time) {}


        const size_t getEventID() const override {
            return 1;
        }

        const double getLookahead() const override {
            return 1'000'000;
        }

        inline void runObject() {

        }

        static inline void run(BatchSimulator::Event* ev) __attribute__((always_inline)){
            // auto typedEvent = static_cast<SetEvent*>(ev);
            for (size_t i = 0; i < 1'000'000; i++) {
                sum += sum + 1;
            }
            // delete ev;
        }

    };
}
