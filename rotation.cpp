#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>

#define PI 3.14159265
#define LEFT 1
#define RIGHT 2
#define BELOW 4
#define ABOVE 8

using namespace std;

string inputName;  	// define the datalines input file name
string outputName;		// define the datalines output file name
int lineNum;			// define the number of lines to be displayed in the Viewport
int dashLength;			// define the dash length entered by user
int functionNum;		// define the function number to choose by user
double tx, ty;			// define the translate displacements
double sx, sy;			// define the scaling factors
double angleRotate;		// define the angle of rotation
int vx0, vy0;			// define the bottom left corner of Viewport at (Vx0, Vy0)
int vx1, vy1;			// define the top right corner of Viewport at (Vx1, Vy1)
bool orginalLines;		// define a flag to determine if the line is drawing before or after transformation

// define and initial the translate matrix
double	matrixTranslate [3][3] = {
	{1, 0, 0},
	{0, 1, 0}, 
	{0, 0, 1}
};

// define the initial scale matrix
double	matrixScale [3][3] = {
	{1, 0, 0},
	{0, 1, 0}, 
	{0, 0, 1}
};

// define the initial rotate matrix
double matrixRotate [3][3] = {
	{1, 0, 0},
	{0, 1, 0}, 
	{0, 0, 1}
};

// define the initial concatenate matrix
double matrixConcatenate [3][3] = {
	{1, 0, 0},
	{0, 1, 0}, 
	{0, 0, 1}
};

int data [100][4];					// define a dataline array to store the orginal line points
int dataTransformation [100][4];	// define a dataline array to stroe the line points after transformation

void init (void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0); // set display-window color to black
	glMatrixMode (GL_PROJECTION); // set projection parameters
	gluOrtho2D (0.0, 600.0, 0.0, 600.0); // set the x-coordinate from 0 to 500 and y-coordinate from 0 to 500
}

void setPixel (int x, int y)
{
	glColor3f (1.0, 1.0, 1.0); // activate the pixel by setting the point color to white, draw orginal lines
	glBegin (GL_POINTS);
		glVertex2i (x, y);	// set the point
	glEnd ();
	glFlush (); // process all openGL routines as quickly as possible
}

void setPixelRed (int x, int y)
{
	glColor3f (1.0, 0.0, 0.0); // activate the pixel by setting the point color to red, drwa lines after transformation
	glBegin (GL_POINTS);
		glVertex2i (x, y);	// set the point
	glEnd ();
	glFlush (); // process all openGL routines as quickly as possible
}

void offPixel (int x, int y)
{
	glColor3f (0.0, 0.0, 0.0); // deactivate the pixel by setting the point color to black
	glBegin (GL_POINTS);
		glVertex2i (x, y);	// set the point
	glEnd ();
	glFlush (); // process all openGL routines as quickly as possible
}

int round (double r)
{
	return int (r + 0.5); // round the value to the nearest integer
}

