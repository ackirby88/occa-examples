/* system header files */
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

int checkSelfAssignment(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 0;
    // Updated regex pattern to match optional type and qualifier keywords
    std::regex selfAssignPattern(R"(\b(?:const\s+)?(?:\w+\s+)*(\w+)\s*=\s*\1\s*;)");

    int ret = 0;
    while (std::getline(file, line)) {
        lineNumber++;

        // Check if line matches self-assignment pattern
        if (std::regex_search(line, selfAssignPattern)) {
            std::cout << "\x1B[0;31m [ERROR]: \x1B[0m" << "Self-assignment detected: File " << filename << ", Line #" << lineNumber << ": " << line << std::endl;
            ret = 1;
        }
    }
    file.close();
    return ret;
}

int main(int argc, char **argv) {
    if(argc < 2) {

        std::cout << "PROGRAM: This program checks source code files for variable self assignment, e.g., 'double *flux = flux;'" << std::endl;
        std::cout << "         Function-to-macro conversions for OCCA can cause variables to be inadvertently self-assigned, therefore causing undefined behavior." << std::endl;
        std::cout << "USAGE: ./CheckVarSelfAssign <file>" << std::endl;
        return 0;
    }
    std::string filename = argv[1]; // filename as se
    int ret = checkSelfAssignment(filename);
    return ret;
}
