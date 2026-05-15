// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
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

static pugi::xml_document create_test_document() {
    pugi::xml_document doc;
    doc.append_child("root");
    return doc;
}

static pugi::xml_document create_multi_node_document() {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    root.append_child("child1");
    root.append_child("child2");
    root.append_child("child3");
    return doc;
}

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t& offset, size_t max_len) {
    if (offset >= size) return "";
    size_t len = data[offset] % max_len + 1;
    offset = (offset + 1) % size;
    if (offset + len > size) len = size - offset;
    std::string_view result(reinterpret_cast<const char*>(data + offset), len);
    offset += len;
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;

    size_t offset = 0;
    
    // Create multiple documents to test cross-document cases
    pugi::xml_document doc1 = create_multi_node_document();
    pugi::xml_document doc2 = create_multi_node_document();
    
    pugi::xml_node root1 = doc1.child("root");
    pugi::xml_node root2 = doc2.child("root");
    
    // Get some child nodes to work with
    pugi::xml_node child1 = root1.child("child1");
    pugi::xml_node child2 = root1.child("child2");
    pugi::xml_node child3 = root1.child("child3");
    
    // Test append_child
    std::string_view name1 = get_string_view(Data, Size, offset, 10);
    pugi::xml_node new_child1 = root1.append_child(name1);
    
    // Test prepend_child
    std::string_view name2 = get_string_view(Data, Size, offset, 10);
    pugi::xml_node new_child2 = root1.prepend_child(name2);
    
    // Test insert_child_before
    if (!child1.empty()) {
        std::string_view name3 = get_string_view(Data, Size, offset, 10);
        pugi::xml_node new_child3 = root1.insert_child_before(name3, child1);
    }
    
    // Test insert_child_after
    if (!child2.empty()) {
        std::string_view name4 = get_string_view(Data, Size, offset, 10);
        pugi::xml_node new_child4 = root1.insert_child_after(name4, child2);
    }
    
    // Test prepend_move - move child3 to front of root1's children
    if (!child3.empty()) {
        pugi::xml_node moved1 = root1.prepend_move(child3);
    }
    
    // Test insert_move_before
    if (!child1.empty() && !child2.empty()) {
        // Create a new node to move
        pugi::xml_node temp = root1.append_child("temp");
        if (!temp.empty()) {
            pugi::xml_node moved2 = root1.insert_move_before(temp, child2);
        }
    }
    
    // Test cross-document move (should fail)
    if (!root2.child("child1").empty()) {
        pugi::xml_node moved3 = root1.prepend_move(root2.child("child1"));
    }
    
    // Test with empty nodes
    pugi::xml_node empty_node;
    root1.insert_child_before("test", empty_node);
    root1.prepend_move(empty_node);
    root1.insert_move_before(empty_node, child1);
    root1.insert_move_before(child1, empty_node);
    
    // Test with same node (should fail for move operations)
    if (!child1.empty()) {
        root1.insert_move_before(child1, child1);
    }
    
    // Create a more complex structure
    pugi::xml_document doc3;
    pugi::xml_node root3 = doc3.append_child("parent");
    for (int i = 0; i < 5 && offset + 1 < Size; i++) {
        std::string_view child_name = get_string_view(Data, Size, offset, 5);
        pugi::xml_node child = root3.append_child(child_name);
        
        // Add attributes to some nodes
        if (i % 2 == 0 && offset + 2 < Size) {
            std::string_view attr_name = get_string_view(Data, Size, offset, 3);
            std::string_view attr_value = get_string_view(Data, Size, offset, 5);
            child.append_attribute(attr_name.data()) = attr_value.data();
        }
    }
    
    // Test moving nodes in the complex structure
    pugi::xml_node first_child = root3.first_child();
    pugi::xml_node last_child = root3.last_child();
    
    if (!first_child.empty() && !last_child.empty() && first_child != last_child) {
        root3.insert_move_before(last_child, first_child);
    }
    
    // Test chain of operations
    pugi::xml_document doc4;
    pugi::xml_node root4 = doc4.append_child("chain_root");
    
    for (int i = 0; i < 3 && offset + 1 < Size; i++) {
        std::string_view node_name = get_string_view(Data, Size, offset, 4);
        pugi::xml_node node = root4.append_child(node_name);
        
        if (i > 0) {
            pugi::xml_node prev_node = root4.last_child().previous_sibling();
            if (!prev_node.empty()) {
                std::string_view insert_name = get_string_view(Data, Size, offset, 4);
                root4.insert_child_after(insert_name, prev_node);
            }
        }
    }
    
    return 0;
}