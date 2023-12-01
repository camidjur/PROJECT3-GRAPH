#include <iostream>
#include <string>
#include <iomanip>

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

void getStartAndEndPoints(string &startPoint, string &endPoint) {
    cout << "Enter Start Point: ";
    cin.ignore(); // To clear the return character from the buffer
    getline(cin, startPoint);

    cout << "Enter End Point: ";
    getline(cin, endPoint);

    cout << endl;
}

int main() {
    int mode, algorithm;
    string startPoint, endPoint; //Stored as addresses?

    // Title + style
    string authors = "Designed by Camila Djurinsky Zapolski, Pietro Landell, and Ethan Wilson";
    cout << setw((80 + string("Shortest Path Finder").length()) / 2) << "Shortest Path Finder" << endl;
    cout << setw((80 + authors.length()) / 2) << authors << endl;
    cout << endl;

    mode = getTransportationMode();
    algorithm = getSearchAlgorithm();
    getStartAndEndPoints(startPoint, endPoint);

    /*
    // For test/debug
    cout << endl;
    cout << "Here's the following inputs:" << endl;
    cout << "Mode: " << mode << endl << "Algorithm: " << algorithm << endl;
    cout << "Start Point: " << startPoint << endl << "End Point: " << endPoint << endl;
    cout << endl;
    */

    cout << "Finding the shortest path..." << endl;

    // Add  shortest path finding code here

    return 0;
}
