// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* Data, size_t Size) {
    // Write fuzz data to dummy file for potential file-based parsing
    writeDummyFile(Data, Size);
    
    // Parse XML document from memory buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Only proceed if parsing succeeded (but still test APIs on empty/partial structures)
    if (result) {
        // Create xpath_node_set by evaluating an XPath query
        // Use a simple query that should work on most XML structures
        pugi::xpath_node_set node_set = doc.select_nodes("//*");
        
        // 1. Call xpath_node_set::type()
        pugi::xpath_node_set::type_t set_type = node_set.type();
        (void)set_type; // Suppress unused variable warning
        
        // 2. Call xpath_node_set::sort()
        // Use fuzz data to decide sort direction (use first byte modulo 2)
        bool reverse_sort = (Size > 0) ? (Data[0] % 2 == 0) : false;
        node_set.sort(reverse_sort);
        
        // 3. Call xpath_node_set::empty()
        bool is_empty = node_set.empty();
        (void)is_empty;
        
        // Get an xpath_node from the set (or create default if empty)
        pugi::xpath_node xnode;
        if (!node_set.empty()) {
            // Use fuzz data to select which node (modulo set size)
            size_t index = (Size > 0) ? (Data[0] % node_set.size()) : 0;
            xnode = node_set[index];
        } else {
            // Create default xpath_node (null)
            xnode = pugi::xpath_node();
        }
        
        // 4. Call xpath_node::attribute()
        pugi::xml_attribute attr = xnode.attribute();
        (void)attr;
        
        // 5. Call xpath_node::parent()
        pugi::xml_node parent_node = xnode.parent();
        (void)parent_node;
        
        // 6. Call xpath_node::node()
        pugi::xml_node node = xnode.node();
        (void)node;
        
        // 7. Call xml_node::root() on the node from step 6
        if (node) {
            pugi::xml_node root_node = node.root();
            (void)root_node;
        }
        
        // Additional exploration: test with different XPath queries
        if (Size > 1) {
            // Try to extract a simple XPath from fuzz data (limited to safe characters)
            char xpath_buf[256];
            size_t xpath_len = (Size < 255) ? Size : 255;
            for (size_t i = 0; i < xpath_len; i++) {
                char c = Data[i];
                // Only allow safe XPath characters
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
                    (c >= '0' && c <= '9') || c == '/' || c == '*' || 
                    c == '[' || c == ']' || c == '@' || c == '.' || 
                    c == ':' || c == '_' || c == '-') {
                    xpath_buf[i] = c;
                } else {
                    xpath_buf[i] = ' ';
                }
            }
            xpath_buf[xpath_len] = '\0';
            
            try {
                pugi::xpath_node_set alt_set = doc.select_nodes(xpath_buf);
                // Repeat the API sequence on alternative node set
                alt_set.type();
                alt_set.sort(!reverse_sort);
                alt_set.empty();
                
                if (!alt_set.empty()) {
                    pugi::xpath_node alt_xnode = alt_set[0];
                    alt_xnode.attribute();
                    alt_xnode.parent();
                    pugi::xml_node alt_node = alt_xnode.node();
                    if (alt_node) {
                        alt_node.root();
                    }
                }
            } catch (...) {
                // Ignore XPath evaluation errors
            }
        }
    } else {
        // Even if parsing failed, test APIs on empty/default objects
        pugi::xpath_node_set empty_set;
        empty_set.type();
        empty_set.sort();
        empty_set.empty();
        
        pugi::xpath_node default_xnode;
        default_xnode.attribute();
        default_xnode.parent();
        pugi::xml_node default_node = default_xnode.node();
        if (default_node) {
            default_node.root();
        }
    }
    
    return 0;
}