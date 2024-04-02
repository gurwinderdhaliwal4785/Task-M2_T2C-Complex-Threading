#include <iostream>
#include <vector>
#include <cmath>
#include <chrono> // Added for time measurement

using namespace std;
using namespace std::chrono; // Added for time measurement

// Function to calculate distance between two points
double calculateDistance(const vector<double>& point1, const vector<double>& point2) {
    double distance = 0;
    for (int i = 0; i < point1.size(); i++) {
        distance += pow((point1[i] - point2[i]), 2);
    }
    return sqrt(distance);
}

// Function to implement K-Means clustering algorithm
void kMeans(vector<vector<double>>& data, int k) {
    if (data.size() < k) {
        cerr << "Error: Number of clusters exceeds number of data points." << endl;
        return;
    }

    vector<vector<double>> centroids;
    for (int i = 0; i < k; i++) {
        centroids.push_back(data[i]);
    }

    vector<int> clusterAssignment(data.size(), -1);

    bool converged = false;
    while (!converged) {
        converged = true;
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
        for (int i = 0; i < data.size(); i++) {
            int clusterIndex = clusterAssignment[i];
            if (clusterIndex >= 0 && clusterIndex < newCentroids.size()) {
                for (int j = 0; j < data[i].size(); j++) {
                    if (j < newCentroids[clusterIndex].size()) {
                        newCentroids[clusterIndex][j] += data[i][j];
                    }
                }
                clusterSizes[clusterIndex]++;
            }
        }

        for (int i = 0; i < newCentroids.size(); i++) {
            if (clusterSizes[i] > 0) {
                for (int j = 0; j < newCentroids[i].size(); j++) {
                    newCentroids[i][j] /= clusterSizes[i];
                }
            }
        }

        centroids = newCentroids;
    }

    for (int i = 0; i < centroids.size(); i++) {
        if (!centroids[i].empty()) {
            cout << "Cluster " << i + 1 << " centroid: ";
            for (int j = 0; j < centroids[i].size(); j++) {
                cout << centroids[i][j] << " ";
            }
            cout << endl;
        }
    }
}

int main() {
    vector<vector<double>> data = {{1, 2}, {2, 1}, {3, 3}, {4, 2}, {5, 4}, {6, 5}, {7, 6}, {8, 7}, {9, 8}, {10, 9}};
    int k = 3;

    auto start = high_resolution_clock::now(); // Start time measurement

    kMeans(data, k);

    auto stop = high_resolution_clock::now(); // Stop time measurement
    auto duration = duration_cast<microseconds>(stop - start); // Calculate duration

    cout << "Time taken by sequential implementation: " << duration.count() << " microseconds" << endl; // Output duration
    return 0;
}
