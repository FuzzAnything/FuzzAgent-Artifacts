// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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

static class CustomTreeWalker : public pugi::xml_tree_walker {
public:
    explicit CustomTreeWalker(bool skip_children) : m_skip_children(skip_children) {}

    bool begin(pugi::xml_node& node) override {
        if (m_skip_children) {
            return false;
        }
        return true;
    }

    bool end(pugi::xml_node& node) override {
        // Perform post-order logic if needed
        return true;
    }

    // Required pure virtual method to make the class non-abstract
    bool for_each(pugi::xml_node& node) override {
        // Default behavior: do nothing specific, allow traversal
        return true;
    }

private:
    bool m_skip_children;
};

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input data to a C-string for parsing
    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for safety with C-style string functions if needed
    if (xml_str.back() != '\0') {
        xml_str += '\0';
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());

    // Test xml_node::root
    pugi::xml_node root_node = doc.root();
    
    // Test xml_node::first_child
    pugi::xml_node first_child = root_node.first_child();
    
    // Test xml_node::parent
    pugi::xml_node parent_node = root_node.parent();
    // Also test parent on a child if it exists
    if (first_child) {
        pugi::xml_node child_parent = first_child.parent();
        (void)child_parent; // Suppress unused variable warning
    }

    // Test xml_node::next_sibling
    pugi::xml_node next_sibling = root_node.next_sibling();
    if (first_child) {
        pugi::xml_node child_next = first_child.next_sibling();
        (void)child_next;
    }

    // Test xml_tree_walker::begin and xml_tree_walker::end
    // We create two walkers: one that skips children, one that doesn't
    CustomTreeWalker walker_skip(true);
    CustomTreeWalker walker_no_skip(false);

    // Traverse with walker that skips children
    if (root_node) {
        root_node.traverse(walker_skip);
    }

    // Traverse with walker that visits children
    if (root_node) {
        root_node.traverse(walker_no_skip);
    }

    // Additional exploration: traverse from a child node if available
    if (first_child) {
        first_child.traverse(walker_skip);
        first_child.traverse(walker_no_skip);
    }

    // Test root() on a child node
    if (first_child) {
        pugi::xml_node child_root = first_child.root();
        (void)child_root;
    }

    return 0;
}