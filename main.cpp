#include "order.hpp"
#include "utils.hpp"
#include "order_book.hpp"

#include <iostream>
#include <chrono>
#include <vector>
#include <numeric> // for accumulate

int main() {
    const int NUM_ORDERS = 1000;
    const int PRINT_INTERVAL = 100;

    OrderBook book;
    std::vector<long long> latencies_microsec;

    int total_orders = 0;
    int total_market_orders = 0;
    int total_limit_orders = 0;

    std::cout << "Starting Order Book Simulation...\n";

    for (int i = 1; i <= NUM_ORDERS; ++i) {
        Order order = generateRandomOrder();
        total_orders++;
        if (order.type == OrderType::MARKET) total_market_orders++;
        else total_limit_orders++;

        auto start = std::chrono::high_resolution_clock::now();
        book.add_order(order);  // Internally calls match_order()
        auto end = std::chrono::high_resolution_clock::now();

        auto latency = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        latencies_microsec.push_back(latency.count());

        if (i % PRINT_INTERVAL == 0) {
            std::cout << "\n-- After " << i << " Orders --\n";
            book.print_top_of_book();
        }
    }

    std::cout << "\n=== Simulation Complete ===\n";
    std::cout << "Total Orders:         " << total_orders << "\n";
    std::cout << " - Limit Orders:      " << total_limit_orders << "\n";
    std::cout << " - Market Orders:     " << total_market_orders << "\n";

    // Average latency
    if (!latencies_microsec.empty()) {
        long long total_latency = std::accumulate(latencies_microsec.begin(), latencies_microsec.end(), 0LL);
        double avg_latency = static_cast<double>(total_latency) / latencies_microsec.size();
        std::cout << "Average Matching Latency: " << avg_latency << " µs\n";
    }

    std::cout << "\nFinal Top of Book:\n";
    book.print_top_of_book();

    std::cout << "\nFinal Full Order Book:\n";
    book.print_order_book();

    return 0;
}
