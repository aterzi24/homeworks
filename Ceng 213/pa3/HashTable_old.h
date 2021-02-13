#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"

// Do not modify the public interface of this class.
// Otherwise, your code will note compile!
template <class T>
class HashTable {
    struct Entry {
        std::string Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[3];
    };

    int _capacity; // INDICATES THE SIZE OF THE TABLE
    int _size; // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket* _table; // HASH TABLE

    // You can define private methods and variables

    int total_probes;

public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    HashTable(const HashTable<T>& rhs);
    HashTable<T>& operator=(const HashTable<T>& rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE.
    // IF LOAD FACTOR OF THE TABLE IS BIGGER THAN 0.5,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(std::string key, const T& value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, JUST RETURN FROM THE FUNCTION
    // HINT: YOU SHOULD UPDATE ACTIVE & DELETED FIELDS OF THE DELETED ENTRY.
    void Delete(std::string key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THIS FUNCTION MUST RETURN T()
    T Get(std::string key) const;

    // TODO: IMPLEMENT THIS FUNCTION.
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES FROM 0TH ENTRY TO LAST ENTRY
    void Resize(int newCapacity);

    // TODO: IMPLEMENT THIS FUNCTION.
    // RETURNS THE AVERAGE NUMBER OF PROBES FOR SUCCESSFUL SEARCH
    double getAvgSuccessfulProbe();

    // TODO: IMPLEMENT THIS FUNCTION.
    // RETURNS THE AVERAGE NUMBER OF PROBES FOR UNSUCCESSFUL SEARCH
    double getAvgUnsuccessfulProbe();

    // THE IMPLEMENTATION OF THESE FUNCTIONS ARE GIVEN TO YOU
    // DO NOT MODIFY!
    int Capacity() const;
    int Size() const;
};



template <class T>
HashTable<T>::HashTable() {
// TODO: CONSTRUCTOR
  _capacity = 0;
  _size = 0;
  total_probes = 0;
  _table = NULL;
}

template <class T>
HashTable<T>::HashTable(const HashTable<T>& rhs) {
// TODO: COPY CONSTRUCTOR
  _capacity = rhs._capacity;
  _size = rhs._size;
  total_probes = rhs.total_probes;
  _table = NULL;
  if(_capacity)
    _table = new Bucket[_capacity];
  for( int i = 0 ; i < _capacity ; i++ )
  {
      _table[i] = rhs._table[i];
  }
}

template <class T>
HashTable<T>& HashTable<T>::operator=(const HashTable<T>& rhs) {
// TODO: OPERATOR=
  HashTable<T> tmp(rhs);
  _capacity = tmp._capacity;
  _size = tmp._size;
  total_probes = tmp.total_probes;
  if(_table)
    delete[] _table;
  _table = tmp._table;
  return *this;
}

template <class T>
HashTable<T>::~HashTable() {
// TODO: DESTRUCTOR
  delete[] _table;
}

template <class T>
void HashTable<T>::Insert(std::string key, const T& value) {
// TODO: IMPLEMENT THIS FUNCTION.
// INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
// IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
// THE ALREADY EXISTING ONE. IF LOAD FACTOR OF THE TABLE IS BIGGER THAN 0.5,
// RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
  int h;
  if(_capacity == 0)
  {
    Resize(NextCapacity(0));
    h = Hash(key) % _capacity;
    _table[h].entries[0].Key = key;
    _table[h].entries[0].Value = value;
    _table[h].entries[0].Active = true;
    _size  = total_probes = 1;
    return;
  }
  h = Hash(key) % _capacity;
  int i = 1;
  int j;
  while(true)
  {
    for( j = 0 ; j < 3 ; j++ )
    {
      if(_table[h].entries[j].Active == false && _table[h].entries[j].Deleted == false)
        break;
      if(_table[h].entries[j].Active == true && _table[h].entries[j].Key == key)
      {
        _table[h].entries[j].Value = value;
        return;
      }
    }
    if(j != 3)
      break;
    h = (h + i) % _capacity;
    i += 2;
  }

  h = Hash(key) % _capacity;
  i = 1;

  while(true)
  {
    for( j = 0 ; j < 3 ; j++ )
    {
      if(_table[h].entries[j].Active == false)
      {
        _table[h].entries[j].Key = key;
        _table[h].entries[j].Value = value;
        _table[h].entries[j].Active = true;
        _table[h].entries[j].Deleted = false;
        _size++;
        total_probes++;
        break;
      }
    }
    if(j != 3)
      break;
    h = (h + i) % _capacity;
    i += 2;
    total_probes++;
  }

  if(_size / (3.0*_capacity) > 0.5)
    Resize(NextCapacity(_capacity));
}

template <class T>
void HashTable<T>::Delete(std::string key) {
// TODO: IMPLEMENT THIS FUNCTION.
// DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
// IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, JUST RETURN FROM THE FUNCTION
// HINT: YOU SHOULD UPDATE ACTIVE & DELETED FIELDS OF THE DELETED ENTRY.
  int h = Hash(key) % _capacity;
  int i = 1;
  int j;
  while(true)
  {
    for( j = 0 ; j < 3 ; j++ )
    {
      if(_table[h].entries[j].Active == false && _table[h].entries[j].Deleted == false)
        return;
      if(_table[h].entries[j].Active == true && _table[h].entries[j].Key == key)
      {
        _size--;
        _table[h].entries[j].Active = false;
        _table[h].entries[j].Deleted = true;
        return;
      }
    }
    h = (h + i) % _capacity;
    i += 2;
  }
}

template <class T>
T HashTable<T>::Get(std::string key) const {
// TODO: IMPLEMENT THIS FUNCTION. IT SHOULD RETURN THE VALUE THAT
// IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
// IF THE KEY DOES NOT EXIST, THIS FUNCTION MUST RETURN T()
  int h = Hash(key) % _capacity;
  int i = 1;
  int j;
  while(true)
  {
    for( j = 0 ; j < 3 ; j++ )
    {
      if(_table[h].entries[j].Active == false && _table[h].entries[j].Deleted == false)
        return T();
      if(_table[h].entries[j].Active == true && _table[h].entries[j].Key == key)
        return _table[h].entries[j].Value;
    }

    h = (h + i) % _capacity;
    i += 2;
  }
}

template <class T>
void HashTable<T>::Resize(int newCapacity) {
// TODO: IMPLEMENT THIS FUNCTION. AFTER THIS FUNCTION IS EXECUTED
// THE TABLE CAPACITY MUST BE EQUAL TO newCapacity AND ALL THE
// EXISTING ITEMS MUST BE REHASHED ACCORDING TO THIS NEW CAPACITY.
// WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES FROM 0TH ENTRY TO LAST ENTRY
  if(_capacity == 0)
  {
    _capacity = newCapacity;
    _table = new Bucket[_capacity];
    return;
  }
  int old_size;
  Bucket *old = _table;

  _table = new Bucket[newCapacity];
  _size = 0;
  total_probes = 0;
  old_size = _capacity;
  _capacity = newCapacity;

  for( int i = 0 ; i < old_size ; i++ )
    for( int j = 0 ; j < 3 ; j++ )
      if( old[i].entries[j].Active == true )
        Insert(old[i].entries[j].Key, old[i].entries[j].Value);

  delete[] old;
}


template <class T>
double HashTable<T>::getAvgSuccessfulProbe() {
// TODO: IMPLEMENT THIS FUNCTION.
// RETURNS THE AVERAGE NUMBER OF PROBES FOR SUCCESSFUL SEARCH
  return (_size == 0)?1:(double)total_probes/(double)_size;
}

template <class T>
double HashTable<T>::getAvgUnsuccessfulProbe() {
// TODO: IMPLEMENT THIS FUNCTION.
// RETURNS THE AVERAGE NUMBER OF PROBES FOR UNSUCCESSFUL SEARCH
  int total = 0;
  for (unsigned i = 0; i < _capacity; i++){
      int pr = 1;
      int h = i;

      while(true) {
        int j, k=1;
        for( j = 0 ; j < 3 ; j++ )
          if(_table[h].entries[j].Active == false)
            break;

        if(j != 3)
          break;

        pr++;
        h = (h + k) % _capacity;
        k += 2;
      }

      total += pr;
  }
  return (double)total / _capacity;
}

template <class T>
int HashTable<T>::Capacity() const {
    return _capacity;
}

template <class T>
int HashTable<T>::Size() const {
    return _size;
}

#endif
