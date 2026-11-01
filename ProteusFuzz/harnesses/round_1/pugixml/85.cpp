// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>
#include <string_view>

// Custom walker to test begin and end callbacks
// Must implement all pure virtual methods: begin, end, and for_each
class TestWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test logic: allow traversal for nodes with names starting with 'a'
        if (node.name()) {
            return node.name()[0] == 'a';
        }
        return true;
    }

    bool end(pugi::xml_node& node) override {
        // Test logic: stop traversal if node is a comment
        return node.type() != pugi::node_comment;
    }

    // Required pure virtual method implementation
    bool for_each(pugi::xml_node& node) override {
        // Default behavior: continue traversal
        return true;
    }
};

static void test_root_and_parent(pugi::xml_node node) {
    // Test root()
    pugi::xml_node root_node = node.root();
    (void)root_node; // Suppress unused warning

    // Test parent()
    pugi::xml_node parent_node = node.parent();
    (void)parent_node; // Suppress unused warning
}

static void test_next_sibling(pugi::xml_node node) {
    // Test next_sibling() without name
    pugi::xml_node next1 = node.next_sibling();
    (void)next1;

    // Test next_sibling(name) with string_view_t
    std::string_view a_str = "test";
    pugi::xml_node next2 = node.next_sibling(a_str.data());
    (void)next2;
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment: Create a document and load the fuzzer data
    pugi::xml_document doc;
    
    // Load the fuzzed data as XML
    // If parsing fails, we still want to test the API on the empty/failed document state
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);

    // Get the root node to start traversal and testing
    pugi::xml_node root_node = doc.root();

    // Test xml_node::root
    test_root_and_parent(root_node);

    // Test xml_node::next_sibling on root
    test_next_sibling(root_node);

    // Traverse the tree using the custom walker to test begin and end
    // This exercises begin() and end() callbacks on various nodes
    if (root_node) {
        TestWalker walker;
        root_node.traverse(walker);
    }

    // Additional exploration: Iterate through children to test sibling navigation deeper
    for (pugi::xml_node child = root_node.first_child(); child; child = child.next_sibling()) {
        test_root_and_parent(child);
        test_next_sibling(child);
        
        // Recursively traverse children to ensure deeper states are covered
        TestWalker walker2;
        child.traverse(walker2);
    }

    // Cleanup is handled automatically by RAII (pugi::xml_document destructor)
    return 0;
}