// MazeProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

#define M_PI   3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)

char wndname[] = "TurtleWindow";
//Increase the resolution by 1000x1000 so you can see 30x30 grid
Mat image = Mat::zeros(1000, 1000, CV_8UC3);
Scalar WHITE(255, 255, 255);
Scalar BLACK(0, 0, 0);
const int DELAY = 1;
Point _curPosition(250, 250);
int _direction = 0;
int rowLength = 5;
int columnLength = 3;



// 
// Must be called in main() before any other drawing function
//






void init()
{
	imshow(wndname, image);
	waitKey(DELAY);
}

//
// Move the pen to the given coordinates without leaving a mark
// 
// Note (0,0)     refers to the upper left corner
//      (500,500) refers to the bottom right corner  
//
void changePosition(int x, int y)
{
	_curPosition.x = x;
	_curPosition.y = y;
}

//
// point in the direction given in degrees
// 0   ==> point right
// 90  ==> point down
// 180 ==> point left
// 270 ==> point up
//
void changeDirection(int direction)
{
	_direction = direction;
}

//
// Moves the pen forward the given number of pixels
// Note leaves a mark creating a line from the previous point
// to the new point
//
void moveForward(int nPixels)
{
	int x = static_cast<int>(round(nPixels * cos(degToRad(_direction))));
	int y = static_cast<int>(round(nPixels * sin(degToRad(_direction))));

	Point newPoint = Point(x + _curPosition.x, y + _curPosition.y);
	line(image, _curPosition, newPoint, WHITE);
	_curPosition = newPoint;
	// cout << "moved to " << newPoint.x << "," << newPoint.y << endl;
	imshow(wndname, image);
	waitKey(DELAY);
}

void moveForward(int nPixels, Scalar color, bool repaint = true)
{
	int x = static_cast<int>(round(nPixels * cos(degToRad(_direction))));
	int y = static_cast<int>(round(nPixels * sin(degToRad(_direction))));

	Point newPoint = Point(x + _curPosition.x, y + _curPosition.y);
	line(image, _curPosition, newPoint, color);
	_curPosition = newPoint;
	// cout << "moved to " << newPoint.x << "," << newPoint.y << " dir:" << _direction << endl;
	if (repaint)
	{
		imshow(wndname, image);
		waitKey(DELAY);
	}
}

class Square
{
private:
	int xPos, yPos;
	bool TopRemoved = false;
	bool BottomRemoved = false;
	bool LeftRemoved = false;
	bool RightRemoved = false;
	int gridX = 0;
	int gridY = 0;
	bool gridVisited = false;
public:

	Square()
	{
		xPos = 0;
		yPos = 0;
	}

	Square(int x, int y)
	{
		xPos = x;
		yPos = y;
		
	}

	void Draw()
	{
		int startingAngle = 0;
		changePosition(xPos, yPos);

		for (int i = 0; i < 4; i++)
		{
			changeDirection(startingAngle);
			moveForward(25);
			startingAngle += 90;

		}
		if (TopRemoved)
		{
			RemoveTop();
		}
		if (BottomRemoved)
		{
			RemoveBottom();
		}
		if (LeftRemoved)
		{
			RemoveLeft();
		}
		if (RightRemoved)
		{
			RemoveRight();
		}
	}

	void SetCoordinates(int x, int y)
	{
		xPos = x;
		yPos = y;
	}
	void DrawRandom()
	{
		Square sqArray[50];
		for (int i = 0; i < 50; i++)
		{
			int randomY = rand() % 350;
			int randomX = rand() % 350;
			sqArray[i].SetCoordinates(randomX, randomY);
			sqArray[i].Draw();
		}

	}
	void RemoveTop()
	{
		changePosition(xPos, yPos);
		changeDirection(0);
		moveForward(25, BLACK, true);
		TopRemoved = true;
	}
	void RemoveBottom()
	{
		changePosition(xPos, yPos + 25);
		changeDirection(0);
		moveForward(25, BLACK, true);
		BottomRemoved = true;
	}
	void RemoveLeft()
	{
		changePosition(xPos, yPos);
		changeDirection(90);
		moveForward(25, BLACK, true);
		LeftRemoved = true;
	}
	void RemoveRight()
	{
		changePosition(xPos + 25, yPos);
		changeDirection(90);
		moveForward(25, BLACK, true);
		RightRemoved = true;
	}

