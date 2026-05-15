// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing failed, return early
    if (!result) {
        return 0;
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Define predicates for find_node and find_child
    struct SimplePredicate {
        bool operator()(pugi::xml_node node) const {
            return node.name() && std::strlen(node.name()) > 0;
        }
    };
    
    // 1. Test find_node
    SimplePredicate pred;
    (void)root.find_node(pred);
    
    // 2. Test last_child
    (void)root.last_child();
    
    // 3. Test child with name parameter
    // Create null-terminated string from fuzzer input
    if (Size > 0) {
        // Ensure we have a null-terminated string
        size_t name_len = Size < 100 ? Size : 100;
        char* name_buf = new char[name_len + 1];
        std::memcpy(name_buf, Data, name_len);
        name_buf[name_len] = '\0';
        
        std::string_view name_sv(name_buf, name_len);
        (void)root.child(name_sv);
        
        delete[] name_buf;
    } else {
        // Test with empty name
        std::string_view empty_sv("");
        (void)root.child(empty_sv);
    }
    
    // 4. Test previous_sibling
    (void)root.previous_sibling();
    
    // 5. Test first_child
    (void)root.first_child();
    
    // 6. Test find_child
    (void)root.find_child(pred);
    
    // Traverse children to test functions on different nodes
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        // Test all functions on child nodes
        (void)child.find_node(pred);
        (void)child.last_child();
        
        if (Size > 0) {
            size_t child_name_len = Size < 50 ? Size : 50;
            char* child_name_buf = new char[child_name_len + 1];
            std::memcpy(child_name_buf, Data, child_name_len);
            child_name_buf[child_name_len] = '\0';
            
            std::string_view child_name_sv(child_name_buf, child_name_len);
            (void)child.child(child_name_sv);
            
            delete[] child_name_buf;
        }
        
        (void)child.previous_sibling();
        (void)child.first_child();
        (void)child.find_child(pred);
    }
    
    return 0;
}