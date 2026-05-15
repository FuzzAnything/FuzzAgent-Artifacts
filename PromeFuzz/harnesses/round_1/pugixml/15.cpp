// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

static std::string create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len = 100) {
    size_t len = size < max_len ? size : max_len;
    std::string result;
    result.reserve(len + 1);
    for (size_t i = 0; i < len; ++i) {
        char c = static_cast<char>(data[i] % 128);
        if (c == '\0') c = 'a';
        result.push_back(c);
    }
    result.push_back('\0');
    return result;
}

static void populate_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    pugi::xml_node root = doc.append_child("root");
    
    for (int i = 0; i < 10; ++i) {
        std::string child_name = "child" + std::to_string(i);
        pugi::xml_node child = root.append_child(child_name.c_str());
        
        for (int j = 0; j < 3; ++j) {
            std::string grandchild_name = "grandchild" + std::to_string(j);
            child.append_child(grandchild_name.c_str());
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;
    
    pugi::xml_document doc1;
    pugi::xml_document doc2;
    
    populate_document(doc1, Data, Size);
    populate_document(doc2, Data, Size);
    
    pugi::xml_node root1 = doc1.child("root");
    pugi::xml_node root2 = doc2.child("root");
    
    if (!root1 || !root2) return 0;
    
    std::string name1 = create_null_terminated_string(Data, Size);
    std::string name2 = create_null_terminated_string(Data + Size/2, Size/2);
    
    pugi::xml_node child1 = root1.child(name1.c_str());
    pugi::xml_node child2 = root1.child(name2.c_str());
    
    pugi::xml_node first_child = root1.first_child();
    pugi::xml_node last_child = root1.last_child();
    
    if (first_child) {
        pugi::xml_node moved1 = root1.prepend_move(first_child);
    }
    
    if (last_child) {
        pugi::xml_node moved2 = root1.prepend_move(last_child);
    }
    
    if (child1) {
        pugi::xml_node moved3 = root1.append_move(child1);
    }
    
    if (child2) {
        pugi::xml_node moved4 = root1.append_move(child2);
    }
    
    std::vector<pugi::xml_node> children;
    for (pugi::xml_node child = root1.first_child(); child; child = child.next_sibling()) {
        children.push_back(child);
    }
    
    if (children.size() >= 2) {
        pugi::xml_node moved5 = root1.insert_move_before(children[0], children[1]);
    }
    
    if (children.size() >= 3) {
        pugi::xml_node moved6 = root1.insert_move_before(children[1], children[2]);
    }
    
    if (children.size() >= 2) {
        pugi::xml_node moved7 = root1.insert_move_after(children[0], children[1]);
    }
    
    if (children.size() >= 3) {
        pugi::xml_node moved8 = root1.insert_move_after(children[1], children[2]);
    }
    
    if (children.size() >= 1) {
        pugi::xml_node moved9 = root1.append_move(children[0]);
    }
    
    return 0;
}