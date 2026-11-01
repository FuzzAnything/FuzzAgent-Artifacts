// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
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

static void safe_fuzz(const uint8_t* Data, size_t Size) {
    pugi::xml_document doc;
    pugi::xpath_variable_set var_set;
    pugi::xpath_query query;

    // 1. pugi::xpath_variable_set::add
    // We try to add a variable if we have enough data to form a name
    if (Size > 0) {
        const char* var_name = "fuzz_var";
        
        // Try different types
        for (int i = 0; i < 4; ++i) {
            pugi::xpath_value_type type = static_cast<pugi::xpath_value_type>(i % 5);
            var_set.add(var_name, type);
        }
    }

    // 2. pugi::xml_document::load_buffer
    // Load the raw fuzzing data as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    pugi::xpath_node context_node;
    if (doc.root().type() == pugi::node_element) {
        context_node = pugi::xpath_node(doc.root());
    } else {
        context_node = pugi::xpath_node(); 
    }

    // Prepare a query string from the input data to ensure diverse query testing
    std::string query_str;
    if (Size > 0) {
        // Find first null or use whole buffer if no null
        const void* null_ptr = std::memchr(Data, 0, Size);
        size_t null_pos = (null_ptr == nullptr) ? Size : static_cast<size_t>(reinterpret_cast<const char*>(null_ptr) - reinterpret_cast<const char*>(Data));
        
        if (null_ptr == nullptr) {
            query_str.assign(reinterpret_cast<const char*>(Data), Size);
        } else {
            query_str.assign(reinterpret_cast<const char*>(Data), null_pos);
        }
    } else {
        query_str = "//";
    }

    // Construct the query
    try {
        query = pugi::xpath_query(query_str.c_str());
    } catch (...) {
        return;
    }

    // 3. pugi::xpath_query::evaluate_boolean
    try {
        bool b = query.evaluate_boolean(context_node);
        (void)b;
    } catch (...) {
    }

    // 4. pugi::xpath_query::evaluate_number
    try {
        double n = query.evaluate_number(context_node);
        (void)n;
    } catch (...) {
    }

    // 5. pugi::xpath_query::evaluate_string
    try {
        pugi::string_t s = query.evaluate_string(context_node);
        (void)s;
    } catch (...) {
    }

    // 6. pugi::xpath_query::evaluate_node
    try {
        pugi::xpath_node n = query.evaluate_node(context_node);
        (void)n;
    } catch (...) {
    }

    // 7. pugi::xpath_query::evaluate_node_set
    try {
        pugi::xpath_node_set ns = query.evaluate_node_set(context_node);
        (void)ns;
    } catch (...) {
    }
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    safe_fuzz(Data, Size);
    
    return 0;
}