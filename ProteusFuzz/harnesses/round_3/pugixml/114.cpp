// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
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
#include <cstring>
#include <string>

static bool custom_begin(pugi::xml_node& node) {
    // Custom logic for begin traversal
    return true;
}

static bool custom_end(pugi::xml_node& node) {
    // Custom logic for end traversal
    return true;
}

static bool custom_for_each(pugi::xml_node& node) {
    // Custom logic for for_each traversal
    return true;
}

class CustomTreeWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        return custom_begin(node);
    }

    bool end(pugi::xml_node& node) override {
        return custom_end(node);
    }

    bool for_each(pugi::xml_node& node) override {
        return custom_for_each(node);
    }
};

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    std::string xml_string(reinterpret_cast<const char*>(Data), Size);

    // Parse the input data
    pugi::xml_parse_result result = doc.load_string(xml_string.c_str());
    if (!result) {
        // Parsing failed, but we can still test with an empty document
    }

    // Test xml_node::root
    pugi::xml_node root_node = doc.root();
    if (root_node) {
        pugi::xml_node root_from_root = root_node.root();
        (void)root_from_root; // Suppress unused variable warning
    }

    // Test xml_node::next_sibling (both overloads)
    if (root_node) {
        pugi::xml_node next_sibling_no_name = root_node.next_sibling();
        (void)next_sibling_no_name; // Suppress unused variable warning

        std::string_view a_str = "test";
        pugi::xml_node next_sibling_with_name = root_node.next_sibling(a_str.data());
        (void)next_sibling_with_name; // Suppress unused variable warning
    }

    // Test xml_tree_walker::begin, xml_tree_walker::end, and xml_tree_walker::for_each
    CustomTreeWalker walker;
    if (root_node) {
        doc.traverse(walker);
    }

    // Test xml_node::parent
    if (root_node) {
        pugi::xml_node parent_node = root_node.parent();
        (void)parent_node; // Suppress unused variable warning
    }

    // Cleanup is handled automatically by pugi::xml_document destructor
    return 0;
}