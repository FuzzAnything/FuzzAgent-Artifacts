// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstring>
#include <string>

static bool parse_xml_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        doc.load_string("<root/>");
        return true;
    }
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    
    if (!parse_xml_document(doc, Data, Size)) {
        // If parsing fails, try with a minimal valid document to ensure we can test the API
        if (!doc.load_string("<root><a/><b/></root>")) {
            return 0;
        }
    }

    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        // Try to find any child if root is not found
        root = doc.first_child();
        if (root.empty()) {
            // Create a structure if document is empty
            pugi::xml_node new_root = doc.append_child("root");
            new_root.append_child("a");
            new_root.append_child("b");
            new_root.append_child("c");
            root = doc.child("root");
        }
    }

    // 1. pugi::xml_node::child
    std::string_view a_str("a");
    pugi::xml_node node1 = root.child(a_str.data());

    // 2. pugi::xml_node::child
    std::string_view b_str("b");
    pugi::xml_node node2 = root.child(b_str.data());

    // 3. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();

    // 4. pugi::xml_node::last_child
    pugi::xml_node last = root.last_child();

    // Prepare nodes for move operations. 
    // We need valid nodes from the same document to avoid immediate failure.
    // If the previous steps didn't find nodes, create them.
    pugi::xml_node move_source1, move_source2, move_source3, move_source4, move_source5;
    pugi::xml_node target_parent = root;
    pugi::xml_node reference_node = root;

    if (node1.empty()) {
        move_source1 = target_parent.append_child("move1");
    } else {
        move_source1 = node1;
    }

    if (node2.empty()) {
        move_source2 = target_parent.append_child("move2");
    } else {
        move_source2 = node2;
    }
    
    // Ensure we have enough nodes for complex moves
    if (move_source1.empty() || move_source2.empty()) {
        move_source1 = target_parent.append_child("temp1");
        move_source2 = target_parent.append_child("temp2");
    }

    // 5. pugi::xml_node::prepend_move
    // Move move_source1 to the beginning of target_parent's children
    if (!move_source1.empty() && move_source1.parent() == target_parent) {
        target_parent.prepend_move(move_source1);
    }

    // 6. pugi::xml_node::prepend_move (second call)
    // Try to move move_source2
    if (!move_source2.empty() && move_source2.parent() == target_parent) {
        target_parent.prepend_move(move_source2);
    }

    // 7. pugi::xml_node::append_move
    // Create a new node to move if needed
    pugi::xml_node node_to_append = target_parent.append_child("append_target");
    if (!node_to_append.empty()) {
        target_parent.append_move(node_to_append);
    }

    // 8. pugi::xml_node::append_move (second call)
    pugi::xml_node node_to_append2 = target_parent.append_child("append_target2");
    if (!node_to_append2.empty()) {
        target_parent.append_move(node_to_append2);
    }

    // 9. pugi::xml_node::insert_move_before
    // Need a reference node and a node to move
    pugi::xml_node ref_before = target_parent.first_child();
    pugi::xml_node move_before = target_parent.append_child("move_before_target");
    
    if (!ref_before.empty() && !move_before.empty()) {
        target_parent.insert_move_before(move_before, ref_before);
    }

    // 10. pugi::xml_node::insert_move_before (second call)
    ref_before = target_parent.first_child();
    move_before = target_parent.append_child("move_before_target2");
    if (!ref_before.empty() && !move_before.empty()) {
        target_parent.insert_move_before(move_before, ref_before);
    }

    // 11. pugi::xml_node::insert_move_after
    pugi::xml_node ref_after = target_parent.first_child();
    pugi::xml_node move_after = target_parent.append_child("move_after_target");
    
    if (!ref_after.empty() && !move_after.empty()) {
        target_parent.insert_move_after(move_after, ref_after);
    }

    // 12. pugi::xml_node::insert_move_after (second call)
    ref_after = target_parent.first_child();
    move_after = target_parent.append_child("move_after_target2");
    if (!ref_after.empty() && !move_after.empty()) {
        target_parent.insert_move_after(move_after, ref_after);
    }

    // 13. pugi::xml_node::append_move
    pugi::xml_node final_append = target_parent.append_child("final_append_target");
    if (!final_append.empty()) {
        target_parent.append_move(final_append);
    }

    return 0;
}