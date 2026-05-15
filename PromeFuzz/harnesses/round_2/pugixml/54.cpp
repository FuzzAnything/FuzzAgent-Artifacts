// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for XML parsing
    std::ofstream file("./dummy_file", std::ios::binary);
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Parse XML from file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Only proceed if parsing succeeded
    if (result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Test API functions on various nodes
        // 1. Test on root node
        pugi::xml_node prev_sibling = root.previous_sibling();
        pugi::xml_node first_child = root.first_child();
        std::string path_str = root.path('/');
        std::string alt_path_str = root.path('.');
        
        // 2. Test child() function with various names
        // Use string_view for child name parameter
        std::string_view empty_name = "";
        std::string_view test_name = "test";
        pugi::xml_node child_empty = root.child(empty_name);
        pugi::xml_node child_test = root.child(test_name);
        
        // 3. Test on first child if exists
        if (first_child) {
            pugi::xml_node last_child = first_child.last_child();
            pugi::xml_node parent = first_child.parent();
            pugi::xml_node prev_sibling_child = first_child.previous_sibling();
            
            // Test path on first child
            std::string child_path = first_child.path('/');
            
            // Test child() on first child
            pugi::xml_node child_of_child = first_child.child(empty_name);
        }
        
        // 4. Test on a null/empty node
        pugi::xml_node empty_node;
        pugi::xml_node empty_prev = empty_node.previous_sibling();
        pugi::xml_node empty_first = empty_node.first_child();
        pugi::xml_node empty_last = empty_node.last_child();
        pugi::xml_node empty_parent = empty_node.parent();
        std::string empty_path = empty_node.path('/');
        pugi::xml_node empty_child = empty_node.child(empty_name);
        
        // 5. Traverse document and test functions on multiple nodes
        for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
            // Test all API functions on each node
            pugi::xml_node node_prev = node.previous_sibling();
            pugi::xml_node node_first = node.first_child();
            pugi::xml_node node_last = node.last_child();
            pugi::xml_node node_parent = node.parent();
            std::string node_path = node.path('/');
            pugi::xml_node node_child = node.child(empty_name);
            
            // Also test with a non-empty name
            if (node.name() && strlen(node.name()) > 0) {
                std::string_view node_name = node.name();
                pugi::xml_node named_child = node.child(node_name);
            }
            
            // Recursively test on children
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                pugi::xml_node child_prev = child.previous_sibling();
                pugi::xml_node child_first = child.first_child();
                pugi::xml_node child_last = child.last_child();
                pugi::xml_node child_parent = child.parent();
                std::string child_path = child.path('/');
                pugi::xml_node child_child = child.child(empty_name);
            }
        }
        
        // 6. Test with constructed xml_node from internal pointer
        // Create null node struct pointer
        pugi::xml_node_struct* null_struct = nullptr;
        pugi::xml_node constructed_node(null_struct);
        
        // Test API functions on constructed node
        pugi::xml_node const_prev = constructed_node.previous_sibling();
        pugi::xml_node const_first = constructed_node.first_child();
        pugi::xml_node const_last = constructed_node.last_child();
        pugi::xml_node const_parent = constructed_node.parent();
        std::string const_path = constructed_node.path('/');
        pugi::xml_node const_child = constructed_node.child(empty_name);
    }
    
    // Cleanup - remove dummy file
    std::remove("./dummy_file");
    
    return 0;
}