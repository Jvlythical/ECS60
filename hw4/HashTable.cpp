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
//
        /**
         * Construct the hash table.
         */
        template <class HashedObj>
        QuadraticHashTable<HashedObj>::QuadraticHashTable( const HashedObj & notFound, int size )
          : array( nextPrime( size ) ), ITEM_NOT_FOUND( notFound )
        {
            makeEmpty( );
        }
        
        /**
         * Make the hash table logically empty.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::makeEmpty( )
        {
            currentSize = 0;
            for( int i = 0; i < array.size( ); i++ )
                array[ i ].info = EMPTY;
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
            int currentPos = findPos( x.person );
            array[ currentPos ] = HashEntry( x, ACTIVE );

            // Rehash; see Section 5.5
            if( ++currentSize > array.size( ) / 2 ) 
                rehash( );
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
        
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::printAll() {
            cout << "Size: " << array.size() << endl;
            for(int i = 0; i < array.size(); i++) {
                cout << array[i].element << endl;
            }
        }
        
        /**
         * Find item x in the hash table.
         * Return the matching item, or ITEM_NOT_FOUND, if not found.
         */
        template <class HashedObj>
        const HashedObj & QuadraticHashTable<HashedObj>::find( const Person & x ) const
        {
            int currentPos = findPos( x );

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
        int QuadraticHashTable<HashedObj>::findPos(const Person &x) const
        {
            int collisionNum = 0;
            int uid = hash( x, array.size( ) );
            int currentPos = uid % array.size();
            
            HashedObj node = array[ currentPos ].element;

            while( array[ currentPos ].info != EMPTY && node.uid != uid && 
            	strcmp(node.person.firstName, x.firstName) == 0 &&  strcmp(node.person.lastName, x.lastName) == 0
            ) {
              currentPos += 2 * ++collisionNum - 1;  // Compute ith probe
              if( currentPos >= array.size( ) ) {

              	currentPos -= array.size( );
              }

              node = array[currentPos].element;
            }
            
          return currentPos;
        }
        
/* ==========================
 *  Expand the hash table.
 * ==========================
 */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::rehash( )
        {
            vector<HashEntry> oldArray = array;

            // Create new double-sized, empty table
            array.resize( nextPrime( 2 * oldArray.size( ) ) );
            for( int j = 0; j < array.size( ); j++ )
                array[ j ].info = EMPTY;

            // Copy table over
            currentSize = 0;
            for( int i = 0; i < oldArray.size( ); i++ ) {
                HashedObj node = oldArray[ i ].element;
                if( oldArray[ i ].info == ACTIVE )
                    insert( node);
            }
        }
