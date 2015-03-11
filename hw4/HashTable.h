        #ifndef _QUADRATIC_PROBING_H_
        #define _QUADRATIC_PROBING_H_
        
        #include "mystring.h"

        // QuadraticProbing Hash table class
        //
        // CONSTRUCTION: an initialization for ITEM_NOT_FOUND
        //               and an approximate initial size or default of 101
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // void remove( x )       --> Remove x
        // Hashable find( x )     --> Return item that matches x
        // void makeEmpty( )      --> Remove all items
        // int hash( String str, int tableSize )
        //                        --> Static method to hash strings

        template <class HashedObj>
        class QuadraticHashTable
        {
          public:
            explicit QuadraticHashTable( const HashedObj & notFound, int size = 101 );
            QuadraticHashTable( const QuadraticHashTable & rhs )
              : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
                array( rhs.array ), currentSize( rhs.currentSize ) { }

            void makeEmpty( );
            void insert( const HashedObj & x );
            const QuadraticHashTable & operator=( const QuadraticHashTable & rhs );
            enum EntryType { ACTIVE, EMPTY, DELETED };
            
            void remove( const HashedObj & x );

            const HashedObj & find( const Person &x ) const;
            const HashedObj & find(int uid ) const;
            int findPos( int uid ) const;
						~QuadraticHashTable();
						
          private:
            struct HashEntry
            {
                HashedObj element;
                EntryType info;

                HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
                  : element( e ), info( i ) { }
            };

            //vector<HashEntry> array;
            int AR_SIZE;
						HashEntry *array;
						
            int currentSize;
            const HashedObj ITEM_NOT_FOUND;
            bool isPrime( int n ) const;
            int nextPrime( int n ) const;
            bool isActive( int currentPos ) const;
            int hash( const string & key, int tableSize ) const;
            int hash( const Person & x, int tableSize ) const;
        };

        #include "HashTable.cpp"
        #endif
