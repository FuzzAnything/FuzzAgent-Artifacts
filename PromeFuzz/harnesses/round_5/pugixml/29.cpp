// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
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
#include <fstream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Create null-terminated XML string from fuzzer input
    std::string xml_string;
    if (Size > 0) {
        xml_string.assign(reinterpret_cast<const char*>(Data), Size);
        // Ensure null termination for load_string
        if (xml_string.back() != '\0') {
            xml_string.push_back('\0');
        }
    } else {
        xml_string = "\0";
    }

    // Initialize XML document
    pugi::xml_document doc;
    
    // Call load_string with the XML data
    pugi::xml_parse_result result = doc.load_string(xml_string.c_str());
    
    // Use the result to avoid dead code elimination
    static volatile auto dummy_status = result.status;
    (void)dummy_status;
    
    // Use a fixed name for child nodes to ensure we can chain calls
    const char* child_name = "node";
    
    // Chain of child calls (8 times as specified)
    pugi::xml_node current = doc.child(child_name);
    current = current.child(child_name);
    current = current.child(child_name);
    current = current.child(child_name);
    current = current.child(child_name);
    current = current.child(child_name);
    current = current.child(child_name);
    current = current.child(child_name);
    
    // Call text() on the current node
    pugi::xml_text text_obj = current.text();
    
    // Use text object to avoid dead code elimination
    static volatile const char* dummy_text = text_obj.as_string();
    (void)dummy_text;
    
    // Two more child calls
    current = current.child(child_name);
    current = current.child(child_name);
    
    // Call both overloads of child_value
    const char* value_with_name = current.child_value(child_name);
    const char* value_without_name = current.child_value();
    
    // Use the results to avoid dead code elimination
    static volatile const char* dummy_value1 = value_with_name;
    static volatile const char* dummy_value2 = value_without_name;
    (void)dummy_value1;
    (void)dummy_value2;
    
    return 0;
}