	void SetGrid(int x, int y)
	{
		gridX = x;
		gridY = y;
	}
	void GetGridCoords()
	{
		cout << "XGrid: " << gridX << endl;
		cout << "YGrid: " << gridY << endl;
	}
	//Getters for grid coords, can be proven useful for making the algorithms
	//for spotting neighbors.
	int getgridX()
	{
		return gridX;
	}
	int getgridY()
	{
		return gridY;
	}
	

	//Mark visited in the two dimensional array of squares.
	void markVisited()
	{
		gridVisited = true;
	}
	//Returns if the variable gridVisited was ever visited 
	bool hasBeenVisited()
	{
		return gridVisited;
	}
	//Find nearest neighbor to visit. Do it off of random?
	//Do bounds checking to make sure the maze doesn't break off.
	//Have some specific cases based off of direction. May have to
	//Look into the video to see how the algorithm goes through a
	//maze to generate one.
	//Might have to be outside of square class and must be a function to 
	//have an array as a pass in parameter.
	//Step 2
	
	/*
	Step 3 remove a wall between the neighbor.
	Step 4 
	enqueue the current square, 
	add a loop until the queue is empty (preferabbly while loop)
	if the neighbor hasn't been visited, enqueue the neighbor and remove walls
	if the neighbor has already been visited, dequeue.
	*/

};


class QueueSquare
{
private:
	Square QueueArray[500];
	int front = 0;
	int rear = 0;
	int size = 0;
	int capacity = 500;
public:


	bool isEmpty()
	{
		return (size == 0);
	}
	bool isFull()
	{
		return (size == capacity);
	}

	void enqueue(Square element)
	{
		if (!isFull())
		{
			rear = (rear + 1) % capacity;
			//wrap around solution for queues.
			QueueArray[rear] = element;
			size++;
		}
		else
		{
			cout << "Queue overflow!" << endl;
			exit(2);
		}
	}

	Square dequeue()
	{
		if (!isEmpty())
		{
			front = (front + 1) % capacity;
			Square value = QueueArray[front];
			size--;
			return value;
		}
		else
		{
			cout << "This queue is empty!" << endl;
			exit(2);
		}
	}
};

void PopulateGrid(Square pArray[][5]);
void PopulateGrid(Square pArray[30][30]);
	//Find nearest neighbor to visit. Do it off of random?
	//Do bounds checking to make sure the maze doesn't break off.
	//Have some specific cases based off of direction. May have to
	//Look into the video to see how the algorithm goes through a
	//maze to generate one.
	//Might have to be outside of square class and must be a function to 
	//have an array as a pass in parameter.
	//Always have the exit be the last element of the array (bottom right)
	//Step 2
Square FindUnvisitedNeighbor(Square pArray[3][5], int pGridX, int pGridY);
Square FindUnvisitedNeighbor(Square pArray[30][30], int pGridX, int pGridY);
void DrawGrid3x5(Square pArray[3][5]);
void DrawGrid30x30(Square pArray[30][30]);



