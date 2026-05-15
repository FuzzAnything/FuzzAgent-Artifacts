// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
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

static void* custom_allocate(size_t size) {
    return malloc(size);
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Set custom memory management functions first
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
    
    // Create an XML document and parse input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        // If parsing fails, create a simple document for testing
        doc.append_child("root").append_child("child1").text().set("value1");
        doc.child("root").append_child("child2").text().set("value2");
        doc.child("root").append_child("child3").text().set("value3");
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Sequence of API calls as specified
    pugi::xml_node node1 = root.first_child();
    pugi::xml_node node2 = node1.next_sibling();
    const char* value = node2.value();
    
    // Use fuzzer input for set_value if we have data
    if (Size > 0) {
        // Ensure null-terminated string for set_value
        std::string str(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(100)));
        node2.set_value(str.c_str());
    } else {
        node2.set_value("test_value");
    }
    
    pugi::xml_node node3 = node2.first_child();
    pugi::xml_node node4 = node3.next_sibling();
    pugi::xml_node node5 = node4.next_sibling();
    pugi::xml_node parent = node5.parent();
    
    // Remove a child node if possible
    if (parent) {
        pugi::xml_node first_child = parent.first_child();
        if (first_child) {
            parent.remove_child(first_child);
        }
    }
    
    // Set memory management functions again at the end
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
    
    return 0;
}