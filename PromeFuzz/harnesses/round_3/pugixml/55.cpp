// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
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
#include <fstream>

static void create_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a dummy file with fuzzer data
    create_dummy_file(Data, Size);
    
    // Try to parse the dummy file as XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Create some test nodes for operations
    pugi::xml_node root = doc.root();
    
    // Test set_name
    if (!root.empty()) {
        std::string_view name_str(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(100)));
        root.set_name(name_str);
        
        // Test on child nodes too
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            child.set_name(name_str);
        }
    }
    
    // Test last_child
    pugi::xml_node last_child = root.last_child();
    
    // Test next_sibling
    pugi::xml_node current = root.first_child();
    while (!current.empty()) {
        pugi::xml_node next = current.next_sibling();
        current = next;
    }
    
    // Test child with name lookup
    if (Size > 1) {
        // Ensure null-terminated string for child lookup
        size_t name_len = std::min(Size, static_cast<size_t>(50));
        char* name_buf = new char[name_len + 1];
        memcpy(name_buf, Data, name_len);
        name_buf[name_len] = '\0';
        
        std::string_view child_name(name_buf, name_len);
        pugi::xml_node found_child = root.child(child_name);
        
        // Also test on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            child.child(child_name);
        }
        
        delete[] name_buf;
    }
    
    // Test previous_sibling
    current = root.last_child();
    while (!current.empty()) {
        pugi::xml_node prev = current.previous_sibling();
        current = prev;
    }
    
    // Test insert_move_after
    // Create a separate document for move operations
    pugi::xml_document doc2;
    pugi::xml_node root2 = doc2.append_child("root2");
    pugi::xml_node child1 = root2.append_child("child1");
    pugi::xml_node child2 = root2.append_child("child2");
    pugi::xml_node child3 = root2.append_child("child3");
    
    // Try various move operations
    if (!child1.empty() && !child2.empty()) {
        root2.insert_move_after(child1, child2);
    }
    
    if (!child2.empty() && !child3.empty()) {
        root2.insert_move_after(child2, child3);
    }
    
    // Also test with nodes from the parsed document
    if (!root.empty() && !last_child.empty()) {
        pugi::xml_node first_child = root.first_child();
        if (!first_child.empty() && !last_child.empty() && first_child != last_child) {
            root.insert_move_after(first_child, last_child);
        }
    }
    
    return 0;
}