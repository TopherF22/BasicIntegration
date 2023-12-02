#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "py++.h" // Assuming this includes the definition of StockData

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.csv>" << std::endl;
        return 1;
    }

    const char *inputFilename = argv[1];
    std::ifstream inputFile(inputFilename);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file: " << inputFilename << std::endl;
        return 1;
    }

    // Skip the header line
    std::string header;
    std::getline(inputFile, header);

    // Read and print data
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        StockData stock;

        // Extract date and adjClose from the CSV line
        std::getline(ss, stock.date, ',');
        ss >> stock.adjClose;

    }

    // Calculate the average
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);
    std::getline(inputFile, header); // Skip the header line

    double sum = 0.0;
    int count = 0;
    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        StockData stock;

        // Extract date and adjClose from the CSV line
        std::getline(ss, stock.date, ',');
        ss >> stock.adjClose;

        sum += stock.adjClose;
        count++;
    }

    double average = sum / count;

    // Write average to a new CSV file
    const char *outputFilename = "output.csv";
    std::ofstream outputFile(outputFilename);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening the output file: " << outputFilename << std::endl;
        return 1;
    }

    // Write header
    outputFile << "Average" << std::endl;

    // Write average value
    outputFile << average << std::endl;


    return 0;
}
