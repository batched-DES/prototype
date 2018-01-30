#pragma once

#include "Event.h"

namespace ProofOfConcept {

    class SetEvent : public BatchSimulator::Event {

    public:
        SetEvent(double time) : BatchSimulator::Event(time) {}

        const size_t getEventID() const override {
            return 2;
        }

        const double getLookahead() const override {
            return 1'000'000;
        }

        static inline void run(BatchSimulator::Event* ev) __attribute__((always_inline)){
            // auto typedEvent = static_cast<SetEvent*>(ev);
            sum = 10;
            //delete ev;
        }

    };
}