/**
 * \file    function_to_macro.cxx
 * \author  akirby
 *
 * \brief   Converts functions to macro
 */

/* header files */
#include "function_to_macro.hxx"

std::string convert_function_to_macro(const std::string& function) {
    // Regular expression to match the function signature and parameters
    std::regex signature_regex(R"(^[\w\s\*]+?\(([\w\s,\*\[\]\@]+?)\)\s*\{)");
    std::smatch signature_match;
    if (!std::regex_search(function, signature_match, signature_regex)) {
        throw std::invalid_argument("Function signature not found");
    }

    // Extract parameters
    std::string parameters = signature_match[1].str();

    // Extract function body
    size_t body_start = function.find('{') + 1;
    size_t body_end = function.rfind('}');
    std::string body = function.substr(body_start, body_end - body_start);

    // Clean up parameters
    std::istringstream param_stream(parameters);
    std::string param;
    std::vector<std::string> param_names;
    while (std::getline(param_stream, param, ',')) {
        param_names.push_back(clean_param(param));
    }

    // Create macro signature
    std::ostringstream macro_signature;
    size_t func_name_end = function.find('(');
    std::string func_name = function.substr(0, func_name_end);
    func_name = func_name.substr(func_name.find_last_of(" \t") + 1); // Remove return type
    macro_signature << "#define " << func_name << "(";
    for (size_t i = 0; i < param_names.size(); ++i) {
        macro_signature << param_names[i];
        if (i < param_names.size() - 1) {
            macro_signature << ", ";
        }
    }
    macro_signature << ") {";

    // Create macro body
    std::ostringstream macro_body;
    std::istringstream body_stream(body);
    std::string line;
    bool first_line = true;
    while (std::getline(body_stream, line)) {
        if (!first_line) {
            macro_body << " \n";
        }
        macro_body << line;
        first_line = false;
    }

    return add_backslash_at_140(macro_signature.str() + macro_body.str()) + "}";
}

int function_to_macro(FileHandler &oklfile,std::string okl_basename){
    // Read the contents of the original file
    std::ifstream &infile = oklfile.getStream();

    std::cout << " > Building macro: " << okl_basename;
    if (!infile) {
        std::cerr << "[Function_to_Macro] Error opening file:" << oklfile.getFilename() << std::endl;
        return 1;
    }

    // Extract and replace functions with macros in the original file contents
    std::ostringstream original_content;
    original_content << infile.rdbuf();
    std::string original_contents = original_content.str();

    // Update include guards to append _MACRO to avoid conflicts with original function
    original_contents = update_include_guards(original_contents);

    /* Set regex expression based on max nested '{}' in function */
    std::regex nested_brace_expression = get_regex_level(5);
    std::string updated_contents;

    auto function_begin = std::sregex_iterator(original_contents.begin(), original_contents.end(), nested_brace_expression);
    auto function_end = std::sregex_iterator();

    size_t last_pos = 0;
    for (std::sregex_iterator i = function_begin; i != function_end; ++i) {
        std::smatch match = *i;
        std::string function_code = match.str();

        // Append content before the function
        updated_contents.append(original_contents, last_pos, match.position() - last_pos);

        try {
            std::string macro_code = convert_function_to_macro(function_code);
            updated_contents.append(macro_code);
        } catch (const std::exception& ex) {
            std::cerr << "\n" << RED ">> WARNING: " COLOR_OFF << ex.what() << " (commented-out function?) - skipping.";
            // In case of error, keep the original function code
            updated_contents.append(function_code);
        }

        last_pos = match.position() + match.length();
    }
    // Append end CPP guard
    updated_contents.append("\n#endif");

    // Write the updated contents to a new file
    std::string outpath = "./okl/" + okl_basename + ".macro";
    std::ofstream outfile(outpath);
    if (!outfile) {
        std::cerr << "[Function_to_Macro] Error creating new file." << outpath << std::endl;
        return 1;
    }
    outfile << updated_contents;
    std::cout << GREEN " > Success! " COLOR_OFF << outpath << std::endl;
    return 0;
}