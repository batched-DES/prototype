#include <vector>
#include <utility>

#include "Event.h"

namespace BatchSimulator {

    class EventHeap {

    private:
        static constexpr size_t D = 4;
        std::vector<Event*> vector;

        inline size_t getParentIndex(size_t i) {
            return (i - 1) / D;
        }

    public:

        inline bool empty() const {
            return vector.empty();
        }

        inline const size_t size() const {
            return vector.size();
        }

        inline Event* top() const {
            return vector.front();
        }

        void push(Event* element) {
            vector.push_back(std::move(element));
            size_t index = vector.size() - 1;
            size_t parent = getParentIndex(index);

            while (index != 0 && *vector[index] < *vector[parent]) {
                std::swap(vector[index], vector[parent]);
                index = parent;
                parent = getParentIndex(index);
            }
        }

        void pop() {
            vector[0] = std::move(vector.back());
            vector.pop_back();

            size_t index = 0;

            while (D * index + 1 < vector.size()) {
                size_t min = D * index + 1;
                size_t m = min + 1;
                while (m < vector.size() && m <= D * index + D) {
                    if (*vector[m] < *vector[min]) {
                        min = m;
                    }
                    m++;
                }

                if (*vector[min] < *vector[index]) {
                    std::swap(vector[min], vector[index]);
                    index = min;
                } else {
                    return;
                }
            }

        }
    };
}
