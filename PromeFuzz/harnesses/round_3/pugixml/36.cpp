// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for parsing
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return 0;
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    if (!result) return 0;
    
    // Get document element (root node)
    pugi::xml_node root = doc.document_element();
    if (root.empty()) return 0;
    
    // Prepare null-terminated strings for node/attribute names
    // Use first 100 bytes or less if input is smaller
    const size_t name_len = (Size > 100) ? 100 : Size;
    char node_name1[101] = {0};
    char attr_name1[101] = {0};
    char node_name2[101] = {0};
    char attr_name2[101] = {0};
    
    if (Size > 0) {
        size_t copy_len1 = (name_len > 0) ? name_len : 0;
        if (copy_len1 > 0) {
            memcpy(node_name1, Data, copy_len1);
            node_name1[copy_len1] = '\0';
        }
        
        size_t copy_len2 = (Size > 100 && Size-100 > 0) ? 
                          ((Size-100 > 100) ? 100 : Size-100) : 0;
        if (copy_len2 > 0) {
            memcpy(attr_name1, Data + 100, copy_len2);
            attr_name1[copy_len2] = '\0';
        }
        
        size_t copy_len3 = (Size > 200 && Size-200 > 0) ?
                          ((Size-200 > 100) ? 100 : Size-200) : 0;
        if (copy_len3 > 0) {
            memcpy(node_name2, Data + 200, copy_len3);
            node_name2[copy_len3] = '\0';
        }
        
        size_t copy_len4 = (Size > 300 && Size-300 > 0) ?
                          ((Size-300 > 100) ? 100 : Size-300) : 0;
        if (copy_len4 > 0) {
            memcpy(attr_name2, Data + 300, copy_len4);
            attr_name2[copy_len4] = '\0';
        }
    }
    
    // Execute the sequence of API calls
    pugi::xml_node child1 = root.child(node_name1);
    pugi::xml_attribute attr1 = root.attribute(attr_name1);
    pugi::xml_node child2 = root.child(node_name2);
    pugi::xml_attribute attr2 = root.attribute(attr_name2);
    
    pugi::xml_attribute next1 = attr2.next_attribute();
    pugi::xml_attribute next2 = next1.next_attribute();
    pugi::xml_attribute prev1 = next2.previous_attribute();
    pugi::xml_attribute prev2 = prev1.previous_attribute();
    pugi::xml_attribute next3 = prev2.next_attribute();
    pugi::xml_attribute prev3 = next3.previous_attribute();
    
    // Clean up
    std::remove("./dummy_file");
    
    return 0;
}