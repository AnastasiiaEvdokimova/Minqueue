/*minqueue.h*/

//
// Min queue that stores (key, value) pairs using a min-heap 
// implementation.  When pop is called, the key from the 
// (key, value) pair with the smallest value is returned; if 
// two pairs have the same value, the smaller key is returned.
// Push and pop have O(lgN) time complexity.
//
// <<Anastasiia Evdokimova >>
//
// U. of Illinois, Chicago
//

#pragma once

#include <iostream>
#include <vector>
#include <exception>
#include <map>
#include <stdexcept>
#include <utility> 

using namespace std;


template<typename TKey, typename TValue>
class minqueue
{
private:

struct Data {
TKey key;
TValue value;
};

//vector will hold the actual tree
vector <Data> heap;
//map will keep track of each element's position in the vector
map <TKey, int> positionMap;
unsigned int size;

/*
 * Swap places of values stored by the indices i and j
 * and updates the position map
 * */
void swap(unsigned int index1, unsigned int index2)
{
	Data temp = heap[index1];
	heap[index1] = heap[index2];
	heap[index2] = temp;
	positionMap[heap[index1].key] = index1;
	positionMap[heap[index2].key] = index2;
}

/*
 * Returns the index of the element with the minimal value (or key, if values are equal)
 * */
int getIndexMin(unsigned int index1, unsigned int index2)
{
	//if there is no element at the index1
	if (index1 < 0 || index1 >= size || (positionMap.find(heap[index1].key) == positionMap.end()))
	{
	//if there is no element at the index2
		if (index2 < 0 || index2 >= size || (positionMap.find(heap[index1].key) == positionMap.end()))
			return -1; //there is no minimal element
		else
			return index2; 
	}
	//no element at index2
	if (index2 < 0 || index2 >= size || (positionMap.find(heap[index2].key) == positionMap.end()))
	{
		if (index1 < 0 || index1 >= size || (positionMap.find(heap[index1].key) == positionMap.end()))
			return -1; //there is no minimal element
		else
			return index1;
	}
	if ((heap[index1].value < heap[index2].value) ||
	((heap[index1].value == heap[index2].value && heap[index1].key < heap[index2].key)))
	{
		return index1;
	}
	else
	{
		return index2;
	}
}

/*
 * If the element at the index is smaller than it's parents, it should swap places with them
 * */
void slideUp(int index)
{
	int parentIndex = (index - 1)/2;
	//while the child's value is less than parent's or (if values are equal) it's key is less
	while (index > 0 && parentIndex >= 0 && getIndexMin(index, parentIndex) == index)
	{
		swap(index, parentIndex);
		index = parentIndex;
		parentIndex = (index - 1)/2;
	}
}

/*
 * If any of the children of the element at index are smaller, they should take its place
 * */
void slideDown(unsigned int index)
{
	//get the minimal child's index
	unsigned int leftChildIndex = index * 2 + 1;
	unsigned int rightChildIndex = index * 2 + 2;
	//find the minimal of the children
	int minChildIndex = getIndexMin(leftChildIndex, rightChildIndex);
	//while a minimal child is less than our element
	while (index < size && minChildIndex != -1 && getIndexMin(index, minChildIndex) == minChildIndex)
	{
		//slide it down
		swap(index, minChildIndex);
		index = minChildIndex;
		leftChildIndex = index * 2 + 1;
		rightChildIndex = index * 2 + 2;
		minChildIndex = getIndexMin(leftChildIndex, rightChildIndex);
	}
}

/*
 * Debug - prints the heap array
 * */
void printHeap()
{
		for (unsigned int i = 0; i < size; i++)
		{
			cout << heap[i].key << "-" << heap[i].value << " ";
		}
		cout << endl;
}


public:
  //
  // default constructor:
  //
  // Queue has a max capacity for efficient implementation.
  // This max capacity must be specified at queue creation.
  //
  minqueue(int capacity)
  {
    heap.resize(capacity);
		size = 0;
  }
	
	/*
	 * Copy constructor
	 * */
	minqueue(const minqueue &other)
	{
		heap = other.heap;
		positionMap = other.positionMap;
		size = other.size;
	}
  
