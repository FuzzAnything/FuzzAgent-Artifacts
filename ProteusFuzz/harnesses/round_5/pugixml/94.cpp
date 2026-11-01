// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
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

static std::string create_safe_string(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    std::string result(reinterpret_cast<const char*>(data), size);
    if (!result.empty() && result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    pugi::xml_document doc;
    
    // Create a minimal valid XML document to work with
    if (!doc.load_string("<root><item id='1'>Data</item><item id='2'>More</item></root>")) {
        return 0;
    }

    pugi::xpath_variable_set variables;
    pugi::xpath_variable_set variables_const; 

    // Split input into segments for different operations
    size_t pos = 0;
    
    // Helper to get a null-terminated string from fuzz data
    auto get_safe_string = [&](size_t start, size_t len) -> const char* {
        if (start >= Size) return "";
        size_t actual_len = (start + len > Size) ? (Size - start) : len;
        if (actual_len == 0) return "";
        
        static std::string buffer;
        buffer.clear();
        buffer.assign(reinterpret_cast<const char*>(Data + start), actual_len);
        // Ensure null termination
        if (!buffer.empty() && buffer.back() != '\0') {
            buffer.push_back('\0');
        }
        return buffer.c_str();
    };

    // Define segment sizes based on input size
    size_t seg1_len = (Size / 4) + 1;
    size_t seg2_len = (Size / 4) + 1;
    size_t seg3_len = (Size / 4) + 1;
    
    const char* query_str = get_safe_string(0, seg1_len);
    const char* var_name1 = get_safe_string(seg1_len, seg2_len);
    const char* var_name2 = get_safe_string(seg1_len + seg2_len, seg3_len);

    // 2. Test API Functions

    // Test: pugi::xpath_variable_set::add
    // Try adding variables of different types
    pugi::xpath_variable* var_num = variables.add(var_name1, pugi::xpath_type_number);
    pugi::xpath_variable* var_str = variables.add(var_name2, pugi::xpath_type_string);
    pugi::xpath_variable* var_bool = variables.add(var_name1, pugi::xpath_type_boolean); 
    pugi::xpath_variable* var_nodes = variables.add(var_name2, pugi::xpath_type_node_set);

    // Test: pugi::xpath_variable_set::set
    if (var_num) {
        double val = static_cast<double>(Data[0]);
        variables.set(var_name1, val);
    }
    
    variables.set(var_name2, 1.0); 

    // Test: pugi::xpath_variable_set::get
    pugi::xpath_variable* retrieved = variables.get(var_name1);
    if (retrieved) {
        const char* name = retrieved->name();
        (void)name; 
        
        const char* str_val = retrieved->get_string();
        (void)str_val;
    }

    // Test: pugi::xpath_variable_set::get (const version)
    const pugi::xpath_variable* var_const = variables_const.get("test_var");
    (void)var_const;

    // Test: pugi::xml_node::select_nodes
    // pugixml does not throw exceptions for XPath errors in the standard build,
    // but returns an empty node set or invalid query result. 
    // We wrap in try-catch just in case of internal assertions or specific builds,
    // catching standard exceptions.
    try {
        pugi::xpath_node_set results = doc.select_nodes(query_str, &variables);
        
        // Iterate to exercise the node set logic
        for (auto it = results.begin(); it != results.end(); ++it) {
            pugi::xpath_node node = *it;
            if (node.node()) {
                const char* val = node.node().value();
                (void)val;
            }
        }
    } catch (const std::exception&) {
        // Expected behavior for malformed queries in some configurations
    } catch (...) {
        // Catch any other unexpected exceptions
    }

    // Additional state exploration:
    // Try with empty variables set
    try {
        pugi::xpath_node_set results_empty = doc.select_nodes(query_str, nullptr);
        (void)results_empty;
    } catch (...) {
        // Ignore exceptions
    }

    // 3. Cleanup
    // pugixml uses RAII, so doc and variables are cleaned up automatically.

    return 0;
}