// bresenham line drawing algorithm
void bresenhamAlg (bool orginal, int dashLength, int x0, int y0, int x1, int y1)
{
	int dx = abs (x1 - x0);
	int dy = abs (y1 - y0);
	int x, y;
	
	// draw the solid line
	if (dashLength == 0) {
		if (dy == 0) { // draw the horizontal line
			if (x0 < x1) { // set the left point to the starting point
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				x1 = x0;
				y1 = y0;
			}

			if (orginal)
				setPixel (x, y); // activate the starting point
			else
				setPixelRed (x, y); // activate the starting point

			while (x < x1) {
				x++;
				if (orginal)
					setPixel (x, y); // activate the starting point
				else
					setPixelRed (x, y); // activate the starting point
			}
		}
		else if (dx == 0) {
			if (y0 < y1) { // set the left point to the starting point
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				y1 = y0;
				x1 = x0;
			}
			if (orginal)
				setPixel (x, y); // activate the starting point
			else
				setPixelRed (x, y); // activate the starting point
			while (y < y1) {
				y++;
				if (orginal)
					setPixel (x, y); // activate the starting point
				else
					setPixelRed (x, y); // activate the starting point
			}
		}
		else if (dx >= dy){ //slope |m| <= 1.0;
			int e = (dy << 1) - dx;
			int inc1 = dy << 1;
			int inc2 = (dy - dx) << 1;	
			if (x0 < x1) { // set the left point to the starting point
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				x1 = x0;
				y1 = y0;
			}

			if (orginal)
				setPixel (x, y); // activate the starting point
			else
				setPixelRed (x, y); // activate the starting point
			
			while (x < x1) {
				if (e < 0)
					e += inc1;
				else {
					if (y < y1) {
						y++;
						e += inc2;
					} else {
						y--;
						e += inc2;
					}
				}
				x++;
				if (orginal)
					setPixel (x, y); // activate the starting point
				else
					setPixelRed (x, y); // activate the starting point
			}
		} 
		else { //slope |m| > 1.0;
			int e = (dx << 1) - dy;
			int inc1 = dx << 1;
			int inc2 = (dx - dy) << 1;
			if (y0 < y1) { // set the left point to the starting point
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				y1 = y0;
				x1 = x0;
			}
			if (orginal)
				setPixel (x, y); // activate the starting point
			else
				setPixelRed (x, y); // activate the starting point
			while (y < y1) {
				if (e < 0)
					e += inc1;
				else {
					if (x > x1){
						x--;
						e += inc2;
					} else {
						x++;
						e += inc2;
					}
				}
				y++;
				if (orginal)
					setPixel (x, y); // activate the starting point
				else
					setPixelRed (x, y); // activate the starting point
			}
		}
	}
	// draw the dashed line
	else { 
		int dashed = 0; // define a dashed flag to determined if set or turn off the pixel
		int dot = 0; // used to count the activated pixels;

		if (dx >= dy){ //slope |m| <= 1.0;
			int e = (dy << 1) - dx;
			int inc1 = dy << 1;
			int inc2 = (dy - dx) << 1;
			// set the left point to the starting point
			if (x0 < x1) {
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				x1 = x0;
				y1 = y0;
			}
			if (orginal)
				setPixel (x, y); // activate the starting point
			else
				setPixelRed (x, y); // activate the starting point
			dot++;
			while (x < x1) {
				if (e < 0)
					e += inc1;
				else {
					if (y < y1) {
						y++;
						e += inc2;
					} else {
						y--;
						e += inc2;
					}
				}
				x++;
				// if the dashed flag is on, deactivate the number of pixels determined by the dash length
				if (dashed) { 
					offPixel (x, y);
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 0;
					}
				} else {
					if (orginal)
						setPixel (x, y); // activate the starting point
					else
						setPixelRed (x, y); // activate the starting point
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 1;
					}
				}		
			}
		} else { // slope |m| > 1.0
			int e = (dx << 1) - dy;
			int inc1 = dx << 1;
			int inc2 = (dx - dy) << 1;

			if (y0 < y1) { // set the left point to the starting point
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				y1 = y0;
				x1 = x0;
			}
			
			if (orginal)
				setPixel (x, y); // activate the starting point
			else
				setPixelRed (x, y); // activate the starting point
			dot++;
			while (y < y1) {
				if (e < 0)
					e += inc1;
				else {
					if (x > x1){
						x--;
						e += inc2;
					} else {
						x++;
						e += inc2;
					}
				}
				y++;
				if (dashed) { // if the dashed flag is on, deactivate the number of pixels determined by the dash length
					offPixel (x, y);
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 0;
					}
				} else {
					if (orginal)
						setPixel (x, y); // activate the starting point
					else
						setPixelRed (x, y); // activate the starting point
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 1;
					}
				}				
			} 
		}
	}
}

// basic translate function to move from one location to another
void basicTranslate (double Tx, double Ty)
{
	matrixTranslate [2][0] = Tx;	// assign the horizontal translate displacement 
	matrixTranslate [2][1] = Ty;	// assign the vertical translate displacement

	// print out the whole translate matrix
	cout << "Translation Matrix" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) 
			cout << matrixTranslate [i][j] << ", ";
		cout << endl;
	}
	cout << endl;
}

