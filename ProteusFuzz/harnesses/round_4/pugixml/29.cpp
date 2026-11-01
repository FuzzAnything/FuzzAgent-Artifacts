// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
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
#include <cstdint>
#include <cstring>
#include <string>

static std::string prepare_xml_input(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return "<root/>";
    }
    
    std::string result;
    result.reserve(Size + 20);
    
    // Ensure we have a valid XML structure
    result = "<root>";
    
    // Copy input data, filtering out null bytes to ensure null-termination works correctly
    for (size_t i = 0; i < Size; ++i) {
        if (Data[i] != 0) {
            result.push_back(static_cast<char>(Data[i]));
        }
    }
    
    result += "</root>";
    
    // Ensure null termination
    result.push_back('\0');
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    
    // Prepare input string
    std::string input_str = prepare_xml_input(Data, Size);
    
    // 1. pugi::xml_document::load_string
    pugi::xml_parse_result result = doc.load_string(input_str.c_str());
    
    // If parsing failed, we still continue to test error handling paths
    // but we need a valid root node for subsequent operations
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // Try to get any node if document_element fails
        root = doc.first_child();
    }
    
    if (!root) {
        // If no nodes exist, return early to avoid invalid operations
        return 0;
    }
    
    // 2. pugi::xml_node::child (1st call)
    pugi::xml_node child1 = root.child("child1");
    
    // 3. pugi::xml_node::child (2nd call)
    pugi::xml_node child2 = child1.child("child2");
    
    // 4. pugi::xml_node::child (3rd call)
    pugi::xml_node child3 = child2.child("child3");
    
    // 5. pugi::xml_node::child (4th call)
    pugi::xml_node child4 = child3.child("child4");
    
    // 6. pugi::xml_node::child (5th call)
    pugi::xml_node child5 = child4.child("child5");
    
    // 7. pugi::xml_node::child (6th call)
    pugi::xml_node child6 = child5.child("child6");
    
    // 8. pugi::xml_node::child (7th call)
    pugi::xml_node child7 = child6.child("child7");
    
    // 9. pugi::xml_node::text
    pugi::xml_text text_node = child7.text();
    // Access the text to ensure the proxy is used
    const char* text_value = text_node.get();
    (void)text_value; // Suppress unused variable warning
    
    // 10. pugi::xml_node::child (8th call)
    pugi::xml_node child8 = child7.child("child8");
    
    // 11. pugi::xml_node::child_value (overload 1: no name)
    const char* value1 = child8.child_value();
    (void)value1; // Suppress unused variable warning
    
    // 12. pugi::xml_node::child_value (overload 2: with name)
    const char* value2 = child8.child_value("child9");
    (void)value2; // Suppress unused variable warning
    
    return 0;
}