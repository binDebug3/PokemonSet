#ifndef BST_H
#define BST_H

#include <sstream>
#include <string>

using namespace std;

template<typename Teml>
class BST {
private:
	//NODE nested struct--------------------------------------------------------------------
	struct Node {
		Teml nodeValue;
		Node* leftChild;
		Node* rightChild;
		//CONSTRUCTORS
		Node(const Teml& listNode) :
			nodeValue(listNode),
			leftChild(NULL),
			rightChild(NULL)
		{}
		Node(const Teml& listNode, Node* leftPtr, Node* rightPtr) :
			nodeValue(listNode),
			leftChild(leftPtr),
			rightChild(rightPtr)
		{}
		//SETTERS
		void setValue(const Teml nodeValue) {
			this->nodeValue = nodeValue;
		}
		void setLeftChild(Node* ptr) {
			leftChild = ptr;
		}
		void setRightChild(Node* ptr) {
			rightChild = ptr;
		}
		//PRINT
		string toString() const {
			stringstream out;
			out << this->nodeValue << " ";
			return out.str();
		}
		friend ostream& operator<< (ostream& os, Node& thing) {
			os << thing.toString();
			return os;
		}
	};
	//--------------------------------------------------------------------------------------
	Node* root;
	unsigned int numNodes;

public:
	BST(void) :
		root(NULL),
		numNodes(0)
	{}
	~BST(void) {
		clearTree();
	}

	//Return true if node added to BST, else false
	virtual bool addNode(const Teml& addN) { return addNode(addN, root); }
	bool addNode(const Teml& addN, Node*& nodePos);

	//Return true if node removed from BST, else false
	virtual bool removeNode(const Teml& removeN) { return removeNode(removeN, root, NULL); }
	bool removeNode(const Teml& removeN, Node* nodePos, Node* parent);

	// Return true if findN is found
	virtual bool find(const Teml& findN) { return find(findN, root); }
	bool find(const Teml& findN, const Node* nodePos) {
		if (nodePos == NULL)				return false;
		if (nodePos->nodeValue == findN)	return true;
		if (findN < nodePos->nodeValue)		return find(findN, nodePos->leftChild);
		if (findN > nodePos->nodeValue)		return find(findN, nodePos->rightChild);
		return false;
	}
	
	//Return number of nodes
	int size(void) const {
		return numNodes;
	}
	
	//Return true if BST cleared of all nodes, else false
	bool clearTree() {
		while (root != NULL)
			removeNode(root->nodeValue, root, NULL);
		if (numNodes != 0)
			return false;
		return true;
	}

	// Return a level order traversal of a BST as a string
	string toString() const;
	friend ostream& operator<< (ostream& os, BST<Teml>& thing) {
		os << thing.toString();
		return os;
	}
	void print(Node* nodePos, stringstream& os) const {
		if (nodePos != NULL) {
			print(nodePos->leftChild, os);
			os << nodePos->nodeValue << ", ";
			print(nodePos->rightChild, os);
		}
	}
};
#endif


//ADD------------------------------------------------------------------------------------
template <typename Teml>
bool BST<Teml>::addNode(const Teml& addN, Node*& nodePos) {
	if (nodePos == NULL) {
		Node* newNode = new Node(addN);
		nodePos = newNode;
		++numNodes;
		return true;
	}
	if (addN == nodePos->nodeValue) return false;
	if (addN < nodePos->nodeValue) return addNode(addN, nodePos->leftChild);
	if (addN > nodePos->nodeValue) return addNode(addN, nodePos->rightChild);
	return false;
}
//REMOVE---------------------------------------------------------------------------------
template <typename Teml>
bool BST<Teml>::removeNode(const Teml& removeN, Node* nodePos, Node* parent) {
	if (nodePos == NULL) return false;
	if (removeN < nodePos->nodeValue) return removeNode(removeN, nodePos->leftChild, nodePos);
	if (removeN > nodePos->nodeValue) return removeNode(removeN, nodePos->rightChild, nodePos);
	if (removeN == nodePos->nodeValue) {
		Node* child = nodePos;
		if (nodePos->leftChild != NULL) {			//check left child
			parent = nodePos->leftChild;			//set temp nodes
			if (parent->rightChild != NULL) {		//check right child of parent
				child = parent->rightChild;
				while (child->rightChild != NULL) {	//iterate until the predecessor is reached
					child = child->rightChild;
					parent = parent->rightChild;
				}
			}
			else {									//if parent is end, update temp nodes
				child = parent;
				parent = nodePos;
			}
		}
		else if (nodePos == root) {
			parent = NULL;
			child = root;
		}
		else if (nodePos->rightChild != NULL) {		//if no left child, check right child
			parent = nodePos->rightChild;
			if (parent->rightChild != NULL) {		//check next level right child
				child = parent->rightChild;
				while (child->rightChild != NULL) {
					child = child->rightChild;
					parent = parent->rightChild;
				}
			}
			else {									//if parent is the end, update nodes
				child = parent;
				parent = nodePos;
			}
		}
		else {										//if nodePos is the last node, update nodes
			child = nodePos;
		}
		if (parent != NULL) {						//update parent appropriately
			Teml oldParentValue = parent->nodeValue;
			if (child->leftChild != NULL) {
				if (parent->leftChild == NULL)
					parent->setRightChild(child->leftChild);
				else
					parent->setLeftChild(child->leftChild);
			}
			else if (child->nodeValue < oldParentValue)
				parent->setLeftChild(NULL);
			else
				parent->setRightChild(NULL);
		}
		else if (child == root)						//update root if necessary
			root = root->rightChild;
		nodePos->setValue(child->nodeValue);		//update child
		delete child;								//update private variables
		--numNodes;
		if (numNodes == 0)
			root = NULL;
		return true;
	}
	return false;
}
//TO STRING call print function to print an in order traversal---------------------------
template <typename Teml>
string BST<Teml>::toString() const {
	stringstream out;
	if (root == NULL) out << " empty";
	else {
		string line;
		print(root, out);
		getline(out, line);
		//get rid of the last comma
		line.pop_back();
		line.pop_back();
		out.str(line);
	}
	return out.str();
}