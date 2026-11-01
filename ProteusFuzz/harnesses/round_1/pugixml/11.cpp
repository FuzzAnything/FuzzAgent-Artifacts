// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <vector>

static pugi::xml_document* create_document_from_fuzz(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Ensure we have valid UTF-8 or ASCII input for parsing
    // Convert raw bytes to a string, replacing invalid chars if necessary
    std::string str(reinterpret_cast<const char*>(data), size);
    
    // Basic sanitization to avoid parsing errors on binary data
    // Replace nulls with spaces to prevent early termination in string parsing
    for (char& c : str) {
        if (c == '\0') c = ' ';
    }
    
    // Add a root element if the string doesn't start with '<' to ensure valid XML structure
    if (str.empty() || str[0] != '<') {
        str = "<root>" + str + "</root>";
    }

    pugi::xml_parse_result result = doc->load_string(str.c_str());
    if (!result) {
        // Even if parsing fails partially, we might have a valid document structure to test
        // We proceed to test the API on whatever was loaded
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_fuzz(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->root();
    if (!root) {
        // If root is empty, create a simple structure to test move operations
        pugi::xml_node temp = doc->append_child("root");
        root = temp;
    }

    // Ensure we have some children to work with for move operations
    if (!root.first_child()) {
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
    }

    // 1. pugi::xml_node::child
    // Try to find a child named "child1"
    pugi::xml_node node1 = root.child("child1");
    if (!node1) {
        // If not found, try to find any existing child name or create one
        if (root.first_child()) {
            const char* name = root.first_child().name();
            if (name) node1 = root.child(name);
        }
    }

    // 2. pugi::xml_node::child (again)
    pugi::xml_node node2 = root.child("child2");
    if (!node2 && root.first_child()) {
        const char* name = root.first_child().name();
        if (name) node2 = root.child(name);
    }

    // 3. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();

    // 4. pugi::xml_node::last_child
    pugi::xml_node last = root.last_child();

    // Prepare nodes for move operations
    // We need valid nodes within the same document.
    // If previous nodes are empty, create new ones to ensure we have something to move.
    if (!node1) node1 = root.append_child("move_target_1");
    if (!node2) node2 = root.append_child("move_target_2");
    
    // Create a placeholder for the target of moves if needed
    pugi::xml_node target_node = root.first_child();
    if (!target_node) target_node = root.append_child("target");

    // 5. pugi::xml_node::prepend_move
    // Move node1 to the beginning of root's children
    pugi::xml_node res5 = root.prepend_move(node1);

    // 6. pugi::xml_node::prepend_move (again)
    // Try to move node2 (if valid) or create a new one
    if (!node2) node2 = root.append_child("move_target_2b");
    pugi::xml_node res6 = root.prepend_move(node2);

    // 7. pugi::xml_node::append_move
    // Move a node to the end. We need a source node.
    // If node1 was moved, it's still valid but at a different position.
    // Let's create a fresh node to append if needed.
    pugi::xml_node append_src = root.append_child("append_src");
    pugi::xml_node res7 = root.append_move(append_src);

    // 8. pugi::xml_node::append_move (again)
    pugi::xml_node append_src2 = root.append_child("append_src2");
    pugi::xml_node res8 = root.append_move(append_src2);

    // 9. pugi::xml_node::insert_move_before
    // Move a node before target_node
    pugi::xml_node insert_before_src = root.append_child("insert_before_src");
    pugi::xml_node res9 = root.insert_move_before(insert_before_src, target_node);

    // 10. pugi::xml_node::insert_move_before (again)
    pugi::xml_node insert_before_src2 = root.append_child("insert_before_src2");
    pugi::xml_node res10 = root.insert_move_before(insert_before_src2, target_node);

    // 11. pugi::xml_node::insert_move_after
    // Move a node after target_node
    pugi::xml_node insert_after_src = root.append_child("insert_after_src");
    pugi::xml_node res11 = root.insert_move_after(insert_after_src, target_node);

    // 12. pugi::xml_node::insert_move_after (again)
    pugi::xml_node insert_after_src2 = root.append_child("insert_after_src2");
    pugi::xml_node res12 = root.insert_move_after(insert_after_src2, target_node);

    // 13. pugi::xml_node::append_move
    // Final append move
    pugi::xml_node final_append_src = root.append_child("final_append_src");
    pugi::xml_node res13 = root.append_move(final_append_src);

    // Cleanup
    delete doc;

    return 0;
}