/*
 * File:   function_to_macro.hxx
 * Author: akirby
 *
 * Created on July 24, 2024, 4:21 PM
 */

#ifndef FUNCTION_TO_MACRO_HXX
#define FUNCTION_TO_MACRO_HXX

/* system header files */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <stdexcept>

/* header files */
#include "colors.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ================= */
/* CLASS Definitions */
/* ================= */
/* FileHandler Class definition for function_to_macro */
class FileHandler {
  public:
    FileHandler(const std::string& filename) :
        filename_(filename),
        infile_(filename) {
        if(!infile_) throw std::runtime_error(RED "Unable to open file: " COLOR_OFF + filename);
    }

    const std::string& getFilename() const {
        return filename_;
    }

    std::ifstream& getStream() {
        return infile_;
    }

  private:
    std::string filename_;
    std::ifstream infile_;
};

/* ================= */
/* utility functions */
/* ================= */
// Helper function to trim whitespace from both ends of a string
static
std::string trimutil(const std::string& str) {
    const auto strBegin = str.find_first_not_of(" \t\n\r");
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(" \t\n\r");
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

// Helper function to remove pointers and type qualifiers from a parameter string
static
std::string clean_param(const std::string& param) {
    std::string cleaned = param;
    cleaned = std::regex_replace(cleaned, std::regex(R"(\*)"), ""); // Remove pointers
    cleaned = std::regex_replace(cleaned, std::regex(R"((const|restrict|volatile|static)\s+)"), ""); // Remove type qualifiers
    return trimutil(cleaned.substr(cleaned.find_last_of(" \t") + 1));
}

// Helper function to add backslash at the 140th character of each line
static
std::string add_backslash_at_140(const std::string& input) {
    std::istringstream stream(input);
    std::ostringstream result;
    std::string line;
    while (std::getline(stream, line)) {
        if (line.length() > 140) {
            result << line.substr(0, 140) << " \\\n" << line.substr(140) << "\\\n";
        } else {
            result << line << std::string(140 - line.length(), ' ') << " \\\n";
        }
    }
    return result.str();
}

// Helper function to update include guards
static
std::string update_include_guards(const std::string& content) {
    std::regex guard_regex(R"(#ifndef\s+(\w+))");
    std::smatch match;
    std::string updated_content = content;

    if (std::regex_search(content, match, guard_regex)) {
        std::string guard = match[1].str();
        std::string updated_guard = guard + "_MACRO";
        updated_content = std::regex_replace(updated_content, std::regex(guard), updated_guard);
    }

    return updated_content;
}

static
std::regex get_regex_level(int level){
    if(level==3){
        std::regex reg(R"((void\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)\s*\{((?:[^{}]*|\{(?:[^{}]*|\{(?:[^{}]*|\{[^{}]*\})*\})*\})*)\}))");
        return reg;
    } else
    if(level==4){
        std::regex reg(R"((void\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)\s*\{((?:[^{}]*|\{(?:[^{}]*|\{(?:[^{}]*|\{(?:[^{}]*|\{[^{}]*\})*\})*\})*\})*)\}))");
        return reg;
    } else
    if(level==5){
        std::regex reg(R"((void\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)\s*\{((?:[^{}]*|\{(?:[^{}]*|\{(?:[^{}]*|\{(?:[^{}]*|\{(?:[^{}]*|\{[^{}]*\})*\})*\})*\})*\})*)\}))");
        return reg;
    } else { // use 5 levels
        std::regex reg(R"((void\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)\s*\{((?:[^{}]*|\{(?:[^{}]*|\{(?:[^{}]*|\{(?:[^{}]*|\{(?:[^{}]*|\{[^{}]*\})*\})*\})*\})*\})*)\}))");
        return reg;
    }
}

/** Converts a function into a macro.
 *
 * @param [in] oklfile      file containing functions to be converted to macros
 * @return status of conversion
 */
int function_to_macro(FileHandler &oklfile,std::string okl_basename);

#ifdef __cplusplus
}
#endif
#endif /* FUNCTION_TO_MACRO_HXX */