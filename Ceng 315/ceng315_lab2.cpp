#include <iostream>
#include <climits>
#include <cmath>

#define WAYa

// do not add extra libraries here


/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    size      : size of arr
    ascending : true for ascending, false for descending 
    
    you can use ceil function from cmath
    
*/

int QuickSort (int*& arr, int size, bool ascending)
{
	if(size <= 1)
		return 0;
	
    int pivot = arr[size-1];
    int k = 0;
    int numOfComp = 0;
    
    if(ascending)
    {
    	for( int i = 0 ; i < size-1 ; i++ )
	    {
	    	if(arr[i] < pivot)
	    		std::swap(arr[k++], arr[i]);
	    		
	    	++numOfComp;
		}
	}
	else
	{
		for( int i = 0 ; i < size-1 ; i++ )
	    {
	    	if(arr[i] > pivot)
	    		std::swap(arr[k++], arr[i]);
	    		
	    	++numOfComp;
		}
	}
	std::swap(arr[size-1], arr[k]);
	
	numOfComp += QuickSort(arr, k, ascending);
	int *newArr = arr + (k + 1);
	numOfComp += QuickSort(newArr, size-k-1, ascending);
	
	return numOfComp;
}

int QuickSort3 (int*& arr, int size, bool ascending)
{
    if(size <= 1)
    	return 0;
    
    int pivot = arr[size-1];
    int i = 0;
    int k = 0;
    int p = size-1;
    int numOfComp = 0;
    
    if(ascending)
    {
    	while(i < p)
	    {
	    	if(arr[i] < pivot)
	    	{
	    		std::swap(arr[i++], arr[k++]);
	    		++numOfComp;
	    	}
	    	else if(arr[i] == pivot)
	    	{
	    		std::swap(arr[i], arr[--p]);
	    		numOfComp += 2;
			}
			else
			{
				++i;
				numOfComp += 2;
			}
		}
	}
	else
	{
		while(i < p)
	    {
	    	if(arr[i] > pivot)
	    	{
	    		std::swap(arr[i++], arr[k++]);
	    		++numOfComp;
	    	}
	    	else if(arr[i] == pivot)
	    	{
	    		std::swap(arr[i], arr[--p]);
	    		numOfComp += 2;
			}
			else
			{
				++i;
				numOfComp += 2;
			}
		}
	}
	
	int m = std::min(p-k, size-p);
	for( i = 0 ; i < m ; i++ )
	{
		std::swap(arr[k+i], arr[size-m+i]);
	}
	
	numOfComp += QuickSort3(arr, k, ascending);
	int *newArr = arr + (size-p+k);
	numOfComp += QuickSort3(newArr, p-k, ascending);
	
	return numOfComp;
}

int main()
{
	#ifndef WAY
	int array1[] = { -12, -1, 6, 13 };
	int size1 = 4;
	
	int array2[] = { 17, 2, -15, -17, 5, -7 };
	int size2 = 6;
	
	int array3[] = { 1, -11, -6, -10, -11, 1, -6, -16 };
	int size3 = 8;
	
	int array4[] = {-3, 1, -11, 1, 3, 0, -12, -3, 4, -12, -3, -7, 5, 0, 5, -6, 6, 4, -4, 4, -11, 0, -12, -8, -3, -8, -1, -2};
	int size4 = 28;
	
	int array5[] = {2,9,16,23,30,37,44,51,39,27,15,3,48,93,138,183,511,412,321,126,269,1502,111,365,8715,2165950,333333,11};
	int size5 = 28;
	
	int *arr = array1;
	std::cout << QuickSort(arr, size1, true) << '\n';
	for(int i = 0; i < size1 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	
	arr = array2;
	std::cout << QuickSort(arr, size2, true) << '\n';
	for(int i = 0; i < size2 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	
	arr = array3;
	std::cout << QuickSort(arr, size3, false) << '\n';
	for(int i = 0; i < size3 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	
	arr = array4;
	std::cout << QuickSort(arr, size4, true) << '\n';
	for(int i = 0; i < size4 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	
	arr = array5;
	std::cout << QuickSort3(arr, size5, false) << '\n';
	for(int i = 0; i < size5 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	#endif
	
	#ifdef WAY
	int array1[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2,};
	int size1 = 10;
	
	int array2[] = { 0, 8, 8, -4, 8, -4, 8, -4, -4, -4, -4 };
	int size2 = 11;
	
	int array3[] = { 4, 3, 4, 4, 4, 0, 4, 3, 4, 3, 3, 3, 4, 0, 4, };
	int size3 = 15;
	
	int *arr = array1;
	std::cout << QuickSort3(arr, size1, true) << '\n';
	for(int i = 0; i < size1 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	
	arr = array2;
	std::cout << QuickSort3(arr, size2, true) << '\n';
	for(int i = 0; i < size2 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	
	arr = array3;
	std::cout << QuickSort3(arr, size3, false) << '\n';
	for(int i = 0; i < size3 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	#endif
}
