        #include <iostream>
        #include "QuadraticProbing.h"

        using namespace std;

        /**
         * Internal method to test if a positive number is prime.
         * Not an efficient algorithm.
         */
        template <class HashedObj>
        bool QuadraticHashTable<HashedObj>::isPrime( int n ) const
        {
            if( n == 2 || n == 3 )
                return true;

            if( n == 1 || n % 2 == 0 )
                return false;

            for( int i = 3; i * i <= n; i += 2 )
                if( n % i == 0 )
                    return false;

            return true;
        }

        /**
         * Internal method to return a prime number at least as large as n.
         * Assumes n > 0.
         */
        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::nextPrime( int n ) const
        {
            return n;
            if( n % 2 == 0 ) n++;

            for( ; !isPrime( n ); n += 2 );
            
            return n;
        }

        /**
         * Construct the hash table.
         */
        template <class HashedObj>
        QuadraticHashTable<HashedObj>::QuadraticHashTable( const HashedObj & notFound, int size )
          : AR_SIZE( nextPrime( size ) ), ITEM_NOT_FOUND( notFound )
        {
					array = new HashEntry[AR_SIZE];
          makeEmpty( );
        }

				template <class HashedObj>
        QuadraticHashTable<HashedObj>::~QuadraticHashTable() {
					delete(array);
				}
         
        
        /**
         * Make the hash table logically empty.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::makeEmpty( )
        {
            currentSize = 0;
            for( int i = 0; i < AR_SIZE; i++ ) {
							array[ i ].info = EMPTY;
							array[ i ].element.uid = 0;
					}
        }

        /**
         * Deep copy.
         */
        template <class HashedObj>
        const QuadraticHashTable<HashedObj> & QuadraticHashTable<HashedObj>::
        operator=( const QuadraticHashTable<HashedObj> & rhs )
        {
            if( this != &rhs )
            {
                array = rhs.array;
                currentSize = rhs.currentSize;
            }
            return *this;
        }


        /**
         * Return true if currentPos exists and is active.
         */
        template <class HashedObj>
        bool QuadraticHashTable<HashedObj>::isActive( int currentPos ) const
        {
            return array[ currentPos ].info == ACTIVE;
        }

        /**
         * A hash routine for string objects.
         */
        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::hash( const string & key, int tableSize ) const
        {
            int hashVal = 0;

            for( int i = 0; i < key.length( ); i++ )
                hashVal = 37 * hashVal + key[ i ];

            hashVal %= tableSize;
            if( hashVal < 0 )
                hashVal += tableSize;

            return hashVal;
        }
        
        /**
         * Insert item x into the hash table. If the item is
         * already present, then do nothing.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::insert(const HashedObj & x )
        {
            // Insert x as active
            int currentPos = findPos( x.uid );
            array[ currentPos ] = HashEntry( x, ACTIVE );
        }


        /**
         * Generate unique ID
         */
         template <class HashedObj>
        int QuadraticHashTable<HashedObj>::hash( const Person & p, int tableSize ) const
        {
						int seed = 131, uid = 0;
						
						for(unsigned int i = 0; i < strlen(p.firstName); i++) uid = uid * seed + p.firstName[i];
						for(unsigned int i = 0; i < strlen(p.lastName); i++) uid = uid * seed + p.lastName[i];
						uid = uid * seed + p.year;
						uid = uid * seed + p.gender;

            if( uid < 0 ) uid = -uid;
            return uid ;
        }
        
        /**
         * Find item x in the hash table.
         * Return the matching item, or ITEM_NOT_FOUND, if not found.
         */
        template <class HashedObj>
        const HashedObj & QuadraticHashTable<HashedObj>::find( const Person & x ) const
        {
            int currentPos = findPos( hash(x, AR_SIZE) );
            return array[ currentPos ].element;
        }
        
                
        /**
         * Remove item x from the hash table.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::remove(const HashedObj & x)
        {
            int currentPos = findPos( x.person );
            if( isActive( currentPos ) )
                array[ currentPos ].info = DELETED;
        }
        
        /**
         * Method that performs quadratic probing resolution.
         * Return the position where the search for x terminates.
         */
        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::findPos(int uid) const
        {
            int collisionNum = 0;
            int currentPos = uid % AR_SIZE;
            
            HashedObj node = array[ currentPos ].element;

            while( array[ currentPos ].info != EMPTY && node.uid != uid) {
              currentPos += 2 * ++collisionNum - 1;  // Compute ith probe
              if( currentPos >= AR_SIZE ) {

              	currentPos -= AR_SIZE;
              }

              node = array[currentPos].element;
            }
            
          return currentPos;
        }
        

				template <class HashedObj>
        const HashedObj & QuadraticHashTable<HashedObj>::find( int uid ) const
        {
            int currentPos = findPos( uid );
            return array[ currentPos ].element;
        }
