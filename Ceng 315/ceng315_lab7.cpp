#include <iostream>
#include <climits>
#include <cmath>
#include <utility>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

int find_min(int*& cost, bool*& visited, int n)
{
	int min = 99999999;
	int index;
	for (int i = 0; i < n; i++)
	{
		if(visited[i] == false && cost[i] != 0 && cost[i] < min)
		{
			min = cost[i];
			index = i;
		}
	}
	return index;
}

void reverse(vector<int>& vc)
{
	int size = vc.size();
	for (int i = 0; i < size / 2; i++)
		std::swap(vc[i], vc[size-i-1]);
}

// literally dijkstra from src to dest
int min_path(int**& times, int**& costs, int n, int src, int dest, vector<int>& cost_path)
{
	int *cost = new int[n]();
	int *trace_back = new int[n];
	bool *visited = new bool[n]();
	
	int index = src;
	
	while (index != dest)
	{
		visited[index] = true;
		for (int i = 0; i < n; i++)
		{
			int temp = times[index][i] * costs[index][i];
			if (temp != 0 && (cost[i] == 0 || cost[i] > temp + cost[index]))
			{
				cost[i] = temp + cost[index];
				trace_back[i] = index;
			}
		}
		// next node to be visited
		index = find_min(cost, visited, n);	
	}
	// construct solution
	while (index != src)
	{
		cost_path.push_back(index);
		index = trace_back[index];
	}
	cost_path.push_back(src);
	reverse(cost_path);
	
	int min = cost[dest];
	
	delete[] cost;
	delete[] trace_back;
	delete[] visited;
	
	return min;
}

void construct_goodies_array(vector<pair<int,int>>& goodies, vector<pair<int,int>>*& goodies_array)
{
	int size = goodies.size();
	for (int i = 0; i < size; i++)
	{
		vector<pair<int,int>> temp;
		for (int j = 0; j < size; j++)
		{
			if(i == j)
				continue;
			temp.push_back(goodies[j]);
		}
		goodies_array[i] = temp;
	}
}

// concatenate from 3rd element of vc2
void concatenate_vectors(vector<int>& vc1, vector<int>& vc2)
{
	int size = vc2.size();
	
	for (int i = 2; i < size; i++)
		vc1.push_back(vc2[i]);
}

void Wubbalubbadubdub(int**& times, int**& costs, int n, int src, int dest, vector<pair<int,int>>& goodies, vector<int>& cost_path)
{	
	// from src to dest without any goodies
	int min = min_path(times, costs, n, src, dest, cost_path);	
	int size = goodies.size();
	
	// if no goodies then above is the only solution
	if(goodies.size() != 0)
	{
		vector<pair<int,int>> *goodies_array = new vector<pair<int,int>>[size];
		// construct goodies_array
		// for each i, goodies_array[i] is goodies[0:size except i]
		construct_goodies_array(goodies, goodies_array);
		
		// go from src to goodies[i] directly and then find best way from goodies[i] to dest
		// compare all solution and find the best
		for (int i = 0; i < size; i++)
		{
			vector<int> temp_path1, temp_path2;
			// min path from src to goodies[i] directly
			int temp1 = min_path(times, costs, n, src, goodies[i].first, temp_path1);
			// best way to goodies[i] to dest but this time goodies[i] is substracted from goodies
			Wubbalubbadubdub(times, costs, n, goodies[i].first, dest, goodies_array[i], temp_path2);
			int temp2 = temp_path2[0];
			// if better solution then save it
			if (temp1 + temp2 - goodies[i].second < min)
			{
				min = temp1 + temp2 - goodies[i].second;
				cost_path = temp_path1;
				concatenate_vectors(cost_path, temp_path2);
			}
		}
		delete[] goodies_array;
	}
	
	cost_path.insert(cost_path.begin(), min);
}


// ------------------------------------------------------
void randomArray(int** times, int** costs, int n) 
{
  for(int i = 0; i < n; i++){
    times[i] = new int[n];
    costs[i] = new int[n];
    for(int j = i; j < n; j++){
      if(j == i || rand()%100 < 20){
        times[i][j] = 0;
        costs[i][j] = 0;
      }
      else{
        times[i][j] = rand()%100;
        costs[i][j] = rand()%100;
      }
    }
  }
  for(int i = 0; i < n; i++){
    for(int j = 0; j < i; j++){
      times[i][j] = times[j][i];
      costs[i][j] = costs[j][i];
    }
  }
}

void measureTime(){

  clock_t begin;
  clock_t end;
  double duration;

  int seed;
  std::cout << "enter seed" << std::endl;
  std::cin >> seed;
  srand(seed);

  int n = rand() % 128;
  int **times = new int*[n];
  int **costs = new int*[n];
  
  randomArray(times, costs, n);

  std::vector<std::pair<int,int>> goodies;
  if(rand()%100 < 50) goodies.push_back(std::make_pair(rand()%n, rand()%5000));
  if(rand()%100 < 50) goodies.push_back(std::make_pair(rand()%n, rand()%5000));
  std::vector<int> cost_path;

  if ((begin = clock ()) == -1) 
    std::cerr << "clock err" << std::endl; 

  Wubbalubbadubdub(times, costs, n, rand()%n, rand()%n, goodies, cost_path);

  if ((end = clock ()) == -1) 
    std::cerr << "clock err" << std::endl;
  duration = ((double)end-begin) / CLOCKS_PER_SEC * 1000000;

  std::cerr << duration <<" microsecs " << std::endl;
  for(auto x:cost_path) std::cout << x << ' ';
  std::cout << std::endl;
}

int main()
{
    measureTime();
    

    return 0;
}
