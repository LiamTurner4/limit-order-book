#include "order.hpp"
#include "utils.hpp"
#include <iostream>
#include <ctime>

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Order randomOrder = generateRandomOrder();
    Order randomOrder2 = generateRandomOrder();
    std::cout << randomOrder.id << " " << randomOrder.price << " " << randomOrder.quantity << "\n";
    std::cout << randomOrder2.id << " " << randomOrder2.price << " " << randomOrder2.quantity << "\n";

    return 0;
};
