// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get root node - if parsing failed, we'll work with empty node
    pugi::xml_node root = doc.document_element();
    
    // Prepare names for child/children calls
    // We'll use a fixed set of names to ensure null-termination
    const char* names[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    
    // First sequence: 8 child() calls
    pugi::xml_node node1 = root.child(names[0]);
    pugi::xml_node node2 = node1.child(names[1]);
    pugi::xml_node node3 = node2.child(names[2]);
    pugi::xml_node node4 = node3.child(names[3]);
    pugi::xml_node node5 = node4.child(names[4]);
    pugi::xml_node node6 = node5.child(names[5]);
    pugi::xml_node node7 = node6.child(names[6]);
    pugi::xml_node node8 = node7.child(names[7]);
    
    // Second sequence: 4 children() calls
    pugi::xml_object_range<pugi::xml_named_node_iterator> range1 = node8.children(names[0]);
    pugi::xml_object_range<pugi::xml_named_node_iterator> range2 = node8.children(names[1]);
    pugi::xml_object_range<pugi::xml_named_node_iterator> range3 = node8.children(names[2]);
    pugi::xml_object_range<pugi::xml_named_node_iterator> range4 = node8.children(names[3]);
    
    // Third sequence: various end() calls
    auto it1 = range1.end();
    pugi::xml_node first_child1 = node8.first_child();
    auto it2 = range2.end();
    auto it3 = range3.end();
    auto it4 = range4.end();
    pugi::xml_node first_child2 = node8.first_child();
    pugi::xml_node last_child1 = node8.last_child();
    auto it5 = range1.end();
    auto it6 = range2.end();
    auto it7 = range3.end();
    auto it8 = range4.end();
    auto it9 = range1.end();
    auto it10 = range2.end();
    pugi::xml_node first_child3 = node8.first_child();
    auto it11 = range1.end();
    auto it12 = range2.end();
    auto it13 = range1.begin();
    auto it14 = range2.end();
    auto it15 = range1.end();
    auto it16 = range1.begin();
    
    // The above calls exercise the API functions as requested
    // No explicit cleanup needed - RAII handles it
    
    return 0;
}