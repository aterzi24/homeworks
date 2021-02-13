#include <iostream>
#include <climits>
#include <cmath>
#include <utility>
#include <vector>
#include <stack>

void copyMatrix(int**& source, int**& dest, int size)
{
	dest = new int*[size];
	for (int i = 0; i < size; i++)
	{
		dest[i] = new int[size];
		for (int j = 0; j < size; j++)
			dest[i][j] = source[i][j];
	}
}

void fwAlgorithm(int**& matrix, int size)
{
	int i, j, k;
	
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
        {
        	if (i == j)
        		continue;
            for (k = 0; k < size; k++)
               	if (i != k && matrix[j][i] < 100 && matrix[i][k] < 100 )
                	matrix[j][k] = std::min(matrix[j][k], matrix[j][i] + matrix[i][k]);
        }
}

int minCost(int**& graph, int V, int s1, int s2, int d1, int d2, int A, int B)
{
	int** matrix;
	copyMatrix(graph, matrix, V);
	
	fwAlgorithm(matrix, V);

	int i, j;
	int min = A*(matrix[s1][d1]+matrix[s2][d2]);
	
	for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
        {
        	if (i == j)
        		continue;
        	int temp = A*(matrix[s1][i]+matrix[s2][i]+matrix[j][d1]+matrix[j][d2]) + B*matrix[i][j];
        	if (temp < min)
        		min = temp;
		}
    
    for (i = 0; i < V; i++)
		delete[] matrix[i];
	delete[] matrix;
    
    return min;
}

int main()
{
	return 0;
}
