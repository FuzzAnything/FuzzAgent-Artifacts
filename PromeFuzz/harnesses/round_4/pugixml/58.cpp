// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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
#include <functional>

static void fuzz_find_child(pugi::xml_node& node) {
    // Define a predicate that checks if node has an attribute with value < 20
    auto pred = [](pugi::xml_node child) -> bool {
        for (pugi::xml_attribute attr : child.attributes()) {
            int value = attr.as_int(0);
            if (value < 20) return true;
        }
        return false;
    };
    
    node.find_child(pred);
}

static void fuzz_previous_sibling(pugi::xml_node& node, const std::string_view& name) {
    node.previous_sibling(name.data());
}

static void fuzz_child(pugi::xml_node& node, const std::string_view& name) {
    node.child(name.data());
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create XML document and parse input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing failed, we can still test on empty nodes
    if (!result) {
        // Create empty nodes and test functions
        pugi::xml_node empty_node;
        
        // Test all functions on empty node
        empty_node.parent();
        empty_node.first_child();
        empty_node.last_child();
        empty_node.previous_sibling();
        
        // For functions requiring string parameters
        std::string_view empty_sv;
        fuzz_previous_sibling(empty_node, empty_sv);
        fuzz_child(empty_node, empty_sv);
        fuzz_find_child(empty_node);
        
        return 0;
    }
    
    // Get document root
    pugi::xml_node root = doc.document_element();
    
    // Test functions on root node
    root.parent();
    root.first_child();
    root.last_child();
    root.previous_sibling();
    
    // Create string_view from input for name parameters
    // Ensure null termination by copying to a buffer
    size_t name_len = Size > 100 ? 100 : Size;
    char name_buf[101];
    memcpy(name_buf, Data, name_len);
    name_buf[name_len] = '\0';
    std::string_view name_sv(name_buf, name_len);
    
    // Test functions with name parameters
    fuzz_previous_sibling(root, name_sv);
    fuzz_child(root, name_sv);
    fuzz_find_child(root);
    
    // Traverse children to test functions on different nodes
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        child.parent();
        child.first_child();
        child.last_child();
        child.previous_sibling();
        
        fuzz_previous_sibling(child, name_sv);
        fuzz_child(child, name_sv);
        fuzz_find_child(child);
        
        // Test on grandchildren too
        for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
            grandchild.parent();
            grandchild.first_child();
            grandchild.last_child();
            grandchild.previous_sibling();
            
            fuzz_previous_sibling(grandchild, name_sv);
            fuzz_child(grandchild, name_sv);
            fuzz_find_child(grandchild);
        }
    }
    
    return 0;
}