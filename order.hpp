#pragma once

#include <cstdint>
#include <chrono>

enum class Side{
    BUY,
    SELL
};

enum class OrderType{
    MARKET,
    LIMIT
};

struct Order{
    uint64_t id;
    double price;
    int quantity;
    Side side;
    OrderType type;
    std::chrono::high_resolution_clock::time_point timestamp;
};