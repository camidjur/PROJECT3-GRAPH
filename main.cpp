#include <iomanip>
#include "shortest-paths-algorithms.cpp"

using namespace std;

int getTransportationMode() {
    int mode = 0;
    cout << "Select Transportation Mode:" << endl;
    cout << "1. Walking" << endl;
    cout << "2. Biking" << endl;
    cout << "3. Car" << endl;
    cout << "4. Public Transportation" << endl;
    cout << "Enter your choice: ";
    cin >> mode;

    if (mode >= 1 && mode <= 4) {
        return mode; // Valid input, break the loop
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return getTransportationMode();
    }
}

int getSearchAlgorithm() {
    int algorithm;
    cout << "Choose Search Algorithm:" << endl;
    cout << "1. Bellman Ford's Search" << endl;
    cout << "2. Dijkstra’s Search" << endl;
    cout << "Enter your choice: ";
    cin >> algorithm;

    if (algorithm == 1 || algorithm == 2) {
        return algorithm; // Valid input, break the loop
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return getSearchAlgorithm();
    }
}

void getStartAndEndPoints(Coordinate &startPoint, Coordinate &endPoint) {
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
    std::map<Coordinate, std::vector<std::pair<double, Coordinate>>> data = getGraph(mode);
    algorithm = getSearchAlgorithm();
    getStartAndEndPoints(startPoint, endPoint);

    // wait for valid input
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
