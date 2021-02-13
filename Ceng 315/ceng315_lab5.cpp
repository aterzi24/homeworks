#include <iostream>
#include <climits>
#include <cmath>
#include <utility>
#include <vector>
#include <random>
#include <chrono> 

void reverse(std::vector<int>& vc)
{
	int size = vc.size();
	for (int i = 0; i < size / 2; i++)
		std::swap(vc[i], vc[size-i-1]);
}

int BuyStocks (int**& market, int n1, int n2, std::vector<int>& solution)
{
	int i, j;
	
	solution.clear();
    int **sum = new int*[n1];
    int **path = new int*[n1];
    for (i = 0; i < n1; i++)
    {
    	sum[i] = new int[n2];
    	path[i] = new int[n2];
	}
	
	int fixed = n1 - 1;
	for (i = 0; i < n2 - fixed; i++)
		sum[0][i] = market[0][i];
		
	int empty = 1;
	int max;
	int index;
	for (i = 1; i < n1; i++)
	{
		max = sum[i-1][empty-1];
		index = empty - 1;
		for (j = empty; j < n2 - fixed + empty; j++)
		{
			sum[i][j] = max + market[i][j];
			path[i][j] = index;
			if (sum[i-1][j] > max)
			{
				max = sum[i-1][j];
				index = j;
			}
		}
		empty++;
	}
	
	max = sum[fixed][fixed];
	index = fixed;
	for (i = n1; i < n2; i++)
	{
		if (sum[fixed][i] > max)
		{
			max = sum[fixed][i];
			index = i;
		}
	}
	solution.push_back(index);
	for (i = fixed; i > 0; i--)
	{
		index = path[i][index];
		solution.push_back(index);
	}
	
	reverse(solution);
	
	for (i = 0; i < n1; i++)
    {
    	delete[] sum[i];
    	delete[] path[i];
	}
	delete[] sum;
	delete[] path;
	
	return max;
}

std::ostream &operator<<(std::ostream &output, std::vector<int> vec){
    output << "{" ;
    for(int i = 0; i < vec.size(); i++){
        output << vec[i] << ",";
    }
    output << "\b";
    output << "}";    
    return output;
}

void randomArray(int*& array, int size, int min, int interval) 
{
  
  for (int i = 0; i < size; i++) 
  {
    array[i] = min + (rand() % interval);
  }
}



void test_case_random(int n1, int n2){
    std::cout << "*********Test Random*********" << std::endl;

    int **market = new int*[n1];
    for(int i = 0; i < n1; i++){
        market[i] = new int[n2];
    }
    for(int i = 0; i < n1; i++){
        randomArray(market[i], n2, -50, 100);
    }

    /*for(int i = 0; i < n1; i++){
        for(int j = 0; j < n2; j++){
            std::cout << market[i][j] << " ";
        }
        std::cout << std::endl;
    }*/

    std::vector<int> solution;

    auto start = std::chrono::steady_clock::now();


    int value = BuyStocks(market, n1, n2, solution);

    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Elapsed Time\t: " << duration*1e-6 << "s\n";
    std::cout << "Value \t\t: " << value << std::endl;
    std::cout << "Solution \t:" << solution << std::endl;


    for(int i = 0; i < n1; i++){
        delete [] market[i];
    }
    delete [] market;

}




