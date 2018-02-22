#include "BatchScheduler.h"
#include "Register.h"

namespace BatchSimulator {

    constexpr size_t ipow_(size_t base, size_t exp) {
        return exp > 1 ? ipow_(base, (exp >> 1) + (exp & 1)) * ipow_(base, exp >> 1) : base;
    }

    constexpr size_t ipow(size_t base, size_t exp) {
        return exp < 1 ? 1 : ipow_(base, exp);
    }

    constexpr size_t MAX_BATCH_SIZE = 3;

    constexpr size_t EVENT_HANDLER_COUNT = sizeof(Register::eventHandlers) / sizeof(Register::eventHandlers[0]);
    constexpr size_t EVENT_TYPE_COUNT = EVENT_HANDLER_COUNT + 1;

    constexpr size_t BATCH_COUNT = (size_t) (
            ((1 - (long) ipow(EVENT_TYPE_COUNT, MAX_BATCH_SIZE + 1)) / (1 - (long) EVENT_TYPE_COUNT)) - 1);


    void (*batchHandlers[BATCH_COUNT])(Event *events[MAX_BATCH_SIZE]);

    template<bool CONSTRAINT, size_t I, size_t OFFSET>
    class TemplateIf {
    };

    template<size_t I, size_t OFFSET>
    class TemplateIf<true, I, OFFSET> {
    public:
        static inline void iff(Event *events[MAX_BATCH_SIZE]) {
            Register::eventHandlers[I - 1](events[0 + OFFSET]);
        }
    };

    template<size_t I, size_t OFFSET>
    class TemplateIf<false, I, OFFSET> {
    public:
        static inline void iff(Event *events[MAX_BATCH_SIZE]) {
            // event is \nu event
            // prevent segementation fault
        }
    };

    template<size_t I, size_t OFFSET>
    class BatchGenerator {
    public:
        static inline void generate(Event *events[MAX_BATCH_SIZE]) {

            // calculate batch by applying the horner scheme
            constexpr size_t index = I % EVENT_TYPE_COUNT;
            TemplateIf<0 < index, index, OFFSET>::iff(events);
            BatchGenerator<I / EVENT_TYPE_COUNT, OFFSET + 1>::generate(events);
        }
    };

    template<size_t OFFSET>
    class BatchGenerator<0, OFFSET> {
    public:
        static inline void generate(Event *events[MAX_BATCH_SIZE]) {
            // horner scheme is done
        }
    };

    template<size_t I>
    class BatchEnumerator {
    public:
        static inline void enumerate() {
            batchHandlers[I] = BatchGenerator<I, 0>::generate;
            BatchEnumerator<I + 1>::enumerate();
        }
    };

    template<>
    class BatchEnumerator<BATCH_COUNT> {
    public:
        static inline void enumerate() { /* array is filled completely */ }
    };

    void BatchScheduler::generateBatches() {
        BatchEnumerator<0>::enumerate();
    }

    BatchScheduler &BatchScheduler::getInstance() {
        static BatchScheduler instance;
        return instance;
    }

    void BatchScheduler::warmUp() {
        Register::bootstrap();
    }

    void BatchScheduler::shutdown() {
        Register::shutdown();
    }

    void BatchScheduler::schedule(Event *event) {
        fel.push(event);
    }

    void BatchScheduler::run() {

        size_t eventCount = 0;
        currentTime = 0;
        Event *events[MAX_BATCH_SIZE];

        while (!fel.empty() && eventCount < Register::MAX_EVENT_COUNT) {

            currentTime = fel.top()->getTimestamp();
            size_t batchIdx = fel.top()->getEventID();
            double maxTime = currentTime + fel.top()->getLookahead();

            events[0] = fel.top();

            size_t length = 1;
            fel.pop();

            while (length < MAX_BATCH_SIZE) {
                if (fel.top()->getTimestamp() > maxTime || fel.empty()) {
                    break;
                }

                if (fel.top()->getTimestamp() + fel.top()->getLookahead() < maxTime) {
                    maxTime = fel.top()->getTimestamp() + fel.top()->getLookahead();
                }

                batchIdx += fel.top()->getEventID() * ipow(EVENT_TYPE_COUNT, length);

                events[length] = fel.top();
                length++;
                fel.pop();
            }
            batchHandlers[batchIdx](events);
            eventCount += length;
        }
    }

}
