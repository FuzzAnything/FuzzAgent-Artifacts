// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <cstring>
#include <fstream>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Create XML document and parse input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, create a minimal valid XML document for testing
    if (!result) {
        const char* minimal_xml = "<root><a/><b/><c/><d/></root>";
        doc.load_string(minimal_xml);
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare name strings for child/children functions
    // Use a fixed set of names to ensure null termination
    const char* names[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    size_t name_index = 0;
    
    // Call sequence of functions
    // 8 child() calls
    pugi::xml_node current_node = root;
    for (int i = 0; i < 8; ++i) {
        current_node = current_node.child(names[name_index % 8]);
        name_index++;
    }
    
    // 4 children() calls - initialize directly with function return values
    pugi::xml_object_range<pugi::xml_named_node_iterator> range1 = root.children(names[name_index % 8]);
    name_index++;
    pugi::xml_object_range<pugi::xml_named_node_iterator> range2 = root.children(names[name_index % 8]);
    name_index++;
    pugi::xml_object_range<pugi::xml_named_node_iterator> range3 = root.children(names[name_index % 8]);
    name_index++;
    pugi::xml_object_range<pugi::xml_named_node_iterator> range4 = root.children(names[name_index % 8]);
    name_index++;
    
    // Call end() on range4
    auto it1 = range4.end();
    
    // Call first_child()
    pugi::xml_node first = root.first_child();
    
    // Call end() on range4 three more times
    auto it2 = range4.end();
    auto it3 = range4.end();
    auto it4 = range4.end();
    
    // Call first_child() again
    pugi::xml_node first2 = root.first_child();
    
    // Call last_child()
    pugi::xml_node last = root.last_child();
    
    // Call end() on range4 six more times
    auto it5 = range4.end();
    auto it6 = range4.end();
    auto it7 = range4.end();
    auto it8 = range4.end();
    auto it9 = range4.end();
    auto it10 = range4.end();
    
    // Call first_child() again
    pugi::xml_node first3 = root.first_child();
    
    // Call end() on range4 two more times
    auto it11 = range4.end();
    auto it12 = range4.end();
    
    // Call begin() on range4
    auto it13 = range4.begin();
    
    // Call end() on range4 two more times
    auto it14 = range4.end();
    auto it15 = range4.end();
    
    // Call begin() on range4
    auto it16 = range4.begin();
    
    // Use the iterators and nodes to avoid unused variable warnings
    (void)it1; (void)it2; (void)it3; (void)it4; (void)it5;
    (void)it6; (void)it7; (void)it8; (void)it9; (void)it10;
    (void)it11; (void)it12; (void)it13; (void)it14; (void)it15;
    (void)it16;
    (void)first; (void)first2; (void)first3; (void)last;
    (void)current_node;
    (void)range1; (void)range2; (void)range3; (void)range4;
    
    return 0;
}