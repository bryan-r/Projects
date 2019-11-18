// Project 4 
// CS325 Winter 2016
// Group 8 - Rachael McConnell, Bryan Resner, Ashley Grealish
//
// Read input about cities from file and store. Split the cities into 4 neighborhoods based on their location on the 2d grid/graph. Go through each neighborhood
// creating a path between all cities in that neighborhood.  Cities are selected using the greedy method. Once all cities have been included on the tour, switch 
// to the next neighborhood. Continue until all neighborhoods have been reached and all cities have been included on the tour. Output the total distance traveled
// and the cities in order that they are reached on the tour.
//
// https://web.tuke.sk/fei-cit/butka/hop/htsp.pdf

//#include <cmath>
//#include <cstdlib>
#include <math.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

// Global variables - not the best coding practice, but makes life much easier
int **distMatrix = NULL;
bool useDistMatrix = true;

// Struct consisting of the city's identifier #, the city's x-coordinate, the city's y-coordinate, and the neighborhood the city is located in
// The first three attributes are read in from a file, the fourth is calculated based on the x and y coordinates.
struct City{
	int cityid;
	int xcoord;
	int ycoord;
	int neighborhood;
};


// Retrieve id and x and y coordinates for each city in the tour
vector<City> getInput(char* file, int & xmax, int& ymax){
	ifstream inputFile(file);
	vector<City> allCities;
	int cityid, x, y;

	//http://stackoverflow.com/questions/3946558/c-read-from-text-file-and-separate-into-variable
	// Create a struct for each line in the inputFile
	// Search for the largest x and y value (to be used to find the midpoint to split into neighborhoods)
	while (inputFile >> cityid >> x >> y){
		City c = { cityid, x, y };
		if (c.xcoord > xmax)
			xmax = x;
		if (c.ycoord > ymax)
			ymax = y;

		// Save instance of struct
		allCities.push_back(c);
	}
	inputFile.close();

	return allCities;
}

// Split the 2D graph into 4 neighborhoods based on the location of each city
// Each neighborhood may have a different amount of cities
void assignNeighborhoods(int xmax, int ymax, vector<City> cities, vector<City> neighborhoods[4]) {
	int xmid = xmax / 2;
	int ymid = ymax / 2;
	int i;

	// Loop through all cities, adding each one to a neighborhood
	// Set the neighborhood for the city and add the city to the vector for the specific city
	for (i = 0; i < cities.size(); i++){
    // Put all cities in just one neighborhood - this becomes the nearest neighbor heuristic for tour creation
	  cities[i].neighborhood = 0;
		neighborhoods[0].push_back(cities[i]);
    // Comment out the if-else if set below when using nearest neighbor without neighborhoods
    /*
		if (cities[i].xcoord <= xmid && cities[i].ycoord <= ymid){
			cities[i].neighborhood = 0;
			neighborhoods[0].push_back(cities[i]);
		}
		else if (cities[i].xcoord <= xmid && cities[i].ycoord >= ymid){
			cities[i].neighborhood = 1;
			neighborhoods[1].push_back(cities[i]);
		}
		else if (cities[i].xcoord >= xmid && cities[i].ycoord >= ymid){
			cities[i].neighborhood = 2;
			neighborhoods[2].push_back(cities[i]);
		}
		else{
			cities[i].neighborhood = 3;
			neighborhoods[3].push_back(cities[i]);
		}
    */
	}
}


// Get euclidean distance between city C1 and city C2
int getEuclideanDistance(City C1, City C2){
	//int c1 = pow((float)C1.xcoord - C2.xcoord, 2);
	//int c2 = pow((float)C1.ycoord - C2.ycoord, 2);
	//int d = round(sqrt(c1 + c2));
	//return d;

  double xFactor = (C1.xcoord - C2.xcoord) * (C1.xcoord - C2.xcoord);
  double yFactor = (C1.ycoord - C2.ycoord) * (C1.ycoord - C2.ycoord);
  int distBetween = (int)(sqrt(xFactor + yFactor) + 0.5);
  return distBetween;

}

// Use the distance matrix to get the distances
int getMatrixDistance(City C1, City C2)
{
  int dist = 0;
  if (distMatrix != NULL)
  {
    dist = distMatrix[C1.cityid][C2.cityid];
  }

  return dist;
}

