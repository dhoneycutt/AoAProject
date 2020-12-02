#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

void TASK1(int, int, int, vector< vector<int> >);
void TASK2(int, int, int, vector< vector<int> >);
void TASK3(int, int, int, vector< vector<int> >);
void TASK4(int, int, int, vector< vector<int> >);
void TASK5(int, int, int, vector< vector<int> >);

int main(int argc, char *argv[])
{
	//Arguments
	int TASK;
	if(argc == 2) {
		TASK = atoi(argv[1]);
	}
	else {
		cout << "Program must be called with 1 and only 1 argument." << endl << "Press ENTER to exit." << endl;
		cin.get();
		return -1;
	}
	
	
	//Read in problem information
	string temp;
	int m, n, h;
	cout << "Enter the variables m, n, and h (separated by a space):" << endl;
	getline(cin, temp);
	stringstream(temp) >> m >> n >> h;
	
	//Declare array
	vector< vector<int> > p(m, vector<int>(n));


	cout << "Now enter the matrix p row by row with a space between each entry:" << endl;
	for(int i = 0; i < m; i++) {
		getline(cin, temp);
		istringstream reader(temp);
		int j = 0;
		for(int number; reader >> number;) {
			p[i][j] = number;
			j++;
		}
	}

	switch(TASK) {
		case 1: TASK1(m, n, h, p); break;
		case 2: TASK2(m, n, h, p); break;
		case 3: TASK3(m, n, h, p); break;
		case 4: TASK4(m, n, h, p); break;
		case 5: TASK5(m, n, h, p); break;
		default: cout << "Argument must be an integer 1-5." << endl;
	}
	
	cout << "Press ENTER to exit";
	cin.get();
}

//Recursive function for Task1
int MAX1(int m, int n, int h, vector< vector<int> > p, int i, int j, vector< vector<int> >& mz) {
	//Return 0 if outside of range
	if(i < 0 || j < 0 || i >= m || j >= n) {
		return 0;
	}
	//Return 0 if this square is not part of a valid sub-matrix
	else if(p[i][j] < h) {
		mz[i][j] = 0;
		return 0;
	}
	//Return the stored value if already stored in memoization table
	else if(mz[i][j] != -1) {
		return mz[i][j];
	}
	//Otherwise, return the min value of the cell above it, to the left, or diagonally up and left plus one
	else {
		mz[i][j] = (min({MAX1(m, n, h, p, i-1, j, mz), MAX1(m, n, h, p, i, j-1, mz), MAX1(m, n, h, p, i-1, j-1, mz)}) + 1);
		//cout << mz[i][j] << endl << mz[i-1][j] << endl << mz[i][j-1] << endl;
		return mz[i][j];
	}
	return -100;
}

void TASK1(int m, int n, int h, vector< vector<int> > p) {
	//Declare memoization matrix
	int s=0;
	int x, y;
	vector< vector<int> > mz(m, vector<int>(n, -1));
	
	//Call the recursive function for every index (will reference the table if able to so overlap will not be an issue)
	for(int i=0; i < m; i++) {
		for(int j=0; j < n; j++) {
			MAX1(m, n, h, p, i, j, mz);
		}
	}

	//Search the memoization matrix for the largest valid square ending with bottom right corner at (i,j)
	for(int i=0; i < m; i++) {
		for(int j=0; j < n; j++) {
			if(mz[i][j] > s) {
				s = mz[i][j];
				x = i;
				y = j;
			}
		}
	}

	cout << "s = " << s << "x" << s << "; x1 = " << x-s+1 << "; y1 = " << y-s+1 << "; x2 = " << x << "; y2 = " << y << ";" << endl;

	//This implementation takes O(mn) extra space due to the mn size memoization matrix mz
}

