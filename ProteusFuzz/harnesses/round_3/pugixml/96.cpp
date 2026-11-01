// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
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

static bool fuzzer_tree_walker_begin(pugi::xml_node& node) {
    (void)node;
    return true;
}

static bool fuzzer_tree_walker_end(pugi::xml_node& node) {
    (void)node;
    return true;
}

static bool fuzzer_tree_walker_for_each(pugi::xml_node& node) {
    (void)node;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated to prevent buffer overflows in string processing
    // We create a buffer with a null terminator appended
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(buffer.data());

    // Test pugi::xml_node::root
    pugi::xml_node root_node = doc.root();
    if (root_node) {
        pugi::xml_node root_from_root = root_node.root();
        (void)root_from_root;
    }

    // Test pugi::xml_node::parent
    pugi::xml_node current = doc;
    if (current) {
        pugi::xml_node parent = current.parent();
        (void)parent;
    }
    
    // Traverse children to test parent on non-root nodes
    for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling()) {
        pugi::xml_node parent_of_child = child.parent();
        (void)parent_of_child;
    }

    // Test pugi::xml_node::next_sibling overloads
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        // Overload 1: next_sibling()
        pugi::xml_node next = node.next_sibling();
        (void)next;

        // Overload 2: next_sibling(name)
        std::string_view a_str = "test_name";
        pugi::xml_node next_named = node.next_sibling(a_str);
        (void)next_named;
    }

    // Test pugi::xml_tree_walker::begin, end, and for_each
    struct FuzzWalker : public pugi::xml_tree_walker {
        bool begin(pugi::xml_node& node) override {
            return fuzzer_tree_walker_begin(node);
        }
        bool end(pugi::xml_node& node) override {
            return fuzzer_tree_walker_end(node);
        }
        bool for_each(pugi::xml_node& node) override {
            return fuzzer_tree_walker_for_each(node);
        }
    };

    FuzzWalker walker;
    doc.traverse(walker);

    return 0;
}