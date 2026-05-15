// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
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
#include <string_view>

static void* custom_allocate(size_t size) {
    return malloc(size);
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Get root node or create one if parsing failed
    pugi::xml_node root = doc.root();
    if (root.empty()) {
        root = doc.append_child("root");
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
    }
    
    // First sequence of API calls
    pugi::xml_node node1 = root.first_child();
    pugi::xml_node node2 = node1.next_sibling();
    
    if (!node2.empty()) {
        const char* val = node2.value();
        (void)val; // Use value to avoid unused variable warning
        
        // Use a small string for set_value to avoid excessive memory usage
        static const char small_str[] = "test";
        node2.set_value(small_str);
    }
    
    // Second sequence of API calls
    pugi::xml_node node3 = root.first_child();
    pugi::xml_node node4 = node3.next_sibling();
    pugi::xml_node node5 = node4.next_sibling();
    pugi::xml_node parent = node5.parent();
    
    if (!parent.empty() && !parent.first_child().empty()) {
        parent.remove_child(parent.first_child());
    }
    
    // Set memory management functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
    
    return 0;
}