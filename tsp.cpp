#include <iostream> // to use cin, cout
#include <stdio.h>  // to use printf(*s, vargs...)
#include <limits.h> // to use MAX_INT

using namespace std;

// Global
int n;		//Number of nodes in weight array
int **dist; //Weight array

int visited_all; //Bit boolean

int **dp;   //Memoization array to reduce recursion cost
int **road; //Memoization array to reconstruct salesman tour

//Functions
void tspEngine(int mask, int startPos); //Start point of solution
int tsp(int mask, int pos);				//To get optimal cost
void printPath(int mask, int pos);		//To print optiaml path

//Utilty
void takeInput();								   // take user input
int **allocateMemory(int size, int isMemoization); //To allocate memory for dynamic arrays
void initiateMemo(int **arr, int n);			   //Initate dp, and road array

int main()
{
	cout << "Enter No. Of Cities: ";
	cin >> n;

	//Setting up our bit boolean to 1111...1
	//Where number of ones equal to number of dist array nodes
	visited_all = (1 << n) - 1; //e.g. n = 4, then visited_all = 1111 =16-1=15

	//dynamic allocation of weight array
	dist = allocateMemory(n, 0);
	//take user weight array
	takeInput();

	// dynamic allocation of memoization arrays
	dp = allocateMemory(n, 1);
	road = allocateMemory(n, 1);

	// initiate memoization arrays with (-1);
	initiateMemo(dp, n);
	initiateMemo(road, n);

	//sloving tsp
	tspEngine(1, 0); // mask = 0001, startPos = 1;

	return 0;
}

void tspEngine(int mask, int startPos)
{
	printf("Optimal Cost: %5d \n", tsp(mask, startPos));
	printf("Optimal Path: ");
	printPath(mask, startPos);
	printf("%2d\n", startPos + 1);
}

int tsp(int mask, int pos)
{
	//if all cities have been visited return
	//dist betn last pos and starting point
	if (mask == visited_all)
	{
		return dist[pos][0];
	}
	if (dp[pos][mask] != -1)
	{
		return dp[pos][mask];
	}
	int ans = INT_MAX;
	for (int city = 0; city < n; city++)
	{
		if ((mask & (1 << city)) == 0)
		{
			int newAns = dist[pos][city] + tsp(mask | (1 << city), city);
			if (ans > newAns)
			{
				ans = newAns;
				road[pos][mask] = city;
			}
		}
	}
	return dp[pos][mask] = ans;
}

void printPath(int mask, int pos)
{
	printf("%2d  ", pos + 1);
	if (road[pos][mask] == -1)
		return;
	int x = road[pos][mask];
	int m = mask | (1 << x);
	mask = mask | m;
	printPath(mask, x);
}

// Utilty functions
void takeInput()
{
	cout << "Enter weight array: " << endl;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> dist[i][j];
	cout << endl; 
}

int **allocateMemory(int size, int isMemoization)
{
	int **arr = new int *[size];
	if (isMemoization == 0)
	{
		for (int i = 0; i < size; i++)
			arr[i] = new int[n];
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			arr[i] = new int[(1 << size) - 1];
		}
	}
	return arr;
}

void initiateMemo(int **arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < (1 << n); j++)
		{
			arr[i][j] = -1;
		}
	}
}