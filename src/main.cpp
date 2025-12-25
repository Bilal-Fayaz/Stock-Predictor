#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "predictor.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// Updated renderChart to show the "AI Trend Line"
void renderChart(StockPredictor& predictor) {
    ImGui::Begin("Stock Price Chart", nullptr, ImGuiWindowFlags_NoCollapse);

    if (predictor.hasData()) {
        auto& data = predictor.getData();
        if (!data.empty()) {
            // 1. Prepare Historical Data
            static std::vector<double> x, y;
            x.clear(); y.clear();
            for (const auto& point : data) {
                x.push_back(point.day);
                y.push_back(point.price);
            }

            // 2. Prepare Trend Line (The "AI" visual)
            // We need two points to draw a line: Day 1 and Day N+1
            static std::vector<double> lineX, lineY;
            lineX.clear(); lineY.clear();

            // We need to re-run predict() temporarily to get slope(m) and intercept(b)
            // In a real app, you might cache this, but this is fine for now.
            PredictionResult res = predictor.predict();

            // Point A: Start of data (Day 1)
            lineX.push_back(1);
            lineY.push_back(res.slope * 1 + res.intercept);

            // Point B: End of prediction (Day N + 5 for a forecast view)
            double futureDay = data.size() + 5;
            lineX.push_back(futureDay);
            lineY.push_back(res.slope * futureDay + res.intercept);

            // 3. Plot Everything
            if (ImPlot::BeginPlot("Stock Price Trend", ImVec2(-1, -1))) {
                ImPlot::SetupAxes("Day", "Price ($)");

                // Draw the actual stock prices (Blue Line)
                ImPlot::PlotLine("Historical Price", x.data(), y.data(), x.size());

                // Draw the AI Trend Line (Red Line)
                ImPlot::SetNextLineStyle(ImVec4(1, 0, 0, 1), 2.0f); // Red color, 2px thick
                ImPlot::PlotLine("AI Trend Line", lineX.data(), lineY.data(), lineX.size());

                // Optional: Draw the specific predicted point as a marker
                double nextDay = data.size() + 1;
                double nextPrice = res.predictedPrice;
                ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 6, ImVec4(0, 1, 0, 1), IMPLOT_AUTO, ImVec4(0, 1, 0, 1));
                ImPlot::PlotScatter("Next Prediction", &nextDay, &nextPrice, 1);

                ImPlot::EndPlot();
            }
        }
    }
    ImGui::End();
}

int main() {
    // Initialize GLFW [cite: 273-289]
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed\n";
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(1280, 720, "AI Stock Predictor", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup ImGui and ImPlot [cite: 291-296]
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext(); // [cite: 470]
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Application State
    StockPredictor predictor;
    PredictionResult result;
    bool showPrediction = false;

    // Main loop [cite: 298]
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI Implementation [cite: 504-549]
        // Sidebar
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(300, 720));
        ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::Text("Stock Predictor");
        ImGui::Separator();

        // Load Button
        if (ImGui::Button("Load Stock Data", ImVec2(-1, 40))) {
            // Ensure this path matches your file structure!
            if (predictor.loadCSV("../data/sample_data.csv")) {
                showPrediction = false;
            }
        }

        // Data Info
        if (predictor.hasData()) {
            ImGui::Text("Data Points: %d", (int)predictor.getData().size());
            ImGui::Text("Current Price: $%.2f", predictor.getData().back().price);
        }

        ImGui::Separator();

        // Predict Button
        if (ImGui::Button("Predict Next Price", ImVec2(-1, 50))) {
            if (predictor.hasData()) {
                result = predictor.predict();
                showPrediction = true;
            }
        }

        // Show Prediction Results
        if (showPrediction) {
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Predicted Price: $%.2f", result.predictedPrice);
            ImGui::Text("Trend: %s", result.trend.c_str());
            ImGui::Text("Confidence: %.1f%%", result.confidence);
        }
        ImGui::End();

        // Main chart area
        ImGui::SetNextWindowPos(ImVec2(300, 0));
        ImGui::SetNextWindowSize(ImVec2(980, 720));
        renderChart(predictor);

        // Rendering [cite: 308-315]
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup [cite: 318-322]
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext(); // [cite: 495]
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}