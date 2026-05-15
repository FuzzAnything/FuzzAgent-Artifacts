// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
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
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Create XML document and parse some XML content
    pugi::xml_document doc;
    
    // Use fuzzer input as XML content, but ensure it's null-terminated
    if (Size == 0) return 0;
    
    // Create a buffer with null terminator
    char* xml_buffer = new char[Size + 1];
    memcpy(xml_buffer, Data, Size);
    xml_buffer[Size] = '\0';
    
    // Try to parse the XML
    pugi::xml_parse_result result = doc.load_string(xml_buffer);
    delete[] xml_buffer;
    
    // Get root node (or document node if parsing failed)
    pugi::xml_node node = doc.document_element();
    if (!node) {
        node = doc;
    }
    
    // Create xpath_variable_set
    pugi::xpath_variable_set vars;
    
    // Add variables first (set requires existing variables)
    // We need to create variables using add() before setting them
    // Create variables of different types
    vars.add("double_var", pugi::xpath_type_number);
    vars.add("string_var", pugi::xpath_type_string);
    vars.add("bool_var", pugi::xpath_type_boolean);
    vars.add("nodeset_var", pugi::xpath_type_node_set);
    
    // Create a dummy xpath_node_set for the nodeset variable
    pugi::xpath_node_set empty_nodeset;
    
    // Now set the variables using the overloads in the specified order
    // 1. double overload
    double double_val = Size > 0 ? static_cast<double>(Data[0]) / 255.0 : 0.5;
    vars.set("double_var", double_val);
    
    // 2. string overload
    const char* str_val = Size > 0 ? "test_string" : "";
    vars.set("string_var", str_val);
    
    // 3. bool overload
    bool bool_val = Size > 0;
    vars.set("bool_var", bool_val);
    
    // 4. xpath_node_set overload
    vars.set("nodeset_var", empty_nodeset);
    
    // Now select nodes with an XPath query
    // Create a simple XPath query from the input
    if (Size > 0) {
        char* query_buffer = new char[Size + 1];
        memcpy(query_buffer, Data, Size);
        query_buffer[Size] = '\0';
        
        // Try to select nodes with the query
        pugi::xpath_node_set selected_nodes = node.select_nodes(query_buffer, &vars);
        
        delete[] query_buffer;
    }
    
    // Get variables (non-const version)
    pugi::xpath_variable* var1 = vars.get("double_var");
    pugi::xpath_variable* var2 = vars.get("string_var");
    
    // Get variables (const version)
    const pugi::xpath_variable_set& const_vars = vars;
    const pugi::xpath_variable* var3 = const_vars.get("bool_var");
    const pugi::xpath_variable* var4 = const_vars.get("nodeset_var");
    
    // Cleanup happens automatically when objects go out of scope
    
    return 0;
}