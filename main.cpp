#include <iomanip>
#include "shortest-paths-algorithms.cpp"

using namespace std;

int getTransportationMode() {
    /* The purpose of this function is to provide the user with different modes of 
    * transportation that can be used to travel from one vertex to another. This 
    * will alter the time needed to travel this distance and the gas emissions it 
    * produces in the process. 
    */
    int mode = 0;

    // menu list 
    cout << "Select Transportation Mode:" << endl;
    cout << "1. Walking" << endl;
    cout << "2. Biking" << endl;
    cout << "3. Car" << endl;
    cout << "4. Public Transportation" << endl;
    cout << "Enter your choice: ";
    cin >> mode;

    // validating input
    if (mode >= 1 && mode <= 4) {
        return mode;
    // if the input is not valid, recursively calls function until valid input
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return getTransportationMode();
    }
}

int getSearchAlgorithm() {
    /* The purpose of this function is to provide the user with a choice as to
    * which shortest path algorithm they want the program to use to find the 
    * path from one vertex to another. 
    */
    int algorithm;

    // menu list
    cout << "Choose Search Algorithm:" << endl;
    cout << "1. Bellman Ford's Search" << endl;
    cout << "2. Dijkstra’s Search" << endl;
    cout << "Enter your choice: ";
    cin >> algorithm;

    // input validation
    if (algorithm == 1 || algorithm == 2) {
        return algorithm; 
    // if input is not valid, recursively call function until valid input
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return getSearchAlgorithm();
    }
}

void getStartAndEndPoints(Coordinate &startPoint, Coordinate &endPoint) {
    /* The purpose of this function is to allow the user to enter a latitude and 
    * a longitude for the source vertex as well as a latitude and longitude for the
    * target vertex. This is later checked with a data set that holds all the existing data 
    * points for Gainesville to validate the input. 
    */
    // source vertex
    cout << "Enter the starting latitude coordinate: ";
    cin.ignore(); // To clear the return character from the buffer
    string latitude;
    getline(cin, latitude);
    double lat1 = std::stod(latitude);

    cout << "Enter the starting longitude coordinate: ";
    string longitude;
    getline(cin, longitude);
    double lon1 = std::stod(longitude);
    startPoint = Coordinate {lat1, lon1};

    // target vertex
    cout << "Enter the ending latitude coordinate: ";
    getline(cin, latitude);
    double lat2 = std::stod(latitude);

    cout << "Enter the ending longitude coordinate: ";
    getline(cin, longitude);
    double lon2 = std::stod(longitude);
    endPoint = Coordinate {lat2, lon2};

    cout << endl;
}

int main() {
    int mode, algorithm;
    Coordinate startPoint, endPoint;

    // Title + style
    string authors = "Designed by Camila Djurinsky Zapolski, Pietro Landell, and Ethan Wilson";
    cout << setw((80 + string("Shortest Path Finder").length()) / 2) << "Shortest Path Finder" << endl;
    cout << setw((80 + authors.length()) / 2) << authors << endl;
    cout << endl;

    mode = getTransportationMode();
    // sets up map to utilize as graph representation
    std::map<Coordinate, std::vector<std::pair<double, Coordinate>>> data = getGraph(mode);
    algorithm = getSearchAlgorithm();
    // calls for input
    getStartAndEndPoints(startPoint, endPoint);

    // wait for valid input if initial input was not in data set 
    while (data.find(startPoint) == data.end() || data.find(startPoint) == data.end()) {
        cout << "Source or target coordinates not found in the graph." << endl;
        getStartAndEndPoints(startPoint, endPoint);
    }

    cout << "Finding the shortest path..." << endl;

    if (algorithm == 1) {
        bellman(data, mode, startPoint, endPoint);
    } else {
        dijkstra(data, mode, startPoint, endPoint);
    }

    return 0;
}
