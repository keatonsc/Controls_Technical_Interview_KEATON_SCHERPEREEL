/****************************************************************************
	[Project] Controls Technical Interview
*****************************************************************************
	[Lead developer] Rachel Harris, rharris at dephy dot com.
	[Contributors] 
*****************************************************************************
	[This file] elevator_problem: Moves an elevator through a building to get 
	people to the correct floor in the least amount of time. 
****************************************************************************
	[Created] 2022-11-07 | rharris | Initial creation
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************
#include "elevator_problem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************************************************
// Variable(s)
//****************************************************************************
struct building_s myBuilding;

//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************
static void initBuilding(void);
static int8_t setNextElevatorStop(struct building_s building);
static void moveElevator(struct elevator_s * elevator);
static void stopElevator(struct building_s * building);
static void drawBuilding(struct building_s building, int8_t door_status);
static void drawFloor(struct floor_s floor, int8_t floorNumber, struct elevator_s elevator, int8_t doorStatus);
static void drawElevator(struct elevator_s elevator, int8_t doorStatus);
static void delay(int16_t ms);

//****************************************************************************
// Functions You (the Interviewee) Should Edit:
//****************************************************************************

//this function gets the number of occupants in the elevator as well as getting the frequency of destination floors in the elevator
int8_t getElevatorOccupancy(int8_t *destOccupancyArr, int8_t *curPassengers){
	int8_t numOccupants = 0; // number of Occupants
	for (int8_t i=0; i < ELEVATOR_MAX_CAPACITY; i++){
		if (curPassengers[i] > -1){ //add occupant
			destOccupancyArr[curPassengers[i]] += 1; // put in frequency table
			numOccupants++;
		}
	}
	return numOccupants;
}

int8_t getMaxOccupantDestFreq(int8_t *destOccupancyArr, int8_t curFloor){
	//find the max frequency of destination floors in elevator checking nearest floors first (default up)
	int8_t max = 0;
	int8_t maxFloor = 0;
	for (int8_t i=1; i < BUILDING_HEIGHT; i++){
		// check up
		if (curFloor + i < BUILDING_HEIGHT){
			if (destOccupancyArr[curFloor + i] > max){
				max = destOccupancyArr[curFloor + i];
				maxFloor = curFloor + i;
			}
		}
		if (curFloor - i > -1){
			if (destOccupancyArr[curFloor - i] > max){
				max = destOccupancyArr[curFloor - i];
				maxFloor = curFloor - i;
			}
		}
	}
	return maxFloor;
}

//Returns the floor the elevator should STOP at next
//To stop at a floor means to open the doors and let passengers on and off. It 
//is possible to pass through a floor without stopping there.
//Note: The output should be a number between 0 and (BUILDING_HEIGHT-1), inclusive
static int8_t setNextElevatorStop(struct building_s building)
{
	// two main approaches: heuristic based or simulate forward best strategy
	// choosing heuristic based for speed for the moment

	//initialize array for storing the destination floor frequency of elevator occupants
	int8_t occupants[BUILDING_HEIGHT] = {0};
	int8_t numOccupants = getElevatorOccupancy(occupants,building.elevator.passengers); // gives 0 if elevator empty
	int8_t curFloor = building.elevator.currentFloor;
	int8_t desiredNextStop = 0; //initialize output


	// two main conditions: elevator empty or elevator has occupants
	if (numOccupants > 0){ // elevator has occupants
		//heuristic: go to floor where the majority of occupants want to go
		desiredNextStop = getMaxOccupantDestFreq(occupants,curFloor); //get max freq floor
		// to increase efficiency check for two things: people who want to get off along the way or people who can get on along the way
		if (desiredNextStop > curFloor){
			curFloor++; // begin simulating forward on this strategy
		} else {
			curFloor--;
		}
		while (curFloor != desiredNextStop){
			if (occupants[curFloor] > 0){ // check frequency array to see if someone wants to get off along the way (watch indexing)
				desiredNextStop = curFloor; // that is then the next stop
				break;
			}
			if (numOccupants < ELEVATOR_MAX_CAPACITY && (building.floors[curFloor].departures[1] > -1 || building.floors[curFloor].departures[0] > -1)){ // check if people can be picked up along the way (watch indexing)
				desiredNextStop = curFloor;
				break;
			}
			if (desiredNextStop > curFloor){ // update current floor
				curFloor++;
			} else {
				curFloor--;
			}
		}
	} else { //elevator empty
		// heuristic: go to nearest floor with people
		for (int8_t i = 1; i < BUILDING_HEIGHT; i++){ // cycle through possible next floors default to go up
			// check floor above first
			if (curFloor + i < BUILDING_HEIGHT){
				if (building.floors[curFloor+i].departures[1] > -1 || building.floors[curFloor+i].departures[0] > -1){ // if someone waiting (watch indexing)
					desiredNextStop = curFloor + i;
					break;
				}
			}
			// check floor below
			if (curFloor - i > -1){
				if (building.floors[curFloor-i].departures[1] > -1 || building.floors[curFloor+i].departures[0] > -1){ // if someone waiting
					desiredNextStop = curFloor - i;
					break;
				}
			}
		}
	}
	return desiredNextStop;
}


//****************************************************************************
//YOU CAN REVIEW THE CODE BELOW BUT DO NOT EDIT IT UNLESS YOU'RE 3000% SURE 
//THERE'S A BUG IN MY CODE! (which admittedly, is possible)
//****************************************************************************


//****************************************************************************
// Public function(s):
//****************************************************************************

//Main function to run the elevator simulation
void main(void)
{
	//Seed my random number generator
	srand(time(0));

	//Initialize the building
	initBuilding();

	//Draw the initial state of the building, with elevator doors closed
	system("clear");
	drawBuilding(myBuilding,1);
	fflush(stdout);
	delay(1000);

	//Run the elevator for a set period of time
	for(int8_t i = 0; i < 60; i++)
	{
		//Choose the next floor to stop at
		myBuilding.elevator.nextStop = setNextElevatorStop(myBuilding);

		//Move the elevator closer to the next stop
		moveElevator(&myBuilding.elevator);

		//Draw the building, with elevator doors closed
		system("clear");
		drawBuilding(myBuilding,1);
		fflush(stdout);
		delay(1000);

		//If necessary, stop the elevator to let people on/off
		if(myBuilding.elevator.currentFloor == myBuilding.elevator.nextStop)
		{
			//Redraw the building, with elevator doors open
			system("clear");
			drawBuilding(myBuilding,0);
			fflush(stdout);
			delay(1000);
			i++;

			//Let people on/off the elevator
			stopElevator(&myBuilding);

			//Redraw the building, with elevator doors open, showing the change
			system("clear");
			drawBuilding(myBuilding,0);
			fflush(stdout);
			delay(1000);
			i++;

			//Redraw the building, with elevator doors closed
			system("clear");
			drawBuilding(myBuilding,1);
			fflush(stdout);
			delay(1000);
			i++;
		}

		//Check if all people have been taken to their correct floors
		int8_t numPeopleAtDestination = 0;
		for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
		{
			numPeopleAtDestination += myBuilding.floors[f].arrivals;
		}

		if(numPeopleAtDestination == 10)
		{
			printf("SUCCESS: You got all the people through! The total time was: %i seconds!\n", i);
			return;
		}
	}

	//If the animation time's out, let them know their score.
	int8_t numPeopleAtDestination = 0;
	for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
	{
		numPeopleAtDestination += myBuilding.floors[f].arrivals;
	}
	printf("FAIL: You ran out of time. You got %i/10 people through in 60 seconds.\n", numPeopleAtDestination);
}

//****************************************************************************
// Private function(s):
//****************************************************************************
static void initBuilding(void)
{
	memset(&myBuilding, -1, sizeof(struct building_s));

	myBuilding.elevator.currentFloor = rand() % BUILDING_HEIGHT;
	myBuilding.elevator.nextStop = myBuilding.elevator.currentFloor;

	for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
	{
		for(int8_t j = 0; j < 2; j++)
		{
			int destination = rand() % BUILDING_HEIGHT;
			while(destination == f)
			{
				destination = rand() % BUILDING_HEIGHT;
			}
			myBuilding.floors[f].departures[j] = destination;
		}
		myBuilding.floors[f].arrivals = 0;
	}
}

static void moveElevator(struct elevator_s * elevator)
{
	if(elevator->nextStop < elevator->currentFloor)
	{
		elevator->currentFloor--;
	}
	else if(elevator->nextStop > elevator->currentFloor)
	{
		elevator->currentFloor++;
	}
}

static void stopElevator(struct building_s * building)
{
	//Let out anyone who needs to get off
	for(int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if(building->elevator.passengers[i] == building->elevator.currentFloor) 
		{
			building->floors[building->elevator.currentFloor].arrivals++;
			building->elevator.passengers[i] = -1;
		}
	}

	//Let in anyone who needs to get on, but only if capacity allows
	for(int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if(building->elevator.passengers[i] == -1) 
		{
			for(int8_t j = 0; j < 2; j++)
			{
				if(building->floors[building->elevator.currentFloor].departures[j] != -1)
				{
					building->elevator.passengers[i] = building->floors[building->elevator.currentFloor].departures[j];
					building->floors[building->elevator.currentFloor].departures[j] = -1;
					break;
				}
			}
		}
	}
}

static void drawBuilding(struct building_s building, int8_t doorStatus)
{
	printf("          DEPARTURES|     |ARRIVALS\n");

	for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
	{
		drawFloor(building.floors[f],f,building.elevator,doorStatus);
	}
}

static void drawFloor(struct floor_s floor, int8_t floorNumber, struct elevator_s elevator, int8_t doorStatus)
{

	printf("FLOOR %i:          ", floorNumber);
	
	for(int8_t i = 0; i < 2; i++)
	{
		if(floor.departures[i] == -1){ printf(" "); }
		else{ printf("%i",floor.departures[i]); }
	}

	if(elevator.currentFloor == floorNumber){ drawElevator(elevator, doorStatus); }
	else{printf("|     |"); }

	for(int8_t i = 0; i < floor.arrivals; i++)
	{
		printf("x");
	}

	printf("\n");
}

static void drawElevator(struct elevator_s elevator, int8_t doorStatus)
{
	if(doorStatus == 1){ printf("| "); }
	else{ printf("  "); }

	for(int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if(elevator.passengers[i] == -1){ printf("_"); }
		else{ printf("%i",elevator.passengers[i]); }
	}

	if(doorStatus == 1){ printf(" |"); }
	else{ printf("  "); }
}

static void delay(int16_t ms)
{
    //Storing start time
    clock_t startTime = clock();
 
    //Looping till required time is not achieved
    while(clock() < startTime + ms);
}
