#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <limits>
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

//modeOfT will be one of the following:
// 1 walking, 2 biking, 3 car 4 bus
void statements(int weight, int modeOfT, int source, int target){
    /* The purpose of this function is to create consistency within the statements that
     * will be printed for each path query, for both Dijkstra's and Bellman Ford's algorithms.
     */
    cout << "The shortest distance from vertex: " << source << " to vertex: " << target << " is " << weight << " miles" << endl;
    int timeWalking =weight / mphWalking;
    int timeBiking = weight / mphBiking;
    int timeCar = weight / mphCar;
    int timeBus = weight / mphBus;

    int emissionsCar = weight * CO2inCarperMile;
    int emissionsBus = weight * CO2inBusperMile;

    if(modeOfT == 1){
        cout << "Walking, this distance should take " << timeWalking << " mph." << endl;
        cout << "This is " << timeCar / timeWalking << " times slower than driving, "
        << timeBus / timeWalking << " times slower than taking a bus, and "
        << timeBiking / timeWalking << " times slower than riding a bike." << endl;

        cout << "Your total gas emissions for this path is: 0 grams" << endl;
    }
    if(modeOfT == 2){
        cout << "Biking, this distance should take " << timeBiking << " mph." << endl;
        cout << "This is " << timeCar / timeBiking << " times slower than driving, "
             << timeBus / timeBiking << " times slower than taking a bus, and "
             << timeBiking / timeWalking << " times faster than walking." << endl;

        cout << "Your total gas emissions for this path is: 0 grams" << endl;
    }
    if(modeOfT == 3){
        cout << "Driving, this distance should take " << timeCar << " mph." << endl;
        cout << "This is " << timeCar / timeBus << " times faster than taking a bus, "
             << timeCar / timeBiking << " times faster than riding a bike, and "
             << timeCar / timeWalking << " times faster than walking." << endl;

        cout << "Your total gas emissions for this path is: " << emissionsCar << " grams" << endl;
        cout<< "This is " << emissionsCar / emissionsBus << " times more than the emissions of taking a bus and "
        << emissionsCar << " times more than the emissions of walking or biking." << endl;
    }
    if(modeOfT == 4){
        cout << "Taking a bus, this distance should take " << timeBus << " mph." << endl;
        cout << "This is " << timeCar / timeBus << " times slower than driving, "
             << timeBus / timeBiking << " times faster than riding a bike, and "
             << timeBus / timeWalking << " times faster than walking." << endl;

        cout << "Your total gas emissions for this path is: " << emissionsBus << " grams" << endl;
        cout<< "This is " << emissionsCar / emissionsBus << " times less than the emissions of driving and "
            << emissionsBus << " times more than the emissions of walking or biking." << endl;
    }

}

void dijkstra(map<int, vector<pair<int, int>>>& graph, int source, int target, int modeOfT){
    /* The purpose of this function is to find the shortest paths to all vertices in the graph,
     * from the source vertex. This will allow us to find the shortest path between two vertices,
     * as long as the starting vertex is the source, when using the weights as markers of distance.
     *
     * The outline of this function was taken from class notes given by Dr. Kapoor, Module 8
     */

    // uses a min heap to keep track of min distance between vertices
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    // vector representation of d[v]
    // using numeric_limits<int>::max() is equivalent to using infinity
    vector<int> d(graph.size(), numeric_limits<int>::max());

    d[source] = 0; // establishes the source algorithm

    pq.push({source, 0}); // vertex and distance

    while(!pq.empty()){ // while we have not visited all the vertices
        int u = pq.top().first; // gets the source
        pq.pop();
        // will iterate through all the adjacent vertices to u
        for(const auto entry : graph[u]){
            int v = entry.first; // destination of edge
            int weight = entry.second; // weight of edge

            // relaxes the weights
            if(d[u] + weight < d[v]){
                d[v] = d[u] + weight;
                // next vertex added to test shortest paths
                pq.push({v, d[v]});
            }
        }
    }
    int shortestDistance = d[target];
    statements(shortestDistance, modeOfT, source, target);
}
void bellman(map<int, vector<pair<int, int>>>& graph, int modeOfT, int source, int target){
    /* The purpose of this function is to find the shortest paths to all vertices in the graph,
     * allowing for negative weights. This will allow us to find the shortest path between two vertices,
     * as long as there is no negative-weight cycles, when using the weights as markers of distance.
     *
     * pseudocode and conceptual understanding taken from:
     * https://www.geeksforgeeks.org/bellman-ford-algorithm-dp-23/
     */

    // vector representation of d[v]
    // numeric_limits<int>::max() acts infinity
    vector<int> d(graph.size(), numeric_limits<int>::max());
    d[source] = 0;

    // bellman ford's iterates |V|-1 times
    for(int i = 0; i < graph.size()-1; i++){
        // had to add an extra for loop to be able to iterate through the map
        for(const auto entry : graph){
            // source of edge
            int u = entry.first;
            // iterated through every edge
            for(const auto edge : entry.second){
                // destination of edge
                int v = edge.first;
                // weight of edge
                int w = edge.second;

                // relaxes edges
                if(d[u] != numeric_limits<int>::max() && d[u] + w < d[v]){
                    d[v] = d[u] + w;
                }
            }
        }
    }
    //check for negative cycles - realistically not necessary for this data set
    // distance is not a negative measurement
    for(const auto entry : graph){
        int u = entry.first;
        for(const auto edge : entry.second){
            int v = edge.first;
            int w = edge.second;
            // repeats same if statement as during relaxation
            // if this is true, then there has to be a negative-weight cycle
            if(d[u] != numeric_limits<int>::max() && d[u] + w < d[v]){
                cout << "There is a negative-weight cycle!" << endl;
                return;
            }
        }
    }
    int shortestDistance = d[target];
    statements(shortestDistance, modeOfT, source, target);
}