int main()
{
	//Play around with this value to generate different 3x5 mazes.
	srand(time(NULL));
	init();
	Square mySquare(100, 100);
	Square otherSquare(250, 230);
	Square myArray[3][5];
	QueueSquare myQueue;
	QueueSquare bigQueue;
	Square bigArray[30][30];

	/*
	//PopulateGrid(myArray);
	//DrawGrid3x5(myArray);


	//Square next = FindUnvisitedNeighbor(myArray, 2, 2);
	//next.Draw();
	//next.GetGridCoords();
	
	
	myArray[0][0].RemoveLeft();
	Square current = myArray[0][0];
	int currentX = current.getgridX();
	int currentY = current.getgridY();
	myQueue.enqueue(current);
	int counter = 0;
	
	
	 Uncomment this to see the 3x5 Maze
	while (!myQueue.isEmpty())
	{
		Square next = FindUnvisitedNeighbor(myArray, currentX, currentY);
		int nextX = next.getgridX();
		int nextY = next.getgridY();
		if (next.hasBeenVisited())
		{
			current = myQueue.dequeue();
			currentX = current.getgridX();
			currentY = current.getgridY();
		}
		else if(!next.hasBeenVisited())
		{
			myQueue.enqueue(next);
			currentX = next.getgridX();
			currentY = next.getgridY();
			//currentX = nextX;
			//currentY = nextY;
		}
		else
		{
			myQueue.dequeue();
		}
		
	}
	myArray[2][4].RemoveRight();
	*/
	PopulateGrid(bigArray);
	DrawGrid30x30(bigArray);

	bigArray[0][0].RemoveLeft();
	Square current = bigArray[0][0];
	int currentX = current.getgridX();
	int currentY = current.getgridY();
	bigQueue.enqueue(current);

	
	while (!bigQueue.isEmpty())
	{
		Square next = FindUnvisitedNeighbor(bigArray, currentX, currentY);
		int nextX = next.getgridX();
		int nextY = next.getgridY();
		if (next.hasBeenVisited())
		{
			current = bigQueue.dequeue();
			currentX = current.getgridX();
			currentY = current.getgridY();
		}
		else if (!next.hasBeenVisited())
		{
			bigQueue.enqueue(next);
			currentX = next.getgridX();
			currentY = next.getgridY();
			//currentX = nextX;
			//currentY = nextY;
		}
		else
		{
			bigQueue.dequeue();
		}
	}
	bigArray[29][29].RemoveRight();



	waitKey();




}


//Draws a grid of squares but also populates it with data
//Might have to make another function to populate a given array with data.
//The way grid coordinates are set are backwards due to how 2d arrays work
//Left bracket is y axis and right bracket is x axis
//x = row, y = column
//2darray[2][4] x = 4 and y = 2
//Fix this tomorrow.
void PopulateGrid(Square pArray[][5])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			int x = (25 * j);
			int y = (25 * i);
			pArray[i][j].SetCoordinates(x, y);
			pArray[i][j].SetGrid(i, j);
		}
	}
}
//Larger version of populate grid but for 30x30 mazes.
void PopulateGrid(Square pArray[30][30])
{
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			int x = (25 * j);
			int y = (25 * i);
			pArray[i][j].SetCoordinates(x, y);
			pArray[i][j].SetGrid(i, j);
		}
	}
}

void DrawGrid3x5(Square pArray[3][5])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			pArray[i][j].Draw();
		}
	}
}

void DrawGrid30x30(Square pArray[30][30])
{
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			pArray[i][j].Draw();
		}
	}
}

