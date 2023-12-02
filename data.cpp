#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <stdexcept>

// Define a structure to represent a coordinate (latitude, longitude) pair
struct Coordinate {
    double latitude;
    double longitude;

    // Custom comparison function for Coordinates
    bool operator<(const Coordinate& other) const {
        return latitude < other.latitude || (latitude == other.latitude && longitude < other.longitude);
    }
    // Custom comparison function for Coordinates
    bool operator==(const Coordinate& other) const {
        return latitude == other.latitude && longitude == other.longitude;
    }
};

// Function to calculate the Euclidean distance between two coordinates
double calculateDistance(const Coordinate& coord1, const Coordinate& coord2) {
    return 61.8 * std::sqrt(std::pow(coord2.latitude - coord1.latitude, 2) + std::pow(coord2.longitude - coord1.longitude, 2));
}

std::map<Coordinate, std::vector<std::pair<double, Coordinate>>> getGraph(int mode) {
    std::string file;
    if (mode == 1) {
        file = "/Users/Pietro/Desktop/COP3530/OSMTrial/data/walking.geojson";
    } else if (mode == 2) {
        file = "/Users/Pietro/Desktop/COP3530/OSMTrial/data/biking.geojson";
    } else {
        file = "/Users/Pietro/Desktop/COP3530/OSMTrial/data/driving.geojson";
    }
    // Open the GeoJSON file
    std::ifstream inputFile(file);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the GeoJSON file." << std::endl;
    }

    // Define the graph as a map with coordinates as keys and vectors of weights and adjacent coordinates as values
    std::map<Coordinate, std::vector<std::pair<double, Coordinate>>> graph;


    // Read and parse the GeoJSON file
    std::string line;
    while(std::getline(inputFile, line)) {
        size_t posCoordinates = line.find("\"type\": \"LineString\",");
        if (posCoordinates != std::string::npos) {
            break;
        }
    }
    while (std::getline(inputFile, line)) {
        // Check for the presence of "coordinates" in the line
        size_t posCoordinates = line.find("\"coordinates\": [");
        if (posCoordinates != std::string::npos) {
            // Continue reading lines until coordinates closing "]"
            std::getline(inputFile, line);
            std::getline(inputFile, line);

            line = line.substr(12);
            if (line.back() == ',') {
                line.pop_back();
            }
            double lon1 = std::stod(line);

            std::getline(inputFile, line); // get the latitude line
            line = line.substr(12);
            double lat1 = std::stod(line);

            Coordinate coor1{lat1, lon1};

            std::getline(inputFile, line);

            while (std::getline(inputFile, line) && line.find("]") == std::string::npos) {
                // Extract the longitude
                std::getline(inputFile, line);
                line = line.substr(12);
                if (line.back() == ',') {
                    line.pop_back();
                }
                double lon2 = std::stod(line);

                // Extract the latitude
                std::getline(inputFile, line);
                line = line.substr(12);
                double lat2 = std::stod(line);

                // Create a coordinate object
                Coordinate currentCoordinate{lat2, lon2};

                // Add the coordinate to the graph
                std::pair<double, Coordinate> vec(calculateDistance(coor1, currentCoordinate), currentCoordinate);
                graph[coor1].push_back(vec);

                std::pair<double, Coordinate> vec2(calculateDistance(coor1, currentCoordinate), coor1);
                graph[currentCoordinate].push_back(vec2);

                coor1 = currentCoordinate;

                std::getline(inputFile, line);
            }
        }
    }
    // Close the file
    inputFile.close();

    return graph;
}