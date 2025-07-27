#include "order_book.hpp"
#include <iostream>
#include <iomanip>


OrderBook::OrderBook(){
    // constructor not needed currently
};


void OrderBook::add_order(Order& order){
    match_order(order);
    // if after matching orders there is still some left over and it is a limit order, then store it
    // if it is a market order we discard the left over order as market orders are "fill now or forget"
    if (order.quantity > 0 and order.type == OrderType::LIMIT){
        // OrderLocation struct is needed to allow cancel and modify functions to work
        OrderLocation current_order_location;
        current_order_location.price = order.price;

        if (order.side == Side::BUY){
            current_order_location.side = Side::BUY;
            current_order_location.index = bids[order.price].size(); // if bids[order.price] has size 3, the next order added will have index 3

            order_location[order.id] = current_order_location;

            // if bids map contains order(s) at that price add it to the back of the deque
            if (bids.count(order.price)) {
                bids[order.price].push_back(order);

            // else create a deque at that price with the order in it 
            } else {
                bids[order.price] = std::deque<Order>{order};
            }
        }
        else{
            current_order_location.side = Side::SELL;
            current_order_location.index = asks[order.price].size();

            order_location[order.id] = current_order_location;

            if (asks.count(order.price)) {
                asks[order.price].push_back(order);
            } else {
                asks[order.price] = std::deque<Order>{order};
            }
        }
    }
}


void OrderBook::match_order(Order& order){
    if (order.side == Side::BUY){
        match_against_book(order, asks);
    }
    else{
        match_against_book(order, bids);
    }
}


template <typename MapType>
void OrderBook::match_against_book(Order& order, MapType& book){
    for (auto price_level = book.begin(); price_level != book.end() && order.quantity > 0;){
            
        // if order is limit type and the max buy price is less than the current order sell price, break the loop
        if (order.type == OrderType::LIMIT){
            if (order.side == Side::BUY && order.price < price_level->first){
                break;
            }
            else if(order.side == Side::SELL && order.price > price_level->first){
                break;
            }
        }
        // store deque of orders at that price level
        auto& orders_deque = price_level->second;

        while (!orders_deque.empty() && order.quantity > 0) {
            Order& resting_order = orders_deque.front();

            int matched_quantity = std::min(resting_order.quantity, order.quantity);
            resting_order.quantity -= matched_quantity;
            order.quantity -= matched_quantity;

            // if resting order has all been bought, remove it
            if (resting_order.quantity == 0)
                orders_deque.pop_front();
        }

        // if order has been fulfilled, remove it
        if (orders_deque.empty()) {
            // Remove empty price level
            price_level = book.erase(price_level);  // erase returns next iterator
        } else {
            ++price_level;
        }
    }
};


void OrderBook::cancel_order(const uint64_t order_id){
    OrderLocation cancelled_order = order_location[order_id];
    std::deque<Order>* deque;

    // get the deque containing cancelled order
    if (cancelled_order.side == Side::BUY){
        deque = &bids[cancelled_order.price];
    }
    else{
        deque = &asks[cancelled_order.price];
    }
    if (cancelled_order.index < deque->size()) {
        deque->erase(deque->begin() + cancelled_order.index);
        // if deque is empty remove it so we dont have a price in the map with no asks/bids attached
        if (deque->empty()) {
            if (cancelled_order.side == Side::BUY) {
                bids.erase(cancelled_order.price);
            } else {
                asks.erase(cancelled_order.price);
            }
        }
    }
};

void OrderBook::modify_order(const uint64_t order_id, const double new_price, const int new_quantity){
    // to modify an order we delete it and then add a new one with the new price and quantity
    OrderLocation modified_order_loc = order_location[order_id];
    Order order_to_modify;

    if (modified_order_loc.side == Side::BUY){
        order_to_modify = bids[modified_order_loc.price][modified_order_loc.index];
    }
    else{
        order_to_modify = asks[modified_order_loc.price][modified_order_loc.index];
    }
    
    cancel_order(order_id);
    Order new_order;
    new_order.id = order_id;
    new_order.price = new_price;
    new_order.quantity = new_quantity;
    new_order.side = order_to_modify.side;
    new_order.type = order_to_modify.type;
    new_order.timestamp = std::chrono::high_resolution_clock::now();

    add_order(new_order);
}

void OrderBook::print_top_of_book() const {
    std::cout << "=== Top of Book ===\n";

    if (!bids.empty()) {
        const auto& top_bid = bids.begin();
        int bid_quantity = 0;
        for (const auto& order : top_bid->second)
            bid_quantity += order.quantity;

        std::cout << "Best Bid:  " << std::fixed << std::setprecision(2)
                  << top_bid->first << " (" << bid_quantity << " shares)\n";
    } else {
        std::cout << "Best Bid:  None\n";
    }

    if (!asks.empty()) {
        const auto& top_ask = asks.begin();
        int ask_quantity = 0;
        for (const auto& order : top_ask->second)
            ask_quantity += order.quantity;

        std::cout << "Best Ask:  " << std::fixed << std::setprecision(2)
                  << top_ask->first << " (" << ask_quantity << " shares)\n";
    } else {
        std::cout << "Best Ask:  None\n";
    }

    std::cout << "===================\n";
}

void OrderBook::print_order_book() const {
    std::cout << "\n=== Full Order Book ===\n";

    std::cout << "\n-- Asks (Sell Orders) --\n";
    if (asks.empty()) {
        std::cout << "None\n";
    } else {
        for (const auto& [price, orders] : asks) {
            int total_quantity = 0;
            for (const auto& order : orders)
                total_quantity += order.quantity;

            std::cout << std::fixed << std::setprecision(2)
                      << "Ask " << price << ": " << total_quantity << " shares\n";
        }
    }

    std::cout << "\n-- Bids (Buy Orders) --\n";
    if (bids.empty()) {
        std::cout << "None\n";
    } else {
        for (const auto& [price, orders] : bids) {
            int total_quantity = 0;
            for (const auto& order : orders)
                total_quantity += order.quantity;

            std::cout << std::fixed << std::setprecision(2)
                      << "Bid " << price << ": " << total_quantity << " shares\n";
        }
    }

    std::cout << "========================\n";
}
