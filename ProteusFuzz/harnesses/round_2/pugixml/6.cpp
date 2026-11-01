// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

static void try_load_and_evaluate(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Step 2: Load buffer
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    // We continue even if parsing fails to test error handling paths, 
    // but we need a valid root for context if possible.
    
    // Step 1: Prepare variable set and add variable
    pugi::xpath_variable_set vars;
    // Try adding a variable of different types to explore states
    pugi::xpath_variable* var = vars.add("test_var", pugi::xpath_type_string);
    (void)var; // Suppress unused warning

    // Prepare a context node (root of the document)
    // If load failed, root() might be empty, which is a valid state to test
    pugi::xpath_node context_node(doc.root());

    // Compile XPath query from input data if possible, or use a safe default
    const char* query_str = reinterpret_cast<const char*>(data);
    // Ensure null termination for safety if the fuzzer input doesn't have one
    // We create a small buffer to hold the query string safely
    const size_t max_query_len = 1024;
    char safe_query[max_query_len + 1];
    size_t query_len = (size < max_query_len) ? size : max_query_len;
    std::memcpy(safe_query, data, query_len);
    safe_query[query_len] = '\0';

    // Try to compile the query
    // Note: xpath_query constructor takes xpath_variable_set*, so we pass &vars
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(safe_query, &vars);
    } catch (...) {
        // If compilation throws, try a fallback query
        try {
            query = pugi::xpath_query("/*", &vars);
        } catch (...) {
            // If fallback also fails, we can't evaluate, but we've tested the constructor
            return;
        }
    }

    if (!query) {
        // Query compilation failed, try a fallback query to ensure we test evaluation logic
        try {
            query = pugi::xpath_query("/*", &vars);
        } catch (...) {
            return;
        }
        if (!query) {
            return;
        }
    }

    // Step 3: Evaluate in the specified order
    try {
        // pugi::xpath_query::evaluate_boolean
        bool b_val = query.evaluate_boolean(context_node);
        (void)b_val;

        // pugi::xpath_query::evaluate_number
        double n_val = query.evaluate_number(context_node);
        (void)n_val;

        // pugi::xpath_query::evaluate_string
        // The return type is pugi::string_t, not xpath_string
        pugi::string_t s_val = query.evaluate_string(context_node);
        (void)s_val;

        // pugi::xpath_query::evaluate_node
        pugi::xpath_node n_node = query.evaluate_node(context_node);
        (void)n_node;

        // pugi::xpath_query::evaluate_node_set
        pugi::xpath_node_set n_set = query.evaluate_node_set(context_node);
        (void)n_set;
    } catch (...) {
        // Catch any exceptions thrown if PUGIXML_NO_EXCEPTIONS is not defined
        // This prevents the fuzzer from crashing on unexpected exceptions
    }
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Basic sanity check to avoid processing extremely large inputs that might cause OOM
    // though the library handles OOM, we want to keep fuzzing efficient.
    if (Size > 1024 * 1024) {
        return 0;
    }

    // Call the helper function to execute the fuzzing logic
    try {
        try_load_and_evaluate(Data, Size);
    } catch (...) {
        // Catch any exceptions that might escape the inner try-catch
    }

    return 0;
}