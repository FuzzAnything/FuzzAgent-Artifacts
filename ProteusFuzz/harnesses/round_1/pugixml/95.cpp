// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include "pugixml.hpp"

static bool custom_walker_begin(pugi::xml_node& node) {
    // Custom logic for begin: allow traversal if node is not null
    return node != pugi::xml_node();
}

static bool custom_walker_end(pugi::xml_node& node) {
    // Custom logic for end: continue traversal
    return true;
}

static void traverse_and_test(pugi::xml_node node) {
    // Check if node is null by comparing with default constructed node
    if (node == pugi::xml_node()) return;

    // Test root()
    pugi::xml_node root_node = node.root();
    (void)root_node;

    // Test first_child()
    pugi::xml_node first_child = node.first_child();
    if (first_child != pugi::xml_node()) {
        traverse_and_test(first_child);
    }

    // Test parent()
    pugi::xml_node parent_node = node.parent();
    (void)parent_node;

    // Test next_sibling()
    pugi::xml_node next_sibling = node.next_sibling();
    if (next_sibling != pugi::xml_node()) {
        traverse_and_test(next_sibling);
    }
}

static void run_custom_walker(pugi::xml_node node) {
    if (node == pugi::xml_node()) return;

    struct CustomWalker : public pugi::xml_tree_walker {
        bool begin(pugi::xml_node& n) override {
            return custom_walker_begin(n);
        }

        bool end(pugi::xml_node& n) override {
            return custom_walker_end(n);
        }

        // Required pure virtual method implementation
        bool for_each(pugi::xml_node& n) override {
            // Default behavior: do nothing special, just continue
            return true;
        }
    };

    CustomWalker walker;
    node.traverse(walker);
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a string from the input data
    std::string xml_data(reinterpret_cast<const char*>(Data), Size);

    // Parse the XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

    // Proceed even if parsing failed to test error handling paths
    pugi::xml_node root = doc.root();

    // Test traversal functions
    traverse_and_test(root);

    // Test custom walker
    run_custom_walker(root);

    // Additional edge case: test on an empty node
    pugi::xml_node empty_node;
    traverse_and_test(empty_node);
    run_custom_walker(empty_node);

    // Test root() on empty node
    pugi::xml_node empty_root = empty_node.root();
    (void)empty_root;

    // Test first_child() on empty node
    pugi::xml_node empty_first = empty_node.first_child();
    (void)empty_first;

    // Test parent() on empty node
    pugi::xml_node empty_parent = empty_node.parent();
    (void)empty_parent;

    // Test next_sibling() on empty node
    pugi::xml_node empty_next = empty_node.next_sibling();
    (void)empty_next;

    return 0;
}