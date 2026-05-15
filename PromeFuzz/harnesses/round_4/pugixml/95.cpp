// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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

// Custom walker class for testing traverse
class FuzzWalker : public pugi::xml_tree_walker {
public:
    FuzzWalker() : pugi::xml_tree_walker() {}
    
    virtual bool begin(pugi::xml_node& node) override {
        // Test the begin callback
        (void)node;
        return true; // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Required virtual function - just return true to continue
        (void)node;
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test the end callback
        (void)node;
        return true;
    }
};

static void test_xml_node_functions(pugi::xml_node node) {
    // Test parent() function
    pugi::xml_node parent = node.parent();
    (void)parent;
    
    // Test next_sibling() function
    pugi::xml_node next_sibling = node.next_sibling();
    (void)next_sibling;
    
    // Test next_sibling() with name parameter
    std::string_view name_sv;
    if (node.name()) {
        name_sv = std::string_view(node.name());
        pugi::xml_node named_sibling = node.next_sibling(name_sv.data());
        (void)named_sibling;
    }
    
    // Test root() function
    pugi::xml_node root = node.root();
    (void)root;
    
    // Test traverse() function if node is valid
    if (node) {
        FuzzWalker walker;
        node.traverse(walker);
    }
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    if (result) {
        // If parsing succeeded, test functions on various nodes
        
        // Test on document node
        test_xml_node_functions(doc);
        
        // Test on root node
        pugi::xml_node root = doc.root();
        test_xml_node_functions(root);
        
        // Test on first child
        pugi::xml_node child = doc.first_child();
        if (child) {
            test_xml_node_functions(child);
            
            // Test on siblings
            pugi::xml_node sibling = child.next_sibling();
            while (sibling) {
                test_xml_node_functions(sibling);
                sibling = sibling.next_sibling();
            }
        }
        
        // Test on a deep node by traversing
        for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                test_xml_node_functions(child);
            }
        }
    } else {
        // If parsing failed, try to create a minimal XML structure and test functions
        pugi::xml_document fallback_doc;
        
        // Create a simple XML structure
        pugi::xml_node root = fallback_doc.append_child("root");
        pugi::xml_node child1 = root.append_child("child1");
        pugi::xml_node child2 = root.append_child("child2");
        child1.append_attribute("attr1") = "value1";
        child2.append_attribute("attr2") = "value2";
        
        // Test functions on the fallback document
        test_xml_node_functions(fallback_doc);
        test_xml_node_functions(root);
        test_xml_node_functions(child1);
        test_xml_node_functions(child2);
    }
    
    // Test with null node
    pugi::xml_node null_node;
    test_xml_node_functions(null_node);
    
    // Test walker directly
    pugi::xml_document walker_doc;
    walker_doc.append_child("test");
    FuzzWalker walker;
    pugi::xml_node walker_node = walker_doc.root();
    walker.begin(walker_node);
    walker.end(walker_node);
    
    return 0;
}