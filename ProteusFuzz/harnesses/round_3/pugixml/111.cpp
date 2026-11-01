// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

static bool custom_walker_begin(pugi::xml_node& node) {
    (void)node;
    return true;
}

static bool custom_walker_end(pugi::xml_node& node) {
    (void)node;
    return true;
}

static bool custom_walker_for_each(pugi::xml_node& node) {
    (void)node;
    return true;
}

class CustomTreeWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        return custom_walker_begin(node);
    }

    bool end(pugi::xml_node& node) override {
        return custom_walker_end(node);
    }

    bool for_each(pugi::xml_node& node) override {
        return custom_walker_for_each(node);
    }
};

extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for load_string
    // We create a buffer with size + 1 to safely add a null terminator
    // without overflowing the original buffer logic if we were to copy.
    // However, load_string expects a null-terminated string.
    // Since Data might contain null bytes or not be null-terminated,
    // we must handle this carefully.
    
    // Strategy: Find the first null byte or use the whole buffer if no null byte exists
    // But load_string will read until null. If Data has no null, it reads past buffer.
    // We need to create a safe copy that is guaranteed null-terminated.
    
    std::vector<char> safe_buffer(Size + 1);
    std::memcpy(safe_buffer.data(), Data, Size);
    safe_buffer[Size] = '\0';

    // Create an XML document and parse the fuzzed input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(safe_buffer.data());

    if (!result) {
        // Parsing failed, but we can still test with an empty document
        // to exercise error handling paths
    }

    // Test xml_node::root
    pugi::xml_node root_node = doc.root();
    if (root_node) {
        // Try to get root from a child node
        pugi::xml_node child = root_node.first_child();
        if (child) {
            pugi::xml_node root_from_child = child.root();
            (void)root_from_child; // Suppress unused variable warning
        }
    }

    // Test xml_node::next_sibling overloads
    pugi::xml_node node = doc.first_child();
    while (node) {
        // Test next_sibling()
        pugi::xml_node next = node.next_sibling();
        
        // Test next_sibling(name)
        // Use a safe subset of the input for the name parameter
        if (Size > 0) {
            // Create a null-terminated string for the name
            // Limit length to avoid huge allocations and ensure null termination
            size_t name_len = (Size < 50) ? Size : 50;
            // Ensure we don't read past the safe buffer
            std::string name_str(safe_buffer.data(), name_len);
            pugi::xml_node next_named = node.next_sibling(name_str.c_str());
            (void)next_named;
        }
        
        node = next;
    }

    // Test xml_tree_walker::begin and xml_tree_walker::end
    CustomTreeWalker walker;
    doc.traverse(walker);

    // Test xml_node::parent
    pugi::xml_node current = doc.first_child();
    while (current) {
        pugi::xml_node parent = current.parent();
        if (parent) {
            // Navigate further up if possible
            pugi::xml_node grandparent = parent.parent();
            (void)grandparent;
        }
        current = current.next_sibling();
    }

    return 0;
}