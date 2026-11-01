// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <vector>
#include <cstdlib>

// Custom tree walker to test begin, end, and for_each callbacks
class FuzzTreeWalker : public pugi::xml_tree_walker {
public:
    FuzzTreeWalker() : depth_limit(0) {}
    void set_depth_limit(int limit) { depth_limit = limit; }

    // Override begin
    bool begin(pugi::xml_node& node) override {
        // Fuzz logic: sometimes skip traversal based on node name or depth
        if (depth_limit > 0 && depth_limit < 100) {
            // Deterministic pseudo-random based on node name length
            if (node.name() && strlen(node.name()) % depth_limit == 0) {
                return false;
            }
        }
        return true;
    }

    // Override end
    bool end(pugi::xml_node& node) override {
        // Fuzz logic: sometimes stop traversal after processing
        if (node.name() && strlen(node.name()) > 0) {
            if (strlen(node.name()) % 7 == 0) {
                return false;
            }
        }
        return true;
    }

    // Override for_each (required pure virtual method)
    bool for_each(pugi::xml_node& node) override {
        // Fuzz logic: sometimes skip processing
        if (node.name() && strlen(node.name()) > 0) {
            if (strlen(node.name()) % 13 == 0) {
                return false;
            }
        }
        return true;
    }

private:
    int depth_limit;
};

// Helper to parse XML from buffer
static pugi::xml_document parse_xml_from_buffer(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    
    // Ensure null termination for safety, though pugixml handles binary data
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';
    
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    (void)result;
    return doc;
}

// Helper to get a random node from the document for testing
static pugi::xml_node get_random_node(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (doc.empty()) {
        return pugi::xml_node();
    }
    
    // Try to get root first
    pugi::xml_node root = doc.root();
    if (!root) {
        return pugi::xml_node();
    }
    
    // If data is available, use it to select a node
    if (size > 0) {
        size_t index = data[0] % 100;
        pugi::xml_node current = root;
        pugi::xml_node next = current.first_child();
        
        // Traverse to find a node at pseudo-random index
        int count = 0;
        while (next && count < index) {
            current = next;
            next = current.next_sibling();
            count++;
        }
        return current;
    }
    
    return root;
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) {
        return 0;
    }

    // Parse XML document from input data
    pugi::xml_document doc = parse_xml_from_buffer(Data, Size);
    
    // Test 1: root() on document
    pugi::xml_node root = doc.root();
    if (root) {
        // Test root() on root node (should return itself)
        pugi::xml_node root_of_root = root.root();
        (void)root_of_root;
        
        // Test next_sibling() on root (should be empty)
        pugi::xml_node next_sib = root.next_sibling();
        (void)next_sib;
        
        // Test next_sibling(name) on root
        std::string_view a_str = "nonexistent";
        pugi::xml_node next_sib_named = root.next_sibling(a_str.data());
        (void)next_sib_named;
        
        // Test parent() on root (should be empty)
        pugi::xml_node parent_of_root = root.parent();
        (void)parent_of_root;
    }
    
    // Test 2: Get a random node and test all functions
    pugi::xml_node node = get_random_node(doc, Data, Size);
    
    if (node) {
        // Test root()
        pugi::xml_node node_root = node.root();
        (void)node_root;
        
        // Test next_sibling()
        pugi::xml_node next_sibling = node.next_sibling();
        (void)next_sibling;
        
        // Test next_sibling(name)
        std::string_view a_str = "test";
        pugi::xml_node next_sibling_named = node.next_sibling(a_str.data());
        (void)next_sibling_named;
        
        // Test parent()
        pugi::xml_node parent = node.parent();
        (void)parent;
        
        // Test tree walker on this node
        FuzzTreeWalker walker;
        walker.set_depth_limit(Data[0] % 20);
        node.traverse(walker);
    }
    
    // Test 3: Test on empty node
    pugi::xml_node empty_node;
    pugi::xml_node empty_root = empty_node.root();
    (void)empty_root;
    
    pugi::xml_node empty_next = empty_node.next_sibling();
    (void)empty_next;
    
    std::string_view a_str = "test";
    pugi::xml_node empty_next_named = empty_node.next_sibling(a_str.data());
    (void)empty_next_named;
    
    pugi::xml_node empty_parent = empty_node.parent();
    (void)empty_parent;
    
    // Test 4: Test tree walker on document
    FuzzTreeWalker walker2;
    // Safe access: check size before accessing Data[1]
    int limit2 = (Size > 1) ? (Data[1] % 20) : 0;
    walker2.set_depth_limit(limit2);
    doc.traverse(walker2);
    
    // Test 5: Test tree walker on root
    if (root) {
        FuzzTreeWalker walker3;
        // Safe access: check size before accessing Data[2]
        int limit3 = (Size > 2) ? (Data[2] % 20) : 0;
        walker3.set_depth_limit(limit3);
        root.traverse(walker3);
    }
    
    return 0;
}