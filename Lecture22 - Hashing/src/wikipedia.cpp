#include "wikipedia.h"
#include <iostream>
#include <math.h>
#include "random.h"
using namespace std;

const int INIT_SIZE = 50000;

Wikipedia::Record * Wikipedia::findRecord(int bucket, string key) {
    Record * head = buckets[bucket];
    while(head != NULL) {
        if(head->key == key) {
            return head;
        }
        head = head->link;
    }
    return NULL;
}

Wikipedia::Wikipedia() {
    nBuckets = INIT_SIZE;
    buckets = new Wikipedia::Record*[nBuckets];
    for(int i = 0; i < nBuckets; i++) {
        buckets[i] = NULL;
    }
}

Wikipedia::~Wikipedia() {
    for(int i = 0; i < nBuckets; i++) {
        // not included for brevity
        // deleteList(buckets[i]);
    }
    delete[] buckets;
}

bool Wikipedia::contains(string key) {
    int bucketIndex = hash(key);
    return findRecord(bucketIndex, key) != NULL;
}

void Wikipedia::remove(string key) {
    int bucket = hash(key);
    Record * head = buckets[bucket];
    Record * previous = NULL;
    while(head != NULL) {
        if(head->key == key) {
            if(previous == NULL) {
                buckets[bucket] = head->link;
            } else {
                previous->link = head->link;
            }
            delete head;
        }
        previous = head;
        head = head->link;
    }
}

string Wikipedia::get(string key) {
    int bucketIndex = hash(key);
    Record * found = findRecord(bucketIndex, key);
    if(found == NULL) {
        return "";
    }
    return found->value;
}

void Wikipedia::put(string key, string value) {
    int bucketIndex = hash(key);
    Record * oldRecord = findRecord(bucketIndex, key);
    if(oldRecord != NULL) {
        oldRecord->value = value;
    } else {
        Record * newRecord = new Record();
        newRecord->key = key;
        newRecord->value = value;
        newRecord->link = buckets[bucketIndex];
        buckets[bucketIndex] = newRecord;
    }
}

int Wikipedia::hash(string key) {
    unsigned h = 33;
    for(unsigned int i = 0; i < key.length(); i++) {
        char ch = key[i];
        h +=  ch;
    }
    return h % nBuckets;
}

