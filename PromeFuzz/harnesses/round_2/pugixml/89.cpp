// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include "pugixml.hpp"

static class TestWalker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Test xml_node::root() during begin
        (void)node.root();
        // Test xml_node::parent() during begin  
        (void)node.parent();
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Test xml_node::next_sibling() during traversal
        (void)node.next_sibling();
        // Test xml_node::root() during traversal
        (void)node.root();
        // Test xml_node::parent() during traversal
        (void)node.parent();
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test xml_node::root() during end
        (void)node.root();
        // Test xml_node::parent() during end
        (void)node.parent();
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Test on parsed document if successful
    if (result) {
        pugi::xml_node root = doc.root();
        
        // Test xml_node::root()
        (void)root.root();
        
        // Test xml_node::parent() on root
        (void)root.parent();
        
        // Test xml_node::next_sibling() on root
        (void)root.next_sibling();
        
        // Create walker and test traverse
        TestWalker walker;
        root.traverse(walker);
        
        // Test on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            (void)child.root();
            (void)child.parent();
            (void)child.next_sibling();
            
            // Test traverse on child nodes
            child.traverse(walker);
        }
    }
    
    // Test on empty nodes
    pugi::xml_node empty_node;
    (void)empty_node.root();
    (void)empty_node.parent();
    (void)empty_node.next_sibling();
    
    // Test on default constructed document
    pugi::xml_document empty_doc;
    pugi::xml_node empty_root = empty_doc.root();
    (void)empty_root.root();
    (void)empty_root.parent();
    (void)empty_root.next_sibling();
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}