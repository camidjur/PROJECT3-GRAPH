#include <iostream>
#include <string>
#include <iomanip>
#include <utility>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
const string API_KEY = "pk.185ce0cafa395a3cbce614738848d26c";

/*
- API Documentation can be found at https://docs.locationiq.com/docs/introduction
- Helper function to handle the response from libcurl
- Handles data received from the HTTP request.
- contents: a pointer to the delivered data, size: the size of one data unit in bytes, numdata: number of data units, s: pointer to a string
 */

static size_t WriteCallback(void *contents, size_t size, size_t numdata, string *s) {
    // Calculate the total size of the data (num of units * the size of each unit)
    size_t newLength = size * numdata;

    try {
        // Add the data to the string passed by the user
        s->append((char*)contents, newLength);
        // Returns the total size of the data handled in this call, libcurl uses this value to know how much data has been processed
        return newLength;
    } catch(std::bad_alloc &e) {
        // If there's a memory allocation error during append, catch the exception
        return 0;
    }
}

// Function to get geographic coordinates for a given address.
// It uses the LocationIQ Geocoding API to convert a physical address into geographic coordinates.
// Also uses the curl library for URL handling
pair<double, double> getCoordinates(const string &address, const string &apiKey) {
    // Initializes variables for the libcurl function and the response.
    CURL *curl;
    CURLcode res;
    string readBuffer;

    /* Builds the URL for the API request, the address is URL-encoded to properly be used
     * in an HTTP request.
     */
    string url = "https://eu1.locationiq.com/v1/search.php?key=" + apiKey + "&q=" + curl_easy_escape(curl, address.c_str(), address.length()) + "&format=json";

    // Begins a curl session.
    curl = curl_easy_init();
    if(curl) {
        // Sets the options for the curl session which are the URL, the function to handle the response which is (WriteCallback),
        // and a pointer to a string where the response is stored.
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Performs the HTTP request, data is stored in readBuffer.
        res = curl_easy_perform(curl);
        // Cleans up the curl session.
        curl_easy_cleanup(curl);

        // Checks if the request was performed successfully.
        if (res != CURLE_OK) {
            // If there was an error, return the default coords
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            return make_pair(0.0, 0.0);
        }

        try {
            // Scans the JSON response to extract the latitude and longitude.
            // The JSON response is expected to be in a specific format returned by LocationIQ.
            auto jsonResponse = json::parse(readBuffer);
            string latString = jsonResponse.at(0).at("lat").get<string>();
            string lonString = jsonResponse.at(0).at("lon").get<string>();

            // Convert the latitude and longitude from string to double.
            double latitude = stod(latString);
            double longitude = stod(lonString);

            // Return the coordinates as a pair of doubles.
            return make_pair(latitude, longitude);
        } catch (const json::exception& e) {
            // If there was an error in parsing the JSON response, output it and return default coordinates.
            cerr << "JSON parsing error: " << e.what() << endl;
            return make_pair(0.0, 0.0);
        }
    }
    // If curl initialization fails, return default coordinates.
    return make_pair(0.0, 0.0);
}

string getStartAndEndPoints(const string &apiKey) {
    string startAddress, endAddress;
    pair<double, double> startCoords, endCoords;
    string result;

    cout << "Enter Start Point Address: ";
    cin.ignore(); // To clear the return character from the buffer to be safe
    getline(cin, startAddress);

    cout << "Enter End Point Address: ";
    getline(cin, endAddress);

    // Convert addresses to coordinates
    startCoords = getCoordinates(startAddress, apiKey);
    endCoords = getCoordinates(endAddress, apiKey);

    // Checks if the coordinates are valid (not (0,0))
    if (startCoords.first != 0.0 && startCoords.second != 0.0 && endCoords.first != 0.0 && endCoords.second != 0.0) {
        // Stores the coordinates as a string in the format "startcoord1 startcoord2 endcoord1 endcoord2"
        result =  to_string(startCoords.first) + " " + to_string(startCoords.second) + "\n";
        result +=  to_string(endCoords.first) + " " + to_string(endCoords.second);
    } else {
        cout << "Invalid address(es). Please try again." << endl;
        return getStartAndEndPoints(apiKey); // Recursive call for retry
    }

    return result;
}

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
    cout << endl;
}

int getSearchAlgorithm() {
    int algorithm;
    cout << "Choose Search Algorithm:" << endl;
    cout << "1. Depth First Search" << endl;
    cout << "2. Dijkstra’s Search" << endl;
    cout << "Enter your choice: ";
    cin >> algorithm;

    if (algorithm == 1 || algorithm == 2) {
        return algorithm; // Valid input, break the loop
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return getSearchAlgorithm();
    }
    cout << endl;
}

// Old Function
/*
void getStartAndEndPoints(string &startPoint, string &endPoint) {
    cout << "Enter Start Point: ";
    cin.ignore(); // To clear the return character from the buffer
    getline(cin, startPoint);

    cout << "Enter End Point: ";
    getline(cin, endPoint);

    cout << endl;
}
*/

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

    string coords = getStartAndEndPoints(API_KEY);

    cout << coords << endl;
    

    /*
    // For test/debug
    cout << endl;
    cout << "Here's the following inputs:" << endl;
    cout << "Mode: " << mode << endl << "Algorithm: " << algorithm << endl;
    cout << "Start Point: " << startPoint << endl << "End Point: " << endPoint << endl;
    cout << endl;
    */

    cout << "Finding the shortest path..." << endl << endl;

    // Add  shortest path finding code here

    return 0;
}
