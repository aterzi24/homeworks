#include <iostream>

int MergeSort (int*& arr, int size, bool ascending)
{
    if(size == 1)
		return 0;
		
	if(size == 2)
	{
	    if(ascending)
	    {
	        if(arr[0] > arr[1])
			    std::swap(arr[0], arr[1]);
	    }
	    else
	    {
	        if(arr[0] < arr[1])
	            std::swap(arr[0], arr[1]);
	    }
		
		return 1;
	}
	
	int noOfComp = 0;
	int middle = (size+1) / 2;
	noOfComp += MergeSort(arr, middle, ascending);
	
	int *halfArray = arr+middle;
	noOfComp += MergeSort(halfArray, size-middle, ascending);
	
	int index1 = 0;
	int index2 = middle;
	int index3 = 0;
	
	int *newArray = new int[size];
	
	if(ascending)
	{
	    while(index1 < middle && index2 < size)
    	{
    		if(arr[index1] < arr[index2])
    			newArray[index3++] = arr[index1++];
    		else
    			newArray[index3++] = arr[index2++];
    		++noOfComp;
    	}
	
    	while(index1 < middle)
    		newArray[index3++] = arr[index1++];
    	
    	while(index2 < size)
    		newArray[index3++] = arr[index2++];
	}
	else
	{
		while(index1 < middle && index2 < size)
    	{
    		if(arr[index1] > arr[index2])
    			newArray[index3++] = arr[index1++];
    		else
    			newArray[index3++] = arr[index2++];
    		++noOfComp;
    	}
	
    	while(index1 < middle)
    		newArray[index3++] = arr[index1++];
    	
    	while(index2 < size)
    		newArray[index3++] = arr[index2++];
	}
	
	for( int i = 0 ; i < size ; i++ )
		arr[i] = newArray[i];
	
	delete[] newArray;  
	
	return noOfComp;
}

int FunkySort (int*& arr, int size, bool ascending)
{
    if(size == 1)
    	return 0;
    	
    if(ascending)
    {
        if(arr[0] > arr[size-1])
		    std::swap(arr[0], arr[size-1]);
    }
    else
    {
        if(arr[0] < arr[size-1])
            std::swap(arr[0], arr[size-1]);
    }
    
    if(size == 2)
		return 1;
		
	int noOfComp = 1;
	int splitNum = (2*size+2) / 3;
	noOfComp += FunkySort(arr, splitNum, ascending);
	
	int *subArray = arr+(size-splitNum);
	noOfComp += FunkySort(subArray, splitNum, ascending);
	
	noOfComp += FunkySort(arr, splitNum, ascending);
	
	return noOfComp;
}

int main()
{
	int array1[] = { 3, 2, 7, 10 };
	int size1 = 4;
	
	int array2[] = { -11, 2, 6, 7, -1, 5 };
	int size2 = 6;
	
	int array3[] = { 7, 1, -2, 5, 17, 8, -5, 3, 2, -3, 0 };
	int size3 = 11;
	
	int *arr = array1;
	//std::cout << MergeSort(arr, size1, false) << '\n';
	std::cout << FunkySort(arr, size1, false) << '\n';
	for(int i = 0; i < size1 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	
	arr = array2;
	//std::cout << MergeSort(arr, size2, true) << '\n';
	std::cout << FunkySort(arr, size2, true) << '\n';
	for(int i = 0; i < size2 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
	
	arr = array3;
	//std::cout << MergeSort(arr, size3, true) << '\n';
	std::cout << FunkySort(arr, size3, true) << '\n';
	for(int i = 0; i < size3 ; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
}
