#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#include <ctime>

using namespace std;

constexpr int SIZE      = 1000000;
constexpr int MAX_VALUE = 1000000;

int fr[MAX_VALUE];

int* generateArray( int size )
{
    srand( time( nullptr ) );
    int* array = new int[size];
    for( int i = 0; i < size; ++i ) {
        ///array[i] = -i + 1000000; /// desc array (fara +const, nu merge pt count si radix (fara nr negative!)
        ///array[i] = 50; /// constant array
        ///array[i] = i; /// ascending array
        array[i] = rand() % MAX_VALUE;      
    }

    return array;
}


template <class T>
void bubbleSort( T array[], int size )
{
    while( true ) {
        bool swapped = false;
        for( int i = 0; i < size - 1; ++i ) {
            if( array[i] > array[i + 1] ) {
                swap( array[i], array[i + 1] );
                swapped = true;
            }
        }

        if( !swapped ) {
            break;
        }
    }
}

template <class T>
void countSort( T array[], int size )
{
    for( int i = 0; i < MAX_VALUE; ++i ) {
        fr[i] = 0;
    }

    int minval = INT_MAX, maxval = INT_MIN;
    for( int i = 0; i < size; ++i ) {
        ++fr[array[i]];
        minval = min( array[i], minval );
        maxval = max( array[i], maxval );
    }

    for( int cnt = 0, i = minval; i <= maxval; ++i ) {
        if( fr[i] ) {
            for( int j = 0; j < fr[i]; ++j ) {
                array[cnt++] = i;
            }
        }
    }
}

void radixCountSort( int array[], int size, int pt )
{
    for( int i = 0; i < 10; ++i ) {
        fr[i] = 0;
    }

    for( int i = 0; i < size; ++i ) {
        ++fr[( array[i] / pt ) % 10];
    }

    // compute position in sorted array
    for( int i = 1; i < 10; ++i ) {
        fr[i] += fr[i - 1];
    }

    int* output = new int[size];  // output array

    // backwards iteration to preserve
    // order of sorted elements in array
    for( int i = size - 1; i >= 0; --i ) {
        int digit             = ( array[i] / pt ) % 10;
        output[fr[digit] - 1] = array[i];
        --fr[digit];
    }

    for( int i = 0; i < size; ++i ) {
        array[i] = output[i];
    }

    delete[] output;
}

void radixSort( int array[], int size )
{
    // compute max number of digits
    int maxDigits = 0;
    for( int i = 0; i < size; ++i ) {
        int x  = array[i];
        int nr = 0;  // number of digits
        while( x ) {
            ++nr;
            x /= 10;
        }

        maxDigits = max( maxDigits, nr );
    }

    for( int i = 0, pt = 1; i < maxDigits; ++i, pt *= 10 ) {
        radixCountSort( array, size, pt );
    }
}

template <class T>
void mergeSort( T array[], int left, int right )
{
    if( left >= right ) {
        return;
    }

    int mid = ( left + right ) / 2;
    mergeSort( array, left, mid );
    mergeSort( array, mid + 1, right );

    int i = left, j = mid + 1, index = 0;
    T*  aux = new T[right - left + 1];
    while( i <= mid && j <= right ) {
        if( array[i] <= array[j] ) {
            aux[index++] = array[i++];
        }
        else {
            aux[index++] = array[j++];
        }
    }

    while( i <= mid ) {
        aux[index++] = array[i++];
    }

    while( j <= right ) {
        aux[index++] = array[j++];
    }

    for( int i = left; i <= right; ++i ) {
        array[i] = aux[i - left];
    }
}

template <class T>
int partition( T array[], int size )
{
    T   pivot = array[( size - 1 ) / 2];  // take the middle element
    int i = -1, j = size;
    while( 1 ) {
        // increment index i until we find first element that doesn't satisfy
        // ordering with pivot
        do {
            ++i;
        } while( array[i] < pivot );
        // decrement index j until we find first element that doesn't satisfy
        // ordering with pivot
        do {
            --j;
        } while( pivot < array[j] );

        if( i >= j )
            return j;  // position to partition from next time

        // swap the elements
        swap( array[i], array[j] );
    }
}

template <class T>
void qsortArray( T array[], int size )
{
    if( size > 1 ) {
        // partition the vector in two sequences and sort each one recursively
        int p = partition( array, size );
        qsortArray( array, p + 1 );
        qsortArray( array + p + 1, size - p - 1 );
    }
}

template <class T>
void printArray( const T array[], int size )
{
    for( int i = 0; i < size; ++i ) {
        cout << array[i] << ' ';
    }
    cout << '\n';
}

int main()
{
   
    int*       v      = generateArray( SIZE );
    int*       array  = new int[SIZE];
    clock_t  start;
    clock_t  finish;

    cout << fixed << setprecision( 4 );  // set 4 decimals for double printing

    // merge sort
    for( int i = 0; i < SIZE; ++i ) {
        array[i] = v[i];
    }
    start = clock();
    mergeSort( array, 0, SIZE - 1 );

    finish = clock();
    cout << "Time elapsed merge sort: "
         << double( finish - start ) / CLOCKS_PER_SEC
         << " seconds\n";

    // quick sort
    for( int i = 0; i < SIZE; ++i ) {
        array[i] = v[i];
    }
    start = clock();
    qsortArray( array, SIZE );
    finish = clock();
    cout << "Time elapsed quick sort: "
         << double( finish - start ) / CLOCKS_PER_SEC
         << " seconds\n";

    // count sort
    for( int i = 0; i < SIZE; ++i ) {
        array[i] = v[i];
    }
    start = clock();
    countSort( array, SIZE );
    finish = clock();
    cout << "Time elapsed count sort: "
         << double( finish - start ) / CLOCKS_PER_SEC
         << " seconds\n";

    // radix sort
    for( int i = 0; i < SIZE; ++i ) {
        array[i] = v[i];
    }
    start = clock();
    radixSort( array, SIZE );
    finish = clock();
    cout << "Time elapsed radix sort: "
         << double( finish - start ) / CLOCKS_PER_SEC
         << " seconds\n";

    // bubble sort
    for( int i = 0; i < SIZE; ++i ) {
        array[i] = v[i];
    }
     start = clock();
     bubbleSort( array, SIZE );
     finish = clock();
     cout << "Time elapsed bubble sort: "
          << double( finish - start ) / CLOCKS_PER_SEC 
          << " seconds\n";

    delete[] array;
    delete[] v;
}
