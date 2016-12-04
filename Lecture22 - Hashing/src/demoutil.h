#ifndef UTIL
#define UTIL

#include <iostream>
#include "hashmap.h"
using namespace std;

struct dataT {
    HashMap<string, string> words;
    HashMap<string, string> articles;
};

void visualizeHashCounts(int * hashCounts, int size);

dataT loadData();

#endif // UTIL

