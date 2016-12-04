// Header file for VectorInt class

#pragma once

#include<ostream>
using namespace std;

const int INITIAL_CAPACITY = 8;

class VectorInt {
public:
    // constructor
    VectorInt();

    // destructor
    ~VectorInt();

    // append value to the end of our array
    void add(int value);

    // insert value at index
    void insert(int index, int value);

    // return the element at index
    int get(int index);

    // remove value from index
    void remove(int index);

    // returns the number of elements
    int size();

    // returns true if there aren't any elements
    bool isEmpty();

    // overload << operator
    friend ostream &operator<<(ostream &out, VectorInt &vec);
private:
    int *elements;
    int capacity;
    int count;
    // expand when necessary
    void expand();
};
