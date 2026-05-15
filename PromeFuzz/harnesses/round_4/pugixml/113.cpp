// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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
#include <cstdint>
#include <cstring>
#include <fstream>

#include "src/pugixml.hpp"

static class FuzzWalker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Store the root node for later comparison
        m_beginRoot = node.root();
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Simple traversal that counts nodes
        m_nodeCount++;
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Verify the end node matches the begin node's root
        pugi::xml_node endRoot = node.root();
        return true;
    }
    
    size_t getNodeCount() const { return m_nodeCount; }
    void reset() { m_nodeCount = 0; }
    
private:
    size_t m_nodeCount = 0;
    pugi::xml_node m_beginRoot;
};

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for XML parsing
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Test parent() and root() functions on various nodes
    pugi::xml_node root = doc.root();
    if (root) {
        // Test on root node
        pugi::xml_node rootParent = root.parent();
        pugi::xml_node rootRoot = root.root();
        
        // Test on document element
        pugi::xml_node documentElement = doc.document_element();
        if (documentElement) {
            pugi::xml_node deParent = documentElement.parent();
            pugi::xml_node deRoot = documentElement.root();
            
            // Test on child nodes
            for (pugi::xml_node child = documentElement.first_child(); child; child = child.next_sibling()) {
                pugi::xml_node childParent = child.parent();
                pugi::xml_node childRoot = child.root();
                
                // Test append_copy
                if (documentElement && child) {
                    pugi::xml_node copy = documentElement.append_copy(child);
                    if (copy) {
                        pugi::xml_node copyParent = copy.parent();
                        pugi::xml_node copyRoot = copy.root();
                    }
                }
            }
        }
        
        // Test traverse with custom walker
        FuzzWalker walker;
        if (root) {
            bool traverseResult = root.traverse(walker);
            walker.reset();
        }
        
        // Test begin() and end() directly
        if (root) {
            FuzzWalker directWalker;
            bool beginResult = directWalker.begin(root);
            bool endResult = directWalker.end(root);
        }
    }
    
    // Test with empty/null nodes
    pugi::xml_node emptyNode;
    pugi::xml_node emptyParent = emptyNode.parent();
    pugi::xml_node emptyRoot = emptyNode.root();
    
    // Test with constructed node from internal pointer (edge case)
    pugi::xml_node constructedNode(nullptr);
    pugi::xml_node constructedParent = constructedNode.parent();
    pugi::xml_node constructedRoot = constructedNode.root();
    
    // Test append_copy with empty node
    if (root) {
        pugi::xml_node emptyCopy = root.append_copy(emptyNode);
    }
    
    // Test traverse on empty node
    FuzzWalker emptyWalker;
    bool emptyTraverse = emptyNode.traverse(emptyWalker);
    
    return 0;
}