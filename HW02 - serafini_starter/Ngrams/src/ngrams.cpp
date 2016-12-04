#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include <unordered_map>
#include <vector>
#include <queue>
#include <map>
#include "random.h"

using namespace std;

//my finctions
void getHeader();
string getNext(map<vector<string>, vector<string>>& collection, vector<string>& window);

int main() {
    getHeader();
    string fileName = getLine("Input file name?");
    //string fileName = "hamlet.txt";

    while (true) {
        fstream file;
        file.open(fileName);
        int nGram;
        do {
            nGram = getInteger("\n\nValue of N?");
        } while (nGram < 1);
        int nWords;
        do {
            nWords = getInteger("# of random words to generate (0 to quit)?");
        } while (nWords < 0);
        if (nWords == 0) {
            cout << "\n\nExiting." << endl;
            return 0;
        }

        const int n = nGram - 1;
        vector<string> prefix(n);

        map<vector<string>, vector<string>> collection;
        vector<string> allWords;
        string word;
        while (file >> word) {
            allWords.push_back(word);
        }
       // cout <<"here";
        for (int i = 0; i < n; i++) {
            allWords.push_back(allWords[i]);
        }
        //cout <<"here1";
        int nthWord = 0;
        while (nthWord < allWords.size() - n) {
            for (int i = 0; i < n; i++) {
                prefix[i] = allWords[nthWord + i];
            }

            // cout <<"here2";

            map<vector<string>, vector<string>> :: iterator it = collection.find(prefix);
            vector<string> suffix;
            if (it == collection.end()) {
                suffix.push_back(allWords[nthWord + n]);
                collection[prefix] = suffix;
            }
            else {
                suffix = collection.at(prefix);
                suffix.push_back(allWords[nthWord + n]);
                collection.at(prefix) = suffix;
            }
           // cout <<"here3";

            nthWord++;
        }

        int random = rand() % collection.size();
        map<vector<string>, vector<string>> :: iterator itRand = collection.begin();
        std::advance(itRand, random);
        vector<string> window = (*itRand).first;
       // cout <<"here4";

        vector<string> text = window;
        while (text.size() < nWords) {
          //  cout <<"here5";
            string next = getNext(collection, window);
            text.push_back(next);
        }

        for (string word : text) {
            cout << word << " ";
        }

    }

    return 0;
}

string getNext(map<vector<string>, vector<string>>& collection, vector<string>& window) {
    vector<string> suffix;
    suffix = collection[window];
    int random = rand() % suffix.size();
    for (int i = 0; i < window.size() - 1; i++) {
        window[i] = window[i+1];
    }
    window[window.size() - 1] = suffix[random];
    return suffix[random];
}

void getHeader() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}
