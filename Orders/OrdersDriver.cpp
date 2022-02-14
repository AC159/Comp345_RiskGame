#include "Orders.h"
#include <iostream>

using namespace std;
using namespace Orders;

/**
 * Test the move functionality of the OrdersList class
 * @param list the object to test
 * @param oldIndex the index of the element to be moved
 * @param newIndex the index where to move the element
 */
void testMove(OrdersList &list, int oldIndex, int newIndex) {
    cout << "/Before moving element #" << oldIndex << " to position #" << newIndex << ":\t" << list;
    list.move(oldIndex, newIndex);
    cout << "\\After moving element #" << oldIndex << " to position #" << newIndex << ":\t\t" << list << endl;
}

/**
 * Test the remove functionality of the OrdersList class
 * @param list the object to test
 * @param index the index of the element to be removed
 */
void testRemove(OrdersList &list, int index) {
    cout << "/Before removing element #" << index << ":\t\t" <<list;
    list.remove(index);
    cout << "\\After removing element #" << index << ":\t\t" << list << endl;
}

// test all functionalities pertaining to orders
void Orders::Order::ordersDriver() {
//int main() {
    cout << "\n -Creating orders of every kind and adding them to OrdersList object...\n" << endl;
    OrdersList list;
    list.add(new Deploy);
    list.add(new Advance);
    list.add(new Bomb);
    list.add(new Blockade);
    list.add(new Airlift);
    list.add(new Negotiate);

    cout << "\n\n -Calling validate and execute functions for all list elements...\n" << endl;
    for (size_t i = 0; i < list.length(); i++) {
        list.element(i)->validate();
        cout << endl;
        list.element(i)->execute();
    }

    cout << "\n\n -Printing list contents before and after removing or moving element...\n" << endl;
    testRemove(list, 2);
    testMove(list, 2, 4);
    testMove(list, 3, 1);
}
//    return 0;
//}

