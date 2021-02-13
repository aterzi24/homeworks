#include <iostream>
#include <climits>
#include <cmath>
#include <utility>
#include <vector>
#include <stack>
#include <bits/stdc++.h>

using namespace std;

bool containQuestionMark(char pat[])
{
	for (int i = 0; pat[i]; i++)
		if (pat[i] == '?')
			return true;
	return false;
}

int findNextState(char pat[], int patSize, int state, char ch)
{
	if (state < patSize && pat[state] == ch)
		return state+1;
		
	int i, j;
	
	for (i = state; i > 0; i--)
	{
		if (pat[i-1] == ch)
		{
			for (j = 0; j < i-1; j++)
				if (pat[j] != pat[state-i+1+j])
					break;
			if (j == i-1)
				return i;
		}
	}
	
	return 0;
}

bool check(char x, char y, char alphabet[])
{
	char first, last;
    first = alphabet[0];
    last = alphabet[1];
    
    if (y == '?')
    {
    	if (first <= x && x <= last)
    		return true;
    	return false;
	}
	if (x == y)
		return true;
	return false;
}

char* search (char txt[], char pat[], char alphabet[])
{
	char *ptr = new char[300];
	ptr[0] = 0;
	
    vector<int> v1, v2;
    int i, j;
    
    int alphSize = alphabet[1] - alphabet[0] + 1;
    int patSize = strlen(pat);
    
    if (containQuestionMark(pat) == false)
	{
		for (i = 0; i <= patSize; i++)
			for (j = 0; j < alphSize; j++)
				v1.push_back(findNextState(pat, patSize, i, j+alphabet[0]));
	}
    
    for (i = 0; txt[i+patSize-1]; i++)
    {
    	for (j = 0; pat[j]; j++)
    	{
    		if (check(txt[i+j], pat[j], alphabet) == false)
    			break;
		}
		if (pat[j] == 0)
			v2.push_back(i+1);
	}
	
	int size1 = v1.size();
	int size2 = v2.size();
	
	for (i = 0; i < size1; i++)
    {
    	char temp[10];
    	if ((i+1) % alphSize == 0)
    		sprintf(temp, "%d\n", v1[i]);
    	else
    		sprintf(temp, "%d ", v1[i]);
    	strcat(ptr, temp);
	}
	
    for (i = 0; i < size2; i++)
    {
    	char temp[10];
    	sprintf(temp, "%d ", v2[i]);
    	strcat(ptr, temp);
	}
	
	return ptr;
}

int main()
{
	char *p = search("ABABABAACBAAABC","A?B","AC");
	cout << p << '\n';
	p = search("AAABAABAABACBB","A?BA","AC");
	cout << p << '\n';
	p = search("AAABAABAABACBB","ABA","AC");
	cout << p << '\n';
}
