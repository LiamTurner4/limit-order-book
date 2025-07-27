#include "utils.hpp"
#include <cstdlib>
#include <ctime>
#include <chrono>

uint64_t generate_unique_id() {
    static uint64_t current_id = 1;
    return current_id++;
}

Order generateRandomOrder() {
    Order randomOrder;
    randomOrder.id = generate_unique_id();
    randomOrder.price = 95.0 + (static_cast<double>(rand() % 1001) / 100.0); // generates price between 95.00 and 105.00
    randomOrder.quantity = (10 + (rand() % 91)) * 10; // generates quantity in multiples of 10 between 100 and 1000
    randomOrder.side = static_cast<Side>(rand() % 2);
    randomOrder.type = static_cast<OrderType>(rand() % 2);
    randomOrder.timestamp = std::chrono::high_resolution_clock::now();
    return randomOrder;
}