// Compute the distance matrix
void computeDistMatrix(vector<City> cities)
{
  int numCities = cities.size();
  distMatrix = new int * [numCities];
  for (int i = 0; i < numCities; i++)
  {
    distMatrix[i] = new int [numCities];
  }

  // Loop through all the cities and compute the distance between each pair and
  // add it to the matrix.  Since the distances going to or coming from each
  // pair of cities is symmetric, we can just copy the same distance into both
  // positions in the distance matrix
  int distBetween;
  int index1;
  int index2;
  for (int i = 0; i < numCities; i++)
  {
    distMatrix[i][i] = 0;
    for (int j = i+1; j < numCities; j++)
    {
      distBetween = getEuclideanDistance(cities[i], cities[j]);
      index1 = cities[i].cityid;
      index2 = cities[j].cityid;
      distMatrix[index1][index2] = distBetween;
      distMatrix[index2][index1] = distBetween;
    }
  }
}

// Save cities in the order that they are visited
// Returns the total distance traveled in the neighborhood
int getPath(vector<City> &unvisitedCities, vector<City> &tspTour, City from, City &to ){

	int index = 0, distance = 0, totalDistance = 0, distanceSoFar, i;

	// Add starting city to the tour and remove from the unvisited cities list
  City tc_start = { unvisitedCities[index].cityid, unvisitedCities[index].xcoord, unvisitedCities[index].ycoord };
	tspTour.push_back(tc_start);
	// Remove that city from the list because it has already been visited
	unvisitedCities.erase(unvisitedCities.begin() + index);

	// Loop through unvisited cities until all cities have been visited
	while (!unvisitedCities.empty()){
		distanceSoFar = 100000000;
		for (i = 0; i < unvisitedCities.size(); i++){
      if (useDistMatrix)
      {
  			distance = getMatrixDistance(from, unvisitedCities[i]);
      }
      else
      {
  			distance = getEuclideanDistance(from, unvisitedCities[i]);
      }
			// Looking for the city closest to the current city
			// Save the location of the closest city
			if (distance < distanceSoFar){
				distanceSoFar = distance;
				index = i;
			}
		}
		from = unvisitedCities[index];
		// Add closest city to tour
    City tc_closest = { unvisitedCities[index].cityid, unvisitedCities[index].xcoord, unvisitedCities[index].ycoord };
		tspTour.push_back(tc_closest);
		// Increment the distance
		totalDistance += distanceSoFar;
		// Remove City that was just visited from the unvisited cities
		unvisitedCities.erase(unvisitedCities.begin() + index);
	}
	// Update the last city to the city just added
	// This is needed when switching neighborhoods
	to = from;

	return totalDistance;
}

// Go from the last city in one neighborhood to the first city in the next
int switchNeighborhood(vector<City> neighborhoods[4], int j, int &index, int &distance, int &distanceSoFar, City &lastCity) {
	int switchDistance = 0;
	// Loop through the new neighborhood to find the closest city to the last city in the previous neighborhood
	for (int i = 0; i < neighborhoods[j].size(); i++){
    if (useDistMatrix)
    {
  		distance = getMatrixDistance(lastCity, neighborhoods[j][i]);
    }
    else
    {
  		distance = getEuclideanDistance(lastCity, neighborhoods[j][i]);
    }
		if (distance < distanceSoFar){
			distanceSoFar = distance;
			// Save the location of the closest city
			index = i;
		}
	}
  if (useDistMatrix)
  {
  	switchDistance = getMatrixDistance(lastCity, neighborhoods[j][index]);
  }
  else
  {
  	switchDistance = getEuclideanDistance(lastCity, neighborhoods[j][index]);
  }
	return switchDistance;
}

// Get the length of the tour
int getTourLength(vector<City> tour)
{
  int tourLen = 0;
  int numCities = (int)tour.size();
  for (int i = 0; i < numCities-1; i++)
  {
    if (useDistMatrix)
    {
      tourLen += getMatrixDistance(tour[i], tour[i+1]);
    }
    else
    {
      tourLen += getEuclideanDistance(tour[i], tour[i+1]);
    }
  }

  // Now we need to add in the length to get back to the first city
  if (useDistMatrix)
  {
    tourLen += getMatrixDistance(tour[numCities-1], tour[0]);
  }
  else
  {
    tourLen += getEuclideanDistance(tour[numCities-1], tour[0]);
  }

  return tourLen;
}

// Swap a pair of cities on the tour
void swapTwoCities(vector<City> &tour, int index1, int index2)
{
  // Index 2 should always be larger than index 1.  If not, then
  // get out of here because we will screw up the tour if it is
  // the other way around
  if (index1 < index2)
  {
    vector<City> temp;
    City c;

    for (int i = index2; i >= index1; i--)
    {
      City c = tour[i];
      temp.push_back(c);
    }

    int count = 0;
    for (int i = index1; i <= index2; i++)
    {
      c = temp[count];
      tour[i] = c;
      ++count;
    }
  }
}