// basic scale function to enlarger or  shrinker a line
void basicScale (double Sx, double Sy)
{
	matrixScale [0][0] = Sx;	// assign the horizontal scaling factor
	matrixScale [1][1] = Sy;	// assign the vertical scaling factor

	// print out the whole scaling matrix
	cout << "Scaling Matrix" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cout << matrixScale [i][j] << ", ";
		cout << endl;
	}
	cout << endl;
}

// basic rotation function
void basicRotate (double angle)
{
	// assign the angle of rotation to the matrix
	matrixRotate [0][0] = cos (angle*PI/180);
	matrixRotate [0][1] = -sin (angle*PI/180);
	matrixRotate [1][0] = sin (angle*PI/180);
	matrixRotate [1][1] = cos (angle*PI/180);
	
	// print out the whole rotation matrix
	cout << "Rotation Matrix" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cout << matrixRotate [i][j] << ", ";
		cout << endl;
	}
	cout << endl;
}

// combine two int transformation matrix into one, the result will be stored in matrix1
void concatenate (double matrix1 [3][3], double matrix2 [3][3])
{
	double matrix [3][3];

	// perform the calculation of matrix1 * matrix2
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 3; column++)
		{
			matrix [row][column] = 0;
			for (int i = 0; i < 3; i++)
			{
				matrix [row][column] = matrix [row][column] + matrix1 [row][i] * matrix2 [i][column];				
			}			
		}
	}

	// print out the cancontenate matrix and assign to the matrixConcatenate applied by later transformation
	cout << "Concatenate Matrix for this step: " << endl;
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 3; column++)
		{
			matrixConcatenate [row][column] = matrix [row][column];
			cout << matrixConcatenate [row][column] << ", ";
		}
		cout << endl;
	}
	cout << endl;
}

// apply the int transformation matrix to the lines
void applyTransformation (double matrix [3][3], int datalines [100][4], int num)
{
	double pointData [4];
	cout << "Transformation dataline result: " << endl;

	// perform the calculation of line point * matrix
	for (int i = 0; i < num; i++) {
		pointData [0] = datalines [i][0] * matrix[0][0] + datalines[i][1] * matrix [1][0] + matrix [2][0]; 
		pointData [1] = datalines [i][0] * matrix[0][1] + datalines[i][1] * matrix [1][1] + matrix [2][1]; 
		pointData [2] = datalines [i][2] * matrix[0][0] + datalines[i][3] * matrix [1][0] + matrix [2][0]; 
		pointData [3] = datalines [i][2] * matrix[0][1] + datalines[i][3] * matrix [1][1] + matrix [2][1]; 
		
		// print out the result after transformation and store them in a new datalines array 
		for (int j = 0; j < 4; j++) {
			dataTransformation [i][j] = round (pointData [j]);
			cout << dataTransformation [i][j] << ", ";
		}
		cout << endl;
	}
}

// draw four edge lines of the viewport
void viewportSpec (int Vx0, int Vy0, int Vx1, int Vy1)
{
	bresenhamAlg (true, dashLength, Vx0, Vy0, Vx1, Vy0);
	bresenhamAlg (true, dashLength, Vx0, Vy0, Vx0, Vy1);
	bresenhamAlg (true, dashLength, Vx0, Vy1, Vx1, Vy1);
	bresenhamAlg (true, dashLength, Vx1, Vy0, Vx1, Vy1);
}

// determine the bit code of the point
int bitCode (int x, int y, int xmin, int ymin, int xmax, int ymax)
{
	int c = 0;
	if (x < xmin)
		c = c | LEFT;	// set ___1 if the point is on the left of the viewport
	else if (x > xmax)
		c = c | RIGHT;	// set __1_ if the point is on the right of the viewport

	if (y < ymin)
		c = c | BELOW;	// set _1__ if the point is below the viewport
	else if (y > ymax)
		c = c | ABOVE;	// set 1___ if the point is above the viewport

	return c;
}

