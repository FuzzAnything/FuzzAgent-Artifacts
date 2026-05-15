// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>

// Custom walker class to test begin() and end() callbacks
class FuzzWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Return true to proceed with traversal
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Required for traversal to work, but not in target list
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Return true to indicate successful cleanup
        return true;
    }
};

static void test_parent_and_root(const pugi::xml_node& node) {
    if (node) {
        // Test parent() function
        pugi::xml_node parent = node.parent();
        
        // Test root() function
        pugi::xml_node root = node.root();
    }
}

static void test_append_copy(pugi::xml_node& target, const pugi::xml_node& proto) {
    if (target && proto) {
        // Test append_copy() function
        pugi::xml_node copied = target.append_copy(proto);
    }
}

static void test_traverse(pugi::xml_node& node) {
    if (node) {
        FuzzWalker walker;
        // Test traverse() function
        node.traverse(walker);
    }
}

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    // Create a dummy file for parsing
    const char* filename = "./dummy_file";
    FILE* f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }
    
    // Write fuzzer data to file
    fwrite(Data, 1, Size, f);
    fclose(f);
    
    // Parse XML from file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    
    if (result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Test parent() and root() on various nodes
        test_parent_and_root(root);
        
        // Test on child nodes if they exist
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            test_parent_and_root(child);
            
            // Test append_copy() with different combinations
            test_append_copy(root, child);
            test_append_copy(child, root);
            
            // Test traverse() on child nodes
            test_traverse(child);
        }
        
        // Test traverse() on root node
        test_traverse(root);
        
        // Test parent() and root() on empty node
        pugi::xml_node empty_node;
        test_parent_and_root(empty_node);
        
        // Test append_copy() with empty nodes
        test_append_copy(empty_node, root);
        test_append_copy(root, empty_node);
        
        // Test traverse() on empty node
        test_traverse(empty_node);
    }
    
    // Clean up
    remove(filename);
    
    return 0;
}