  //
  // fill constructor:
  //
  // This allows for the efficient O(N) construction of
  // a queue with an initial set of keys, all with the same
  // initial value.  The max capacity of the queue is 
  // set to the # of keys provided for initialization;
  // it is assumed the keys are in ascending order.
  //
  minqueue(vector<TKey> keys, TValue initialValue)
  {
    heap.resize(keys.size());
		for (unsigned int i = 0; i < keys.size(); i++)
		{
			heap[i].key = keys[i];
			heap[i].value = initialValue;
			positionMap[keys[i]] = i;
		}
		size = keys.size();
  }
  
  
  //
  // destructor:
  //
  virtual ~minqueue()
  {
    //
    // TODO:
    //
  }
	
	/*
	 * Assigns a copy of the other queue
	 * */
  minqueue& operator=(const minqueue& other)
	{
		if (this != &other)
		{
			this->heap = other.heap;
			this->positionMap = other.positionMap;
			this->size = other.size;
		}
		return *this;
	}
	
  //
  // empty:
  //
  // Returns true if empty, false if not.
  //
  bool empty()
  {
	if (size == 0)
		return true;
	else
		return false;
  }


  //
  // push:
  //
  // Inserts the given (key, value) pair into the queue such that 
  // pop always returns the pair with the minimum value.  If the 
  // key is *already* in the queue, it's value is updated to the
  // given value and the queue reordered.  If the key is not in
  // the queue, the (key, value) pairs is added and the queue
  // reordered.
  //
  // NOTE: if two keys have the same value, i.e. (key1, value) and
  // (key2, value), then those pairs are ordered into ascending value
  // by their key.
  //
  void pushinorder(TKey key, TValue value)
  {
		auto it = positionMap.find(key);
		//if the key is already in the map, update the value
    if (it != positionMap.end())
		{
			heap[it->second].value = value;
			slideUp(it->second);
			slideDown(it->second);
			return;
		}
    
    //
    // we need to insert a new (key, value) pair but the queue is full:
    //
    if (size == heap.size())
    {
			heap.resize(size*2);
    }
		
			//write new data to the heap
		heap[size].key = key;
		heap[size].value = value;
		//add the new element to the index map
		positionMap[key] = size;
		//the positionMap will get updated if the element changes position, so it's used to track the element's index
		it = positionMap.find(key);
		size++;
		slideUp(it->second);
		slideDown(it->second);
  }


  //
  // front:
  //
  // Returns the key at the front of the queue; does *not* pop the 
  // (key, value) pair.  Throws a logic_error exception if the queue
  // is empty.
  //
  TKey minfront()
  {
    if (empty())
    {
      throw logic_error("minqueue::minfront: queue empty");
    }
    
		return heap[0].key;
  }
	
	/*
	 * Returns the value at the front of the queue
	 * */
	TValue minvalue()
  {
    if (empty())
    {
      throw logic_error("minqueue::minfront: queue empty");
    }
    
		return heap[0].value;
  }


  //
  // pop:
  //
  // Pops and discards the (key, value) pair at the front of the queue.
  // Throws a logic_error exception if the queue is empty.
  //
  void minpop()
  {
    if (empty())
    {
      throw logic_error("minqueue::minpop: queue empty");
    }
    positionMap.erase(heap[0].key);
		heap[0] = heap[size - 1];
		size--;
		slideDown(0);
  }
	
	/*
	 * Imitates priority queue behaviour, thus returns a pair of elements
	 * */
	pair<TKey, TValue> top()
  {
		pair<TKey, TValue> minElement; 
    if (empty())
    {
      throw logic_error("minqueue::minpop: queue empty");
    }  
    minElement.first = heap[0].key; 
    minElement.second = heap[0].value; 
		return minElement;
  }
	
	/*
	 * Imitates priority queue behaviour, pushes a pair of elements
	 * */
	void push(pair<TKey, TValue> newElement)
  {
		pushinorder(newElement.first, newElement.second);
  }
	
	/*
	 * Imitates priority queue behaviour, discards first element
	 * */
	void pop()
	{
		minpop();
	}
	
	/*
	 * Prints out the array holding the minqueue elements
	 * */
	void _dump()
	{
		cout << "Size: " << heap.size() << endl;
		printHeap();
	}

};