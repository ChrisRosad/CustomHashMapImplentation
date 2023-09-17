/*
* Name: Christopher Rosado, 2001956343, Assignment Five
* Description: The following custom hashMap class involves 
* two separate tables and double hashing. The following function
* are meant to aid in resizing and finding the location of items
* in order to create a proper hashmap.
*/
#include "hashMap.h"
#include <cmath>
#include <string>
#include <iostream>
using namespace std;
/* Description */
/*
* function_identifier: operator=(const hashMap<t1, t2>& rhs)
* parameters: const hashMap<t1, t2>& rhs)
* description: The operator overload is meant to make copying a map
* to a new map within the main. Allowing for the program look cleaner as well.
* return value: *this
*/
template <class t1, class t2>
const hashMap<t1, t2>& hashMap<t1, t2>::operator=(const hashMap<t1, t2>& rhs)
{
	if (this != &rhs)
	{
		this->deallocateMap();
		this->copyMap(rhs);
	}
	return *this;
}
/* Description */
/*
* function_identifier: operator[](t1 key)
* parameters: t1 key
* description: The operator overload here is meant to act like an 
* array. The key is inputted between the bracket like an index would 
* be. The program will then start to go through a series of checks 
* to ensure that the hashmap is ready to take in a new entry. If 
* items1 or items2 is at 20% capacity then the program will call the 
* resize function to rehash all the current values into a larger table.
* Whether or not that has occured, the program will then continue by 
* hashing the key for the first and, if needed, the second table index 
* for that specifc key. Once complete it will check through both of the 
* tables, the both tables will check if there is a matching key in the 
* first or second node(if they exist) and if not or they do not exist yet
* the program will then check to see if the new node can be placed 
* at the first table, if the table is full then it will move on to the 
* second table and do all of the same checks. If it goes through both tables
* without being able to place a new node or match keys, then it will increment 
* a collision counter and then will rehash using a double hashing technique and 
* will try again until it finds a match. Then in any of the situations mentioned 
* above, it will break out of the loop and will return the address of the node->value. 
* return value: address of t2&
*/
template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
{
	int Index1 = 0, Index2 = 0, CollisionCnt = 0;
	bool matchFound = true;
	
	if(((static_cast<double>(items1) / capacity) * 100) > 20 || ((static_cast<double>(items2) / capacity)* 100) > 20){
		resize(capacity);
	}
	

	Index1 = h1(key) % capacity;
	Index2 = h2(key) % capacity;
	
	node *ChkPntr;
	
	//found matching node
	while(matchFound){
	ChkPntr = table1[Index1];
	if(ChkPntr != nullptr){
		if(ChkPntr->key == key){
			matchFound = false;
			break;

		}
		if(ChkPntr->link != nullptr){
			ChkPntr = ChkPntr->link;
			if(ChkPntr->key == key){
				matchFound = false;
				break;
			}
		}
	}
	ChkPntr = table1[Index1];
	//linked list at index is completely empty
	if(ChkPntr == nullptr){
		table1[Index1] = new node;
		ChkPntr = table1[Index1];
		ChkPntr->key = key;
		ChkPntr->value = t2();
		ChkPntr->link = nullptr;
		break;
	}
	if(ChkPntr->link == nullptr){

		ChkPntr->link = new node;
		ChkPntr = ChkPntr->link;
		ChkPntr->key = key;
		ChkPntr->value = t2();
		ChkPntr->link = nullptr;
		items1+=1;
		break;
	}

	//checking table 2
	ChkPntr = table2[Index2];

	if(ChkPntr != nullptr){
		if(ChkPntr->key == key){
			break;
		}
		if(ChkPntr->link != nullptr){
			ChkPntr = ChkPntr->link;
			if(ChkPntr->key == key){
				break;
			}
		}
	}
	ChkPntr = table2[Index2];
	//linked list at index is completely empty
	if(ChkPntr == nullptr){
		table2[Index2] = new node;
		ChkPntr = table2[Index2];
		ChkPntr->key = key;
		ChkPntr->value = t2();
		ChkPntr->link = nullptr;
		break;
	}
	//checks if it has one spot open
	if(ChkPntr->link == nullptr){
		ChkPntr->link = new node;
		ChkPntr = ChkPntr->link;
		ChkPntr->key = key;
		ChkPntr->value = t2();
		ChkPntr->link = nullptr;
		items2+=1;
		break;
	}
	//If reached here then Collision has offically occured
	CollisionCnt++;
	Index1 = (h1(key) + CollisionCnt * h2(key)) % capacity;
	Index2 = (h2(key) + CollisionCnt * h1(key)) % capacity;
}

return ChkPntr->value;
}
/* Description */
/*
* function_identifier: resize(size_t amount)
* parameters: size_t amount
* description: The resize function is called when the 
* either of the hash tables are at 20% or more capacity
* when called the function will save the addresses of the 
* now old tables and use the class given table1 and table2
* variables to create new and larger tables by using the amount 
* parameter plus the current capacity. Once the new tables are 
* created and all indexes are set to nullptr then the function
* will loop and rehash all of the nodes from the old tables
* onto the new tables. Once complete it will deallocate the 
* current node and move to the next node or next index. Then
* the program will deallocate the old hash tables 
* return value: none
*/
template <class t1, class t2>
void hashMap<t1, t2>::resize(size_t amount)
{
	node **OldTable1;
	node **OldTable2;
	int increment = capacity;
	OldTable1 = table1;
	OldTable2 = table2;
	capacity += amount;
	items1 = items2 = 0;

	table1 = new node*[capacity];
	table2 = new node*[capacity];
	for(int i = 0; i < capacity; i++)
		table1[i] = table2[i] = nullptr;
	
	
	node *Traverser, *deletion;
	for(int i = 0; i < increment; i++){
		Traverser = OldTable1[i];
		if(Traverser != nullptr){
		if(Traverser->link != nullptr){
			Traverser = Traverser->link;
			deletion = Traverser;
			(*this)[Traverser->key] = Traverser->value;
			delete deletion;
			Traverser = OldTable1[i];
		}
		deletion = Traverser;
		(*this)[Traverser->key] = Traverser->value;
		delete deletion;
	}

		Traverser = OldTable2[i];
		if(Traverser != nullptr){
		if(Traverser->link != nullptr){
			Traverser = Traverser->link;
			deletion = Traverser;
			(*this)[Traverser->key] = Traverser->value;
			delete deletion;
			Traverser = OldTable2[i];
		}
		deletion = Traverser;
		(*this)[Traverser->key] = Traverser->value;
		delete deletion;
	}
	}
	delete [] OldTable1;
	delete [] OldTable2;
}
/* Description */
/*
* function_identifier: copyMap(const hashMap<t1, t2>& map)
* parameters: const hashMap<t1, t2>& map
* description: The copy map function will create a new set
* of variables for *this, allocating space within *this tables
* and setting *this variables to be the same as map's variables.
* Then the program will go into a for loop that will check the amount
* of nodes within each index and will copy each node into the 
* *this tables, if it is null then it will move on to the next 
* index. Once complete it will return and the main program will
* have two instances of the same hashmap.
* return value: none
*/
template <class t1, class t2>
void hashMap<t1, t2>::copyMap(const hashMap<t1, t2>& map)
{
	this->table1 = new node *[map.capacity];
	this->capacity = map.capacity;
	this->table2 = new node *[map.capacity];
	this->items1 = map.items1;
	this->items2 = map.items2;
	
	node *Tab1pntr, *Tab2pntr;
	for(int i = 0; i < map.capacity; i++){
		Tab1pntr = map.table1[i];
		this->table1[i] = nullptr;
		if(Tab1pntr != nullptr){
			this->table1[i] = new node;
			Tab2pntr = this->table1[i];
			Tab2pntr->key = Tab1pntr->key;
			Tab2pntr->value = Tab1pntr->value;
			Tab2pntr->link = nullptr;

			if(Tab1pntr->link != nullptr){
				Tab2pntr->link = new node;
				Tab1pntr = Tab1pntr->link;
				Tab2pntr = Tab2pntr->link;
				Tab2pntr->link = nullptr;
				Tab2pntr->key = Tab1pntr->key;
				Tab2pntr->value = Tab1pntr->value;
			}
		}


		Tab1pntr = map.table2[i];
		this->table2[i] = nullptr;
		if(Tab1pntr != nullptr){
			this->table2[i] = new node;
			Tab2pntr = this->table2[i];
			Tab2pntr->key = Tab1pntr->key;
			Tab2pntr->value = Tab1pntr->value;
			Tab2pntr->link = nullptr;

			if(Tab1pntr->link != nullptr){
				Tab2pntr->link = new node;
				Tab1pntr = Tab1pntr->link;
				Tab2pntr = Tab2pntr->link;
				Tab2pntr->link = nullptr;
				Tab2pntr->key = Tab1pntr->key;
				Tab2pntr->value = Tab1pntr->value;
			}
		}
	}

}
/* Description */
/*
* function_identifier: deallocateMap()
* parameters: none
* description: The deallocateMap function will go
* through of the the HashMap's tables, checking if 
* the current index has 0, 1, or 2 nodes. Once checked
* it will deallocate the appropriate amount of nodes within
* that index, then it will do the same with the second table
* once that it checked and completed the loop will continue onto
* the next index until there is no more indices. Then the 
* function will deallocate the tables completely.  
* return value: none 
*/
template <class t1, class t2>
void hashMap<t1, t2>::deallocateMap()
{
	node *Traversal, *Deletion;
	for(int i = 0; i < this->capacity; i++){

		Traversal = this->table1[i];
		if(Traversal != nullptr){
			if(Traversal->link != nullptr){
				Deletion = Traversal->link;
				delete Deletion;
			}
			Deletion = Traversal;
			delete Deletion;
		}

		Traversal = this->table2[i];
		if(Traversal != nullptr){
		if(Traversal->link !=nullptr){
			Deletion = Traversal->link;
			delete Deletion;
		}
		Deletion = Traversal;
		delete Deletion;
	}
	}
	delete [] this->table1;
	delete [] this->table2;
	this->items1 = 0;
	this->items2 = 0;
}
/* Description */
/*
* function_identifier: h1(const string& s) const
* parameters: const string& s
* description: The h1 function is the hash function meant for 
* table1's values, or to be used for double hashing. The function
* will take each of the strings characters and multiply them by 10^i
* and will deposit into the size_t Total. Once complete the function
* will return Total to be used for indexing.  
* return value: size_t Total
*/
template <class t1, class t2>
size_t hashMap<t1, t2>::h1(const string& s) const
{
	size_t Total = 0;
	for(int i = 0; i < s.length(); i++)
		Total += (pow(10, i) * s[i]); 
	return Total;
}
/* Description */
/*
* function_identifier: h2(const string& s) const
* parameters: const string& s
* description: The h2 function is the hash function meant for 
* table2's values, or to be used for double hashing. The function
* will take each of the string characters from end to 
* the beginning, skipping a few characters and multiply them by 10^i
* and will deposit into the size_t Total. Once complete the function
* will return Total to be used for indexing.  
* return value: size_t Total
*/
template <class t1, class t2>
size_t hashMap<t1, t2>::h2(const string& s) const
{
	size_t total = 0;
	for(int i = 0; i < s.length(); i++)
		total += (pow(10, i) * s[s.length() - 1 - i]);
	return total;
}