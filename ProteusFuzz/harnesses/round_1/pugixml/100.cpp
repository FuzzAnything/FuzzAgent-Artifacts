// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <string_view>

static bool traverse_with_custom_walker(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    struct custom_walker : public pugi::xml_tree_walker {
        explicit custom_walker(const uint8_t* d, size_t s) : data(d), size(s), index(0) {}
        
        bool begin(pugi::xml_node& node) override {
            // Use data to influence traversal decision
            if (index < size) {
                if (data[index] % 3 == 0) {
                    index++;
                    return false; // Skip children
                }
            }
            index++;
            return true;
        }
        
        bool end(pugi::xml_node& node) override {
            // Use data to influence traversal decision
            if (index < size) {
                if (data[index] % 5 == 0) {
                    index++;
                    return false; // Stop traversal
                }
            }
            index++;
            return true;
        }
        
        bool for_each(pugi::xml_node& node) override {
            // Default implementation: continue traversal
            // Use data to potentially influence behavior
            if (index < size) {
                index++;
            }
            return true;
        }
        
        const uint8_t* data;
        size_t size;
        size_t index;
    };
    
    custom_walker walker(data, size);
    return doc.traverse(walker);
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a document and load the fuzzer input
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, we can still test with an empty document
        // or try to create a minimal valid XML from the input
        // For now, proceed with empty document to test navigation on null/empty nodes
    }
    
    // Test root()
    pugi::xml_node root = doc.root();
    
    // Test first_child() on root
    if (!root.empty()) {
        pugi::xml_node first = root.first_child();
        // Test parent() on first child
        if (!first.empty()) {
            pugi::xml_node parent = first.parent();
            // Verify parent is root
            (void)parent;
        }
        
        // Test next_sibling()
        pugi::xml_node next = first.next_sibling();
        (void)next;
    }
    
    // Test traversal with custom walker
    traverse_with_custom_walker(doc, Data, Size);
    
    // Test navigation on potentially invalid nodes
    pugi::xml_node invalid_node;
    pugi::xml_node invalid_root = invalid_node.root();
    pugi::xml_node invalid_first = invalid_node.first_child();
    pugi::xml_node invalid_parent = invalid_node.parent();
    pugi::xml_node invalid_next = invalid_node.next_sibling();
    
    (void)invalid_root;
    (void)invalid_first;
    (void)invalid_parent;
    (void)invalid_next;
    
    return 0;
}