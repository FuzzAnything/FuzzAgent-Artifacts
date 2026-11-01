// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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
#include <string>
#include <string_view>

static class CustomWalker : public pugi::xml_tree_walker {
public:
    CustomWalker() : depth_limit(0) {}
    void set_depth_limit(int limit) { depth_limit = limit; }

    bool begin(pugi::xml_node& node) override {
        // Calculate depth manually since xml_node::depth() might not exist in all versions
        int current_depth = 0;
        pugi::xml_node temp = node;
        while (temp.parent()) {
            current_depth++;
            temp = temp.parent();
        }
        
        if (depth_limit >= 0 && current_depth > static_cast<unsigned int>(depth_limit)) {
            return false;
        }
        return true;
    }

    bool end(pugi::xml_node& node) override {
        (void)node;
        return true;
    }

    bool for_each(pugi::xml_node& node) override {
        (void)node;
        return true;
    }

private:
    int depth_limit;
};

static class DefaultWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        (void)node;
        return true;
    }

    bool end(pugi::xml_node& node) override {
        (void)node;
        return true;
    }

    bool for_each(pugi::xml_node& node) override {
        (void)node;
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    if (!result) {
        // Parsing failed, but we can still test with an empty document
        // or try to parse a minimal valid XML if the input is completely invalid
        if (Size < 10) {
            // Fallback for very small inputs
            return 0;
        }
    }

    // Test pugi::xml_node::root
    pugi::xml_node root_node = doc.root();
    if (root_node) {
        // Verify root is consistent
        if (root_node.root() != root_node) {
            // This should not happen for a valid root, but good to check
        }
    }

    // Test pugi::xml_node::parent
    pugi::xml_node current_node = root_node;
    while (current_node) {
        pugi::xml_node parent = current_node.parent();
        if (parent) {
            // Traverse up one level
            if (parent.parent() != current_node.parent().parent()) {
                // Consistency check
            }
        }
        // Move to next sibling to explore more nodes
        current_node = current_node.next_sibling();
    }

    // Test pugi::xml_node::next_sibling (both overloads)
    current_node = root_node;
    while (current_node) {
        pugi::xml_node next1 = current_node.next_sibling();
        
        // Try with a name (using a string_view derived from input data if possible)
        std::string_view name_view(reinterpret_cast<const char*>(Data), Size > 10 ? 10 : Size);
        pugi::xml_node next2 = current_node.next_sibling(name_view);
        
        // Also try with a fixed name to ensure coverage
        pugi::xml_node next3 = current_node.next_sibling("test");
        
        current_node = next1;
    }

    // Test pugi::xml_tree_walker::begin and pugi::xml_tree_walker::end
    CustomWalker walker;
    // Use a portion of the input to set depth limit
    int limit = 0;
    if (Size > 0) {
        limit = static_cast<int>(Data[0]) % 10;
    }
    walker.set_depth_limit(limit);
    
    doc.traverse(walker);

    // Test with a default walker as well
    DefaultWalker default_walker;
    doc.traverse(default_walker);

    return 0;
}