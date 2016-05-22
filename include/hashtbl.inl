#ifndef _HASHTBL_HPP_
#define _HASHTBL_HPP_

#include <iostream>

#include "hashtbl.hpp"

namespace MyHashTable {

	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	HashTbl<KeyType, DataType, KeyHash, KeyEqual>::HashTbl ( int _initTableSize = DEFAULT_SIZE )
	{
		mSize = _initTableSize;
		mCount = 0;
	}

	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	HashTbl<KeyType, DataType, KeyHash, KeyEqual>::~HashTbl () 
	{
		for(unsigned long int i = 0; i < mSize; i++) {
			delete[] mpDataTable[i];
		}
	}

	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::insert ( const KeyType & Thekey, const DataType & TheData) throw ( std::bad_alloc ) {
		KeyHash TheClientHashFunction;

		unsigned long int ClientHashReturn = TheClientHashFunction(Thekey);
		unsigned long int ThePosition = ClientHashReturn % mSize;

		Entry NewTablePosition = mpDataTable[ThePosition];
		Entry LastTablePosition = mpDataTable[ThePosition].end();

		KeyEqual TheClientEqualFunction;

		for( /* Empty */ ;NewTablePosition != LastTablePosition; NewTablePosition++) {
			if(TheClientEqualFunction(/* COMO CHAMAR O DADO DE NewTablePosition */, Thekey)) {
				NewTablePosition = TheData;
				return false;
			}
		}

		Entry TheEntry<Thekey, TheData>;
		mpDataTable[ThePosition].push_back(Thekey);
        mCount++;
        
        return true;
	}

	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::remove ( const KeyType & Thekey) {
		KeyHash TheClientHashFunction;

		unsigned long int ClientHashReturn = TheClientHashFunction(Thekey);
		unsigned long int ThePosition = ClientHashReturn % mSize;

		Entry positionToErase = mpDataTable[ThePosition];
		Entry LastTablePosition = mpDataTable[ThePosition].end();

		KeyEqual TheClientEqualFunction;

		for( /* Empty */ ;positionToErase != LastTablePosition; positionToErase++) {
			if(TheClientEqualFunction(/* COMO CHAMAR O DADO DE positionToErase */, Thekey)) {
				positionToErase.erase(positionToErase);
				return true;
			}
		}
        
        return false;
    }

 //    template <typename KeyType, typename DataType, typename KeyHash  = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType> >
 //    bool HashTbl < KeyType, DataType, KeyHash, KeyEqual>::retrieve ( const KeyType &, DataType & ) const;

 //    template <typename KeyType, typename DataType, typename KeyHash  = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType> >
 //    void HashTbl < KeyType, DataType, KeyHash, KeyEqual>::clear ( void );

 //    template <typename KeyType, typename DataType, typename KeyHash  = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType> >
 //    bool HashTbl < KeyType, DataType, KeyHash, KeyEqual>::isEmpty ( void ) const;

 //    template <typename KeyType, typename DataType, typename KeyHash  = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType> >
 //    unsigned long int HashTbl < KeyType, DataType, KeyHash, KeyEqual>::count ( void ) const;

 //    template <typename KeyType, typename DataType, typename KeyHash  = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType> >
 //    void showStructure () const;

	// template <typename KeyType, typename DataType, typename KeyHash  = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType> >
 //    void  < KeyType, DataType, KeyHash, KeyEqual>rehash( void ) 
 //    {
 //    	unsigned long int newCapacity = mSize * 2;
 //    	std::list< Entry > * auxMpDataTable = new std::list< Entry >[newCapacity];
 //    	for(unsigned long int i = 0; i < mSize; i++) {
 //    		if(mpDataTable[i]->empty()) {
 //    			continue;
 //    		}
 //    		auto elem = mpDataTable[i].begin();

 //    	}
 //    }

}

#endif