#include "RSI_Calc.h"

// Function to process input file and generate RSI output
int processStockData(const char* inputFilename, const char* outputFilename) {
    std::ifstream inputFile(inputFilename);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file: " << inputFilename << std::endl;
        return 1;
    }

    std::string header;
    std::getline(inputFile, header);
    std::istringstream headerStream(header);
    
    std::vector<std::string> tickers;
    std::getline(headerStream, header, ','); // Skip the Date column
    while (std::getline(headerStream, header, ',')) {
        tickers.push_back(header);
    }

    std::ofstream outputFile(outputFilename);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening the output file: " << outputFilename << std::endl;
        return 1;
    }

    // Write the header using the order from the input
    outputFile << "Date";
    for (const auto& ticker : tickers) {
        outputFile << "," << ticker;
    }
    outputFile << std::endl;

    std::vector<std::deque<double>> adjClosesList(tickers.size());

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream ss(line);
        std::string date;
        std::getline(ss, date, ',');
        double prevAdjClose = 0;
        std::string adjCloseStr;

        size_t tickerIndex = 0;
        while (std::getline(ss, adjCloseStr, ',')) {
            if (adjCloseStr.empty()) {
                // Skip empty columns and fill in 0 for RSI
                ++tickerIndex;
                continue;
            }

            try {
                double adjClose = std::stod(adjCloseStr);
                adjClosesList[tickerIndex].push_back(adjClose);
            } catch (const std::invalid_argument& ia) {
                std::cout << "Error: " << ia.what() << " (Column: " << tickers[tickerIndex] << ")" << std::endl;
                continue;
            }

            ++tickerIndex;
        }

        // Calculate RSI for each ticker
        outputFile << date;
        for (size_t i = 0; i < tickers.size(); ++i) {
            if (adjClosesList[i].size() >= 14) {
                double RSI = calculateRSI(adjClosesList[i]);

                // Write RSI to file
                outputFile << "," << RSI;
            } else {
                // Write empty field for tickers without enough data
                outputFile << ",";
            }
        }
        outputFile << std::endl;

        // Remove the oldest element from each adjCloses list
        for (auto& adjCloses : adjClosesList) {
            if (adjCloses.size() >= 14) {
                adjCloses.pop_front();
            }
        }
    }

    // Close files and return
    inputFile.close();
    outputFile.close();

    return 0;
}


double calculateRSI(const std::deque<double>& adjCloses) {
    if (adjCloses.size() < 2) {
        // If there are not enough data points, return 0
        return 0;
    }

    double sumGains = 0, sumLosses = 0;

    for (size_t i = 1; i < adjCloses.size(); ++i) {
        double change = adjCloses[i] - adjCloses[i - 1];

        if (change > 0) {
            sumGains += change;
        } else {
            sumLosses -= change;
        }
    }

    double averageGain = sumGains / (adjCloses.size() - 1);
    double averageLoss = sumLosses / (adjCloses.size() - 1);

    if (averageLoss == 0) {
        return 100;  // Avoid division by zero
    }

    double RS = averageGain / averageLoss;
    double RSI = 100 - (100 / (1 + RS));

    return RSI;
}