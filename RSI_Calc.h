#pragma once

#include <deque>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Function to calculate RSI
double calculateRSI(const std::deque<double>& adjCloses);

// Function to process input file and generate RSI output
int processStockData(const char* inputFilename, const char* outputFilename);

