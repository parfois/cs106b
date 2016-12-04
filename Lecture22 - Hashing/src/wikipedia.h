#ifndef WIKIPEDIA
#define WIKIPEDIA
#include <iostream>
using namespace std;

// Rename to Wikipedia!
class Wikipedia {
public:
    /*
     * Constructor: Wikipedia
     * Usage: Wikipedia map;
     * ---------------------
     * Initializes an empty Wikipedia
     */
    Wikipedia();
    ~Wikipedia();

    /*
     * Method: Get
     * Usage: map.get(key);
     * ---------------------
     * Returns the value for key or the empty string, if key is not found.
     */
    string get(string key);

    /*
     * Method: Contains
     * Usage: map.contains(key);
     * ---------------------
     * Checks if a key/value pair with the given key is already in the map.
     */
    bool contains(string key);

    /*
     * Method: Put
     * Usage: map.put(key, value);
     * ---------------------
     * Adds a key/value pair to the map. If the key already exists in the map,
     * it will replace the previous key/value pair with the same key.
     */
    void put(string key, string value);

    /*
     * Method: Remove
     * Usage: map.remove(key);
     * ---------------------
     * Removes the key/value pair associated with the given key.
     */
    void remove(string key);

    /*
     * Method: Hash
     * Usage: map.hash(key);
     * ---------------------
     * Computes an integer in the range of 0 to nBuckets. It is not
     * typical to make this method public. I have done so in the
     * interest of class demonstration.
     */
    int hash(string key);

private:

    /* Type definition for records in the map */
    struct Record {
        string key;
        string value;
        Record * link;
    };

    /* Instance variables */
    Record ** buckets;  // An array of linked lists
    int nBuckets;
    int count;

    /* Private method prototypes */
    Record * findRecord(int bucket, string key);
};

#endif // WIKIPEDIA
