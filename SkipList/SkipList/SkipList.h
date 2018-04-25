#pragma once
#include <iostream>
#include <string>
#include <random>
#include <time.h>

using namespace std;

template <class T>
struct Node { //represents a node in the SkipList
	//stores the value as well as node above, below, and next
	bool sentinel;
	T key;
	Node<T>* prev;
	Node<T>* next;
	Node<T>* above;
	Node<T>* below;

	//boilerplate constructor stuff
	Node() :  Node(true, NULL, NULL) {};
	Node(bool s, Node<T>* p, Node<T>* n) :
		sentinel(s), prev(p), next(n), above(NULL), below(NULL) {};
	Node(T key, Node<T>* p, Node<T>* n) : Node(false, p, n) { this->key = key; }
	
	~Node() { //enables the recursive destructor in SkipList
		delete next;
	}
};

template <class T>
class SkipList {
public:
	//constructor
	SkipList() {
		numInList = 0;
		head = new Node<T>[maxHeight]; // Array of heads: "left-most column on skip list"
		for (int i = 0; i < maxHeight; i++) { //Initialize the nodes in the head array
			//Attach a sentinel to their right
			Node<T>* current = &head[i];
			current->next = new Node<T>(true, current, NULL);
			//link the left sentinel column above and below
			if (i != maxHeight - 1) {
				current->above = &head[i + 1];
			}
			if (i != 0) {
				current->below = &head[i - 1];
			}
		}

		//Initialize random number generator
		generator = mt19937(time(NULL));
		dist = uniform_int_distribution<int>(0, 1);
	}
	//destructor
	~SkipList() {
		delete[] head; //everything should recursively delete (see Node's destructor)
	}

	//Returns true if node with search key exists in the tree
	bool isInlist(T const& key) { return SkiplistSearch(key)->key == key; }

	//Inserts the key in sorted position at the bottom layer, and then creates a tower
	void SkiplistInsertion(T const& key) {
		Node<T>* current = SkiplistSearch(key); //find the node we should insert after
		if (current->key == key) { return; } //prevent duplicate inserts
		numInList++; //increment our counter for size of bottom layer
		insertAfterAbove(current, key); //insert at base layer
		int i = 0; //counter to make sure we keep the top layer empty

		while(coinFlip() && i < maxHeight-2) { //flip a coin to see if we go up a layer
			while (current->above == NULL) { //keep going left until we find a tower
				current = current->prev;
			}
			current = current->above; //go up a layer
			insertAfterAbove(current, key); //insert at this layer
			i++; 
		}
	}

	//Removes the key and its tower from memory, without disturbing the tree
	void SkiplistDeletion(T const& key) {
		Node<T>* current = SkiplistSearch(key); //find the node we should delete
		if (current->key != key) { return; } //didn't find the key, don't delete anything
		numInList--; //decrement our counter for size of bottom layer
		do {
			//remove pointers to node from previous and next nodes
			current->prev->next = current->next;
			current->next->prev = current->prev;
			//store the pointer for the node above and delete node
			Node<T>* temp = current;
			current = current->above;
			temp->next = NULL; //prevents our recursive destructor from activating
			delete temp;
		} while (current != NULL);
	}
	bool isEmpty() { return numInList == 0; }
	int numItems() { return numInList; }
	void printList() {
		for (int i = maxHeight-1; i >= 0; i--) { //go through each level starting from top
			Node<T>* current = &head[i];
			while ((current->next) != NULL) {//print out the values of everything at this level
				current->sentinel ? 
					cout << "*sentinel*" :
				    cout << current -> key;
				cout << " ";
				current = current->next;
			}
			cout << "*sentinel*";
			cout << "\n";
		}
	}

private:
	//Private Head array
	Node<T> *head; //head node is defined as a node with a stored value of NULL
	static const int maxHeight = 6; //tallest "stack" of nodes possible

	//Private utility methods and data
	int numInList;

	//Mersenne Twister RNG
	mt19937 generator;
	uniform_int_distribution<int> dist;
	bool coinFlip() { return dist(generator) == 0; }  //50/50 to return T/F

	//Returns a pointer to the node corresponding to the search key
	//SkiplistSearch returns the "previous" node if exact key not found
	Node<T>* SkiplistSearch(T const& key) {
		Node<T> * current = &head[maxHeight - 1]; //top left sentinel
		while (current->below != NULL) { //stops us from going past bottom layer
			current = current->below; //go down a layer
			while (!current->next->sentinel && key >= current->next->key) { //keep going right if the key is larger than right node
				current = current->next;
			}
		}
		return current;
	}

	//insert after the first node, stitching the inserted node between previous and next
	//then, stitch the inserted node to the node below it if we aren't on the bottom layer
	void insertAfterAbove(Node<T>* base, T key) {
		//Stitch before and after
		Node<T>* temp = base->next;
		base->next = new Node<T>(key, base, temp);
		temp->prev = base->next;
		//Walk down one layer, and then walk to the right until we find the tower for the key
		if (base->below != NULL) {
			temp = base->below;
			while (temp->key != key) {
				temp = temp->next;
			}
			//Stitch above and below
			temp->above = base->next;
			base->next->below = temp;
		}
	}
};