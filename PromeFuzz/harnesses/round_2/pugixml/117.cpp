// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
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

static class FuzzWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test parent() on the root node
        if (node.parent()) {
            // Root node shouldn't have a parent
            volatile auto dummy = node.parent();
            (void)dummy;
        }
        
        // Test root() on the root node
        auto root = node.root();
        if (root != node) {
            // Should be the same node for root
            volatile auto dummy = root;
            (void)dummy;
        }
        
        // Return true to continue traversal
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Test parent() on each node
        auto parent = node.parent();
        if (parent) {
            volatile auto dummy = parent;
            (void)dummy;
        }
        
        // Test root() on each node
        auto root = node.root();
        if (root) {
            volatile auto dummy = root;
            (void)dummy;
        }
        
        // Test next_sibling() on each node
        auto next = node.next_sibling();
        if (next) {
            volatile auto dummy = next;
            (void)dummy;
        }
        
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Test parent() and root() one more time at the end
        auto parent = node.parent();
        auto root = node.root();
        
        volatile auto dummy1 = parent;
        volatile auto dummy2 = root;
        (void)dummy1;
        (void)dummy2;
        
        return true;
    }
} fuzz_walker;

extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    // First, try to parse the input as XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_ws_pcdata);
    
    if (!result) {
        // If parsing fails, create a minimal valid XML document for testing
        const char* minimal_xml = "<root><child1/><child2/><child3/></root>";
        result = doc.load_string(minimal_xml);
        
        if (!result) {
            // Should never happen with our minimal XML
            return 0;
        }
    }
    
    // Get the root node
    pugi::xml_node root = doc.root();
    
    if (root) {
        // Test traverse() with our custom walker
        root.traverse(fuzz_walker);
        
        // Test parent() on root
        auto parent = root.parent();
        if (parent) {
            volatile auto dummy = parent;
            (void)dummy;
        }
        
        // Test root() on root
        auto root_check = root.root();
        if (root_check) {
            volatile auto dummy = root_check;
            (void)dummy;
        }
        
        // Test next_sibling() on root's children
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            auto next = child.next_sibling();
            if (next) {
                volatile auto dummy = next;
                (void)dummy;
                
                // Test parent() on sibling
                auto sibling_parent = next.parent();
                if (sibling_parent) {
                    volatile auto dummy2 = sibling_parent;
                    (void)dummy2;
                }
                
                // Test root() on sibling
                auto sibling_root = next.root();
                if (sibling_root) {
                    volatile auto dummy3 = sibling_root;
                    (void)dummy3;
                }
            }
        }
        
        // Also test next_sibling() with string_view parameter
        // Create a string_view from the data if possible
        if (Size > 0) {
            // Use first few bytes to create a name for testing
            size_t name_len = Size > 100 ? 100 : Size;
            std::string_view name(reinterpret_cast<const char*>(Data), name_len);
            
            // Try to find next sibling with this name (unlikely to match, but tests the API)
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                auto next_named = child.next_sibling(name.data());
                if (next_named) {
                    volatile auto dummy = next_named;
                    (void)dummy;
                }
            }
        }
    }
    
    // Test with an empty node
    pugi::xml_node empty_node;
    auto empty_parent = empty_node.parent();
    auto empty_root = empty_node.root();
    auto empty_next = empty_node.next_sibling();
    
    volatile auto dummy1 = empty_parent;
    volatile auto dummy2 = empty_root;
    volatile auto dummy3 = empty_next;
    (void)dummy1;
    (void)dummy2;
    (void)dummy3;
    
    return 0;
}