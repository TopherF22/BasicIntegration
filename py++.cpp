
#include "RSI_Calc.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.csv>" << std::endl;
        return 1;
    }

    const char* inputFilename = argv[1];
    const char* outputFilename = "output.csv";

    // Call the function from the header
    int result = processStockData(inputFilename, outputFilename);

    return result;
}