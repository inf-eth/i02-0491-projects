//********************** Header Files *******************
// Linear Algebra Lab ver. 1.2

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
using namespace std;
///////////////////////  Functions Start  ///////////////////////
//***************************************************************
////////////////// First Time Display Function //////////////////

void FirstDisplay(void)
{

	cout << "*  Matrices are to be entered row-wise. Enter the entries of a row separated by    spaces. Press <enter> after you have entered the entries of a row." << endl
		 << "   (Remember, Number of entries in a row = Number of columns) "<< endl << endl
		 << "-> You can exit any time by entering 'E' and restart by entering 'R'." << endl << endl;
}

//////////////////////// Function End ///////////////////////////
//***************************************************************
//////////////// Function to display the matrix /////////////////

void Display(double Matrix[][10], int Row, int Col)
{
	
	if(Row == 0 && Col == 0)
	{
		cout << "The matrix has 0*0 dimension." << endl << endl;
		return;
	}

	for (int row = 0; row < Row; row ++)
	{
		for (int col = 0; col < Col; col++)
		{
			cout << setw(7) << setprecision(3) << Matrix[row][col] ;
		}
		cout << endl << endl;
	}

}

///////////////////////// Function End //////////////////////////
//***************************************************************
///////////////// Function to input the matrix //////////////////

void Input(double Matrix[][10], int Row, int Col)
{
	int main();

	for (int row = 0; row < Row; row ++)
	{
		cout << endl << "Row " << row + 1 << ":" << endl;
		cout <<">>";
		for (int col = 0; col < Col; col++)
		{
			char Temp;
			cin >> Temp;
			
			if(Temp == 'e' || Temp == 'E')
			{
				cout << endl << "Thank you for using LA LAB. See you soon !!! "<< endl << endl;
				exit(0);
			}

			if(Temp == 'r' || Temp == 'R')
			{
				cout << endl << "Restarting ... "<< endl << endl;
				main();
			}

			Matrix[row][col] = atof(&Temp);
		}
		
	}

}

///////////////////////// Function End //////////////////////////	// Just a function from outer space. Havent been used
//***************************************************************	// in the program.(Hmmm ... why did I include it anyway?
////////////////// Function to Calculate Transpose //////////////	// Well probably I was sleeping when I included it).

void Transpose(double RMatrix[][10],int Row, int Col)
{
	
	double hold;	
	// Outer Loop that increments rows
	for(int R = 0; R < Row; R++)
	{
		
		// Inner loop that increments columns
		for(int C = 0; C < Col; C++)
		{
			hold = RMatrix[R][C]; 
			RMatrix[R][C] = RMatrix[C][R];
			RMatrix[C][R] = hold;
		}
	
	}

}

/////////////////////////// Function End ////////////////////////
//***************************************************************
/////////////////////////// Echelon Form ////////////////////////

/*
To understand how this function calculates the echelon form of a matrix it is important to get a good mental picture
of what it does. Some basic knowledge of row-reduction and linear algebra is necessary to understand how it works.

When we want to row-reduce a matrix we need and create pivot postions. The first non-zero entry in a row such that all
entries below it are zeroes. To make a pivot entry we create zeroes below it by row-operations. There are three basic
row operations:

1.Interchanging two rows.
2.Adding a multiple of a row to another row.
3.Replacing a row with its multiple. (Think of it as multiplying a row by a real number)

The maximum number of pivots in a matrix is the row number or coulumn number, whichever, is the smaller. If equal, any
one will do. I know this sounds rubbish at the moment but continue reading you will know why we are using it. 

The process of creating zeroes below a pivot entry is carried out for each pivot. If we know how many pivots there are
(see above or below) then we can implement a for-loop that runs for that many iterations picking a new pivot entry every
time it runs and creating zeroes below it. This is exactly how this function calculated echelon form and then reduced
echelon form.

To understand how zeroes are created below a pivot think of what should be done. Yes, we multiply a pivot by a number 'n' 
such that it becomes the negative of the entry below it. We add the n-multiple of the pivot row to the row below it. This 
creates zero below the pivot entry. We repeat the procedure for every entry below the pivot entry.

The number 'n' is determined as,

		n = -(entry below pivot) / pivot
		
Note: If an entry below a pivot is zero then n will be zero and there will be no change in the row below the pivot row.
*/

