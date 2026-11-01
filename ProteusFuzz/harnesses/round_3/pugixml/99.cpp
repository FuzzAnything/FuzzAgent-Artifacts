// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
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
#include <string_view>

static void safe_parse_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Try parsing as string
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    // If parsing failed, try to create a minimal valid document to test navigation
    if (!result) {
        doc.reset();
        doc.append_child("root");
    }
}

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xml_document doc;
    safe_parse_document(doc, Data, Size);
    
    // Get root node
    pugi::xml_node root_node = doc.root();
    
    // Test xml_node::root()
    // This should return the document root if called on any node in the tree
    if (!root_node.empty()) {
        pugi::xml_node r = root_node.root();
        (void)r; // Suppress unused variable warning
    }
    
    // Test xml_node::parent()
    // Navigate to a child if exists, then check parent
    if (!root_node.empty() && root_node.first_child()) {
        pugi::xml_node child = root_node.first_child();
        pugi::xml_node parent = child.parent();
        (void)parent;
        
        // Test parent on root (should be empty)
        pugi::xml_node root_parent = root_node.parent();
        (void)root_parent;
    }
    
    // Test xml_node::next_sibling() overloads
    if (!root_node.empty() && root_node.first_child()) {
        pugi::xml_node first_child = root_node.first_child();
        
        // Test next_sibling() without argument
        pugi::xml_node next_sibling_no_arg = first_child.next_sibling();
        (void)next_sibling_no_arg;
        
        // Test next_sibling(name) with string_view
        std::string_view test_name = "test";
        pugi::xml_node next_sibling_with_name = first_child.next_sibling(test_name);
        (void)next_sibling_with_name;
        
        // Also try with empty string
        std::string_view empty_name = "";
        pugi::xml_node next_sibling_empty = first_child.next_sibling(empty_name);
        (void)next_sibling_empty;
    }
    
    // Test xml_tree_walker::begin and xml_tree_walker::end
    // Create a custom walker to test these virtual methods
    // Must implement all pure virtual methods: begin, end, and for_each
    struct TestWalker : public pugi::xml_tree_walker {
        bool begin(pugi::xml_node& node) override {
            // Always continue traversal
            (void)node;
            return true;
        }
        
        bool end(pugi::xml_node& node) override {
            // Always continue traversal
            (void)node;
            return true;
        }

        bool for_each(pugi::xml_node& node) override {
            // Always continue traversal
            (void)node;
            return true;
        }
    };
    
    TestWalker walker;
    if (!root_node.empty()) {
        root_node.traverse(walker);
    }
    
    // Create an empty node to test edge cases
    pugi::xml_node empty_node;
    
    // Test root() on empty node
    pugi::xml_node empty_root = empty_node.root();
    (void)empty_root;
    
    // Test parent() on empty node
    pugi::xml_node empty_parent = empty_node.parent();
    (void)empty_parent;
    
    // Test next_sibling() on empty node
    pugi::xml_node empty_next = empty_node.next_sibling();
    (void)empty_next;
    
    std::string_view sv = "test";
    pugi::xml_node empty_next_named = empty_node.next_sibling(sv);
    (void)empty_next_named;
    
    return 0;
}