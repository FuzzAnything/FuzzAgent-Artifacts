// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"

// Helper to safely convert fuzzer input to a valid C-string
static std::string convert_to_string(const uint8_t* data, size_t size) {
    std::string result;
    result.resize(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = data[i] ? static_cast<char>(data[i]) : ' ';
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    pugi::xml_document doc;
    // Load a minimal valid XML structure
    if (!doc.load_string("<root><item id='1'>Text</item></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // Prepare input strings
    std::string input_str = convert_to_string(Data, Size);
    const char* c_str = input_str.c_str();

    // Create an xpath_node from the root for context
    pugi::xpath_node context_node(root);

    // Construct xpath_query
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(c_str);
    } catch (...) {
        // If compilation fails, use a safe default query
        query = pugi::xpath_query("count(//node())");
    }

    // 2. Invoke target functions in the specified order

    // Step 1: pugi::xml_node::child
    std::string child_name = input_str.substr(0, std::min(input_str.size(), static_cast<size_t>(50)));
    for (auto& c : child_name) {
        if (c == '\0') c = 'x';
    }
    pugi::xml_node child1 = root.child(child_name.c_str());

    // Step 2: pugi::xml_node::attribute
    std::string attr_name = input_str.substr(0, std::min(input_str.size(), static_cast<size_t>(50)));
    for (auto& c : attr_name) {
        if (c == '\0') c = 'x';
    }
    pugi::xml_attribute attr = child1.attribute(attr_name.c_str());
    if (!child1) {
        attr = root.attribute(attr_name.c_str());
    }

    // Step 3: pugi::xml_node::child (again)
    pugi::xml_node child2 = child1.child(child_name.c_str());
    if (!child1) {
        child2 = root.child(child_name.c_str());
    }

    // Prepare context for XPath evaluation
    pugi::xpath_node eval_context = child2 ? pugi::xpath_node(child2) : pugi::xpath_node(root);

    // Step 4: pugi::xpath_query::evaluate_boolean
    bool bool_result = false;
    try {
        bool_result = query.evaluate_boolean(eval_context);
    } catch (...) {}

    // Step 5: pugi::xpath_query::evaluate_number
    double num_result = 0.0;
    try {
        num_result = query.evaluate_number(eval_context);
    } catch (...) {}

    // Step 6: pugi::xpath_query::evaluate_string (Overload 1: returns string_t)
    // Note: pugixml uses string_t which is std::string or std::wstring depending on config.
    // We use pugi::string_t explicitly.
    pugi::string_t str_result1;
    try {
        str_result1 = query.evaluate_string(eval_context);
    } catch (...) {}

    // Step 7: pugi::xpath_query::evaluate_string (Overload 2: buffer)
    // We need a buffer of char_t type.
    pugi::char_t buffer[256];
    size_t str_result2_len = 0;
    try {
        str_result2_len = query.evaluate_string(buffer, sizeof(buffer) / sizeof(pugi::char_t), eval_context);
    } catch (...) {}

    // Step 8: pugi::xpath_query::evaluate_node_set
    pugi::xpath_node_set node_set;
    try {
        node_set = query.evaluate_node_set(eval_context);
    } catch (...) {}

    // Step 9: pugi::xpath_query::evaluate_node
    pugi::xpath_node single_node;
    try {
        single_node = query.evaluate_node(eval_context);
    } catch (...) {}

    // 3. Cleanup
    // pugixml handles memory automatically via RAII.

    return 0;
}