void EchelonForm(double RMatrix[][10],int Row,int Col)
{	
	
	int R;
	int C;
	
	int loops;
	int maxpivots;					// The maximum number of pivot positions in a matrix is the row number or the column
									// number whichever is the smaller. If number of rows and columns is equal then	
	if (Row == Col)					// it could be either one.
	{							
		maxpivots = Row;			// The number of pivots - 1 is the maximum number of times the matrix can undergo
	}								// the operation to create zeroes below a pivot positions. This happens only if 
									// there is no free variable and otherwise if there are free variables.
	else if (Row > Col)
	{								// Here the maximum number of pivots are determined for our BIIG(just a name for the 
		maxpivots = Col;			// for loop in this function) for loop to determine how many times it has to run(loops). 
	}

	else							 
	{
		maxpivots = Row;
	}
	
	for (loops = 1, R = 0, C = 0; loops <= maxpivots; loops++, R++, C++)
	{
		int rowinterchangeflag = 0;		
		
		//****Structure 1****//	

		if (RMatrix[R][C] == 0)						// The condition if the pivot position under consideration is zero. 
		{											// In this case we may need to interchange rows.
			
			for(int kR = (R+1); kR < Row; kR++)		// Checking if there is a nonzero number below the pivot(already zero).
			{										// If true the rows will be interchanged.
				
				if (RMatrix[kR][C] != 0)
				{
					rowinterchangeflag = 1;			// This tells whether rows are interchanged.

					// The following for loop interchanges two rows
					
					double hold;					// The standard hold variable for swapping.
					
					for(int i = 0; i < Col; i++)
					{
						hold = RMatrix[R][i];
						RMatrix[R][i] = RMatrix[kR][i];
						RMatrix[kR][i] = hold;
					}
				
					R = R - 1;		// Now we have only interchanged the rows and we are still condidering the first pivot; 			
					C = C - 1;		// so these are basically to nullify the increments and take us to square 1 ;).
				
					kR = Row;		// The 'Lets-get-out-of-this-loop' condition.
									// Of course once we have interchanged the rows it is time to exit the loop.
				}
			
			}

			if (rowinterchangeflag != 1)	// Checking whether any row interchanges have taken place
			{
				R = R - 1;					// If there are no row interchanges we take a new pivot position in the same
			}								// row. This check ensures that we get a pivot in the same row.
		
		}

		//****Structure 2****//		// For the 'true' condition 	

		if (RMatrix[R][C] != 0)		// We have a true pivot  (YEAH!! :-) )
		{
			// The following for loop makes zeroes below a pivot position
			
			for (int i = 1; i < (Row - R); i++)		// This loop checks for rows
			{
				double n = -1 * (RMatrix[R+i][C] / RMatrix[R][C]);	// The number which is to be multiplied with the pivot
				
				// This inner loop does the 'stuff'. It adds n-multiple of pivot row to
				// row and that row added to the row below it.		
				for (int C1 = C; C1 < Col; C1++)	
				{									//a row below the pivot row.
					RMatrix[R+i][C1] = RMatrix[R+i][C1] + ( n * RMatrix[R][C1] );
				}
		
			}
	
		}
	
	}
	
}

/////////////////////////// Function End ///////////////////////
//**************************************************************
//////////////////////// Reduced Echelon Form //////////////////

