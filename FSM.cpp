#include <iostream>
#include <map>
#include <climits>
#include <string>
using namespace std;

//Enums
enum FloorType {
	Cramped,
	Normal,
	Empty
	} ;

enum Direction {
	Up,
	Down
} ;


//Constants
#define MAX_AMMO 25
#define STARTING_FLOOR 0
#define STARTING_DIRECTION Up
#define RESPAWN_FLOOR 0
#define RESPAWN_DIRECTION Up

map<int, FloorType> floorMap = {
	{0, Cramped} ,
	{1, Empty} ,
	{2, Empty} ,
	{3, Cramped} ,
	{4, Normal} ,
	{5, Normal} ,
	{6, Empty} ,
	{7, Normal} ,
	{8, Normal} ,
	{9, Cramped}
} ;

map<FloorType, int> noOfBulletsPerFloor = {
	{Cramped, 0} ,
	{Empty, 3} ,
	{Normal, 5}
} ;


//GameMap -- Defines the Environment
class GameMap {
public:
	map <int, FloorType> floors ;
	int maxFloor = INT_MIN ;
	int minFloor = INT_MAX ;

	FloorType getFloor(int floorNo) ;
	void addFloor(int floorNo, FloorType floorType) ;
	int getNoOfFloors(void) ;
} ;

FloorType GameMap::getFloor(int floorNo) {
	return floors[floorNo] ;
}

void GameMap::addFloor(int floorNo, FloorType floorType) {
	floors[floorNo] = floorType ;
	if ( floorNo < minFloor)
		minFloor = floorNo ;
	else if ( floorNo > maxFloor)
		maxFloor = floorNo ;
}

int GameMap::getNoOfFloors(void) {
	return floors.size() ;
}


//GameState -- Defines Gamestate ( How the Game Universe is )
class GameState {
public:
	int currentFloor = STARTING_FLOOR;
	int ammoLeft = MAX_AMMO;
	int ammoUsed = 0;
	Direction movingDirection = STARTING_DIRECTION ;
	int enemiesKilled = 0 ;
	int soldiersDead = 0 ;

	void move(GameMap gameMap) ;
	void reverse(void) ;
	void die(void) ;
	void shoot(GameMap gameMap, int noOfBullets) ;
	void print(void) ;
	int getAmmoUsed(void) ;
} ;

void GameState::move(GameMap gameMap){
	switch(movingDirection) {
		case Up:
			if (currentFloor < gameMap.maxFloor)
				currentFloor++ ;
			break;
		case Down:
			if (currentFloor > gameMap.minFloor)
				currentFloor-- ;
			break;
	}
}

void GameState::reverse(void) {
	switch(movingDirection) {
		case Up :
			movingDirection = Down ;
			break ;
		case Down :
			movingDirection = Up ;
			break ;
	}
}

void GameState::die(void) {
	soldiersDead++ ;
	movingDirection = RESPAWN_DIRECTION ;
	ammoLeft = MAX_AMMO ;
	currentFloor = RESPAWN_FLOOR ;
}

void GameState::shoot(GameMap gameMap, int noOfBullets) {
	if(ammoLeft < noOfBullets){
		ammoUsed += ammoLeft ;
		die() ;
	}
	else {
		ammoLeft -= noOfBullets ;
		ammoUsed += noOfBullets ;
		enemiesKilled++ ;
		move(gameMap);
	}
}

void GameState::print(void) {
	cout << "currentFloor : " << currentFloor << endl ;
	cout << "ammoLeft : " << ammoLeft << endl ;
	cout << "movingDirection : " << movingDirection << endl ;
	cout << "enemiesKilled : " << enemiesKilled << endl ;
	cout << "soldiersDead : " << soldiersDead << endl ;
}

int GameState::getAmmoUsed(void) {
	return ammoUsed;
}


//Functions to Progress the Game
void shootAndMove(GameMap gameMap, GameState &currentGameState, int numberOfFoes) {
	currentGameState.shoot(gameMap,noOfBulletsPerFloor[gameMap.getFloor(currentGameState.currentFloor)]) ;
}


GameState nextState(GameMap gameMap, GameState &currentGameState, char command) {
	switch ( command ) {
		case 'U' :
			currentGameState.movingDirection = Up ;
			currentGameState.move(gameMap) ;
			break ;
		case 'D' :
			currentGameState.movingDirection = Down ;
			currentGameState.move(gameMap) ;
			break ;
	}
	return currentGameState ;
}

GameState nextState(GameMap gameMap, GameState &currentGameState, int numberOfFoes) {
	switch ( numberOfFoes ) {
		case 0 :
			currentGameState.move(gameMap) ;
			break ;
		case 1 :
			shootAndMove(gameMap, currentGameState, numberOfFoes) ;
			break ;
		case 2 :
			currentGameState.reverse() ;
			currentGameState.move(gameMap) ;
			break ;
	}
	return currentGameState ;
}

GameMap initializeMap() {
	GameMap gameMap ;
	for (map<int,FloorType>::iterator it=floorMap.begin(); it!=floorMap.end(); ++it)
    	gameMap.addFloor(it->first,it->second);

    return gameMap ;
}

void printOutput(GameState gameState) {
	cout << "Current Floor - " << gameState.currentFloor << endl ;
	cout << "Ammo Consumed - " << gameState.getAmmoUsed() << endl ;
	cout << "Foes Eliminated - " << gameState.enemiesKilled << endl ;
	cout << "Soldiers Dead - " << gameState.soldiersDead << endl ;
}

int main() {
	string input ;

	GameMap gameMap = initializeMap() ;
	GameState currentGameState ;

	getline(cin, input) ;

	for ( string::iterator it=input.begin(); it!=input.end(); ++it) {
		switch(*it){
			case 'U' :
			case 'D' :
				nextState(gameMap, currentGameState, *it) ;
				break ;
			case '0' :
			case '1' :
			case '2' :
				nextState(gameMap, currentGameState, (int)(*it - '0')) ;
				break ;
		}
	}

	printOutput(currentGameState) ;
	return 0;
}
