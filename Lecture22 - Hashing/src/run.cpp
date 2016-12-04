#include <iostream>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <assert.h>
#include "console.h"
#include "strlib.h"
#include "wikipedia.h"
#include "graph.h"
#include "demoutil.h"
#include "sound.h"
#include "simpio.h"
using namespace std;

const int N_BUCKETS = 50000;
const int MAX_ELEMS = N_BUCKETS;
const int DELTA = 10000;
const double MAX_TIME = 2;

void showHashUsage(int nElements, dataT & data) {
    Wikipedia myMap;
    Vector<string> keys = data.words.keys();
    int * hashCounts = new int[N_BUCKETS];
    for(int i = 0; i < N_BUCKETS; i++) {
        hashCounts[i] = 0;
    }

    for(int i = 0; i < nElements; i++) {
        string key = keys[i];
        hashCounts[myMap.hash(key)]++;
    }
    visualizeHashCounts(hashCounts, N_BUCKETS);
    delete[] hashCounts;
}

double experiment(int nElements, dataT & data) {
    Wikipedia myMap;
    assert(nElements <= data.words.size());
    Vector<string> keys = data.words.keys();

    // First add nElements number of records to the map
    for(int i = 0; i < nElements; i++) {
        string key = keys[i];
        string value = data.words.get(key);
        myMap.put(key, value);
    }

    // Then get the same nElements records from the map
    clock_t totalTime = 0;
    for(int i = 0; i < nElements; i++) {
        string key = keys[i];
        clock_t start = clock();
        string value = myMap.get(key);
        clock_t end = clock();
        totalTime += end - start;
        if(value == "") {
            cerr << "Test Fail: Inserted element not found!" << endl;
            exit(1);
        }
    }

    // Return the number of seconds it took...
    return (totalTime / (double) CLOCKS_PER_SEC) * 1000000;
}

void get(dataT & data) {
    string key = getLine("Enter article title: ");
    key = toLowerCase(key);
    if(!data.articles.containsKey(key)) {
        cout << "Article not in Wikipedia!" << endl;
    } else {
        cout << data.articles.get(key) << endl;
    }
}

void put(dataT & data) {
    string key = getLine("Enter article title: ");
    string value = getLine("Enter article html: ");
    key = toLowerCase(key);
    data.articles.put(key, value);
}

void test(dataT & data) {
    Graph graph("Hash Map Test", MAX_ELEMS, MAX_TIME);
    cout << "elems" << "\t" << "time (micro s)" << endl;
    for(int i = DELTA; i <= MAX_ELEMS; i += DELTA) {
        double duration = experiment(i, data);
        double perElem = duration / i;
        cout << i << "\t" << perElem << endl;
        graph.addPoint(i, perElem);
    }
    showHashUsage(MAX_ELEMS, data);
}

int main() {
    cout << "Loading data..."<< endl;
    dataT data = loadData();
    cout << "Welcome to Wikipedia! " << endl;
    cout << " Get : Looks up and article by name " << endl;
    cout << " Put : Adds an article to wikipedia " << endl;
    cout << " Test : Runs a sweet of tests " << endl;
    cout << "----------------------------" << endl;

    while(true) {
        string cmd = getLine("Enter Command");
        cmd = toLowerCase(cmd);
        if(cmd == "get") {
            get(data);
        } else if(cmd == "put") {
            put(data);
        } else if(cmd == "test") {
            test(data);
        } else {
            cout <<"Command: " << cmd << " not recognized." << endl;
        }
        cout << endl;
    }

    return 0;
}
