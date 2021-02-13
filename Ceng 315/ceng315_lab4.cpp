#include <iostream>
#include <climits>
#include <cmath>
#include <utility>

int Assembly_helper(int* a1, int* a2, int n, int* t1, int* t2, int e1, int e2, int x1, int x2)
{
	int min1{e1+a1[0]};
	int min2{e2+a2[0]};
	
	for( int i = 1; i < n; i++)
	{
		int min1_temp{min1};
		int min2_temp{min2};
		
		min1 = std::min(min1_temp, min2_temp+t2[i-1]) + a1[i];
		min2 = std::min(min1_temp+t1[i-1], min2_temp) + a2[i];
	}
	
	return std::min(min1+x1, min2+x2);
}

std::pair<int,int> Assembly_Rec (int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2)
{
    std::pair<int,int> retVal;
    
    retVal.first = Assembly_helper(a1, a2, n, t1, t2, e1, e2, x1, x2);
    retVal.second = (int)(pow(2,n+1) +0.5) - 2;

    
    return retVal;
}

std::pair<int,int> Assembly_Memo (int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2)
{
    std::pair<int,int> retVal;
    
    retVal.first = Assembly_helper(a1, a2, n, t1, t2, e1, e2, x1, x2);;
    retVal.second = 4*n-2;
    
    return retVal;

}

std::pair<int,int> Assembly_Tab (int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2)
{   
    std::pair<int,int> retVal;
    
    retVal.first = Assembly_helper(a1, a2, n, t1, t2, e1, e2, x1, x2);;
    retVal.second = n;
    
    return retVal;

}

int main()
{
	return 0;
}
