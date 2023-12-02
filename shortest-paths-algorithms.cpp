#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <limits>
#include "data.cpp"
using namespace std;
// AUTHOR: CAMILA DJURINSKY ZAPOLSKI

// gas emissions in grams, walking and biking = 0
#define CO2inCarperMile 400;
#define CO2inBusperMile  300;

// average speed in mph
#define mphWalking 3;
#define mphBiking 11;
// assumption of speed limit of urban area, speed limit 20-35
#define mphCar 30;
// speeds ranging from 10 to 30 based on traffic, stops, and routes
#define mphBus 20;

// Creates a hash function to be able to handle finding latitude and longitude
namespace std {
    template <>
    struct hash<Coordinate> {
        size_t operator()(const Coordinate& coord) const {
            return hash<double>()(coord.latitude) ^ (hash<double>()(coord.longitude) << 1);
        }
    };
}

//modeOfT will be one of the following:
// 1 walking, 2 biking, 3 car 4 bus
void statements(double weight, int modeOfT, Coordinate source, Coordinate target){
    /* The purpose of this function is to create consistency within the statements that
     * will be printed for each path query, for both Dijkstra's and Bellman Ford's algorithms.
     */
    cout << "The shortest distance from vertex: (" << source.latitude << ", " << source.longitude << ") to vertex: (" << target.latitude << ", " << target.longitude << ") is " << weight << " miles" << endl;
    double timeWalking =weight / mphWalking;
    double timeBiking = weight / mphBiking;
    double timeCar = weight / mphCar;
    double timeBus = weight / mphBus;

    double emissionsCar = weight * CO2inCarperMile;
    double emissionsBus = weight * CO2inBusperMile;

    if(modeOfT == 1){
        cout << "Walking, this distance should take " << setprecision(3) << timeWalking << " hours." << endl;
        cout << "This is " << timeWalking / timeCar << " times slower than driving, "
        << timeWalking / timeBus << " times slower than taking a bus, and "
        << timeWalking / timeBiking << " times slower than riding a bike." << endl;

        cout << "Your total gas emissions for this path is: 0 grams" << endl;
    }
    if(modeOfT == 2){
        cout << "Biking, this distance should take " << timeBiking << " hours." << endl;
        cout << "This is " << timeBiking / timeCar << " times slower than driving, "
             << timeBiking / timeBus << " times slower than taking a bus, and "
             << timeWalking / timeBiking << " times faster than walking." << endl;

        cout << "Your total gas emissions for this path is: 0 grams" << endl;
    }
    if(modeOfT == 3){
        cout << "Driving, this distance should take " << timeCar << " hours." << endl;
        cout << "This is " << timeBus / timeCar << " times faster than taking a bus, "
             << timeBiking / timeCar << " times faster than riding a bike, and "
             << timeWalking / timeCar << " times faster than walking." << endl;

        cout << "Your total gas emissions for this path is: " << emissionsCar << " grams" << endl;
        cout<< "This is " << emissionsCar / emissionsBus << " times more than the emissions of taking a bus and "
        << emissionsCar << " grams more than the emissions of walking or biking." << endl;
    }
    if(modeOfT == 4){
        cout << "Taking a bus, this distance should take " << timeBus << " hours." << endl;
        cout << "This is " << timeBus / timeCar << " times slower than driving, "
             << timeBiking / timeBus << " times faster than riding a bike, and "
             << timeWalking / timeBus << " times faster than walking." << endl;

        cout << "Your total gas emissions for this path is: " << emissionsBus << " grams" << endl;
        cout<< "This is " << emissionsCar / emissionsBus << " times less than the emissions of driving and "
            << emissionsBus << " grams more than the emissions of walking or biking." << endl;
    }

}

