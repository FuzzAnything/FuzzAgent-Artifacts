// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
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
#include <string_view>
#include <cstring>
#include <vector>

static bool g_fuzzed = false;

class FuzzTreeWalker : public pugi::xml_tree_walker {
public:
    FuzzTreeWalker() : depth_limit(0), skip_next(false), current_depth(0) {}
    
    void set_depth_limit(int limit) { depth_limit = limit; }
    void set_skip_next(bool skip) { skip_next = skip; }

    virtual bool begin(pugi::xml_node& node) override {
        // Test begin logic
        if (depth_limit > 0 && current_depth >= depth_limit) {
            return false;
        }
        if (skip_next) {
            skip_next = false;
            return false;
        }
        current_depth++;
        return true;
    }

    virtual bool end(pugi::xml_node& node) override {
        // Test end logic
        (void)node;
        if (current_depth > 0) {
            current_depth--;
        }
        return true;
    }

    virtual bool for_each(pugi::xml_node& node) override {
        // Required pure virtual method
        (void)node;
        return true;
    }

private:
    int depth_limit;
    bool skip_next;
    int current_depth;
};

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input to string, ensuring null termination for parsing
    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    // Even if parsing fails, we can test some API behaviors on empty/invalid nodes
    // But to test traversal and navigation effectively, we need a valid tree.
    // We will proceed regardless to test error handling paths.
    
    // 1. Test pugi::xml_node::root
    pugi::xml_node root_node = doc.root();
    if (!root_node) {
        // If root is null, test navigation on null node
        pugi::xml_node null_parent = root_node.parent();
        (void)null_parent;
        pugi::xml_node null_next = root_node.next_sibling();
        (void)null_next;
        pugi::xml_node null_next_named = root_node.next_sibling("test");
        (void)null_next_named;
    } else {
        // Test on valid root
        pugi::xml_node parent = root_node.parent();
        (void)parent; // Should be null for root
        
        pugi::xml_node root_of_root = root_node.root();
        (void)root_of_root; // Should be same as root_node
        
        pugi::xml_node next_sib = root_node.next_sibling();
        (void)next_sib;
        
        std::string_view name_view = "nonexistent";
        pugi::xml_node next_sib_named = root_node.next_sibling(name_view.data());
        (void)next_sib_named;
    }

    // 2. Test pugi::xml_tree_walker::begin and end
    FuzzTreeWalker walker;
    
    // Try different configurations with bounds checking
    if (Size >= 1) {
        int limit = Data[0] % 10;
        walker.set_depth_limit(limit);
    }
    if (Size >= 2) {
        walker.set_skip_next((Data[1] % 2) == 1);
    }

    // Traverse the document
    doc.traverse(walker);

    // Additional random node traversal if document is valid
    if (result && doc.root()) {
        pugi::xml_node current = doc.root();
        // Traverse siblings randomly
        for (int i = 0; i < 5; ++i) {
            pugi::xml_node next = current.next_sibling();
            if (next) {
                current = next;
            } else {
                break;
            }
        }
        
        // Traverse by name
        if (Size > 2) {
            size_t name_len = std::min(Size - 2, static_cast<size_t>(10));
            std::string name_str(reinterpret_cast<const char*>(Data + 2), name_len);
            // Ensure null termination for safety
            name_str += '\0';
            
            pugi::xml_node found = current.next_sibling(name_str.c_str());
            (void)found;
        }
        
        // Test parent navigation
        pugi::xml_node p = current.parent();
        (void)p;
        
        // Test root from a child
        pugi::xml_node r = current.root();
        (void)r;
    }

    return 0;
}