// display datalines containing num lines in the viewing area
void displayPixels (int datalines [100][4], int num)
{
	int x0, y0, x1, y1;	
	int c0, c1;
	
	for (int i = 0; i < num; i++) {
		bool done = false;
		x0 = datalines [i][0];
		y0 = datalines [i][1];
		x1 = datalines [i][2];
		y1 = datalines [i][3];	
		c0 = bitCode (x0, y0, vx0, vy0, vx1, vy1);	// the bit code of the line segment starting point 
		c1 = bitCode (x1, y1, vx0, vy0, vx1, vy1);	// the bit code of the line segment ending point
		do
		{
			if (c0 == 0 && c1 == 0)	// the line segment is visable
			{
				done = true;
				bresenhamAlg (orginalLines, dashLength, x0, y0, x1, y1);
			}
			else if (c0 & c1)	// the line segment is invisable
			{
				done = true;
			}
			else	// none of the condition above, clip the line segment
			{
				double x, y;
				int c = c0 ? c0 : c1;

				// fine the interscetion point of the line segment and the viewport
				if (c & ABOVE)
				{
					y = vy1;
					x = x0 + double(x1 - x0) * double(y - y0) / double(y1 - y0);
				}
				else if (c & BELOW)
				{
					y = vy0;
					x = x0 + double(x1 - x0) * double(y - y0) / double(y1 - y0);
				
				}
				else if (c & RIGHT)
				{
					x = vx1;
					y = y0 + double(y1 - y0) * double(x - x0) / double(x1 - x0);
				
				}
				else
				{
					x = vx0;
					y = y0 + double(y1 - y0) * double(x - x0) / double(x1 - x0);				
				}

				// clip the line segment by the intersection point and go back to the algorithm
				if (c == c0)
				{
					x0 = round (x);
					y0 = round (y);
					c0 = bitCode (x0, y0, vx0, vy0, vx1, vy1);
				}
				else
				{
					x1 = round (x);
					y1 = round (y);
					c1 = bitCode (x1, y1, vx0, vy0, vx1, vy1);
				}
			}
		} while (done == false);
	}
}

// read datalines from an external file
void inputLines (string datalines, int num)
{
	ifstream inFile;
	inFile.open (datalines);	// open the input file 
	
	cout << endl;
	cout << "Original datalines: " << endl;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inFile >> data [i][j]; // read the points in the file, and store them in a data array
			cout << data [i][j] << ", "; // print out the data
		}
		cout << endl;
	}
	cout << endl;
	inFile.close ();
}

// output datalines containing num lines to an external file
void outputLines (string datalinesOutput, int num)
{
	ofstream outFile;
	outFile.open (datalinesOutput); // open the output file
	for (int i = 0; i < num; i ++)
	{
		for (int j = 0; j < 4; j++)
			outFile << dataTransformation[i][j] << " ";	// write the point from the datalines array to the output file
		outFile << endl;
	}
	cout << endl;
}

// window to viewport transformation
void windowToViewport (int datalines [100][4], int num)
{
	double a ,b, c, d;
	int dataViewport [100][4];
	a = 300.0 / 600;
	b = 150;
	c = 200.0 / 600;
	d = 200;
	
	bresenhamAlg (true, dashLength, vx0, vy0, vx1, vy0);
	bresenhamAlg (true, dashLength, vx0, vy0, vx0, vy1);
	bresenhamAlg (true, dashLength, vx0, vy1, vx1, vy1);
	bresenhamAlg (true, dashLength, vx1, vy0, vx1, vy1);

	for (int i = 0; i < num; i++)
	{
		dataViewport [i][0] = round (a * datalines [i][0] + b);		
		dataViewport [i][1] = round (c * datalines [i][1] + d);		
		dataViewport [i][2] = round (a * datalines [i][2] + b);		
		dataViewport [i][3] = round (c * datalines [i][3] + d);
		bresenhamAlg (orginalLines, dashLength, dataViewport [i][0], dataViewport [i][1], dataViewport [i][2], dataViewport [i][3]);
	}

	cout << endl;
	cout << "Mapping the datalines from window to viewport: " << endl;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << dataViewport [i][j] << ", ";
		}
		cout << endl;
	}
	cout << endl;
}

