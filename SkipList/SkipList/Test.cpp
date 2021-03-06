#pragma once
#include "stdafx.h"
#include "SkipList.h"
#include <string>
#include <math.h>
#include <time.h>
using namespace std;


int main()
{
	//Insert 100 random numbers from 0-99 into the Skiplist
	SkipList<int> sl = SkipList<int>();
	int inserted [100];
	for (int i = 0; i < 100; i++) {
		inserted[i] = rand() % 100;
		sl.SkiplistInsertion(inserted[i]);
		if (i % 10 == 0) {
			sl.printList();
			cout << endl;
		}
	}
	sl.printList();

	cout << "\nIs 100 in the list?" << endl; 
	string result = sl.isInlist(100) ? "Yes" : "No";
	cout << result << endl; 
	cout << "\nNumber of items in list: " << sl.numItems() << endl;
	
//Remove those numbers from the Skiplist
	for (int i = 0; i < 100; i++) {	
		sl.SkiplistDeletion(inserted[i]);
		if (sl.isInlist(inserted[i])) {
			cout << "ITEM NOT REMOVED" << endl;
		}
		if (i % 10 == 0) {
			sl.printList();
			cout << endl;
		}
	}
	sl.printList();
    return 0;
}



