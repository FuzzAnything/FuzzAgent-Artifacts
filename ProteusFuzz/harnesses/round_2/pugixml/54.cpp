// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static class CustomWalker : public pugi::xml_tree_walker {
public:
    CustomWalker() : depth_limit(0) {}
    explicit CustomWalker(int limit) : depth_limit(limit) {}

    bool begin(pugi::xml_node& node) override {
        // Default behavior: allow traversal
        return true;
    }

    bool end(pugi::xml_node& node) override {
        // Default behavior: continue traversal
        return true;
    }

    // Required pure virtual method from xml_tree_walker
    bool for_each(pugi::xml_node& node) override {
        // Default behavior: continue traversal
        return true;
    }

    int depth_limit;
};

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to dummy file
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");

    // If parsing fails, still try to test with empty/invalid nodes
    pugi::xml_node root_node;
    if (result) {
        root_node = doc.root();
    } else {
        root_node = pugi::xml_node(); // Empty node
    }

    // Test 1: xml_node::next_sibling() - no name
    pugi::xml_node next_sib = root_node.next_sibling();
    (void)next_sib; // Suppress unused warning

    // Test 2: xml_node::next_sibling(name) - with name
    // Ensure we don't read out of bounds if Size is 0, but checked above.
    // We cast to const char* for string_view construction.
    std::string_view name_str(reinterpret_cast<const char*>(Data), Size);
    pugi::xml_node next_sib_named = root_node.next_sibling(name_str);
    (void)next_sib_named;

    // Test 3: xml_node::root()
    pugi::xml_node root_of_node = root_node.root();
    (void)root_of_node;

    // Test 4: xml_node::parent()
    pugi::xml_node parent_node = root_node.parent();
    (void)parent_node;

    // Test 5 & 6: xml_tree_walker::begin and end
    // Create a walker instance
    CustomWalker walker;
    // Traverse if document is valid
    if (result) {
        doc.traverse(walker);
    } else {
        // Even if invalid, we can instantiate and call methods manually if needed,
        // but traverse is the standard way.
        // For fuzzing, we ensure the class is instantiated and methods are compiled.
        (void)walker;
    }

    // Additional exploration: traverse with a walker that has a depth limit
    CustomWalker walker_limited(5);
    if (result) {
        doc.traverse(walker_limited);
    }

    // Test with an empty node explicitly
    pugi::xml_node empty_node;
    pugi::xml_node empty_next = empty_node.next_sibling();
    (void)empty_next;
    pugi::xml_node empty_next_named = empty_node.next_sibling(name_str);
    (void)empty_next_named;
    pugi::xml_node empty_root = empty_node.root();
    (void)empty_root;
    pugi::xml_node empty_parent = empty_node.parent();
    (void)empty_parent;

    // Clean up is handled by RAII (pugi::xml_document destructor)
    return 0;
}