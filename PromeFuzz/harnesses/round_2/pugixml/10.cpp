// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
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
#include <string>
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Create a temporary file with the fuzzer input
    const char* filename = "./dummy_file";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Parse XML document from file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    if (!result) {
        return 0;
    }

    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare string names for child() calls
    std::string name1 = "test1";
    std::string name2 = "test2";
    
    // Execute API sequence
    pugi::xml_node child1 = root.child(name1.c_str());
    pugi::xml_node child2 = root.child(name2.c_str());
    pugi::xml_node first = root.first_child();
    pugi::xml_node last = root.last_child();
    
    // Move operations - use available nodes
    if (child1 && child2) {
        pugi::xml_node moved1 = root.prepend_move(child1);
        pugi::xml_node moved2 = root.prepend_move(child2);
    }
    
    if (first && last) {
        pugi::xml_node moved3 = root.append_move(first);
        pugi::xml_node moved4 = root.append_move(last);
    }
    
    // Get new references after previous moves
    pugi::xml_node new_child1 = root.child(name1.c_str());
    pugi::xml_node new_child2 = root.child(name2.c_str());
    pugi::xml_node new_first = root.first_child();
    pugi::xml_node new_last = root.last_child();
    
    // Insert move operations
    if (new_child1 && new_child2) {
        pugi::xml_node moved5 = root.insert_move_before(new_child1, new_child2);
        pugi::xml_node moved6 = root.insert_move_before(new_child2, new_child1);
    }
    
    if (new_first && new_last) {
        pugi::xml_node moved7 = root.insert_move_after(new_first, new_last);
        pugi::xml_node moved8 = root.insert_move_after(new_last, new_first);
    }
    
    // Final append_move
    pugi::xml_node final_child = root.child(name1.c_str());
    if (final_child) {
        pugi::xml_node moved9 = root.append_move(final_child);
    }

    return 0;
}