void test_case_1(){
    std::cout << "*********Test1*********" << std::endl;
    std::vector<int> sol;
    std::vector<int> correctSol = {1,3,4};
    int correctVal = 53;
    int n1 = 3, n2 = 5;

    int market[n1][n2] = { { 7, 23, -5, -24, 16 },

                           { 5, 21, -4, 10, 23 }, 

                           { -21, 5, -4, -20, 20 } };

    int **p = new int*[n1];
    for(int i = 0; i < n1; i++){
        p[i] = new int[n2];
    }
    for(int i = 0; i < n1; i++){
        for(int j = 0; j < n2; j++){
            p[i][j] = market[i][j];
        }
    }

    int value = BuyStocks(p, n1, n2, sol);

    if(value == correctVal){
        std::cout << "Value \t\t: Correct" << std::endl;
        if(sol == correctSol){
            std::cout << "Solution \t: Correct" << std::endl;
        }
        else{
            std::cout << "Solution \t: Incorrect" << "\tExpected : " << correctSol << "\tFound : " << sol << std::endl;
        }
    }
    else{
        std::cout << "Value \t\t: Incorrect" << "\tExpected : " << correctVal << "\tFound : " <<  value << std::endl;
        if(sol == correctSol){
            std::cout << "Solution \t: Correct" << std::endl;
        }
        else{
            std::cout << "Solution \t: Incorrect" << "\tExpected : " << correctSol << "\tFound : " << sol << std::endl;
        }
    }

    for(int i = 0; i < n1; i++){
        delete [] p[i];
    }
    delete [] p;


}

void test_case_3(){
    std::cout << "*********Test3*********" << std::endl;
    std::vector<int> sol;
    std::vector<int> correctSol = {0, 2};
    int correctValue = 9;
    int n1 = 2, n2 = 3;

    int market[n1][n2] = { { 10, -2, 5 },
                        { 5, -4, -1} };

    int **p = new int*[n1];
    for(int i = 0; i < n1; i++){
        p[i] = new int[n2];
    }
    for(int i = 0; i < n1; i++){
        for(int j = 0; j < n2; j++){
            p[i][j] = market[i][j];
        }
    }

    int value = BuyStocks(p, n1, n2, sol);

    if(value == correctValue){
        std::cout << "Value \t\t: Correct" << std::endl;
        if(sol == correctSol){
            std::cout << "Solution \t: Correct" << std::endl;
        }
        else{
            std::cout << "Solution \t: Incorrect" << "\tExpected : " << correctSol << "\tFound : " << sol << std::endl;
        }
    }
    else{
        std::cout << "Value \t\t: Incorrect" << "\tExpected : " << correctValue << "\tFound : " <<  value << std::endl;
        if(sol == correctSol){
            std::cout << "Solution \t: Correct" << std::endl;
        }
        else{
            std::cout << "Solution \t: Incorrect" << "\tExpected : " << correctSol << "\tFound : " << sol << std::endl;
        }
    }

    for(int i = 0; i < n1; i++){
        delete [] p[i];
    }
    delete [] p;
}

void test_case_2(){
    std::cout << "*********Test2*********" << std::endl;
    std::vector<int> sol;
    std::vector<int> correctSol = {0, 1};
    int correctValue = -3;
    int n1 = 2, n2 = 2;

    int market[n1][n2] = { {-5, 10},
                        {-2, 2} };

    int **p = new int*[n1];
    for(int i = 0; i < n1; i++){
        p[i] = new int[n2];
    }
    for(int i = 0; i < n1; i++){
        for(int j = 0; j < n2; j++){
            p[i][j] = market[i][j];
        }
    }

    int value = BuyStocks(p, n1, n2, sol);

    if(value == correctValue){
        std::cout << "Value \t\t: Correct" << std::endl;
        if(sol == correctSol){
            std::cout << "Solution \t: Correct" << std::endl;
        }
        else{
            std::cout << "Solution \t: Incorrect" << "\tExpected : " << correctSol << "\tFound : " << sol << std::endl;
        }
    }
    else{
        std::cout << "Value \t\t: Incorrect" << "\tExpected : " << correctValue << "\tFound : " <<  value << std::endl;
        if(sol == correctSol){
            std::cout << "Solution \t: Correct" << std::endl;
        }
        else{
            std::cout << "Solution \t: Incorrect" << "\tExpected : " << correctSol << "\tFound : " << sol << std::endl;
        }
    }

    for(int i = 0; i < n1; i++){
        delete [] p[i];
    }
    delete [] p;
}




int main(){

    test_case_1();
    test_case_2();
    test_case_3();
    
    test_case_random(10,30); // first argument : n1     second argument : n2
    

    return 0;
}
