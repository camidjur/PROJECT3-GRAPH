#include <string>
#include <iomanip>
#include <utility>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include "shortest-paths-algorithms.cpp"

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
        // and a pointer to a string where the response is stored
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Performs the HTTP request, data is stored in readBuffer
        res = curl_easy_perform(curl);
        // Cleans up the curl session
        curl_easy_cleanup(curl);

        // Checks if the request was performed successfully
        if (res != CURLE_OK) {
            // If there was an error, return the default coords
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            return make_pair(0.0, 0.0);
        }

        try {
            // Searches the JSON response to find "lat" & "lon" and returns them
            // The JSON response is in a specific format defined by LocationIQ
            auto jsonResponse = json::parse(readBuffer);
            string latString = jsonResponse.at(0).at("lat").get<string>();
            string lonString = jsonResponse.at(0).at("lon").get<string>();

            // Convert the latitude and longitude from string to double
            double latitude = stod(latString);
            double longitude = stod(lonString);

            // Return the coords as a pair of doubles.
            return make_pair(latitude, longitude);
        } catch (const json::exception& e) {
            // If there was an error in searching the JSON response, output it and return default coords
            cerr << "JSON parsing error: " << e.what() << endl;
            return make_pair(0.0, 0.0);
        }
    }
    // If curl initialization fails, return default coordinates
    return make_pair(0.0, 0.0);
}

string getStartAndEndPoints(const string &apiKey) {
    string startAddress, endAddress;
    pair<double, double> startCoords, endCoords;
    string result;

    cout << "Enter the address in the format: Street Number, Street Name, City, State, Country." << endl;
    cout << "Example: '123 Main St, Gainesville, FL, USA'" << endl;
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
        return mode;
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return getTransportationMode();
    }
}

int getSearchAlgorithm() {
    int algorithm;
    cout << "Choose Search Algorithm:" << endl;
    cout << "1. Depth First Search" << endl;
    cout << "2. Dijkstra’s Search" << endl;
    cout << "Enter your choice: ";
    cin >> algorithm;

    if (algorithm == 1 || algorithm == 2) {
        return algorithm; // Valid input
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return getSearchAlgorithm();
    }
    cout << endl;
}

int main() {
    int mode, algorithm;
    string startPoint, endPoint; // Entered as addresses and stores as coordinates

    // Title + style
    string authors = "Designed by Camila Djurinsky Zapolski, Pietro Landell, and Ethan Wilson";
    cout << setw((80 + string("Shortest Path Finder").length()) / 2) << "Shortest Path Finder" << endl;
    cout << setw((80 + authors.length()) / 2) << authors << endl;
    cout << endl;

    // Gather all info requires to run algorithms
    mode = getTransportationMode();
    algorithm = getSearchAlgorithm();
    string coords = getStartAndEndPoints(API_KEY);
    double lat1, lon1, lat2, lon2;
    std::istringstream iss(coords);
    iss >> lat1 >> lon1 >> lat2 >> lon2;
    Coordinate start{lat1, lon1};
    Coordinate end{lat2, lon2};

    cout << "Finding the shortest path..." << endl << endl;

    // Make the data
    std::map<Coordinate, std::vector<std::pair<double, Coordinate>>> data = getGraph(mode);
    if (algorithm == 1) {
        bellman(data, mode, start, end);
    } else {
        dijkstra(data, mode, start, end);
    }

    return 0;
}