//Given a grid coordinate, choose an unvisited neighboring square.
Square FindUnvisitedNeighbor(Square pArray[3][5], int pGridX, int pGridY)
{
	
	pArray[pGridX][pGridY].markVisited();
	Square current = pArray[pGridX][pGridY];	
	//Square current = pArray[pGridY][pGridX];
	int curX = current.getgridX();
	int curY = current.getgridY();
	String choice[4];
	int size = 0;


	//Go left
	//Going left means minus 1 for X
	//Make sure this does not become less than 0
	if (curX - 1 >= 0 && !pArray[pGridX-1][pGridY].hasBeenVisited()) //UP - PREVIOUSLY LEFT
	{
		//choice[size] = "LEFT";
		choice[size] = "UP";
		size++;
	}
	//Go right
	//Going right means plus 1 for X
	//Make sure this does not become larger than rowLength
	if (curX + 1 < columnLength && !pArray[pGridX][pGridY+1].hasBeenVisited()) //DOWN - PREVIOUSLY RIGHT
	{		
		//choice[size] = "RIGHT";
		choice[size] = "DOWN";
		size++;
	}
	//Go up
	//Going up means minus 1 for Y
	//Make sure this does not become less than 0
	if (curY - 1 >= 0 && !pArray[pGridX][pGridY - 1].hasBeenVisited()) //LEFT - PREVIOUSLY UP
	{
		//choice[size] = "UP";
		choice[size] = "LEFT";
		size++;
	}
	//Go down
	//Going down means plus 1 for Y
	//Make sure this does not becomme larger than columnLength
	if (curY + 1 < rowLength && !pArray[pGridX][pGridY+1].hasBeenVisited()) //RIGHT - PREVIOUSLY DOWN
	{
		//choice[size] = "DOWN";
		choice[size] = "RIGHT";
		size++;
	}
	//else
	//{
		//current.RemoveRight();
		//return current;
	//}

	int randomchoice;

	if (size != 0)
	{
		randomchoice = rand() % size;
	}
	else
	{
		return current;
	}


	if (choice[randomchoice] == "UP") //UP - PREVIOUSLY LEFT
	{
		int newX = pGridX - 1;
		if (newX >= 0)
		{
			
			cout << "UP" << endl;
			pArray[pGridX][pGridY].RemoveTop();
			pArray[newX][pGridY].RemoveBottom();
			return pArray[newX][pGridY];
			//pArray[pGridY][pGridX].RemoveLeft();
			//pArray[pGridY][newX].RemoveRight();
			//return pArray[pGridY][newX];
		}
	}
	else if (choice[randomchoice] == "DOWN" ) //DOWN - PREVIOUSLY RIGHT
	{
		int newX = pGridX + 1;
		if (newX < columnLength)
		{
			cout << "DOWN" << endl;
			
			
			pArray[pGridX][pGridY].RemoveBottom();
			pArray[newX][pGridY].RemoveTop();
			return pArray[newX][pGridY];
			
			//pArray[pGridY][pGridX].RemoveRight();
			//pArray[pGridY][newX].RemoveLeft();			
			//return pArray[pGridY][newX];
		}

	}
	else if (choice[randomchoice] == "LEFT") //LEFT - PREVIOUSLY UP
	{
		int newY = pGridY - 1;
		if (newY >= 0)
		{
			cout << "LEFT" << endl;
			pArray[pGridX][pGridY].RemoveLeft();
			pArray[pGridX][newY].RemoveRight();
			return pArray[pGridX][newY];
			//pArray[pGridY][pGridX].RemoveTop();
			//pArray[newY][pGridX].RemoveBottom();
			//return pArray[newY][pGridX];
		}

	}
	else if (choice[randomchoice] == "RIGHT") //RIGHT - PREVIOUSLY DOWN
	{
		int newY = pGridY + 1;
		if (newY < rowLength )
		{
			cout << "RIGHT" << endl;
			
			pArray[pGridX][pGridY].RemoveRight();
			pArray[pGridX][newY].RemoveLeft();
			return pArray[pGridX][newY];
			
			//pArray[pGridY][pGridX].RemoveBottom();
			//pArray[newY][pGridX].RemoveTop();
			//return pArray[newY][pGridX];
		}
	}

	
}