// Two opt tour improvement heuristic
int improveTwoOpt(vector<City> &tour)
{
  int numCities = (int)tour.size();
  bool startOver = false;
  int numSwaps = 0;

  //int minTourLen = getTourLength(tour);

  // Set the while loop control so we can bail based on some criteria
  int dist1;
  int dist2;
  int delta;
  bool done = false;
    
  double timeTaken = 0.0;
  clock_t perfTime = clock();

  // Loop until no more swaps available to improve the tour length
  while (!done)
  {
    startOver = false;
    for (int i = 1; i < numCities && !startOver; i++)
    {
      for (int j = i+1; j < numCities-1 && !startOver; j++)
      {
        // Use triangle inequality to figure out if a swap makes sense
        if (useDistMatrix)
        {
          dist1 = getMatrixDistance(tour[i-1], tour[j]) + getMatrixDistance(tour[i], tour[j+1]);
          dist2 = getMatrixDistance(tour[i-1], tour[i]) + getMatrixDistance(tour[j], tour[j+1]);
        }
        else
        {
          dist1 = getEuclideanDistance(tour[i-1], tour[j]) + getEuclideanDistance(tour[i], tour[j+1]);
          dist2 = getEuclideanDistance(tour[i-1], tour[i]) + getEuclideanDistance(tour[j], tour[j+1]);
        }
        delta = dist2 - dist1;
        // If there are a large number of cities, only swap if the delta improvement is "large enough"
        //if (dist1 < dist2)
        //if ( ((numCities > 1000) && (delta > 100)) || ((numCities <= 1000) && (delta > 0)) )
        if (delta > 0)
        {
          // This swap reduces the length of the tour, so make the swap
          swapTwoCities(tour, i, j);
          //minTourLen = getTourLength(tour);
          ++numSwaps;
          startOver = true;
        }
        else
        {
          startOver = false;
        }
          perfTime = clock() - perfTime;
          timeTaken += ((double)perfTime)/CLOCKS_PER_SEC;
          perfTime = clock();
          
          //When time adds up to over a minute it will stop trying to improve the tour
          //In other words, it will only do a minutes worth of improvements
          if (timeTaken > 60.0) {
              startOver = true;
              done = true;
          }
      }
    }

    // If we aren't starting over, then there are no more swaps that improve the length of the tour
    if (!startOver)
    {
      break;
    }
  }

  return numSwaps;
}


