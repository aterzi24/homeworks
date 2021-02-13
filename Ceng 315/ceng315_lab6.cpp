#include <iostream>
#include <climits>
#include <cmath>
#include <utility>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>

void DFS(int**& graph, int n, int k, bool*& started, std::vector<int>& topSort)
{
	started[k] = 1;
	for (int i = 0; i < n; i++)
		if (graph[k][i] && started[i] == 0)
			DFS(graph, n, i, started, topSort);
	
	topSort.push_back(k);
}

void reverse(std::vector<int>& vc)
{
	int size = vc.size();
	for (int i = 0; i < size / 2; i++)
		std::swap(vc[i], vc[size-i-1]);
}

void makeComponent(int**& graph, int n, int index, bool*& started, std::vector<int>& temp)
{
	started[index] = 1;
	temp.push_back(index);
	for (int i = 0; i < n; i++)
		if (graph[i][index] && started[i] == 0)
			makeComponent(graph, n, i, started, temp);
}

void findSCC(int**& graph, int n, std::vector<std::vector<int>>& scc, std::vector<int>& topSort)
{
	int i;
	bool *started = new bool[n]();
	int index;
	
	for (i = 0; i < n; i++)
	{
		index = topSort[i];
		if(started[index] == 0)
		{
			std::vector<int> temp;
			makeComponent(graph, n, index, started, temp);
			scc.push_back(temp);
		}
	}
	
	if(scc.size() == n)
		scc.clear();
	else
		topSort.clear();
	
	delete[] started;
}

void SCC_TopSort (int**& graph, int n, std::vector<std::vector<int>>& scc, std::vector<int>& topSort)
{
    bool *started = new bool[n]();
    
    for (int i = 0; i < n; i++)
    	if(started[i] == 0)
    		DFS(graph, n, i, started, topSort);
    
    reverse(topSort);
    
    findSCC(graph, n, scc, topSort);
	
	delete[] started;
}

int main()
{
	srand(time(0));
    
    int n;
    
    std::vector<std::vector<int>> scc;
    std::vector<int> topSort;
    
    int arr0[][7] = {{0, 1 ,0, 1, 0, 0, 0 }, { 0, 0, 1, 0, 0, 0, 0 }, { 1, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0, 1, 0 }, { 0, 0, 0, 0, 0, 0, 1 }, { 0, 0, 0, 0, 1, 0, 0}}; 
    int arr1[][7] = {{0, 0 ,0, 0, 0, 0, 0 }, { 1, 0, 0, 0, 0, 0, 0 }, { 1, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 1, 0, 1, 0, 0, 0 }, { 0, 0, 1, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 1, 0}};
    int arr2[][8] = {{0, 1, 0, 1, 1, 0, 0, 0}, { 0, 0, 1, 0, 1, 0, 0, 0}, { 1, 0, 0, 0, 0, 0, 0, 0}, { 0, 0, 0, 0, 1, 1, 0, 0}, { 0, 0, 1, 0, 0, 0, 0, 0}, { 0, 0, 0, 0, 1, 0, 0, 0}, { 0, 0, 0, 1, 0, 1, 0, 1}, { 0, 0, 0, 0, 1, 0, 0, 0}};
    int arr3[][10]= {{0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, { 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, { 0, 0, 0, 0, 1, 0, 0, 1, 1, 0}, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}};
    int arr4[][13]= {{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}; 

    n=7;        
    int** graph = new int*[n];
    for (int j=0 ; j<n ; j++){
        graph[j] = new int[n];
        graph[j] = arr0[j];
    }
    
    SCC_TopSort (graph, n, scc, topSort);

    for(unsigned i=0 ; i<scc.size() ; i++){
        std::cout << "{";
        for(unsigned j=0 ; j<scc[i].size() ; j++){
            std::cout << scc[i][j] << " ";
        }
        std::cout << "} ";
    }
    std::cout << std::endl;
    
    n=7;        
    delete graph;
    scc.clear() ; topSort.clear();
    graph = new int*[n];
    for (int j=0 ; j<n ; j++){
        graph[j] = new int[n];
        graph[j] = arr1[j];
    }
    
    SCC_TopSort (graph, n, scc, topSort);
    
    std::cout << "{";
    for(unsigned i=0 ; i<topSort.size() ; i++){
        std::cout << topSort[i] << " ";
    }
    std::cout << "} ";
    std::cout << std::endl;

    n=8;        
    delete graph;
    scc.clear() ; topSort.clear();
    graph = new int*[n];
    for (int j=0 ; j<n ; j++){
        graph[j] = new int[n];
        graph[j] = arr2[j];
    }
    
    SCC_TopSort (graph, n, scc, topSort);

    for(unsigned i=0 ; i<scc.size() ; i++){
        std::cout << "{";
        for(unsigned j=0 ; j<scc[i].size() ; j++){
            std::cout << scc[i][j] << " ";
        }
        std::cout << "} ";
    }
    std::cout << std::endl;

    n=10;        
    delete graph;
    scc.clear() ; topSort.clear();
    graph = new int*[n];
    for (int j=0 ; j<n ; j++){
        graph[j] = new int[n];
        graph[j] = arr3[j];
    }
    
    SCC_TopSort (graph, n, scc, topSort);

    for(unsigned i=0 ; i<scc.size() ; i++){
        std::cout << "{";
        for(unsigned j=0 ; j<scc[i].size() ; j++){
            std::cout << scc[i][j] << " ";
        }
        std::cout << "} ";
    }
    std::cout << std::endl;
    
    n=13;        
    delete graph;
    scc.clear() ; topSort.clear();
    graph = new int*[n];
    for (int j=0 ; j<n ; j++){
        graph[j] = new int[n];
        graph[j] = arr4[j];
    }
    
    SCC_TopSort (graph, n, scc, topSort);
    
    std::cout << "{";
    for(unsigned i=0 ; i<topSort.size() ; i++){
        std::cout << topSort[i] << " ";
    }
    std::cout << "} ";
    std::cout << std::endl;

    return 0;
}
