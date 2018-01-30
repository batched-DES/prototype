#pragma once

#include <cstddef>


namespace BatchSimulator {

    /**
     * Event base class. When you inherit from event, you have to implement a static void run(Event* e), which does
     * your event handling.
     */
    class Event {

    protected:
        double timestamp;

    public:
        Event(double _timestamp) {
            timestamp = _timestamp;
        }

        virtual ~Event() = default;

        const bool operator<(const Event &rhs) const {
            return timestamp < rhs.timestamp;
        }

        const double getTimestamp() const {
            return timestamp;
        }

        virtual const double getLookahead() const = 0;

        virtual const size_t getEventID() const = 0;
    };
}