void ReducedEchelonForm(double RMatrix[][10],int Row,int Col)
{
	
	EchelonForm(RMatrix, Row, Col);			// We need the matrix to be in echelon form, already, to start working on it.
	
	int loops;

	int R;
	int C;
	
	double pivot;
	double n;

	// This for-loop scales all the rows 
	for(loops = 0, R = 0; loops < Row; loops++, R++)
	{
		
		for(C = 0; C < Col; C++)
		{
			
			if (RMatrix[R][C] != 0)
			{
				pivot = RMatrix[R][C];

				for(int kC = C; kC < Col; kC++)
				{
					RMatrix[R][kC] = RMatrix[R][kC] / pivot;	// Scaling the rows; dividing each entry in a row 
				}												// by pivot entry.
				
				C = Col;		// The 'Lets-get-out-of-this-loop' condition.
								// We have found the pivot so we need to break out of the for loop
			}
		
		}
	
	}
	
	// The following big for-loop creates zeroes above the pivot positions.
	// As the matrix is already in echelon form pivots are basically the first non-zero entry of a row.
	// We will start from the lower most row.
	// The loop will run for a maxiumum number of row-1 times when there are no free variables (no all-zero row).
	// [Please note that the loop will always go for row-1 times but will not invoke the row operation for an all-zero row]

	for(loops = 0, R = (Row - 1); loops < (Row-1); loops++,R--)
	{
		for(C = 0; C < Col; C++)
		{
			if(RMatrix[R][C] != 0)
			{
				for(int kR = (R-1); kR >= 0; kR--)
				{
					n = -1 * (RMatrix[kR][C] / RMatrix[R][C]);
					
					for(int kC = 0; kC < Col; kC++)
					{
						RMatrix[kR][kC] = RMatrix[kR][kC] + (n * RMatrix[R][kC]);
					}
				
				}
			
				C = Col;	// The 'Lets-get-out-of-this-loop' condition.

			}

		}

	}
	
	// End of for-loop
	
	//********************************************************************************************************************

	// Once I noticed an over-determined 5*3 matrix in reduced form such that there were three 
	// pivots and one of the pivots was in the last row and the two rows above it were all-zeroes. 
	// The last pivot row should have been displayed as the third row. The third and last rows 
	// should have been interchanged.

	// I do not know whether the following fail-safe check works for its intended purpose but, 
	// anyway, it does notcreate any problems.

	//*********** Just a last minute CHECK to check if any rows need to be interchanged *****************
	
	// Well most of the code below has been copied from above and I am too lazy to remove the 
	// comments.

	int maxpivots;				// The maximum number of pivot positions in a matrix is the row number or the column
								// number whichever is the smaller. If number of rows and columns is equal then	
	if (Row == Col)				// it could be either one.
	{							
		maxpivots = Row;		// The number of pivots - 1 is the maximum number of times the matrix can undergo
	}							// the operation to create zeroes below a pivot positions. This happens only if 
								// there is no free variable and otherwise(i.e less than max) if there are free variables.
	else if (Row > Col)
	{							// Here the maximum number of pivots are determined for our BIIG(just a name for the 
		maxpivots = Col;		// for-loop in this function) for loop to determine maximum no. of times it has to run(loops). 
	}

	else							 
	{
		maxpivots = Row;
	}
	
	for (loops = 1, R = 0, C = 0; loops <= maxpivots; loops++, R++, C++)
	{
		int rowinterchangeflag = 0;		
		
		//****Structure 1****//	Only structure

		if (RMatrix[R][C] == 0)						// The condition if the pivot position under consideration is zero. 
		{											// In this case we may need to interchange rows.
			 
			for(int kR = (R+1); kR < Row; kR++)		// Checking if there is a nonzero number below the pivot(already zero).
			{										// If true the rows will be interchanged.
				
				if (RMatrix[kR][C] != 0)
				{
					rowinterchangeflag = 1;			// This tells whether rows are interchanged.

					// The following for loop interchanges two rows
					
					double hold;					// The standard hold variable for swapping.
					for(int i = 0; i < Col; i++)
					{
						hold = RMatrix[R][i];
						RMatrix[R][i] = RMatrix[kR][i];
						RMatrix[kR][i] = hold;
					}
				
					R = R - 1;		// Now we have only interchanged the rows and we are still condidering the first pivot; 			
					C = C - 1;		// so these are basically to nullify the increments and take us to square 1 ;).
				
					kR = Row;		// Of course once we have interchanged the rows it is time to exit the loop.
			
				}
			
			}

			if (rowinterchangeflag != 1)	// Checking whether any row interchanges have taken place
			{
				R = R - 1;					// If there are no row interchanges we check for a new pivot position in the
			}								// same row. This check ensures that we get a pivot in the same row.
		
		}

	}
	
	//****************************************** CHECK End ***************************************************************
}	
	



///////////////////// Function End ///////////////////////////////////////
//************************************************************************
///////////////////// The Menu Function //////////////////////////////////

void Menu(double Matrix[][10], double RMatrix[][10], int Row, int Col)
{
	int main(void);		// Prototype for main

	char option;
	
	cout << endl << "Please enter a choice to perform the requested operation on input matrix:" << endl << endl;
	cout << "1.Row Reduction to Row-Echelon Form" << endl;
	cout << "2.Row Reduction to Reduced Row-Echelon Form " << endl;
	cout << "E.EXIT " << endl;
	cout << endl << ">>";
	
	// Checking whether cin.ignore() should be executed.
	if(cin.peek() == '\r')
	{
		cin.ignore();		// Just to ignore the buffer		
	}

	cin >> option;
	
	switch(option)

	{
	
		case '1':
		{
			
			EchelonForm(RMatrix, Row, Col);
			
			cout << endl << endl << "The original matrix was : " << endl << endl;
			Display(Matrix, Row, Col);
			
			cout << endl << "The row-echelon form of the matrix is : " << endl << endl;
			Display(RMatrix, Row, Col);
			
			main();
		}
		
		case '2':
		{
			
			ReducedEchelonForm(RMatrix, Row, Col);
			
			cout << endl << "The original matrix was : " << endl << endl;
			Display(Matrix, Row, Col);
			
			cout << endl << "The reduced row-echelon form of the matrix is : " << endl << endl;
			Display(RMatrix, Row, Col);
			main();
		}

		case 'E': case 'e':
		{
			cout << endl << "Thank you for using LA LAB. See you soon !!! "<< endl << endl;
			
			exit(0);
		}
		
		default:
		{
			cout << endl << "Invalid input entered. Restarting ... " << endl << endl;
			main();
		}

	}

}					