void showdatalines (int datalines [100][4], int num) 
{
	for (int i = 0; i < num; i++)
	{
		bresenhamAlg (true, dashLength, datalines [i][0], datalines [i][1], datalines [i][2], datalines [i][3]);
	}
}

// display the function implementation in the screen
void display (void)
{
	int turn;
	// define the viewpoint 
	vx0 = 150;
	vy0 = 200;
	vx1 = 450;
	vy1 = 400;
	inputLines (inputName, lineNum);	// read datalines from an external file
	showdatalines (data, lineNum);

	cout << "Turn on the Viewport?: ";
	cin >> turn;
	if (turn) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	viewportSpec (vx0, vy0, vx1, vy1);	// display the viewport
	orginalLines = true;
	displayPixels (data, lineNum);
	//windowToViewport (data, lineNum);	// mappint the window to viewport transformation
	orginalLines = false;
	// implement all the functions by different situation
	switch (functionNum)
	{
		case 1:	// translate only
			basicTranslate (tx, ty);						
			applyTransformation (matrixTranslate, data, lineNum);
			displayPixels (dataTransformation, lineNum);
			//windowToViewport (dataTransformation, lineNum);
			outputLines (outputName, lineNum);
			break;
		case 2:	// scale only	
			basicScale (sx, sy);						
			applyTransformation (matrixScale, data, lineNum);
			displayPixels (dataTransformation, lineNum);
			windowToViewport (dataTransformation, lineNum);
			outputLines (outputName, lineNum);
			break;
		case 3:	// rotate only
			basicRotate (angleRotate);
			applyTransformation (matrixRotate, data, lineNum);
			displayPixels (dataTransformation, lineNum);
			outputLines (outputName, lineNum);
			break;
		case 4:	// translate and scale
			basicTranslate (tx, ty);		
			basicScale (sx, sy);		
			concatenate (matrixTranslate, matrixScale);
			applyTransformation (matrixConcatenate, data, lineNum);
			displayPixels (dataTransformation, lineNum);
			//windowToViewport (dataTransformation, lineNum);
			outputLines (outputName, lineNum);
			break;
		case 5: // translate and rotate
			basicTranslate (tx, ty);		
			basicRotate (angleRotate);
			concatenate (matrixRotate, matrixTranslate);
			applyTransformation (matrixConcatenate, data, lineNum);
			displayPixels (dataTransformation, lineNum);
			//windowToViewport (dataTransformation, lineNum);
			outputLines (outputName, lineNum);
			break;
		case 6:	// scale and rotate
			basicScale (sx, sy);		
			basicRotate (angleRotate);
			concatenate (matrixRotate, matrixScale);
			applyTransformation (matrixConcatenate, data, lineNum);			
			displayPixels (dataTransformation, lineNum);
			//windowToViewport (dataTransformation, lineNum);
			outputLines (outputName, lineNum);
			break;
		case 7:	// translate, scale and rotate
			basicTranslate (tx, ty);		
			basicScale (sx, sy);		
			basicRotate (angleRotate);
			concatenate (matrixTranslate, matrixScale);
			concatenate (matrixConcatenate, matrixRotate);
			applyTransformation (matrixConcatenate, data, lineNum);		
			displayPixels (dataTransformation, lineNum);
			//windowToViewport (dataTransformation, lineNum);
			outputLines (outputName, lineNum);
			break;
		case 8:	// use an example to combine all the transformation functions
			basicTranslate (-tx, -ty);		
			basicRotate (angleRotate);
			concatenate (matrixTranslate, matrixRotate);
			basicTranslate (tx, ty);		
			concatenate (matrixConcatenate, matrixTranslate);
			basicScale (sx, sy);		
			concatenate (matrixConcatenate, matrixScale);
			applyTransformation (matrixConcatenate, data, lineNum);		
			displayPixels (dataTransformation, lineNum);
			//windowToViewport (dataTransformation, lineNum);
			outputLines (outputName, lineNum);
			break;
	}
	}
}