void TASK2(int m, int n, int h, vector< vector<int> > p) {
	//To do the bottom up implementation, start from the first row and move to the right until the end (all values will only reference the value to their left)
	//Then move to the next row, storing the previous row. Continue storing only the active row and the previous row, giving O(n) space

	vector<int> activeRow(n);
	vector<int> prevRow(n);
	int s = 0;
	int x, y;

	//Loop through the matrix p row by row from left to right
	for(int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
				//Current cell is equal to 0 if it does not allow a building of at least height h
				if(p[i][j] < h) {
					activeRow[j] = 0;
				}
				//Otherwise, is equal to the min value of the cell above it, to the left, or diagonally up and left plus one
				else {
					//If in the first row or first column, do not check adjacent since the largest possible square is of size 1
					if(i == 0 || j == 0) {
						activeRow[j] = 1;
					}
					else {
						//Is equal to the minimum of the largest possible squares ending at each of its up and left neighbors, plus one to include itself
						activeRow[j] = min({activeRow[j-1], prevRow[j], prevRow[j-1]}) + 1;
					}
				}

				//Check if the current cell has a larger valid square than previously found
				if(activeRow[j] > s) {
					s = activeRow[j];
					x = i;
					y = j;
				}
		}

		//Row is finished, transfer activeRow to prevRow and initialize activeRow to be all 0s
		for(int j = 0; j < n; j++) {
			prevRow[j] = activeRow[j];
			activeRow[j] = 0;
		}
	}
	
	cout << "s = " << s << "x" << s << "; x1 = " << x-s+1 << "; y1 = " << y-s+1 << "; x2 = " << x << "; y2 = " << y << ";" << endl;

	//This implementation takes O(n) extra space as it only adds two size n arrays (activeRow and prevRow)
}

void TASK3(int m, int n, int h, vector< vector<int> > p) {
	//Initialize the largest size valid sub-matrix found so far to be equal to 0
	int s=0;
	int x1, x2, y1, y2;
	
	//For every cell in the matrix p
	for(int k1 = 0; k1 < m; k1++) {
		for(int l1=0; l1 < n; l1++) {
			//For every cell to the right and down of each cell (to be the bottom right corner)
			for(int k2 = k1; k2<m; k2++) {
				for(int l2=l1; l2<n; l2++) {
					//Check if the size is large enough to be worth checking the sub-matrix elements
					if((k2 - k1 + 1) * (l2 - l1 + 1) > s) {
						bool Valid = 1;
						//Test every cell inside the sub-matrix, if any are invalid then the sub-matrix is invalid
						for(int s1=k1; s1<=k2; s1++) {
							for(int s2=l1; s2<=l2; s2++) {
								if(p[s1][s2] < h){
									Valid = 0;
								}
							}
						}
						if(Valid == 1) {
							s = (k2 - k1 + 1) * (l2 - l1 + 1);
							x1 = k1; y1 = l1; x2 = k2; y2 = l2;
						}
					}
				}
			}
		}
	}

	cout << "s = " << s << "; x1 = " << x1 + 1 << "; y1 = " << y1 + 1 << "; x2 = " << x2 + 1 << "; y2 = " << y2 + 1 << ";" << endl;

	//This implementation requires O(1) extra space since there are no arrays stored, only a constant number of variables
}

void TASK4(int m, int n, int h, vector< vector<int> > p) {
	//Declare the 3 matrices
	vector< vector<int> > height(m, vector<int>(n));
	vector< vector<int> > left(m, vector<int>(n));
	vector< vector<int> > right(m, vector<int>(n));

	//Initialize the largest size valid sub-matrix found so far to be equal to 0
	int s=0;
	int x1, x2, y1, y2;

	//Fill out the height matrix, which is the number of cells starting at p[i,j] and moving up before hitting the edge or finding a cell with value less than h
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if(p[i][j] < h) {
				height[i][j] = 0;
			}
			else {
				//If at top row, don't try to look at the above cell 
				if(i == 0) {
					height[i][j] = 1;
				}
				//Otherwise, add one to the height of the previous one above
				else {
					height[i][j] = height[i-1][j] + 1;
				}
			}
		}
	}
	
	//Fill out the left matrix, which is the width of the largest rectangle of all values greater than or equal to h that have height height[i,j] ending with bottom right corner at (i,j)
	//Because this uses the same method as the height matrix, it does not actually need to reference the height matrix, only itself
	int prev;
	for (int i = 0; i < m; i++) {
		prev = 0;
		for (int j = 0; j < n; j++) {
			//If the value itself is invalid
			if (p[i][j] < h) {
				left[i][j] = 0;
				prev = j;
			}
			//Otherwise, is the minimum of the farthest left you can go in this row or the farthest left you can go in the row above
			else {
				//If in first row or if the given cell is invalid
				if(i == 0) {
					left[i][j] = j - prev;
				}
				else {
					left[i][j] = min({left[i-1][j], j-prev});
				}
			}
		}
	}


	//Fill out the right matrix, which is the same thing as the left matrix but fills from the top right and the sub-rectangles end at bottom left corner (i,j)
	for (int i = 0; i < m; i++) {
		prev = n-1;
		for (int j = n-1; j >= 0; j--) {
			//If the value itself is invalid
			if (p[i][j] < h) {
				right[i][j] = 0;
				prev = j;
			}
			//Otherwise, is the minimum of the farthest left you can go in this row or the farthest left you can go in the row above
			else {
				//If in first row or if the given cell is invalid
				if(i == 0) {
					right[i][j] = prev - j;
				}
				else {
					right[i][j] = min({right[i-1][j], prev-j});
				}
			}
		}
	}
	
	//Calculate the size and location of the best
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < n; j++) {
			int temp = height[i][j] * (left[i][j] + right[i][j] - 1);
			//Check if size is larger than the largest found so far
			if(temp > s) {
				//cout << "i: " << i << "; j: " << j << "; height: " << height[i][j] << "; left: " << left[i][j] << "; right: " << right[i][j] << endl;
				s = temp;
				y1 = j - left[i][j] + 1;
				x1 = i - height[i][j] + 1;
				y2 = j + right[i][j] - 1;
				x2 = i;
			}
		}
	}
	cout << "s = " << s << "; x1 = " << x1 + 1 << "; y1 = " << y1 + 1 << "; x2 = " << x2 + 1 << "; y2 = " << y2 + 1 << ";" << endl;

	//This implementation uses O(mn) extra space due to the three mxn matrices (height, left, and right), with all other variables being of constant size
}

