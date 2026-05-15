// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

static pugi::xml_document doc;
static pugi::xml_node root;

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Reset document for each fuzzing iteration
    doc.reset();
    
    // Create a root node to work with
    root = doc.append_child(pugi::node_element);
    
    // First append_child - with node_type
    pugi::xml_node child1 = root.append_child(pugi::node_element);
    
    // Second append_child - with name
    // Ensure null-terminated string for name
    if (Size > 0) {
        size_t name_len = Size > 100 ? 100 : Size; // Limit name length
        std::string name_str(reinterpret_cast<const char*>(Data), name_len);
        pugi::xml_node child2 = root.append_child(name_str.c_str());
    } else {
        pugi::xml_node child2 = root.append_child("default");
    }
    
    // First child call - try to get child by name
    if (Size > 0) {
        size_t child_name_len = Size > 50 ? 50 : Size;
        std::string child_name(reinterpret_cast<const char*>(Data), child_name_len);
        pugi::xml_node found_child = root.child(child_name.c_str());
    }
    
    // First append_buffer call
    if (Size > 0) {
        // Use the entire fuzzer input as buffer
        pugi::xml_parse_result result = root.append_buffer(Data, Size);
        // Ignore parse errors - they're expected with random input
    }
    
    // Second child call
    if (Size > 10) {
        // Use a portion of the data for child name
        size_t offset = Size > 100 ? 50 : Size/2;
        size_t len = Size - offset > 20 ? 20 : Size - offset;
        std::string child_name2(reinterpret_cast<const char*>(Data + offset), len);
        pugi::xml_node child3 = root.child(child_name2.c_str());
    }
    
    // Third child call
    pugi::xml_node child4 = root.child("test");
    
    // remove_children
    bool removed = root.remove_children();
    
    // Fourth child call (after removal)
    pugi::xml_node child5 = root.child("any");
    
    // Second append_buffer call with different encoding options
    if (Size > 0) {
        // Try with different encoding options
        pugi::xml_parse_result result2 = root.append_buffer(Data, Size, 
            pugi::parse_default | pugi::parse_ws_pcdata, pugi::encoding_auto);
    }
    
    // Final child call
    pugi::xml_node child6 = root.child("final");
    
    return 0;
}