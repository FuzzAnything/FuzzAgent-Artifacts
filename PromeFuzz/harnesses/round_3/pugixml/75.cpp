// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <string_view>

// Custom walker to test traversal callbacks
class TestWalker : public pugi::xml_tree_walker {
public:
    TestWalker() : pugi::xml_tree_walker() {}
    
    virtual bool begin(pugi::xml_node& node) override {
        // Test begin callback
        (void)node;
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Required for traversal, do minimal work
        (void)node;
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test end callback
        (void)node;
        return true;
    }
};

static void test_xml_node_functions(pugi::xml_node node) {
    // Test root() function
    pugi::xml_node root_node = node.root();
    (void)root_node;
    
    // Test parent() function
    pugi::xml_node parent_node = node.parent();
    (void)parent_node;
    
    // Test next_sibling() function
    pugi::xml_node next_sibling = node.next_sibling();
    (void)next_sibling;
    
    // Test traverse() function
    TestWalker walker;
    node.traverse(walker);
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, 
        pugi::parse_default | pugi::parse_trim_pcdata);
    
    // Test functions even if parsing failed (will test on empty/null nodes)
    
    // Test on document root
    pugi::xml_node root = doc.root();
    test_xml_node_functions(root);
    
    // Test on document itself (as a node)
    pugi::xml_node doc_as_node = doc;
    test_xml_node_functions(doc_as_node);
    
    // Test on child nodes if they exist
    for (pugi::xml_node child = root.first_child(); child; 
         child = child.next_sibling()) {
        test_xml_node_functions(child);
        
        // Test on grandchildren
        for (pugi::xml_node grandchild = child.first_child(); grandchild; 
             grandchild = grandchild.next_sibling()) {
            test_xml_node_functions(grandchild);
        }
    }
    
    // Test on empty node
    pugi::xml_node empty_node;
    test_xml_node_functions(empty_node);
    
    // Test standalone walker creation
    TestWalker standalone_walker;
    (void)standalone_walker;
    
    return 0;
}