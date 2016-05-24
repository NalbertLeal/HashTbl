/*
*	@file
*	@brief This file have the class HashTbl implementation.
*/
#ifndef _HASHTBL_HPP_
#define _HASHTBL_HPP_

#include <iostream>
#include <memory>

#include "hashtbl.hpp"

namespace MyHashTable {

	/*!
	*	@brief		This function is the constructor of the class HashTbl. Here mSize, mpDataTable and mCount receive a value.
	*/
	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	HashTbl<KeyType, DataType, KeyHash, KeyEqual>::HashTbl ( int _initTableSize = DEFAULT_SIZE )
	{
		this->mSize = this->thePrime(_initTableSize);
		this->mpDataTable = new std::list< Entry >[this->mSize];
		this->mCount = 0;
	}

	/*!
	*	@brief		This function is the destructor of the class HashTbl. Here the function clear() of HashTbl is called.
	*/
	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	HashTbl<KeyType, DataType, KeyHash, KeyEqual>::~HashTbl () 
	{
		this->clear();
	}

	/*!
	*	@brief		The function insert push a element to the hash.
	*	@param		TheKey 		This parameter receive the key of the data.
	*	@param		TheData		this parameter receive the data to be inserted in the hash.
	*	@return 	The return of this function is true if inserted the element and false in any other case.
	*/
	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::insert ( const KeyType & Thekey, const DataType & TheData) throw ( std::bad_alloc ) {
		KeyHash TheClientHashFunction;

		if(mSize <= mCount) {
			this->rehash();
		}

		unsigned long int ClientHashReturn = TheClientHashFunction(Thekey);
		unsigned long int ThePosition = ClientHashReturn % mSize;

		auto NewTablePosition = mpDataTable[ThePosition].begin();
		auto LastTablePosition = mpDataTable[ThePosition].end();

		KeyEqual TheClientEqualFunction;

		for( /* Empty */ ; NewTablePosition != LastTablePosition; NewTablePosition++) {
			if(TheClientEqualFunction(NewTablePosition->mKey, Thekey)) {
				NewTablePosition->mData = TheData;
				return false;
			}
		}

		if(mSize <= mCount) {
			rehash();
			NewTablePosition = mpDataTable[ThePosition].begin();
			LastTablePosition = mpDataTable[ThePosition].end();
		}

		Entry TheEntry(Thekey, TheData);
		this->mpDataTable[ThePosition].push_back(TheEntry);
        mCount++;
        
        return true;
	}

	/*!
	*	@brief		The function remove erase the element passed on the parameter TheSearchKey.
	*	@param		TheSearchKey 		This parameter receive the key of the element to remove.
	*	@return 	The return of this function is true if erased the element and false in any other case.
	*/
	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::remove ( const KeyType & TheSearchKey) {
		KeyHash TheClientHashFunction;

		unsigned long int ClientHashReturn = TheClientHashFunction(TheSearchKey);
		unsigned long int ThePosition = ClientHashReturn % mSize;

		auto positionToErase = mpDataTable[ThePosition].begin();
		auto LastTablePosition = mpDataTable[ThePosition].end();

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

    /*!
	*	@brief		The function insert push a element to the hash.
	*	@param		TheSearchKey 	This parameter receive the key of the data to search.
	*	@param		TheData			this parameter receive the data.
	*	@return 	The return of this function is true if the element was retrieved and false in any other case.
	*/
    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    bool HashTbl < KeyType, DataType, KeyHash, KeyEqual>::retrieve ( const KeyType & TheSearchKey, DataType & TheData) const {
    	KeyHash TheClientHashFunction;

    	unsigned long int ClientHashReturn = TheClientHashFunction(TheSearchKey);
		unsigned long int ThePosition = ClientHashReturn % mSize;

		auto positionOnBegin = mpDataTable[ThePosition].begin();
		auto LastTablePosition = mpDataTable[ThePosition].end();

		for( /* Empty */ ;positionOnBegin != LastTablePosition; positionOnBegin++) {
			if(TheClientEqualFunction(positionOnBegin->mKey, TheSearchKey) ) {
				TheData = positionOnBegin->mData;
				this->mCount--;
				return true;
			}
		}

    	return false;
    }

    /*!
	*	@brief		The function clear remove every element of the hash.
	*	@return 	This function don't have return.
	*/
    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    void HashTbl < KeyType, DataType, KeyHash, KeyEqual>::clear ( void ) {
    	for(unsigned long int i = 0; i < this->mCount; i++) {
    		this->mpDataTable[i].~list();
    	}
    	this->mCount = 0;
    }

    /*!
	*	@brief		The function isEmpty return true or false. It depends if the hash is empty or not.
	*	@return 	This function return true if this hash don't have any element and false in any other case.
	*/
    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    bool HashTbl < KeyType, DataType, KeyHash, KeyEqual>::isEmpty ( void ) const {
    	return 0 == this->mCount;
    }

    /*!
	*	@brief		The function count return the value of mCount of this hash.
	*	@return 	This function return the value of mCount of this hash.
	*/
    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    unsigned long int HashTbl < KeyType, DataType, KeyHash, KeyEqual>::count ( void ) const {
    	return this->mCount;
    }

    /*!
	*	@brief		The function showStructure print this hash on the console.
	*	@return 	This function don't have a return.
	*/
    template <class KeyType, class DataType, class KeyHash, class KeyEqual>
    void HashTbl < KeyType, DataType, KeyHash, KeyEqual>::showStructure () const {
    	KeyHash TheClientHashFunction;

    	for( auto i = 0; i < this->mSize; ++i) {
			std::cout << i << " :{ key = ";
			for(auto &k : this->mpDataTable[i]) {
				std::cout << TheClientHashFunction(k.mKey) << " ; " << k.mData << " ";
			}
			std::cout << "}\n";
		}
    }

    /*!
	*	@brief		The function rehash multiply by two the capacity of the hash.
	*	@return 	This function don't have a return.
	*/
	template <class KeyType, class DataType, class KeyHash, class KeyEqual>
	void  HashTbl < KeyType, DataType, KeyHash, KeyEqual>::rehash( void ) 
    {
    	unsigned long int newCapacity = this->thePrime(2 * mSize);
    	std::list< Entry > * auxMpDataTable = new std::list< Entry >[newCapacity];
    	KeyHash TheClientHashFunction; 
    	for(unsigned long int i = 0; i < mSize; i++) {
    		if(this->mpDataTable[i].empty()) {
    			continue;
    		}
    		auto elem = mpDataTable[i].begin();

    		for( /* Empty */; elem != mpDataTable[i].end(); elem++) {
    			Entry aux(elem->mKey, elem->mData);

				unsigned long int hashf = TheClientHashFunction(aux.mKey);
    			unsigned long int end(hashf % newCapacity);

    			auxMpDataTable[end].push_back(aux);
    		}
    	}
    	this->~HashTbl();
    	mpDataTable = auxMpDataTable;
    	mSize = newCapacity;
    }

    /*!
	*	@brief		The function thePrime find and return the next prime number.
	*	@param 		TheCapacity 	recive the number that the function going to start to find a prime.
	*	@return 	This function return the next prime number.
	*/
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