int main (int argc, char** argv)
{
	// name of the input file name is provided by user
	cout << "Enter the datalines input file name: ";
	cin >> inputName;

	// name of the output file name is provided by user
	cout << "Enter the datalines output file name: ";
	cin >> outputName;

	// number of lines is provided by user
	cout << "Enter the number of lines to transform from the datalines file: ";
	cin >> lineNum;

	// dash length is provided by user
	cout << "Enter the dash length: "; 
	cin >> dashLength;
	cout << endl;

	// print a list of functions 
	cout << "*****FUNCTION LIST*****" << endl;
	cout << "1. BasicTranslate" << endl;
	cout << "2. BasicScale" << endl;
	cout << "3. BasicRotate" << endl;
	cout << "4. Combine 1&2" << endl;
	cout << "5. Combine 1&3" << endl;
	cout << "6. Combine 2&3" << endl;
	cout << "7. Combine 1&2&3" << endl;
	cout << "8. Rotation 45 degrees (clockwise) about point (1, 2) & scale the result by 2 in horizontal and by 0.5 in the vertical about point (0, 0)" << endl;
	cout << "***********************" << endl;
	
	// promote user to choose a function to implement
	cout << "Choose a function: "; 
	cin >> functionNum;
	cout << endl;

	// check the number of function is valid or not
	while (functionNum > 8 || functionNum < 1)
	{
		cout << "No such function. Please enter again: ";
		cin >> functionNum;
	}

	switch (functionNum)
	{
		case 1:
			cout << "Enter the translation displacement Tx: ";
			cin >> tx;
			cout << "Enter the translation displacement Ty: ";
			cin >> ty;
			break;
		case 2:			
			cout << "Enter the scaling factor Sx: ";
			cin >> sx;
			cout << "Enter the scaling factor Sy: ";
			cin >> sy;
			break;
		case 3:
			cout << "Enter the angle of rotation: ";
			cin >> angleRotate;
			break;
		case 4:
			cout << "Enter the translation displacement Tx: ";
			cin >> tx;
			cout << "Enter the translation displacement Ty: ";
			cin >> ty;
			cout << "Enter the scaling factor Sx: ";
			cin >> sx;
			cout << "Enter the scaling factor Sy: ";
			cin >> sy;
			break;
		case 5: 
			cout << "Enter the translation displacement Tx: ";
			cin >> tx;
			cout << "Enter the translation displacement Ty: ";
			cin >> ty;
			cout << "Enter the angle of rotation: ";
			cin >> angleRotate;
			break;
		case 6:
			cout << "Enter the scaling factor Sx: ";
			cin >> sx;
			cout << "Enter the scaling factor Sy: ";
			cin >> sy;
			cout << "Enter the angle of rotation: ";
			cin >> angleRotate;
			break;
		case 7:	
			cout << "Enter the translation displacement Tx: ";
			cin >> tx;
			cout << "Enter the translation displacement Ty: ";
			cin >> ty;
			cout << "Enter the scaling factor Sx: ";
			cin >> sx;
			cout << "Enter the scaling factor Sy: ";
			cin >> sy;
			cout << "Enter the angle of rotation: ";
			cin >> angleRotate;
			break;
		case 8:
			cout << "Enter the translation displacement Tx: ";
			cin >> tx;
			cout << "Enter the translation displacement Ty: ";
			cin >> ty;
			cout << "Enter the scaling factor Sx: ";
			cin >> sx;
			cout << "Enter the scaling factor Sy: ";
			cin >> sy;
			cout << "Enter the angle of rotation: ";
			cin >> angleRotate;
			break;
	}
	
	glutInit (&argc, argv); // initialize GLUT
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // set display mode
	glutInitWindowPosition (700, 100); // set top-left display-window position
	glutInitWindowSize (600, 600); // set display-window sidth and height
	glutCreateWindow ("Screen"); // create display window

	init (); // execute initialization procedure
	glutDisplayFunc (display); // send graphics to display window
	glutMainLoop (); // display everything and wait
}