void TASK5(int m, int n, int h, vector< vector<int> > p) {
	//To reduce the problem from O(mn) extra space to O(n) extra space, only store the active row and the previous row while creating the 
	//Declare the 6 arrays to replace the 3 matrices used in the task 4 implementation
	vector<int> currHeight(n);
	vector<int> prevHeight(n);
	vector<int> currLeft(n);
	vector<int> prevLeft(n);
	vector<int> currRight(n);
	vector<int> prevRight(n);
	
	
	//Initialize the largest size valid sub-matrix found so far to be equal to 0
	int s=0;
	int x1=-1, x2=-1, y1=-1, y2=-1;

	//The arrays need to be filled out simultaneously to keep track of largest found sub-matrix so far
	for(int i = 0; i < m; i++) {
		//Fill out Height for the row
		for(int j = 0; j < n; j++) {
			//If not valid, height is equal to 0
			if(p[i][j] < h) {
					currHeight[j] = 0;
			}
			else {
				//If first row, don't reference above
				if(i == 0) {
					currHeight[j] = 1;
				}
				else {
					currHeight[j] = prevHeight[j] + 1;
				}
			}
		}

		//Fill out Left for the row
		int prev = 0;
		for(int j = 0; j < n; j++) {
			//If the value itself is invalid
			if (p[i][j] < h) {
				currLeft[j] = 0;
				prev = j;
			}
			//Otherwise, is the minimum of the farthest left you can go in this row or the farthest left you can go in the row above
			else {
				//If in first row or if the given cell is invalid
				if(i == 0) {
					currLeft[j] = j - prev;
				}
				else {
					currLeft[j] = min({prevLeft[j], j-prev});
				}
			}
		}

		//Fill out Right for the row
		prev = n-1;
		for (int j = n-1; j >= 0; j--) {
			//If the value itself is invalid
			if (p[i][j] < h) {
				currRight[j] = 0;
				prev = j;
			}
			//Otherwise, is the minimum of the farthest left you can go in this row or the farthest left you can go in the row above
			else {
				//If in first row or if the given cell is invalid
				if(i == 0) {
					currRight[j] = prev - j;
				}
				else {
					currRight[j] = min({prevRight[j], prev-j});
				}
			}
		}

		//Check row for new largest sub-matrix
		int temp = 0;
		for(int j = 0; j < n; j++) {
			temp = currHeight[j] * (currLeft[j] + currRight[j] -1);
			if(temp > s) {
				s = temp;
				y1 = j - currLeft[j] + 1;
				x1 = i - currHeight[j] + 1;
				y2 = j + currRight[j] - 1;
				x2 = i;
			}
		}

		//Move current rows to previous rows
		for(int j = 0; j < n; j++) {
			prevHeight[j] = currHeight[j];
			prevLeft[j] = currLeft[j];
			prevRight[j] = currRight[j];

			currHeight[j] = 0;
			currLeft[j] = 0;
			currRight[j] = 0;
		}
	}

	if(s == 0) {
		cout << "s =" << s << "; There is no rectangular sub-matrix that allows a building of at least height h.";
	}
	else {
		cout << "s = " << s << "; x1 = " << x1 + 1 << "; y1 = " << y1 + 1 << "; x2 = " << x2 + 1 << "; y2 = " << y2 + 1 << ";" << endl;
	}
	

	//This implementation uses O(n) extra space since it uses 6 size n arrays and all other storage constant
}