// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        return 0; // Skip invalid XML
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Create a null-terminated string for child names
    std::string child_name;
    if (Size > 0) {
        child_name = std::string(reinterpret_cast<const char*>(Data), Size > 100 ? 100 : Size);
        // Ensure null termination
        if (child_name.size() > 0 && child_name.back() != '\0') {
            child_name.push_back('\0');
        }
    } else {
        child_name = "root\0";
    }
    
    // Create string_view for potential use
    std::string_view child_name_view(child_name.c_str(), child_name.size() - 1);
    
    // Call child() 8 times
    pugi::xml_node node1 = root.child(child_name.c_str());
    pugi::xml_node node2 = node1.child(child_name.c_str());
    pugi::xml_node node3 = node2.child(child_name.c_str());
    pugi::xml_node node4 = node3.child(child_name.c_str());
    pugi::xml_node node5 = node4.child(child_name.c_str());
    pugi::xml_node node6 = node5.child(child_name.c_str());
    pugi::xml_node node7 = node6.child(child_name.c_str());
    pugi::xml_node node8 = node7.child(child_name.c_str());
    
    // Call children() 4 times
    pugi::xml_object_range<pugi::xml_named_node_iterator> range1 = node8.children(child_name.c_str());
    pugi::xml_object_range<pugi::xml_named_node_iterator> range2 = node8.children(child_name.c_str());
    pugi::xml_object_range<pugi::xml_named_node_iterator> range3 = node8.children(child_name.c_str());
    pugi::xml_object_range<pugi::xml_named_node_iterator> range4 = node8.children(child_name.c_str());
    
    // Call end() on range4
    auto it1 = range4.end();
    
    // Call first_child() on node8
    pugi::xml_node first_child1 = node8.first_child();
    
    // Call end() 3 times on range4
    auto it2 = range4.end();
    auto it3 = range4.end();
    auto it4 = range4.end();
    
    // Call first_child() on node8 again
    pugi::xml_node first_child2 = node8.first_child();
    
    // Call last_child() on node8
    pugi::xml_node last_child1 = node8.last_child();
    
    // Call end() 6 times on range4
    auto it5 = range4.end();
    auto it6 = range4.end();
    auto it7 = range4.end();
    auto it8 = range4.end();
    auto it9 = range4.end();
    auto it10 = range4.end();
    
    // Call first_child() on node8 again
    pugi::xml_node first_child3 = node8.first_child();
    
    // Call end() 2 times on range4
    auto it11 = range4.end();
    auto it12 = range4.end();
    
    // Call begin() on range4
    auto it13 = range4.begin();
    
    // Call end() 2 times on range4
    auto it14 = range4.end();
    auto it15 = range4.end();
    
    // Call begin() on range4
    auto it16 = range4.begin();
    
    // Use the iterators and nodes to avoid compiler warnings
    (void)it1; (void)it2; (void)it3; (void)it4; (void)it5;
    (void)it6; (void)it7; (void)it8; (void)it9; (void)it10;
    (void)it11; (void)it12; (void)it13; (void)it14; (void)it15; (void)it16;
    (void)node1; (void)node2; (void)node3; (void)node4;
    (void)node5; (void)node6; (void)node7; (void)node8;
    (void)first_child1; (void)first_child2; (void)first_child3;
    (void)last_child1;
    (void)child_name_view;
    
    return 0;
}