///////////////////// Function End ///////////////////////////////////////
//************************************************************************
///////////////////// Function To Check Exit Condition ///////////////////

void CheckInput(int Temp)
{
	int main();

	if(Temp == 'e' || Temp == 'E')
	{
		cout << endl << "Thank you for using LA LAB. See you soon !!! "<< endl << endl;
		exit(0);
	}

	if(Temp == 'r' || Temp == 'R')
	{
		cout << endl << "Restarting ... "<< endl << endl;
		main();
	}

}

///////////////////////////////// Function End ////////////////////////////////
//*****************************************************************************
///////////////////////////////// Functions End ///////////////////////////////

//******************************** Globals ************************************

int first = 1;
	
const int MaxRowSize = 10;
const int MaxColSize = 10;

double Matrix[MaxRowSize][MaxColSize] = {0};	// This is the standard input matrix	
double RMatrix[MaxRowSize][MaxColSize] = {0};	// All operations are performed on this matrix. Should be read 'resultant'.

int row;
int col;

//****************************** Globals End **********************************

///////////////////////////////////////////////////////////////////////////////
//******************************** MAIN ***************************************
///////////////////////////////////////////////////////////////////////////////

int main()
{

	char TempRow;
	char TempCol;
	char optionM;
	
	// This structure is entered if the program has restarted
	if (first == 0)
	{
		cout << endl << "Do you want to work on another matrix ('Y'es / 'N'o / 'E'xit)? ";		//just enter y, Y, e, E or else(anything). 
																								//If this is not y,Y,e or E
		cin >> optionM;																			//then it will take it as
																								//a NO and continue.	
		if (optionM == 'Y' || optionM == 'y')
		{
			cout << endl << "Enter the order of the matrix:" << endl;
			
			cout << "Rows: ";
			cin >> TempRow;
			
			CheckInput(TempRow);

			row = atoi(&TempRow);
			
			cout << "Columns: ";
			cin >> TempCol;

			CheckInput(TempCol);

			col = atoi(&TempCol);
			
			cout << endl << "Please enter the matrix row-wise: " << endl << endl;

			Input(Matrix, row, col);
		}
	
		if (optionM == 'E' || optionM == 'e')
		{
			cout << endl << "Thank you for using LA LAB. See you soon !!! "<< endl << endl;
			
			exit(0);
		}
	}
	
	// This structure is entered only if the program is running for the first time
	if (first == 1)
	{
		first = 0;		// Changing the first flag to 0 to indicate the program is not running for the first time 
		
		FirstDisplay();
		
		cout << endl << "Please define the dimensions of the matrix that you want to row-reduce. " << endl;
		cout << endl << "Enter the order of the matrix:" << endl;

		cout << "Rows: ";
		cin >> TempRow;

		CheckInput(TempRow);
		row = atoi(&TempRow);

		cout << "Columns: ";
		cin >> TempCol;

		CheckInput(TempCol);
		col = atoi(&TempCol);

		cout << endl << "Please enter the matrix row-wise: " << endl;

		Input(Matrix, row, col);
		
		
	}

	// Copying the original matrix into the resultant matrix upon which the operations will be performed
	for (int R=0; R < row; R++)
	{
		for (int C=0; C < col; C++)
		{
			RMatrix[R][C] = Matrix[R][C];
		}
	
	}

	// Displaying the matrix before the menu is called
	cout << endl << "The matrix is :" << endl << endl;
	Display(RMatrix, row, col);

	// Calling the main menu
	Menu(Matrix ,RMatrix , row, col);
	
	return 0;
}

////////////////////////////////////////////////////////////////////////
// **************************** End of Main **************************//
////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
//************************************* End of Program ****************************************//
/////////////////////////////////////////////////////////////////////////////////////////////////
