// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>
#include <string_view>
#include <cstring>

// Custom walker to test begin and end callbacks
class FuzzWalker : public pugi::xml_tree_walker {
public:
    FuzzWalker() : pugi::xml_tree_walker() {}

    bool begin(pugi::xml_node& node) override {
        // Default behavior: continue traversal
        (void)node;
        return true;
    }

    bool end(pugi::xml_node& node) override {
        // Default behavior: continue traversal
        (void)node;
        return true;
    }

    // Required pure virtual method from xml_tree_walker
    bool for_each(pugi::xml_node& node) override {
        // Default behavior: continue traversal
        (void)node;
        return true;
    }
};

static void test_next_sibling(pugi::xml_node& node) {
    // Test next_sibling() - no arguments
    pugi::xml_node next = node.next_sibling();
    (void)next;

    // Test next_sibling(name) - with name
    // Using a dummy name for fuzzing
    pugi::xml_node next_named = node.next_sibling("test");
    (void)next_named;
}

static void test_root(pugi::xml_node& node) {
    // Test root()
    pugi::xml_node root = node.root();
    (void)root;
}

static void test_parent(pugi::xml_node& node) {
    // Test parent()
    pugi::xml_node parent = node.parent();
    (void)parent;
}

static void test_walker(pugi::xml_node& node) {
    FuzzWalker walker;
    // Trigger begin and end callbacks via traverse
    node.traverse(walker);
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a document and parse the input data
    pugi::xml_document doc;
    
    // Convert input to char array for parsing
    std::string xml_data(reinterpret_cast<const char*>(Data), Size);
    
    // Parse the XML document
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());
    
    // Even if parsing fails, we can still test some functions on empty nodes
    // But for meaningful tests, we need at least a root
    if (!result) {
        // If parsing failed, create a minimal valid document to test on
        doc.append_child("root");
    }

    // Get the root node
    pugi::xml_node root_node = doc.root();
    
    // Test root() on root node
    test_root(root_node);
    
    // Test parent() on root node (should return null)
    test_parent(root_node);
    
    // Test next_sibling() on root node (should return null)
    test_next_sibling(root_node);
    
    // Test walker on root node
    test_walker(root_node);
    
    // Iterate through children to test more states
    for (pugi::xml_node child = root_node.first_child(); child; child = child.next_sibling()) {
        test_root(child);
        test_parent(child);
        test_next_sibling(child);
        test_walker(child);
        
        // Test next_sibling with name on child
        pugi::xml_node next_named = child.next_sibling("test");
        (void)next_named;
        
        // Test next_sibling with string_view_t parameter simulation
        // Since the API uses char_t*, we can't directly use string_view_t
        // But we can test with different string literals
        pugi::xml_node next_named2 = child.next_sibling("another");
        (void)next_named2;
    }
    
    // Test with an empty node
    pugi::xml_node empty_node;
    test_root(empty_node);
    test_parent(empty_node);
    test_next_sibling(empty_node);
    test_walker(empty_node);
    
    return 0;
}