//just like findunvisitedneighbor but made for 30x30 grids!
Square FindUnvisitedNeighbor(Square pArray[30][30], int pGridX, int pGridY)
{

	pArray[pGridX][pGridY].markVisited();
	Square current = pArray[pGridX][pGridY];
	//Square current = pArray[pGridY][pGridX];
	int curX = current.getgridX();
	int curY = current.getgridY();
	String choice[4];
	int size = 0;


	//Go left
	//Going left means minus 1 for X
	//Make sure this does not become less than 0
	if (curX - 1 >= 0 && !pArray[pGridX - 1][pGridY].hasBeenVisited()) //UP - PREVIOUSLY LEFT
	{
		//choice[size] = "LEFT";
		choice[size] = "UP";
		size++;
	}
	//Go right
	//Going right means plus 1 for X
	//Make sure this does not become larger than rowLength
	if (curX + 1 < 30 && !pArray[pGridX][pGridY + 1].hasBeenVisited()) //DOWN - PREVIOUSLY RIGHT
	{
		//choice[size] = "RIGHT";
		choice[size] = "DOWN";
		size++;
	}
	//Go up
	//Going up means minus 1 for Y
	//Make sure this does not become less than 0
	if (curY - 1 >= 0 && !pArray[pGridX][pGridY - 1].hasBeenVisited()) //LEFT - PREVIOUSLY UP
	{
		//choice[size] = "UP";
		choice[size] = "LEFT";
		size++;
	}
	//Go down
	//Going down means plus 1 for Y
	//Make sure this does not becomme larger than columnLength
	if (curY + 1 < 30 && !pArray[pGridX][pGridY + 1].hasBeenVisited()) //RIGHT - PREVIOUSLY DOWN
	{
		//choice[size] = "DOWN";
		choice[size] = "RIGHT";
		size++;
	}
	//else
	//{
		//current.RemoveRight();
		//return current;
	//}

	int randomchoice;

	if (size != 0)
	{
		randomchoice = rand() % size;
	}
	else
	{
		return current;
	}


	if (choice[randomchoice] == "UP") //UP - PREVIOUSLY LEFT
	{
		int newX = pGridX - 1;
		if (newX >= 0)
		{

			cout << "UP" << endl;
			pArray[pGridX][pGridY].RemoveTop();
			pArray[newX][pGridY].RemoveBottom();
			return pArray[newX][pGridY];
			//pArray[pGridY][pGridX].RemoveLeft();
			//pArray[pGridY][newX].RemoveRight();
			//return pArray[pGridY][newX];
		}
	}
	else if (choice[randomchoice] == "DOWN") //DOWN - PREVIOUSLY RIGHT
	{
		int newX = pGridX + 1;
		if (newX < 30)
		{
			cout << "DOWN" << endl;


			pArray[pGridX][pGridY].RemoveBottom();
			pArray[newX][pGridY].RemoveTop();
			return pArray[newX][pGridY];

			//pArray[pGridY][pGridX].RemoveRight();
			//pArray[pGridY][newX].RemoveLeft();			
			//return pArray[pGridY][newX];
		}

	}
	else if (choice[randomchoice] == "LEFT") //LEFT - PREVIOUSLY UP
	{
		int newY = pGridY - 1;
		if (newY >= 0)
		{
			cout << "LEFT" << endl;
			pArray[pGridX][pGridY].RemoveLeft();
			pArray[pGridX][newY].RemoveRight();
			return pArray[pGridX][newY];
			//pArray[pGridY][pGridX].RemoveTop();
			//pArray[newY][pGridX].RemoveBottom();
			//return pArray[newY][pGridX];
		}

	}
	else if (choice[randomchoice] == "RIGHT") //RIGHT - PREVIOUSLY DOWN
	{
		int newY = pGridY + 1;
		if (newY < 30)
		{
			cout << "RIGHT" << endl;

			pArray[pGridX][pGridY].RemoveRight();
			pArray[pGridX][newY].RemoveLeft();
			return pArray[pGridX][newY];

			//pArray[pGridY][pGridX].RemoveBottom();
			//pArray[newY][pGridX].RemoveTop();
			//return pArray[newY][pGridX];
		}
	}


}






