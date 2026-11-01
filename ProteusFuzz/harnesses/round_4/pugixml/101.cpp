// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
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
#include <string_view>

// Custom walker to test begin and end callbacks
// Must implement all pure virtual methods: begin, end, and for_each
class TestWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test logic: allow traversal for non-empty nodes
        if (node.type() == pugi::node_element) {
            return true;
        }
        return false;
    }

    bool end(pugi::xml_node& node) override {
        // Test logic: continue traversal
        return true;
    }

    bool for_each(pugi::xml_node& node) override {
        // Test logic: process every node
        (void)node;
        return true;
    }
};

// Helper to write data to a dummy file
static bool write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return false;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

// Helper to parse XML from buffer
static pugi::xml_document parse_from_buffer(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    // Try parsing as UTF-8 first
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    if (result) return doc;

    // If parsing fails, try with a simple fallback or return empty
    return doc;
}

// Helper to parse XML from file
static pugi::xml_document parse_from_file(const char* filename) {
    pugi::xml_document doc;
    doc.load_file(filename);
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test Case 1: Parse from buffer and test node navigation
    pugi::xml_document doc1 = parse_from_buffer(Data, Size);
    if (doc1.empty()) {
        // If parsing failed, try loading from a dummy file with the data
        if (write_dummy_file(Data, Size)) {
            pugi::xml_document doc2 = parse_from_file("./dummy_file");
            if (!doc2.empty()) {
                pugi::xml_node root = doc2.root();
                if (!root.empty()) {
                    // Test parent
                    pugi::xml_node parent = root.parent();
                    (void)parent; // Use to avoid warning

                    // Test root
                    pugi::xml_node r = root.root();
                    (void)r;

                    // Test next_sibling overloads
                    pugi::xml_node next1 = root.next_sibling();
                    (void)next1;
                    
                    std::string_view a_str = "test";
                    pugi::xml_node next2 = root.next_sibling(a_str.data());
                    (void)next2;

                    // Test walker
                    TestWalker walker;
                    root.traverse(walker);
                }
            }
        }
        return 0;
    }

    pugi::xml_node root = doc1.root();
    if (root.empty()) return 0;

    // Test parent on root (should be null)
    pugi::xml_node parent = root.parent();
    (void)parent;

    // Test root on root (should be itself)
    pugi::xml_node r = root.root();
    (void)r;

    // Test next_sibling overloads
    pugi::xml_node next1 = root.next_sibling();
    (void)next1;

    std::string_view a_str = "test";
    pugi::xml_node next2 = root.next_sibling(a_str.data());
    (void)next2;

    // Test walker on root
    TestWalker walker;
    root.traverse(walker);

    // Test walker on a child if exists
    if (root.first_child()) {
        root.first_child().traverse(walker);
    }

    // Test on an invalid node (default constructed)
    pugi::xml_node invalid_node;
    pugi::xml_node invalid_parent = invalid_node.parent();
    (void)invalid_parent;
    pugi::xml_node invalid_root = invalid_node.root();
    (void)invalid_root;
    pugi::xml_node invalid_next1 = invalid_node.next_sibling();
    (void)invalid_next1;
    pugi::xml_node invalid_next2 = invalid_node.next_sibling(a_str.data());
    (void)invalid_next2;

    // Test walker on invalid node
    invalid_node.traverse(walker);

    return 0;
}