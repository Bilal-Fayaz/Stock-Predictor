#include "predictor.h"
#include <iostream>
#include <numeric>
#include <cmath>

PredictionResult StockPredictor::predict() { // [cite: 405]
    if (data.empty()) {
        return {0, 0, 0, 0, "N/A", 0};
    }
    int n = data.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    // Calculate required sums [cite: 411-417]
    for (const auto& point : data) {
        sumX += point.day;
        sumY += point.price;
        sumXY += point.day * point.price;
        sumX2 += point.day * point.day;
    }

    // Linear Regression formulas [cite: 418-422]
    // m = (n*Σxy - Σx*Σy) / (n*Σx² - (Σx)²)
    double denominator = (n * sumX2 - sumX * sumX);
    if (denominator == 0) return {0,0,0,0,"Error",0}; // Safety check

    double m = (n * sumXY - sumX * sumY) / denominator;

    // b = (Σy - m*Σx) / n
    double b = (sumY - m * sumX) / n;

    // Predict next day's price [cite: 423-425]
    int nextDay = n + 1;
    double predictedPrice = m * nextDay + b;

    // Calculate R² (confidence) [cite: 426-435]
    double meanY = sumY / n;
    double ssTotal = 0, ssResidual = 0;

    for (const auto& point : data) {
        double predicted = m * point.day + b;
        ssTotal += (point.price - meanY) * (point.price - meanY);
        ssResidual += (point.price - predicted) * (point.price - predicted);
    }

    double rSquared = (ssTotal == 0) ? 1.0 : (1.0 - (ssResidual / ssTotal));
    double confidence = rSquared * 100.0;

    // Determine trend [cite: 436-439]
    double lastPrice = data.back().price;
    std::string trend = predictedPrice > lastPrice ? "UP" : "DOWN";
    double changePercent = std::abs((predictedPrice - lastPrice) / lastPrice * 100.0);

    return { // [cite: 440-447]
        predictedPrice,
        m,
        b,
        confidence,
        trend,
        changePercent
    };
}