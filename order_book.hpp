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

        void print_top_of_book() const;
        void print_order_book() const;

    private:
        std::map<double, std::deque<Order>, std::greater<>> bids; // An ordered map that associates each price with a deque of buy (bid) orders at that price level. The map is sorted in descending order, so the highest bid prices come first.
        std::map<double, std::deque<Order>> asks; // An ordered map that associates each price with a deque of sell (ask) orders at that price level. The map is sorted in ascending order, so the lowest ask prices come first.

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