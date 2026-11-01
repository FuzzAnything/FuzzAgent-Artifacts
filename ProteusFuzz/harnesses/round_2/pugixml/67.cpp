// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
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
#include <cstring>
#include <fstream>
#include <string>

static bool try_parse_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    
    // Ensure null termination for parsing
    std::string buffer(reinterpret_cast<const char*>(data), size);
    if (buffer.back() != '\0') {
        buffer += '\0';
    }
    
    pugi::xml_parse_result result = doc.load_string(buffer.c_str());
    return result.status == pugi::status_ok;
}

static void test_last_child(pugi::xml_node node) {
    if (node) {
        pugi::xml_node last = node.last_child();
        (void)last; // Suppress unused warning
    }
}

static void test_parent(pugi::xml_node node) {
    if (node) {
        pugi::xml_node p = node.parent();
        (void)p; // Suppress unused warning
    }
}

static void test_path(pugi::xml_node node) {
    if (node) {
        std::string path1 = node.path('/');
        std::string path2 = node.path('.');
        (void)path1;
        (void)path2;
    }
}

static void test_insert_move_after(pugi::xml_document& doc, pugi::xml_node parent, const uint8_t* data, size_t size) {
    if (!parent || size < 2) return;
    
    pugi::xml_document temp_doc;
    if (!try_parse_document(temp_doc, data, size)) return;
    
    pugi::xml_node to_move = temp_doc.first_child();
    if (!to_move) return;
    
    pugi::xml_node after = parent.first_child();
    if (!after) return;
    
    // Attempt to move node
    pugi::xml_node result = parent.insert_move_after(to_move, after);
    (void)result;
}

static void test_insert_child_before(pugi::xml_node parent, const uint8_t* data, size_t size) {
    if (!parent || size < 2) return;
    
    // Extract name from fuzz data if possible
    std::string name_str(reinterpret_cast<const char*>(data), size);
    if (name_str.empty()) name_str = "test";
    
    // Find a sibling to insert before
    pugi::xml_node sibling = parent.first_child();
    if (!sibling) {
        // Try inserting at the beginning
        pugi::xml_node result = parent.insert_child_before(name_str.c_str(), pugi::xml_node());
        (void)result;
    } else {
        pugi::xml_node result = parent.insert_child_before(name_str.c_str(), sibling);
        (void)result;
    }
}

static void test_previous_sibling(pugi::xml_node node) {
    if (node) {
        pugi::xml_node prev = node.previous_sibling();
        (void)prev;
    }
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    bool parsed = try_parse_document(doc, Data, Size);
    
    if (!parsed) {
        // Create a minimal valid document for testing navigation on invalid structures
        doc.append_child("root");
    }
    
    // Get root node
    pugi::xml_node root = doc.first_child();
    if (!root) {
        // If still no root, create one
        root = doc.append_child("root");
    }
    
    // Test last_child on root and its children
    test_last_child(root);
    pugi::xml_node child = root.first_child();
    while (child) {
        test_last_child(child);
        child = child.next_sibling();
    }
    
    // Test parent on various nodes
    test_parent(root);
    child = root.first_child();
    while (child) {
        test_parent(child);
        child = child.next_sibling();
    }
    
    // Test path on various nodes
    test_path(root);
    child = root.first_child();
    while (child) {
        test_path(child);
        child = child.next_sibling();
    }
    
    // Test insert_move_after
    test_insert_move_after(doc, root, Data, Size);
    
    // Test insert_child_before
    test_insert_child_before(root, Data, Size);
    
    // Test previous_sibling on various nodes
    test_previous_sibling(root);
    child = root.first_child();
    while (child) {
        test_previous_sibling(child);
        child = child.next_sibling();
    }
    
    return 0;
}