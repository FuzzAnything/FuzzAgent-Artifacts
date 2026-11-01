// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_attribute::hash_value at pugixml.cpp:5379:37 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_attribute::hash_value at pugixml.cpp:5379:37 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_attribute::hash_value at pugixml.cpp:5379:37 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include "pugixml.hpp"

static bool is_valid_xml_input(const uint8_t* data, size_t size) {
    if (size == 0) return false;
    // Basic check: must start with < and contain at least one >
    if (data[0] != '<') return false;
    bool has_close = false;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == '>') {
            has_close = true;
            break;
        }
    }
    return has_close;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (!is_valid_xml_input(Data, Size)) {
        return 0;
    }

    // Ensure the input is null-terminated for load_string
    // We create a temporary buffer with a null terminator
    // We must not read beyond Size, so we copy to a safe buffer
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    if (!result) {
        // Parsing failed, but we can still try to operate on empty nodes safely
        // However, to ensure meaningful fuzzing, we proceed with the document as is
    }

    // Start from root
    pugi::xml_node root = doc.first_child();
    if (root.empty()) {
        // If no root, try to use document itself if it's valid
        root = doc;
        if (root.empty()) {
            return 0;
        }
    }

    // 1. pugi::xml_node::child
    // We try a few common tag names and also derive one from input if possible
    const char* test_names[] = {"child", "node", "item", "root", "data"};
    pugi::xml_node child_node;
    
    // Try predefined names
    for (int i = 0; i < 5; ++i) {
        child_node = root.child(test_names[i]);
        if (!child_node.empty()) break;
    }
    
    // If not found, try to extract a name from input (simple heuristic)
    if (child_node.empty() && Size > 10) {
        // Look for a tag name pattern in the original data (safe since we checked bounds)
        const char* start = buffer.data();
        const char* end = start + Size;
        const char* tag_start = std::find(start, end, '<');
        if (tag_start != end && *(tag_start + 1) != '/') {
            const char* tag_end = std::find(tag_start + 1, end, '>');
            if (tag_end != end) {
                size_t name_len = tag_end - tag_start - 1;
                if (name_len > 0 && name_len < 100) {
                    char name_buf[101];
                    std::memcpy(name_buf, tag_start + 1, name_len);
                    name_buf[name_len] = '\0';
                    child_node = root.child(name_buf);
                }
            }
        }
    }

    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = child_node.empty() ? root.first_attribute() : child_node.first_attribute();

    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child_node = child_node.empty() ? root.first_child() : child_node.first_child();

    // 4. pugi::xml_node::hash_value (on child_node or root if child_node is empty)
    pugi::xml_node node_for_hash1 = child_node.empty() ? root : child_node;
    size_t hash1 = node_for_hash1.hash_value();

    // 5. pugi::xml_attribute::hash_value
    size_t hash_attr = first_attr.hash_value();

    // 6. pugi::xml_node::hash_value (on first_child_node or root if empty)
    pugi::xml_node node_for_hash2 = first_child_node.empty() ? root : first_child_node;
    size_t hash2 = node_for_hash2.hash_value();

    // 7. pugi::xml_node::hash_value (on root)
    size_t hash3 = root.hash_value();

    // 8. pugi::xml_node::hash_value (on child_node if valid, else root)
    pugi::xml_node node_for_hash4 = child_node.empty() ? root : child_node;
    size_t hash4 = node_for_hash4.hash_value();

    // 9. pugi::xml_attribute::hash_value (on first_attr)
    size_t hash_attr2 = first_attr.hash_value();

    // 10. pugi::xml_node::hash_value (on first_child_node if valid, else root)
    pugi::xml_node node_for_hash5 = first_child_node.empty() ? root : first_child_node;
    size_t hash5 = node_for_hash5.hash_value();

    // 11. pugi::xml_attribute::hash_value (on first_attr)
    size_t hash_attr3 = first_attr.hash_value();

    // Use the hashes to prevent compiler optimization from removing the calls
    volatile size_t dummy = hash1 + hash2 + hash3 + hash4 + hash5 + hash_attr + hash_attr2 + hash_attr3;
    (void)dummy;

    return 0;
}