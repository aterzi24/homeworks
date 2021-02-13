#include <iostream>
#include <climits>
#include <cmath>
#include <string>

void print(long arr[], int n);

int value(long number, long division, int modulus)
{
	return (number/division) % modulus;
}

void CountingSort(long arr[], bool ascending, int n, long division, int modulus)
{
	int *c = new int[modulus]();
	long *b = new long[n];
	
	int i;
	
	for( i = 0 ; i < n ; i++ )	
		++c[value(arr[i], division, modulus)];
	
	if(ascending)
	{
		for( i = 1 ; i < modulus ; i++ )
			c[i] += c[i-1];	
	}
	else
	{
		for( i = modulus-2 ; i >= 0 ; i-- )
			c[i] += c[i+1];
	}
	
	for( i = n-1 ; i >= 0 ; i-- )
	{
		b[c[value(arr[i], division, modulus)]-1] = arr[i];
		--c[value(arr[i], division, modulus)];
	}
		
	for( i = 0 ; i < n ; i++ )
		arr[i] = b[i];
		
	delete b;
	delete c;
}

int RadixSort(long arr[], bool ascending, int n, int l )
{
	int loopCount = 12 / l;
	int modulus = (int)(pow(10,l) +0.5);
	long division = 1;
	
	for( int i = 0 ; i < loopCount ; i++ )
	{	
		CountingSort(arr, ascending, n, division, modulus);
		division *= modulus;
	}
	
	return loopCount*(3*n+modulus-1);
}

void print(long arr[], int n)
{
	for(int i = 0; i < n ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
}

int main()
{
	long array1[] = { 454, 435, 8, 23, 68 };
	
	long array2[] = { 454, 435, 8, 23, 68 };
	
	long array3[] = { 454, 435, 8, 23, 68 };
	
	long array4[] = { 551989238, 396808766, 113775846, 186707985, 444194547 };
	
	long array5[] = { 551989238, 396808766, 113775846, 186707985, 444194547 };
	
	int size1 = 5;
	
	std::cout << RadixSort(array1, false, size1, 1) << '\n';
	print(array1, size1);
	
	std::cout << RadixSort(array2, false, size1, 2) << '\n';
	print(array2, size1);
	
	std::cout << RadixSort(array3, false, size1, 4) << '\n';
	print(array3, size1);
	
	std::cout << RadixSort(array4, false, size1, 4) << '\n';
	print(array4, size1);
	
	std::cout << RadixSort(array5, false, size1, 6) << '\n';
	print(array5, size1);
	return 0;
}
