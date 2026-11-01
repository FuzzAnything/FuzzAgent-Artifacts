// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <string_view>
#include <cstddef>

// Custom walker to test begin and end callbacks
// Must implement all pure virtual methods: begin, end, for_each
static class TestWalker : public pugi::xml_tree_walker {
public:
    TestWalker() : depth_limit(0) {}
    void set_depth_limit(int limit) { depth_limit = limit; }

    bool begin(pugi::xml_node& node) override {
        // Test logic: allow traversal based on depth limit or random choice
        if (depth_limit > 0 && current_depth >= depth_limit) {
            return false;
        }
        current_depth++;
        return true;
    }

    bool end(pugi::xml_node& node) override {
        current_depth--;
        // Always continue unless explicitly stopped
        return true;
    }

    bool for_each(pugi::xml_node& node) override {
        // Default behavior: continue traversal
        return true;
    }

private:
    int depth_limit;
    int current_depth = 0;
};

extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Convert input data to a C-string for parsing
    // Ensure null termination
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    if (input_str.back() != '\0') {
        input_str += '\0';
    }

    // Parse the document
    pugi::xml_parse_result result = doc.load_string(input_str.c_str());
    
    // If parsing failed, we can still test with an empty document or partial structure
    // but the main logic relies on having a root to test navigation
    
    // Get the root node
    pugi::xml_node root = doc.root();
    
    // Test 1: root() on the root node itself (should return itself)
    pugi::xml_node root_of_root = root.root();
    (void)root_of_root;
    
    // Test 2: root() on a child if exists
    if (root.first_child()) {
        pugi::xml_node child = root.first_child();
        pugi::xml_node root_of_child = child.root();
        (void)root_of_child;
    }
    
    // Test 3: parent() on root (should be null)
    pugi::xml_node parent_of_root = root.parent();
    (void)parent_of_root;
    
    // Test 4: parent() on a child
    if (root.first_child()) {
        pugi::xml_node child = root.first_child();
        pugi::xml_node parent_of_child = child.parent();
        (void)parent_of_child;
    }
    
    // Test 5: next_sibling() without name
    if (root.first_child()) {
        pugi::xml_node child = root.first_child();
        pugi::xml_node next_sib = child.next_sibling();
        (void)next_sib;
    }
    
    // Test 6: next_sibling() with name
    // We use a string_view_t (char_t*) derived from input or a static string
    // Since char_t might be wchar_t, we need to be careful. 
    // For simplicity in fuzzing, we assume narrow strings or use a static narrow string.
    // If PUGIXML_WCHAR_MODE is on, this might need adjustment, but standard fuzzer usually runs in narrow mode.
    const char* test_name = "test";
    if (root.first_child()) {
        pugi::xml_node child = root.first_child();
        pugi::xml_node next_sib_named = child.next_sibling(test_name);
        (void)next_sib_named;
    }
    
    // Test 7: xml_tree_walker begin and end
    TestWalker walker;
    // Use a random value from input to set depth limit to explore different states
    int limit = (Size > 0) ? (Data[0] % 10) : 0;
    walker.set_depth_limit(limit);
    
    // Traverse the document
    if (!root.empty()) {
        root.traverse(walker);
    } else {
        // Even if root is empty, traverse might behave differently or safely
        doc.traverse(walker);
    }
    
    // Test 8: root() on an invalid node
    pugi::xml_node invalid_node;
    pugi::xml_node root_of_invalid = invalid_node.root();
    (void)root_of_invalid;
    
    // Test 9: parent() on an invalid node
    pugi::xml_node parent_of_invalid = invalid_node.parent();
    (void)parent_of_invalid;
    
    // Test 10: next_sibling on invalid node
    pugi::xml_node next_of_invalid = invalid_node.next_sibling();
    pugi::xml_node next_of_invalid_named = invalid_node.next_sibling(test_name);
    (void)next_of_invalid;
    (void)next_of_invalid_named;

    return 0;
}