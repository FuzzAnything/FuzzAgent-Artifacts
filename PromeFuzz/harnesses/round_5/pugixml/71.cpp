// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    // Create a document to work with
    pugi::xml_document doc;
    
    // Try to parse the input as XML - if it fails, we'll still have a document to work with
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Create some nodes to work with
    pugi::xml_node root = doc.append_child("root");
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    pugi::xml_node child4 = root.append_child("child4");
    
    // Create some attributes and text nodes to make the tree more interesting
    child1.append_attribute("attr1") = "value1";
    child2.append_attribute("attr2") = "value2";
    child3.append_child(pugi::node_pcdata).set_value("text content");
    
    // Test set_name
    if (Size > 0) {
        // Use the first part of data for node name
        size_t name_len = Size > 100 ? 100 : Size;
        std::string_view name_view(reinterpret_cast<const char*>(Data), name_len);
        
        // Try set_name on different nodes
        child1.set_name(name_view);
        child2.set_name(name_view);
        
        // Also try on empty node
        pugi::xml_node empty_node;
        empty_node.set_name(name_view);
    }
    
    // Test insert_move_after
    // Move child3 after child1
    root.insert_move_after(child3, child1);
    
    // Try to move a node after itself (should fail)
    root.insert_move_after(child1, child1);
    
    // Try to move between different documents (should fail)
    pugi::xml_document doc2;
    pugi::xml_node other_root = doc2.append_child("other_root");
    pugi::xml_node other_child = other_root.append_child("other_child");
    root.insert_move_after(other_child, child2);
    
    // Test last_child
    pugi::xml_node last = root.last_child();
    pugi::xml_node last_of_last = last.last_child();
    
    // Test on empty node
    pugi::xml_node empty_node;
    pugi::xml_node empty_last = empty_node.last_child();
    
    // Test next_sibling
    pugi::xml_node next = child1.next_sibling();
    while (next) {
        pugi::xml_node temp = next.next_sibling();
        next = temp;
    }
    
    // Test child with name
    if (Size > 10) {
        // Use a portion of data for child name search
        size_t search_len = Size > 50 ? 50 : Size;
        std::string_view search_name(reinterpret_cast<const char*>(Data + 10), 
                                     search_len > 10 ? search_len - 10 : 1);
        
        // Ensure null termination for char* version if needed
        std::string search_str(search_name);
        
        pugi::xml_node found = root.child(search_name);
        pugi::xml_node found2 = child1.child(search_name);
        
        // Also test with empty name
        pugi::xml_node empty_name_found = root.child(std::string_view());
    }
    
    // Test previous_sibling
    pugi::xml_node prev = child4.previous_sibling();
    while (prev) {
        pugi::xml_node temp = prev.previous_sibling();
        prev = temp;
    }
    
    // Test previous_sibling with name
    if (Size > 20) {
        size_t prev_name_len = Size > 30 ? 30 : Size - 20;
        std::string_view prev_name(reinterpret_cast<const char*>(Data + 20), prev_name_len);
        
        pugi::xml_node prev_named = child4.previous_sibling(prev_name);
        pugi::xml_node prev_named2 = child3.previous_sibling(prev_name);
    }
    
    // Create a more complex tree structure for additional testing
    pugi::xml_node deep_child = child1.append_child("deep_child");
    deep_child.append_child("deeper_child").append_child("deepest_child");
    
    // Test traversal on deep tree
    pugi::xml_node deepest = root.child("child1").child("deep_child").child("deeper_child").child("deepest_child");
    
    // Test moving nodes within complex tree
    if (deepest) {
        child1.insert_move_after(deepest, child1.last_child());
    }
    
    // Test edge cases with empty nodes
    pugi::xml_node empty1, empty2;
    empty1.insert_move_after(empty2, empty1);
    empty1.last_child();
    empty1.next_sibling();
    empty1.child("test");
    empty1.previous_sibling();
    empty1.previous_sibling("test");
    
    // Clean up - let objects go out of scope naturally
    
    return 0;
}