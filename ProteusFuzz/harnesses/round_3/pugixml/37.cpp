// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
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

static void safe_parse_xml(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Ensure null termination for string conversion
    char* buffer = new char[size + 1];
    if (!buffer) return;
    
    std::memcpy(buffer, data, size);
    buffer[size] = '\0';
    
    pugi::xml_parse_result result = doc.load_string(buffer);
    
    delete[] buffer;
    
    if (!result) {
        // Clear document on parse failure to ensure clean state
        doc.reset();
    }
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Prepare environment: parse XML from fuzzer input
    safe_parse_xml(doc, Data, Size);
    
    // Get root node for XPath queries
    pugi::xml_node root = doc.root();
    if (!root) {
        // Try to create a minimal document if parsing failed completely
        doc.append_child("root");
        root = doc.root();
    }
    
    // Prepare variable set (optional)
    pugi::xpath_variable_set variables;
    
    // We will try multiple safe queries to explore different states
    // We avoid using raw fuzzer input as XPath query to prevent exceptions
    const char* safe_queries[] = {
        "//*", 
        "/*", 
        "//text()", 
        "/*/@*", 
        "count(*)", 
        "name()",
        "self::*",
        "descendant-or-self::*",
        "ancestor::*",
        "following-sibling::*",
        "preceding-sibling::*",
        "child::*",
        "parent::*",
        "attribute::*",
        "namespace::*"
    };
    
    // Also try to construct a query from input if it looks safe (alphanumeric + basic XPath chars)
    char* query_buffer = new char[Size + 1];
    if (!query_buffer) return 0;
    
    bool input_as_query = true;
    for (size_t i = 0; i < Size; ++i) {
        uint8_t c = Data[i];
        // Allow alphanumeric, spaces, and basic XPath operators
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
              (c >= '0' && c <= '9') || c == ' ' || c == '/' || c == '.' || 
              c == '@' || c == '*' || c == '[' || c == ']' || c == '(' || c == ')' ||
              c == '-' || c == '_' || c == ':' || c == '<' || c == '>' || c == '=' ||
              c == '!' || c == '|' || c == '&')) {
            input_as_query = false;
            break;
        }
    }
    
    if (input_as_query) {
        std::memcpy(query_buffer, Data, Size);
        query_buffer[Size] = '\0';
        
        // Try to use input as query with exception handling
        try {
            pugi::xpath_node_set node_set = root.select_nodes(query_buffer, &variables);
            
            // Invoke target functions in specified order:
            // 1. size
            size_t set_size = node_set.size();
            
            // 2. type
            pugi::xpath_node_set::type_t set_type = node_set.type();
            
            // 3. empty
            bool is_empty = node_set.empty();
            
            // 4. first
            pugi::xpath_node first_node = node_set.first();
            
            // 5. end
            auto end_iter = node_set.end();
            
            // Additional exploration: iterate
            for (auto it = node_set.begin(); it != node_set.end(); ++it) {
                (void)it->node();
            }
        } catch (...) {
            // Ignore exceptions from malformed queries
        }
    }
    
    // Always run safe queries to ensure coverage
    for (const char* safe_query : safe_queries) {
        try {
            pugi::xpath_node_set node_set = root.select_nodes(safe_query, &variables);
            
            // Invoke target functions in specified order:
            // 1. size
            size_t set_size = node_set.size();
            
            // 2. type
            pugi::xpath_node_set::type_t set_type = node_set.type();
            
            // 3. empty
            bool is_empty = node_set.empty();
            
            // 4. first
            pugi::xpath_node first_node = node_set.first();
            
            // 5. end
            auto end_iter = node_set.end();
            
            // Additional exploration: iterate
            for (auto it = node_set.begin(); it != node_set.end(); ++it) {
                (void)it->node();
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Cleanup
    delete[] query_buffer;
    
    return 0;
}