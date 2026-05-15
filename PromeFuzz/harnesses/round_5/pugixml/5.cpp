// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Create dummy XML content from fuzz data
    const char* xml_header = "<?xml version=\"1.0\"?><root>";
    const char* xml_footer = "</root>";
    
    // Calculate total size needed
    size_t header_len = strlen(xml_header);
    size_t footer_len = strlen(xml_footer);
    size_t total_len = header_len + Size + footer_len + 1;
    
    char* xml_content = new char[total_len];
    memcpy(xml_content, xml_header, header_len);
    
    // Copy fuzz data as XML content (ensure it's valid UTF-8 by using only safe bytes)
    for (size_t i = 0; i < Size && i < total_len - header_len - footer_len - 1; ++i) {
        // Only copy bytes that are safe for XML (0x20-0x7E and whitespace)
        if ((Data[i] >= 0x20 && Data[i] <= 0x7E) || Data[i] == 0x09 || Data[i] == 0x0A || Data[i] == 0x0D) {
            xml_content[header_len + i] = static_cast<char>(Data[i]);
        } else {
            xml_content[header_len + i] = ' ';
        }
    }
    
    memcpy(xml_content + header_len + Size, xml_footer, footer_len);
    xml_content[header_len + Size + footer_len] = '\0';
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content);
    
    if (result) {
        // Create xpath_node_set by selecting all nodes
        pugi::xpath_node_set node_set = doc.select_nodes("//*");
        
        // 1. Call type() on xpath_node_set
        pugi::xpath_node_set::type_t set_type = node_set.type();
        (void)set_type; // Avoid unused variable warning
        
        // 2. Call sort() on xpath_node_set
        bool reverse = false;
        if (Size > 0) {
            reverse = (Data[0] & 0x01) != 0;
        }
        node_set.sort(reverse);
        
        // 3. Call empty() on xpath_node_set
        bool is_empty = node_set.empty();
        (void)is_empty; // Avoid unused variable warning
        
        if (!node_set.empty()) {
            // Get first xpath_node from the set
            pugi::xpath_node xnode = node_set[0];
            
            // 4. Call attribute() on xpath_node
            pugi::xml_attribute attr = xnode.attribute();
            (void)attr; // Avoid unused variable warning
            
            // 5. Call parent() on xpath_node
            pugi::xml_node parent_node = xnode.parent();
            (void)parent_node; // Avoid unused variable warning
            
            // 6. Call node() on xpath_node
            pugi::xml_node node = xnode.node();
            
            // 7. Call root() on xml_node (if node is valid)
            if (node) {
                pugi::xml_node root_node = node.root();
                (void)root_node; // Avoid unused variable warning
            }
            
            // Try with different xpath_node from the set for more coverage
            if (node_set.size() > 1) {
                size_t index = Size % node_set.size();
                pugi::xpath_node another_xnode = node_set[index];
                
                pugi::xml_attribute another_attr = another_xnode.attribute();
                pugi::xml_node another_parent = another_xnode.parent();
                pugi::xml_node another_node = another_xnode.node();
                
                if (another_node) {
                    pugi::xml_node another_root = another_node.root();
                    (void)another_root;
                }
            }
        }
        
        // Also test with empty xpath_node_set
        pugi::xpath_node_set empty_set;
        empty_set.type();
        empty_set.sort();
        empty_set.empty();
    }
    
    // Clean up
    delete[] xml_content;
    
    return 0;
}