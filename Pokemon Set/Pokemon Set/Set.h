#ifndef SET
#define SET

#include "SetInterface.h"
#include "BST.h"
#include <string>

using namespace std;

template<typename Teml>
class Set : public SetInterface<Teml> {
private:
	BST<Teml> bst;
public:
	Set() {}
	~Set() {
		this->clear();
	}

	// Inserts item into the set
	bool insert(const Teml& item) {
		return bst.addNode(item);
	}

	// Removes all items from the set.
	void clear() {
		bst.clearTree();
	}

	// return: the number of elements contained by the Set.
	size_t size() const {
		return bst.size();
	}

	// return: return 1 if contains element equivalent to item, else 0.
	size_t count(const Teml& item) {
		return bst.find(item);
	}

	// return: string representation of items in Set.
	string toString() const {
		//need to convert to in order
		return bst.toString();
	}
	friend ostream& operator<< (ostream& os, Set<Teml>& thing) {
		os << thing.toString();
		return os;
	}
};
#endif // SET