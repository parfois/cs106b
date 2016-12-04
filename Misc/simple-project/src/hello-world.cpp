// Our first C++ program!
// headers:
#include <iostream>
#include "console.h" // Stanford library
#include "vector.h"
using namespace std;

const int NUM_ELEMENTS = 1000;

// main
int main()
{
    Vector<int> myList;
    cout << "Populating a list of even numbers less than "
         << NUM_ELEMENTS * 2 << endl;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        myList.add(i*2);
    }

    for (int i : myList) {
        cout << i << endl;
    }
    return 0;
}
