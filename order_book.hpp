#pragma once

#include "order.hpp"
#include <map>
#include <deque>
#include <unordered_map>
#include <cstdint>
#include <optional>


class OrderBook{
    public:
        OrderBook();

        void add_order(Order& order);
        void cancel_order(const uint64_t order_id);
        void modify_order(const uint64_t order_id, const double new_price, const int new_quantity);

    private:
        std::map<double, std::deque<Order>, std::greater<>> bids; // a map to map a specific price to a deque of bids at that price. The map is ordered with highest price first.
        std::map<double, std::deque<Order>> asks; // a map to map a specific price to a deque of asks at that price. The map is ordered with lowest price first.

        struct OrderLocation{
            Side side;
            double price;
            int index;
        };

        std::unordered_map<uint64_t, OrderLocation> order_location;

        template <typename MapType>
        void match_against_book(Order& order, MapType& book);

        void match_order(Order& order);


};