#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <cstdlib>
#include <string>

template <class t1, class t2>
class hashMap
{
private:
	struct node
	{
		t1 key;
		t2 value;
		node * link;
	};

	std::size_t items1;
	std::size_t items2;

	std::size_t capacity;

	node ** table1;
	node ** table2;

	void resize(std::size_t);
	void copyMap(const hashMap<t1, t2>&);
	void deallocateMap();

	std::size_t h1(const std::string&) const;
	std::size_t h2(const std::string&) const;

public:
	hashMap() : items1(0), items2(0), capacity(5)
	{
		table1 = new node*[capacity];
		table2 = new node*[capacity];

		for (int i = 0; i < capacity; i++)
			table1[i] = table2[i] = nullptr;
	}
	
	const hashMap<t1, t2>& operator=(const hashMap<t1, t2>&);
	t2& operator[](t1);

	hashMap(const hashMap<t1, t2>& map) { copyMap(map); }
	~hashMap() { deallocateMap(); }
};



#endif