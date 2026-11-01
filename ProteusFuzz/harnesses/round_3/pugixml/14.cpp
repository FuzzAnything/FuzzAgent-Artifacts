// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <fstream>
#include <sstream>

static bool write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
    file.close();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have some valid XML structure to work with
    // We prepend a minimal valid XML structure to the fuzz data if needed
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Ensure the input starts with a valid XML declaration or root if possible, 
    // but pugixml is robust enough to handle garbage if we parse carefully.
    // To ensure we have nodes to move, we wrap the input or append a known structure.
    
    std::string safe_input;
    safe_input.reserve(input_str.size() + 50);
    
    // Try to ensure there's at least a root element if the input is empty or invalid
    // We will parse the raw data, but if it fails completely, we create a fallback.
    pugi::xml_document doc;
    
    // Load from string
    pugi::xml_parse_result result = doc.load_string(input_str.c_str());
    
    // If parsing failed completely (no root), try to create a minimal structure
    // to ensure we have nodes to operate on for the move functions.
    if (!result) {
        // Create a fallback document with some nodes
        pugi::xml_node root = doc.append_child("root");
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
        root.append_child("child4");
        root.append_child("child5");
        root.append_child("child6");
        root.append_child("child7");
        root.append_child("child8");
        root.append_child("child9");
        root.append_child("child10");
    }

    // Helper to get a node safely
    auto get_node = [](pugi::xml_node parent) -> pugi::xml_node {
        if (!parent) return pugi::xml_node();
        return parent.first_child();
    };

    // We need multiple nodes to perform moves. Let's try to find a chain or multiple siblings.
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.first_child();
    }
    if (!root) {
        // If still no root, try to create one dynamically if the document is empty
        root = doc.append_child("root");
        for (int i = 0; i < 10; ++i) {
            root.append_child("node");
        }
    }

    // 1. pugi::xml_node::child
    std::string_view a_str = "child1";
    pugi::xml_node node1 = root.child(a_str.data());
    
    // 2. pugi::xml_node::child (again)
    // Try a different name or same name
    std::string_view b_str = "child2";
    pugi::xml_node node2 = root.child(b_str.data());
    
    // 3. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();
    
    // 4. pugi::xml_node::last_child
    pugi::xml_node last = root.last_child();

    // Prepare nodes for moving. We need distinct nodes.
    // If the initial nodes are null, try to get siblings.
    pugi::xml_node move_source_1 = first;
    pugi::xml_node move_target_1 = last;
    
    if (!move_source_1) {
        move_source_1 = root.first_child();
    }
    if (!move_target_1) {
        move_target_1 = root.first_child();
    }
    
    // Ensure we have at least 2 children to move
    pugi::xml_node sibling1 = root.first_child();
    pugi::xml_node sibling2 = sibling1 ? sibling1.next_sibling() : pugi::xml_node();
    pugi::xml_node sibling3 = sibling2 ? sibling2.next_sibling() : pugi::xml_node();
    pugi::xml_node sibling4 = sibling3 ? sibling3.next_sibling() : pugi::xml_node();
    pugi::xml_node sibling5 = sibling4 ? sibling4.next_sibling() : pugi::xml_node();
    
    // 5. pugi::xml_node::prepend_move
    // Move sibling2 to the beginning of root
    if (sibling2 && root) {
        root.prepend_move(sibling2);
    }
    
    // 6. pugi::xml_node::prepend_move (again)
    if (sibling3 && root) {
        root.prepend_move(sibling3);
    }

    // 7. pugi::xml_node::append_move
    if (sibling4 && root) {
        root.append_move(sibling4);
    }

    // 8. pugi::xml_node::append_move (again)
    if (sibling5 && root) {
        root.append_move(sibling5);
    }

    // 9. pugi::xml_node::insert_move_before
    // Try to move sibling1 before sibling2 (if they exist and are distinct)
    if (sibling1 && sibling2 && sibling1 != sibling2) {
        root.insert_move_before(sibling1, sibling2);
    }

    // 10. pugi::xml_node::insert_move_before (again)
    // Try to move sibling3 before sibling4
    if (sibling3 && sibling4 && sibling3 != sibling4) {
        root.insert_move_before(sibling3, sibling4);
    }

    // 11. pugi::xml_node::insert_move_after
    // Try to move sibling1 after sibling2
    if (sibling1 && sibling2 && sibling1 != sibling2) {
        root.insert_move_after(sibling1, sibling2);
    }

    // 12. pugi::xml_node::insert_move_after (again)
    // Try to move sibling3 after sibling4
    if (sibling3 && sibling4 && sibling3 != sibling4) {
        root.insert_move_after(sibling3, sibling4);
    }

    // 13. pugi::xml_node::append_move
    // Final append move
    if (sibling1 && root) {
        root.append_move(sibling1);
    }

    return 0;
}