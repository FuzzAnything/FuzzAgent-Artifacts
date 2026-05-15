// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::attributes_begin at pugixml.cpp:5628:54 in pugixml.hpp
// pugi::xml_node::attributes_end at pugixml.cpp:5633:54 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <string_view>
#include <fstream>
#include <queue>

static void test_node_functions(pugi::xml_node node, std::string_view name) {
    // Test parent()
    (void)node.parent();
    
    // Test last_child()
    (void)node.last_child();
    
    // Test child() with string_view - ensure null termination for char* variant
    if (!name.empty()) {
        std::string null_terminated_name(name);
        (void)node.child(null_terminated_name.c_str());
    }
    (void)node.child(name);
    
    // Test previous_sibling()
    (void)node.previous_sibling();
    
    // Test first_child()
    (void)node.first_child();
    
    // Test find_child() with different predicates
    (void)node.find_child([](pugi::xml_node child) { return true; });
    (void)node.find_child([](pugi::xml_node child) { return false; });
    (void)node.find_child([](pugi::xml_node child) { 
        return child.attributes_begin() != child.attributes_end(); 
    });
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
    
    // Create string_view from data (limited to avoid huge allocations)
    size_t name_len = Size > 100 ? 100 : Size;
    std::string_view name_view(reinterpret_cast<const char*>(Data), name_len);
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (result) {
        // Test functions on root node
        pugi::xml_node root = doc.root();
        test_node_functions(root, name_view);
        
        // Test functions on all nodes in document
        std::queue<pugi::xml_node> nodes;
        nodes.push(root);
        
        while (!nodes.empty()) {
            pugi::xml_node current = nodes.front();
            nodes.pop();
            
            test_node_functions(current, name_view);
            
            // Add children to queue
            for (pugi::xml_node child = current.first_child(); child; child = child.next_sibling()) {
                nodes.push(child);
            }
        }
    } else {
        // Test on empty node when parsing fails
        pugi::xml_node empty_node;
        test_node_functions(empty_node, name_view);
        
        // Test on constructed empty node
        pugi::xml_node constructed_empty(nullptr);
        test_node_functions(constructed_empty, name_view);
    }
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}