void dijkstra(map<Coordinate, vector<pair<double, Coordinate>>>& graph, int modeOfT, Coordinate source, Coordinate target) {
    /* The purpose of this function is to find the shortest paths to all vertices in the graph,
     * from the source vertex. This will allow us to find the shortest path between two vertices,
     * as long as the starting vertex is the source, when using the weights as markers of distance.
     *
     * The outline of this function was taken from class notes given by Dr. Kapoor, Module 8
     */
    // uses a min heap to keep track of min distance between vertices
    priority_queue<pair<double, Coordinate>, vector<pair<double, Coordinate>>, greater<pair<double, Coordinate>>> pq;
    // unordered map representation of d[v]
    unordered_map<Coordinate, double> d;

    d[source] = 0; // establishes the source algorithm

    pq.push({0, source}); // vertex and distance

    while (!pq.empty()) { // while we have not visited all the vertices
        Coordinate u = pq.top().second; // gets the source
        pq.pop();
        // will iterate through all the adjacent vertices to u
        for (const auto& entry : graph[u]) {
            Coordinate v = entry.second; // destination of edge
            double weight = entry.first; // weight of edge

            if (d.find(v) == d.end() || d[u] + weight < d[v]) {
                d[v] = d[u] + weight;
                // next vertex added to test shortest paths
                pq.push({d[v], v});
            }
        }
    }
    double shortestDistance;
    if (d.find(target) != d.end()) {
        shortestDistance = d[target];
    } else {
        shortestDistance = numeric_limits<double>::infinity();
    }
    statements(shortestDistance, modeOfT, source, target);
}

void bellman(map<Coordinate, vector<pair<double, Coordinate>>>& graph, int modeOfT, Coordinate source, Coordinate target) {
    /* The purpose of this function is to find the shortest paths to all vertices in the graph,
     * allowing for negative weights. This will allow us to find the shortest path between two vertices,
     * as long as there is no negative-weight cycles, when using the weights as markers of distance.
     *
     * pseudocode and conceptual understanding taken from:
     * https://www.geeksforgeeks.org/bellman-ford-algorithm-dp-23/
     */

    // Get the number of vertices in the graph
    size_t numVertices = graph.size();

    // vector representation of d[v]
    // numeric_limits<int>::max() acts infinity
    vector<double> d(numVertices, numeric_limits<double>::infinity());

    // Map each vertex to its index in the d array
    unordered_map<Coordinate, size_t> vertexIndexMap;
    size_t currentIndex = 0;
    for (const auto& entry : graph) {
        vertexIndexMap[entry.first] = currentIndex++;
    }

    // Find the index of the source vertex
    auto sourceIt = vertexIndexMap.find(source);
    size_t sourceIndex = sourceIt->second;
    d[sourceIndex] = 0; // Set the distance of the source vertex to itself to 0

    // bellman ford's iterates |V|-1 times
    for (size_t i = 0; i < numVertices - 1; i++) {
        bool relaxationsOccurred = false;
        // had to add an extra for loop to be able to iterate through the map
        for (auto& entry : graph) {
            // source of edge
            Coordinate u = entry.first;
            // Find the index of the current vertex
            size_t uIndex = vertexIndexMap[u];
            // iterated through every edge
            for (const auto& edge : entry.second) {
                Coordinate v = edge.second; // destination of edge
                double w = edge.first; // weight of edge

                // Find the index of the target vertex
                size_t vIndex = vertexIndexMap[v];

                // relaxes edges
                if (d[uIndex] != numeric_limits<double>::infinity() && d[uIndex] + w < d[vIndex]) {
                    d[vIndex] = d[uIndex] + w;
                    relaxationsOccurred = true;
                }
            }
        }

        if (!relaxationsOccurred) {
            break;  // No relaxations occurred, break early to speed up
        }
    }

    // Find the index of the target vertex
    auto targetIt = vertexIndexMap.find(target);
    size_t targetIndex = targetIt->second;

    double shortestDistance = d[targetIndex];
    statements(shortestDistance, modeOfT, source, target);
}