int main(int argc, char * argv[]){

	// input file should be argv[1]
	// Check if argc == 2
	if (argc != 2)
	{
		printf("Only command line argument is the name of the file that defines the cities for the TSP\n");
		printf("Please add the file name and try again!\n");
		return 1;
	}

  // Set whether we want to use the distance matrix (compute between city distances once and store
  // in a matrix) or recompute each time we need them.  When doing the two-opt improvement step,
  // the matrix should be more efficient than recomputing distances over and over.
  useDistMatrix = false;

  // For timing
  double timeTakenForDistMatrix = 0.0;
  double timeTakenForTourConstruct = 0.0;
  double timeTakenForTourImprove = 0.0;
  clock_t perfTime;

	//char* inputFile = "test-input-3.txt";
	char inputFile[80];
	strcpy(inputFile, argv[1]);

	char outputFile[80];
	ofstream oFile;
	int xmax = 0, ymax = 0, distance=0, index=0, totalDistance = 0, distanceSoFar = 100000000;
	City lastCity;
	vector<City> tourCities, cities, neighborhoods[4];
	
	// Open the input file and store info about the cities in the tour
	cities = getInput(inputFile, xmax, ymax);

  // If we are going to use the distance matrix, let's compute it
  if (useDistMatrix)
  {
    perfTime = clock();
    computeDistMatrix(cities);
    perfTime = clock() - perfTime;
    timeTakenForDistMatrix = ((double)perfTime)/CLOCKS_PER_SEC;
  }

	// Assign each city to a neighborhood, based on its x and y coordinates
	assignNeighborhoods(xmax, ymax, cities, neighborhoods);


	/////// START CALCULATING THE TOUR

  // Start the clock for tour construction
  perfTime = clock();

	// Get path between all of the cities in the first neighborhood
	totalDistance += getPath(neighborhoods[0], tourCities, neighborhoods[0][index], lastCity);
	// Switch to the next neighborhood
  if (neighborhoods[1].size() > 0)
  {
	  totalDistance += switchNeighborhood(neighborhoods, 1, index, distance, distanceSoFar, lastCity);
	  // Get path between all of the cities in the second neighborhood
	  totalDistance += getPath(neighborhoods[1], tourCities, neighborhoods[1][index], lastCity);
  }
  if (neighborhoods[2].size() > 0)
  {
  	distanceSoFar = 100000000;
	  // Switch to next neighborhood
	  totalDistance += switchNeighborhood(neighborhoods, 2, index, distance, distanceSoFar, lastCity);
	  //Get path between all of the cities in the third neighborhood
	  totalDistance += getPath(neighborhoods[2], tourCities, neighborhoods[2][index], lastCity);
  }
  if (neighborhoods[3].size() > 0)
  {
  	distanceSoFar = 100000000;
	  // Switch to next neighborhood
	  totalDistance += switchNeighborhood(neighborhoods, 3, index, distance, distanceSoFar, lastCity);
	  // Get path between all of the cities in the last neighborhood
	  totalDistance += getPath(neighborhoods[3], tourCities, neighborhoods[3][index], lastCity);
  }

  // Stop clock on tour construction
  perfTime = clock() - perfTime;
  timeTakenForTourConstruct = ((double)perfTime)/CLOCKS_PER_SEC;

  // The total distance does not include the distance to get back to the starting city,
  // add this distance in
  int numCities = tourCities.size();
  totalDistance += getEuclideanDistance(tourCities[0], tourCities[numCities-1]);

	////// END CALCULATING THE TOUR

  // Double check that we got the correct tour length
  int tourLenBeforeTwoOpt = getTourLength(tourCities);

  // Start the clock for tour improvement
  perfTime = clock();

  // Let's see if we can improve the tour by performing a two-opt improvement on the
  // tour to see if we can swap pairs of cities to improve the tour, i.e., shorten
  // the length of the tour
  //int numSwaps = 0;
  int numSwaps = improveTwoOpt(tourCities);

  // Stop clock on tour improvement
  perfTime = clock() - perfTime;
  timeTakenForTourImprove = ((double)perfTime)/CLOCKS_PER_SEC;

  // Let's see that the two-opt actually improved the tour
  int tourLenAfterTwoOpt = getTourLength(tourCities);

  // Output results to CSV file so we can view the tour in Excel

	char csvOutFile[80];
	strcpy(csvOutFile, inputFile);
	strcat(csvOutFile, "_tour_nearest_wImprove.csv");
	oFile.open(csvOutFile);
	//oFile << totalDistance << endl;

  // ***** Need to edit this line to sync with the example that is being executed ************
  double tourPercentOverEx3 = tourLenAfterTwoOpt / 1573084.0;

  oFile << "Optimal Tour Length for Example 1: 108159  Example 2: 2579  Example 3: 1573084" << endl;
  oFile << "Tour Construct Length: " << tourLenBeforeTwoOpt << endl;
  oFile << "Tour Construct Time: " << timeTakenForTourConstruct << endl;

  // ***** Need to edit this line to sync with the example that is being executed ************
  oFile << "Tour Improve Length: " << tourLenAfterTwoOpt << "  (" << tourPercentOverEx3 << " wrt to optimal)" << endl;

  oFile << "Tour Improve Time: " << timeTakenForTourImprove << endl;
  oFile << "Tour Improve number of swaps performed: " << numSwaps << endl;
  if (useDistMatrix)
  {
    oFile << "Using Distance Matrix - compute time: " << timeTakenForDistMatrix << endl;
  }
  else
  {
    oFile << "Computing Euclidean Distance between cities every time" << endl;
  }

  // Write the tour information to the spreadsheet so we can plot it
	for (int i = 0; i < tourCities.size(); i++)
  {
		oFile << tourCities[i].cityid << "," << tourCities[i].xcoord << "," << tourCities[i].ycoord << endl;
  }
  // Add the first city to close the tour
  oFile << tourCities[0].cityid << "," << tourCities[0].xcoord << "," << tourCities[0].ycoord << endl;
  // Close the CSV file
	oFile.close();


	// Output results to file
	strcpy(outputFile, inputFile);
	strcat(outputFile, ".tour");
	
	oFile.open(outputFile);
	//oFile << totalDistance << endl;
	oFile << tourLenAfterTwoOpt << endl;
	for (int i = 0; i < tourCities.size(); i++)
		oFile << tourCities[i].cityid << endl;
	oFile.close();

	return 0;
}