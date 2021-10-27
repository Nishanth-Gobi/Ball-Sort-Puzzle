// Ball Sort Puzzle game
// Solved using DP memoization 
// By Nishanth (19PT15)

#include <bits/stdc++.h>
#include <string>

using namespace std;

// Grid represent the current configuration of all stacks
using Grid = vector<string>;

Grid makeGrid(string stacks[], int stackCnt){

	Grid grid;
	for(int i=0;i<stackCnt;i++)
		grid.push_back(stacks[i]);

	return grid;
}

int getMaxStackHeight(Grid grid){

	int max = 0;
	for(int i=0;i<grid.size();i++){

		if (max < grid[i].size())
			max = grid[i].size();
	}

	return max;
}

string toString(Grid grid){

	string st;
	sort(grid.begin(), grid.end());
	for(int i=0;i<grid.size();i++){

		st += (grid[i] + ";");
	}
	return st;
}

bool isSolved(Grid grid, int maxStackHeight){

	for(int i=0;i<grid.size();i++){

		if(!grid[i].size())
			continue;
		else if (grid[i].size() < maxStackHeight)
			return false;
		else if (count(grid[i].begin(), grid[i].end(),grid[i][0]) != maxStackHeight)
			return false;
	}
	return true;
}

bool checkGrid(Grid grid){

	int stackCnt = grid.size();
	int maxStackHeight = getMaxStackHeight(grid);
	int totalBalls = (stackCnt - 2) * maxStackHeight;

	int currBalls = 0;

	for(int i=0;i<grid.size();i++)
		currBalls += grid[i].size();

	// check if the total number of balls is correct
	if (currBalls!=totalBalls){
		cout<<currBalls<<":"<<totalBalls;
		cout<<"*** Insufficient or excess number of balls ***";
		return false;
	}

	unordered_map <char,int> colorFreq;
	vector<char> colors;

	// Ensure that there are exactly 'p' balls of each color
	for(int i=0;i<grid.size();i++){
		for(int j=0;j<grid[i].size();j++){

			if (colorFreq.find(grid[i][j]) == colorFreq.end()){

				colorFreq[grid[i][j]] = 1;
				colors.push_back(grid[i][j]);
			}
			else
				colorFreq[grid[i][j]] += 1;
		}
	}

	for(int i=0;i<colors.size();i++){

		if(colorFreq[colors[i]]!=maxStackHeight){

			cout<<"Incorrect number of "<<colors[i]<<" balls"<<endl;
			return false;
		}
	}
	return true;
}

void printGrid(Grid grid){

	cout<<"--------------------------"<<endl;
	for(int i=0;i<grid.size();i++){
		for (int j=0;j<grid[i].size();j++){

			cout<<grid[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"--------------------------"<<endl;
}

bool isValidMove(string sourceStack, string destinationStack, int maxHeight){

	// Cannot move from empty stack
	// Cannot move to a full stack

	if (sourceStack.size()==0 || destinationStack.size()==maxHeight){
		return false;
	}

	int colorFreq = count(sourceStack.begin(), sourceStack.end(), sourceStack[0]);

	// If the source stack is full and consists of the same color balls don't shift  
	if(colorFreq == maxHeight)
		return false;

	if (destinationStack.size() == 0) {

		// If source stack consistes of balls of a single color don't shift
		if (colorFreq == sourceStack.size())
			return false;
		return true;
	}

	// Check if the movement results in a ball being placed on top another ball of the same color
	return (sourceStack[sourceStack.size()-1] == destinationStack[destinationStack.size()-1]);
}

bool solvePuzzle(Grid grid, unordered_set<string>& visited, vector<vector<int>>& ans, int maxStackHeight){
	
	// Add current configuration to visited

	visited.insert(toString(grid));
	
	// cout<<"---"<<endl;
	printGrid(grid);
	// cout<<getMaxStackHeight(grid)<<endl;
	
	for(int i=0;i<grid.size();i++){

		string sourceStack = grid[i];
		for(int j=0;j<grid.size();j++){

			if(i==j)
				continue;
						
			string destinationStack = grid[j];
			if (isValidMove(sourceStack, destinationStack, maxStackHeight)){

				// Make the move

				Grid newGrid(grid);
				newGrid[j].push_back(newGrid[i].back());
				newGrid[i].pop_back();
				
				// newGrid corresponds to the configuration after the move has been made

				if(isSolved(newGrid, maxStackHeight)){
					// Termination condition
					printGrid(newGrid);
					ans.push_back(vector<int>{i,j,1});
					return true;
				}
				
				if(visited.find(toString(newGrid))==visited.end()){
					// recursively call the algo for the new configuration

					bool solveForRest = solvePuzzle(newGrid, visited, ans, getMaxStackHeight(newGrid));
					
					if(solveForRest){

						// Uncomment to get the compact step of movements

						// vector<int> lastMove = ans[ans.size() - 1];

						// if (lastMove[0] == i && lastMove[1] == j)
						// 	ans[ans.size() - 1][2]++;
						// else
						ans.push_back(vector<int>{ i, j, 1});
						return true;
					}
				}
			}
		}
	}
	return false;
}

int driverFunc()
{
	int p, n;
	Grid grid;

	// Custom input

	p = 4;
	n = 4;
	string stacks[] = {"gbbb", "ybry", "yggy", "rrrg","",""};

	cout<<"P: "<<p<<endl<<"N: "<<n<<endl<<"Stacks:"<<endl<<endl;
	for(int i=0;i<n+2;i++){
		cout<<stacks[i]<<endl;
	}

	// Getting user input

	// cout<<"Number of color: ";
	// cin>>n;
	// cout<<"Number of balls per color: ";
	// cin>>p;

	// string stacks[n+2];

	// cout<<"Enter the intial stack configurations: "<<endl<<endl;
	
	// for(int i=0;i<n;i++)
	// {
	// 	cin>>stacks[i];
	// }
	// stacks[n] = "";
	// stacks[n+1] = "";

	// input validation
	
	grid = makeGrid(stacks, n+2);
	
	if(!checkGrid(grid)){
		cout<<"*** Invalid Grid ***"<<endl;
		return 1;
	}

	// Check if it's already solved 
	if (isSolved(grid, getMaxStackHeight(grid))){
		cout<<"*** The puzzle is already solved ***"<<endl;
		return 0;
	}
	
	unordered_set<string> visited;
	vector<vector<int>> ans;

	solvePuzzle(grid, visited, ans, getMaxStackHeight(grid));

	// Print the steps

	reverse(ans.begin(), ans.end());

	for(int i=0;i<ans.size();i++){
		cout<<"Move "<<ans[i][0] + 1<<" to "<<ans[i][1] + 1;
		// cout<<" x"<<ans[i][2];
		cout<<endl; 
	}
	return 0;
}

int main(){

	while(true){

		system("clear");

		if(driverFunc()){
			cout<<"***Re-enter valid input***"<<endl<<endl;
		}

		char resp;
		cout<<"\n\nAnother input?";
		cin>>resp;
		if(resp=='n' || resp=='N')
			break;
	}
}