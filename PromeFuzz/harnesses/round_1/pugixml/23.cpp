// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        return 0; // Skip invalid XML
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare string views for child names (ensure null-terminated)
    static const pugi::char_t* child_names[] = {
        PUGIXML_TEXT("child1"),
        PUGIXML_TEXT("child2"),
        PUGIXML_TEXT("child3"),
        PUGIXML_TEXT("child4"),
        PUGIXML_TEXT("child5"),
        PUGIXML_TEXT("child6"),
        PUGIXML_TEXT("child7"),
        PUGIXML_TEXT("child8"),
        PUGIXML_TEXT("child9"),
        PUGIXML_TEXT("child10")
    };
    
    // Execute API calls in specified order
    pugi::xml_node node1 = root.child(child_names[0]);
    pugi::xml_node node2 = root.child(child_names[1]);
    pugi::xml_node node3 = root.insert_copy_before(node1, node2);
    pugi::xml_node node4 = root.child(child_names[2]);
    pugi::xml_node node5 = root.insert_copy_before(node3, node4);
    pugi::xml_node node6 = root.child(child_names[3]);
    pugi::xml_node node7 = root.child(child_names[4]);
    pugi::xml_node node8 = root.child(child_names[5]);
    pugi::xml_node node9 = root.child(child_names[6]);
    pugi::xml_node node10 = root.insert_copy_before(node6, node7);
    pugi::xml_node node11 = root.child(child_names[7]);
    pugi::xml_node node12 = root.first_child();
    const pugi::char_t* name1 = root.name();
    pugi::xml_node node13 = root.child(child_names[8]);
    pugi::xml_node node14 = root.insert_copy_before(node8, node9);
    pugi::xml_node node15 = root.child(child_names[9]);
    pugi::xml_node node16 = root.first_child();
    pugi::xml_node node17 = root.child(PUGIXML_TEXT(""));
    pugi::xml_node node18 = root.last_child();
    const pugi::char_t* name2 = root.name();
    pugi::xml_node node19 = root.child(PUGIXML_TEXT("dummy"));
    pugi::xml_node node20 = root.insert_copy_before(node10, node11);
    pugi::xml_node node21 = root.child(PUGIXML_TEXT("test"));
    pugi::xml_node node22 = root.first_child();
    pugi::xml_node node23 = node22.next_sibling();
    pugi::xml_node node24 = root.child(PUGIXML_TEXT(""));
    pugi::xml_node node25 = root.first_child();
    const pugi::char_t* value = root.value();
    
    // Use the results to avoid dead code elimination
    (void)node1;
    (void)node2;
    (void)node3;
    (void)node4;
    (void)node5;
    (void)node6;
    (void)node7;
    (void)node8;
    (void)node9;
    (void)node10;
    (void)node11;
    (void)node12;
    (void)name1;
    (void)node13;
    (void)node14;
    (void)node15;
    (void)node16;
    (void)node17;
    (void)node18;
    (void)name2;
    (void)node19;
    (void)node20;
    (void)node21;
    (void)node22;
    (void)node23;
    (void)node24;
    (void)node25;
    (void)value;
    
    return 0;
}