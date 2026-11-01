// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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

// Custom walker to test begin and end callbacks
class FuzzWalker : public pugi::xml_tree_walker {
public:
    FuzzWalker() : depth(0) {}

    bool begin(pugi::xml_node& node) override {
        // Test basic traversal logic
        if (!node) return true;
        
        // Randomly decide to skip children based on node properties
        if (node.type() == pugi::node_element && node.name() && strlen(node.name()) % 2 == 0) {
            return false;
        }
        
        depth++;
        return true;
    }

    bool end(pugi::xml_node& node) override {
        if (!node) return true;
        
        depth--;
        // Randomly stop traversal based on depth
        if (depth < 0) return false;
        
        return true;
    }

    // Required pure virtual method for xml_tree_walker
    bool for_each(pugi::xml_node& node) override {
        if (!node) return true;
        // Perform some action on the node
        (void)node.name();
        (void)node.type();
        return true;
    }

private:
    int depth;
};

static void test_navigation(pugi::xml_node& node) {
    if (!node) return;

    // Test parent()
    pugi::xml_node parent = node.parent();
    if (parent) {
        // Access parent to ensure no crash
        parent.name();
        parent.type();
    }

    // Test root()
    pugi::xml_node root = node.root();
    if (root) {
        root.name();
        root.type();
    }

    // Test next_sibling() - no argument
    pugi::xml_node next = node.next_sibling();
    if (next) {
        next.name();
        next.type();
    }

    // Test next_sibling(name) - with argument
    std::string_view name_view = "test";
    pugi::xml_node next_named = node.next_sibling(name_view);
    if (next_named) {
        next_named.name();
        next_named.type();
    }
    
    // Try with empty string
    std::string_view empty_view = "";
    pugi::xml_node next_empty = node.next_sibling(empty_view);
    if (next_empty) {
        next_empty.name();
    }
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a string from the fuzz input
    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    
    // Ensure we have valid XML by adding a root if needed
    // This is a simple heuristic to increase chances of valid parsing
    std::string modified_xml;
    if (xml_str.empty() || xml_str.find('<') == std::string::npos) {
        modified_xml = "<root>" + xml_str + "</root>";
    } else {
        modified_xml = xml_str;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(modified_xml.c_str());
    
    // Even if parsing fails, we can still test some behaviors
    // but we need at least a document to work with
    
    FuzzWalker walker;
    
    // Test tree walker with begin and end
    doc.traverse(walker);
    
    // Test navigation functions on root
    if (doc.root()) {
        pugi::xml_node root_node = doc.root();
        test_navigation(root_node);
        
        // Test on children
        for (pugi::xml_node child = doc.root().first_child(); 
             child; 
             child = child.next_sibling()) {
            test_navigation(child);
        }
    }
    
    // Test with empty node
    pugi::xml_node empty_node;
    test_navigation(empty_node);
    
    // Test parent on root (should be null)
    if (doc.root()) {
        pugi::xml_node parent = doc.root().parent();
        // parent should be null for root
        (void)parent;
    }
    
    // Test root on various nodes
    if (doc.root()) {
        pugi::xml_node root = doc.root().root();
        (void)root;
    }
    
    return 0;
}