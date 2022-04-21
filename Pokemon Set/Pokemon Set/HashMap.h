#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "HashMapInterface.h"
#include <iostream>
#include <string>
#include <utility>
#include <sstream>

using namespace std;

template <typename KEY, typename VAL>
class HashMap : public HashMapInterface<KEY, VAL> {
private:
	int capacity;
	pair<KEY, VAL>* myPairs;
	int numFilled;

	void reallocate(void) {							//create new array with double the size and copy old array into the new array
		unsigned int reCapacity = capacity * 2;
		unsigned int index = 0;
		pair<KEY, VAL>* realloc = new pair<KEY, VAL>[reCapacity];
		for (int i = 0; i < capacity; i++) {
			unsigned long long hash = myPairs[i].first[0];
			unsigned int index = 0;
			//create a really big hash number based on the key
			for (unsigned int m = 1; m < myPairs[i].first.length(); ++m) {
				hash = hash * 3 + myPairs[i].first[m];
			}
			//mod it so it fits in the map
			index = hash % reCapacity;

			int k = 1;
			while (true) {
				//find a place to put the new item
				if (realloc[index].first.length()) {
					if (realloc[index].first == myPairs[i].first) break;
					index = (index + k) % reCapacity;
					k += 2;
					continue;
				}
				realloc[index].first = myPairs[i].first;
				realloc[index].second = myPairs[i].second;
				break;
			}
		}
		capacity = reCapacity;
		myPairs = realloc;
		//myPairs.swap(realloc);
		//delete[] realloc;
	}

public:
	HashMap<KEY, VAL>() : 
		myPairs(new pair<KEY, VAL>[DEFAULT_MAP_HASH_TABLE_SIZE]), 
		capacity(DEFAULT_MAP_HASH_TABLE_SIZE), 
		numFilled(0)
	{}
	~HashMap() {
		this->clear();
	}

	// Read/write index access operator.
	VAL& operator[](const KEY& key);

	// return: the number of elements that matchthe key in the Map.
	size_t count(const KEY& key) {
		return 0;
	}

	// Removes all items from the Map.
	void clear() {
		delete[] myPairs;
	}

	// return: number of Key-Value pairs stored in the Map.
	size_t size() const {
		return numFilled;
	}

	// return: maximum number of Key-Value pairs that the Map can hold.
	size_t max_size() const {
		return capacity;
	}

	// return: string representation of Key-Value pairs in Map.
	string toString() const {
		ostringstream os;
		for (int i = 0; i < capacity; ++i) {
			if (myPairs[i].first.length()) {
				os << endl << "  [" << i << ":" << myPairs[i].first << "->" << myPairs[i].second << "]";
			}
		}
		return os.str();
	}
	friend ostream& operator<< (ostream& os, HashMap<KEY, VAL>& thing) {
		os << thing.toString();
		return os;
	}
};
#endif // HASH_MAP


template <typename KEY, typename VAL>
VAL& HashMap<KEY, VAL>::operator[](const KEY& key) {
	unsigned long long hash = key[0];
	unsigned int index = 0;
	//create a really big hash number based on the key
	for (unsigned int i = 1; i < key.length(); ++i) {
		hash = hash * 3 + key[i];
	}
	//mod it so it fits in the map
	index = hash % capacity;

	int k = 1;
	while (true) {
		//find a place to put the new item
		if (myPairs[index].first.length()) {
			if (myPairs[index].first == key) break;
			index = (index + k) % capacity;
			k += 2;
			continue;
		}
		myPairs[index].first = key;
		++numFilled;
		if ((numFilled * 100 / capacity)  > LOAD_THRESHOLD) {
			reallocate();
		}
		break;
	}
	return myPairs[index].second;
}