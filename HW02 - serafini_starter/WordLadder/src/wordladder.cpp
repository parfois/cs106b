#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include <queue>
#include <stack>
#include <set>
#include "strlib.h"

using namespace std;

void getWords(Lexicon& dictionary, string& word1, string& word2);
void coutStack(stack<string> words);

int main() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the \nfirst into the second by changing one letter at a time." << endl;
    //string dictionaryName = getLine("Dictionary file name? ");
    Lexicon dictionary("EnglishWords.txt");

    string word1;
    string word2;
    getWords(dictionary, word1, word2);
    while (word1.size() != word2.size()) {
        cout << "Error: the two words must have same number of characters." << endl;
        getWords(dictionary, word1, word2);
    }
    while (word1.compare(word2) == 0) {
        cout << "Error: the two words must be different." << endl;
        getWords(dictionary, word1, word2);
    }

    stack<string> wordStack;
    queue<stack<string>> ladderQueue;
    wordStack.push(word1);
    ladderQueue.push(wordStack);
    set<string> neighborSet;
    neighborSet.insert(word1);

    while (!ladderQueue.empty()) {
        stack<string> curStack = ladderQueue.front();
        const string& topWord = curStack.top();
        string neighbor;
        for (int i = 0; i < (int) topWord.size(); i++) {
            char c = 'a';
            for (int j = 0; j < 26; j++) {
                neighbor = topWord;
                neighbor.replace(i, 1, 1, c);
                if (dictionary.contains(neighbor)) {
                    set<string>::iterator it;
                    it = neighborSet.find(neighbor);
                    if (it == neighborSet.end()) {
                        if (neighbor.compare(word2) == 0) {
                            cout << "Hooray! we have found a solution (and it is the stack you are working on in the queue)." << endl;
                            cout << "A ladder from " << word2 << " back to " << word1 << ": " << endl;
                            cout << word2 << " ";
                            coutStack(curStack);
                            cout << "Have a nice day.";
                            return 0;
                        }
                        else {
                          //  cout << "saving a neighbor:" << neighbor << endl;
                            stack<string> copyStack = curStack;
                           // cout << "current stack";
                           // coutStack(curStack);
                            copyStack.push(neighbor);
                          //  cout << "copy stack";
                            //coutStack(copyStack);

                            ladderQueue.push(copyStack);
                            neighborSet.insert(neighbor);
                        }
                    }

                }
                c += 1;
            }
        }
        ladderQueue.pop();
    }
    cout << "Sorry, no ladder found." << endl;
    return 0;
}

void getWords(Lexicon& dictionary, string& word1, string& word2) {
    word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
    while (!dictionary.contains(word1)) {
        cout << "Error: word not found in dictionary." << endl;
        word1 = getLine("Word #1 (or Enter to quit): ");
    }
    word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
    while (!dictionary.contains(word2)) {
        cout << "Error: word not found in dictionary." << endl;
        word2 = getLine("Word #2 (or Enter to quit): ");
    }
}
void coutStack(stack<string> words) {
    while(!words.empty()) {
        cout << words.top() << " ";
        words.pop();
    }
    cout << endl;
}
