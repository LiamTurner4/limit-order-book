# C++ Limit Order Book Simulator

This project implements a fast, modular limit order book in C++, simulating real-time order matching in financial markets.

It supports:
- Market and limit orders (BUY/SELL)
- Price-time priority matching
- Fill-or-kill behavior for market orders
- Order book state printing (top and full book)
- Microsecond-level latency tracking with `std::chrono`

### How It Works

Orders are randomly generated and passed to the matching engine.  
- Market orders are matched immediately or discarded.  
- Limit orders are matched if possible or stored in the book.  
- Matching is done by comparing against the opposite side of the book (bids vs asks), using sorted `std::map` containers.
- Functionality for modifying and cancelling orders also included (but not used in the simulation)
