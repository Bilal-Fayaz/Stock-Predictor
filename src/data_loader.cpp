#include "predictor.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool StockPredictor::loadCSV(const std::string& filename) { // [cite: 368]
    std::ifstream file(filename);

    if (!file.is_open()) { // [cite: 370]
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    data.clear();
    std::string line;

    // Skip header line [cite: 376]
    std::getline(file, line);

    int day = 1;
    while (std::getline(file, line)) { // [cite: 379]
        std::stringstream ss(line);
        std::string date, priceStr;

        // Parse CSV: Date,Price [cite: 382-384]
        std::getline(ss, date, ',');
        std::getline(ss, priceStr, ',');

        try {
            StockData point;
            point.day = day++;
            point.price = std::stod(priceStr); // [cite: 388]
            point.date = date;

            data.push_back(point);
        }
        catch (const std::exception& e) { // [cite: 392]
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }
    }

    file.close();

    std::cout << "Loaded " << data.size() << " data points" << std::endl; // [cite: 398]
    return !data.empty();
}