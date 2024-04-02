#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <chrono> // Added for time measurement

using namespace std;
using namespace std::chrono; // Added for time measurement

// Function to calculate distance between two points
double calculateDistance(vector<double>& point1, vector<double>& point2) {
    double distance = 0;
    for (int i = 0; i < point1.size(); i++) {
        distance += pow((point1[i] - point2[i]), 2);
    }
    return sqrt(distance);
}

// Function to calculate the centroid of a cluster
vector<double> calculateCentroid(vector<vector<double>>& cluster) {
    vector<double> centroid(cluster[0].size(), 0);
    for (int i = 0; i < cluster.size(); i++) {
        for (int j = 0; j < cluster[i].size(); j++) {
            centroid[j] += cluster[i][j];
        }
    }
    for (int i = 0; i < centroid.size(); i++) {
        centroid[i] /= cluster.size();
    }
    return centroid;
}

// Function to implement K-Means clustering algorithm in parallel using OpenMP
void kMeansParallel(vector<vector<double>>& data, int k) {
    vector<vector<double>> centroids;
    for (int i = 0; i < k; i++) {
        centroids.push_back(data[i]);
    }

    vector<int> clusterAssignment(data.size(), -1);

    bool converged = false;
    while (!converged) {
        converged = true;
        #pragma omp parallel for
        for (int i = 0; i < data.size(); i++) {
            double minDistance = calculateDistance(data[i], centroids[0]);
            int clusterIndex = 0;
            for (int j = 1; j < centroids.size(); j++) {
                double distance = calculateDistance(data[i], centroids[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    clusterIndex = j;
                }
            }
            if (clusterAssignment[i] != clusterIndex) {
                clusterAssignment[i] = clusterIndex;
                converged = false;
            }
        }

        vector<vector<double>> newCentroids(k);
        vector<int> clusterSizes(k, 0);
        #pragma omp parallel for
        // for (int i = 0; i < data.size(); i++) {
        //     int clusterIndex = clusterAssignment[i];
        //     for (int j = 0; j < data[i].size(); j++) {
        //         newCentroids[clusterIndex][j] += data[i][j];
        //     }
        //     clusterSizes[clusterIndex]++;
        // }
        for (int i = 0; i < data.size(); i++) {
            int clusterIndex = clusterAssignment[i];
            // Add a check for clusterIndex validity
            if (clusterIndex >= 0 && clusterIndex < newCentroids.size()) {
                for (int j = 0; j < data[i].size(); j++) {
                    // Add a check for data[i] size
                    if (j < newCentroids[clusterIndex].size()) {
                        newCentroids[clusterIndex][j] += data[i][j];
                    } 
                }
                clusterSizes[clusterIndex]++;
            } 
}

        for (int i = 0; i < newCentroids.size(); i++) {
            for (int j = 0; j < newCentroids[i].size(); j++) {
                newCentroids[i][j] /= clusterSizes[i];
            }
        }

        centroids = newCentroids;
    }

    for (int i = 0; i < centroids.size(); i++) {
        
        for (int j = 0; j < centroids[i].size(); j++) {
            cout << centroids[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<double>> data = {{1, 2}, {2, 1}, {3, 3}, {4, 2}, {5, 4}, {6, 5}, {7, 6}, {8, 7}, {9, 8}, {10, 9}};
    int k = 3;

    auto start = high_resolution_clock::now(); // Start time measurement

    kMeansParallel(data, k);

    auto stop = high_resolution_clock::now(); // Stop time measurement
    auto duration = duration_cast<microseconds>(stop - start); // Calculate duration

    cout << "Time taken by parallel implementation: " << duration.count() << " microseconds" << endl; // Output duration
    return 0;
}
