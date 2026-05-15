// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* Data, size_t Size) {
    // Ensure null-terminated string for load_string
    std::string xml_string(reinterpret_cast<const char*>(Data), Size);
    if (!xml_string.empty()) {
        xml_string.push_back('\0');
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_string.c_str());
    
    if (result) {
        pugi::xml_node root = doc.document_element();
        
        // Chain of child calls
        pugi::xml_node node = root.child("node1");
        node = node.child("node2");
        node = node.child("node3");
        node = node.child("node4");
        node = node.child("node5");
        node = node.child("node6");
        node = node.child("node7");
        node = node.child("node8");
        
        // Get text object
        pugi::xml_text text_obj = node.text();
        
        // More child calls
        node = node.child("node9");
        node = node.child("node10");
        
        // Child_value overloads
        const char* val1 = node.child_value();
        const char* val2 = node.child_value("child_name");
        
        // Use the values to avoid optimization
        if (val1) (void)val1[0];
        if (val2) (void)val2[0];
    }
    
    // Alternative path with different options
    pugi::xml_document doc2;
    pugi::xml_parse_result result2 = doc2.load_string(xml_string.c_str(), pugi::parse_default | pugi::parse_ws_pcdata);
    
    if (result2) {
        pugi::xml_node root2 = doc2.child("root");
        pugi::xml_node node2 = root2;
        
        // Try with different node names
        const char* child_names[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
        for (int i = 0; i < 8 && node2; ++i) {
            node2 = node2.child(child_names[i]);
        }
        
        if (node2) {
            pugi::xml_text text2 = node2.text();
            node2 = node2.child("text");
            const char* val3 = node2.child_value();
            const char* val4 = node2.child_value("value");
            
            if (val3) (void)val3[0];
            if (val4) (void)val4[0];
        }
    }
    
    // Write dummy file for completeness
    write_dummy_file(Data, Size);
    
    return 0;
}