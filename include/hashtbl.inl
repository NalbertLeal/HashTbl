#ifndef _HASHTBL_HPP_
#define _HASHTBL_HPP_

#include <iostream>
#include <memory>

#include "hashtbl.hpp"

namespace MyHashTable {

	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	HashTbl<KeyType, DataType, KeyHash, KeyEqual>::HashTbl ( int _initTableSize = DEFAULT_SIZE )
	{
		this->mSize = this->thePrime(_initTableSize);
		this->mpDataTable = new std::list< Entry >[this->mSize];
		this->mCount = 0;
	}

	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	HashTbl<KeyType, DataType, KeyHash, KeyEqual>::~HashTbl () 
	{
		this->clear();
	}

	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::insert ( const KeyType & Thekey, const DataType & TheData) throw ( std::bad_alloc ) {
		KeyHash TheClientHashFunction;

		if(mSize <= mCount) {
			this->rehash();
		}

		unsigned long int ClientHashReturn = TheClientHashFunction(Thekey);
		unsigned long int ThePosition = ClientHashReturn % mSize;

		Entry NewTablePosition = mpDataTable[ThePosition].begin();
		Entry LastTablePosition = mpDataTable[ThePosition].end();

		KeyEqual TheClientEqualFunction;

		for( /* Empty */ ; NewTablePosition != LastTablePosition; NewTablePosition++) {
			if(TheClientEqualFunction(NewTablePosition->mKey, Thekey)) {
				NewTablePosition->mData = TheData;
				return false;
			}
		}

		Entry TheEntry(Thekey, TheData);
		mpDataTable[ThePosition].push_back(Thekey);
        mCount++;
        
        return true;
	}

	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::remove ( const KeyType & TheSearchKey) {
		KeyHash TheClientHashFunction;

		unsigned long int ClientHashReturn = TheClientHashFunction(TheSearchKey);
		unsigned long int ThePosition = ClientHashReturn % mSize;

		Entry positionToErase = mpDataTable[ThePosition].begin();
		Entry LastTablePosition = mpDataTable[ThePosition].end();

		KeyEqual TheClientEqualFunction;

		for( /* Empty */ ;positionToErase != LastTablePosition; positionToErase++) {
			if(TheClientEqualFunction(positionToErase->mKey, TheSearchKey) ) {
				mpDataTable[ThePosition].erase(positionToErase);
				this->mCount--;
				return true;
			}
		}
        
        return false;
    }

    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    bool HashTbl < KeyType, DataType, KeyHash, KeyEqual>::retrieve ( const KeyType & TheSearchKey, DataType & TheData) const {
    	KeyHash TheClientHashFunction;

    	unsigned long int ClientHashReturn = TheClientHashFunction(TheSearchKey);
		unsigned long int ThePosition = ClientHashReturn % mSize;

		Entry positionOnBegin = mpDataTable[ThePosition].begin();
		Entry LastTablePosition = mpDataTable[ThePosition].end();

		for( /* Empty */ ;positionOnBegin != LastTablePosition; positionOnBegin++) {
			if(TheClientEqualFunction(positionOnBegin->mKey, TheSearchKey) ) {
				TheData = positionOnBegin->mData;
				this->mCount--;
				return true;
			}
		}

    	return false;
    }

    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    void HashTbl < KeyType, DataType, KeyHash, KeyEqual>::clear ( void ) {
    	for(unsigned long int i = 0; i < this->mCount; i++) {
    		this->mpDataTable[i].~list();
    	}
    	this->mCount = 0;
    }

    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    bool HashTbl < KeyType, DataType, KeyHash, KeyEqual>::isEmpty ( void ) const {
    	return 0 == this->mCount;
    }

    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    unsigned long int HashTbl < KeyType, DataType, KeyHash, KeyEqual>::count ( void ) const {
    	return this->mCount;
    }

    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    void HashTbl < KeyType, DataType, KeyHash, KeyEqual>::showStructure () const {
    	KeyHash TheClientHashFunction;

    	for( auto i = 0; i < this->mSize; ++i) {
			std::cout << i << " :{ key = ";
			for(auto &k : this->mSizempDataTable[i]) {
				std::cout << TheClientHashFunction(k.mKey) << " ; " << k.mData << " ";
			}
			std::cout << "}\n";
		}
    }

	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	void  HashTbl < KeyType, DataType, KeyHash, KeyEqual>::rehash( void ) 
    {
    	unsigned long int newCapacity = this->thePrime(2 * mSize);
    	std::list< Entry > * auxMpDataTable = new std::list< Entry >[newCapacity];
    	KeyHash TheClientHashFunction; 
    	for(unsigned long int i = 0; i < mSize; i++) {
    		if(mpDataTable[i]->empty()) {
    			continue;
    		}
    		Entry elem = mpDataTable[i].begin();

    		for( /* Empty */; elem != mpDataTable[i].end(); elem++) {
    			Entry aux(elem->mKey, elem->mData);

				unsigned long int ThePosition = TheClientHashFunction % mSize;
    			unsigned long int end(ThePosition % newCapacity);

    			auxMpDataTable[end].push_back(aux);
    		}
    	}
    	this->~HashTbl();
    	mpDataTable = auxMpDataTable;
    	mSize = newCapacity;
    }

    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	unsigned long int HashTbl<KeyType, DataType, KeyHash, KeyEqual>::thePrime(int theCapacity) {
		int count = 0, i, e;
		for(i = theCapacity; 1 < i && theCapacity <= i; i++) {
			for(e = i - 1; 1 < e; e--) {
				if(count == (theCapacity - 3) ) {
					return i;
				}
				count++;
			}			
		}
	}


}

#endif