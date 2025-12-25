#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <vector>
#include <string>

// Structure to hold single data point [cite: 334-338]
struct StockData {
    int day;
    double price;
    std::string date;
};

// Structure to hold prediction results [cite: 340-347]
struct PredictionResult {
    double predictedPrice;
    double slope;
    double intercept;
    double confidence;
    std::string trend;
    double changePercent;
};

// Main predictor class [cite: 349-358]
class StockPredictor {
private:
    std::vector<StockData> data;

public:
    bool loadCSV(const std::string& filename);
    PredictionResult predict();
    std::vector<StockData>& getData() { return data; }
    void clearData() { data.clear(); }
    bool hasData() const { return !data.empty(); }
};

#endif