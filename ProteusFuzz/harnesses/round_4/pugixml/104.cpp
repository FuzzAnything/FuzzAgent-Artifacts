// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <pugixml.hpp>

// Custom walker to test begin and end methods
static class CustomWalker : public pugi::xml_tree_walker {
public:
    CustomWalker() : depth_limit(0), stop_on_first(false) {}

    bool begin(pugi::xml_node& node) override {
        if (depth_limit > 0 && current_depth >= depth_limit) {
            return false;
        }
        current_depth++;
        return !stop_on_first;
    }

    bool end(pugi::xml_node& node) override {
        current_depth--;
        if (stop_on_first && current_depth == 0) {
            return false;
        }
        return true;
    }

    // Required by pugixml xml_tree_walker interface
    bool for_each(pugi::xml_node& node) override {
        // Default behavior: do nothing specific, just return true to continue
        (void)node;
        return true;
    }

    int depth_limit;
    bool stop_on_first;
    int current_depth = 0;
};

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);

    // If parsing fails or root is empty, we still want to test API on empty nodes
    pugi::xml_node root = doc.root();

    // Helper to test API on a node (valid or invalid)
    auto test_node_api = [](pugi::xml_node& node) {
        // Test parent()
        pugi::xml_node parent_node = node.parent();
        (void)parent_node;

        // Test root()
        pugi::xml_node root_node = node.root();
        (void)root_node;

        // Test next_sibling()
        pugi::xml_node next_sib = node.next_sibling();
        (void)next_sib;

        // Test next_sibling(name)
        // Use a string_view_t compatible type. In non-wide mode, char_t is char.
        // We construct a string_view directly.
        std::string_view sv = "target";
        pugi::xml_node next_sib_named = node.next_sibling(sv);
        (void)next_sib_named;
    };

    // Test on the root node (even if it might be empty/null)
    test_node_api(root);

    // If parsing failed and root is null, create a minimal valid tree to test walker logic
    if (!result || root.empty()) {
        // Create a simple tree manually to ensure we have something to walk if input was garbage
        doc.append_child("root");
        root = doc.root();
    }

    // Test xml_tree_walker::begin and xml_tree_walker::end via traverse
    CustomWalker walker;
    
    // Vary walker behavior
    walker.depth_limit = (Size % 10);
    walker.stop_on_first = (Size % 2 == 0);

    root.traverse(walker);

    // Additional traversal with different settings
    CustomWalker walker2;
    walker2.depth_limit = 0; 
    walker2.stop_on_first = false;
    root.traverse(walker2);

    return 0;
}