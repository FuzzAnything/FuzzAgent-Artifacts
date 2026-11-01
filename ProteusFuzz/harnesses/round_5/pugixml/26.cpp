// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_attribute::hash_value at pugixml.cpp:5379:37 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_attribute::hash_value at pugixml.cpp:5379:37 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: load XML from fuzz input
    pugi::xml_document doc;
    
    // Ensure the input is null-terminated before passing to load_string
    // We create a buffer with a guaranteed null terminator
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    // Try loading from memory first
    if (!doc.load_string(buffer.data())) {
        // If parsing fails, write to file and try loading from file
        write_dummy_file(Data, Size);
        if (!doc.load_file("./dummy_file")) {
            // If still fails, create a minimal valid document
            doc.append_child("root");
        }
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::child
    // Create a safe null-terminated string for child name
    char safe_name[256];
    size_t copy_len = (Size < 255) ? Size : 255;
    std::memcpy(safe_name, Data, copy_len);
    safe_name[copy_len] = '\0';
    
    pugi::xml_node child_node = root.child(safe_name);

    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();

    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child_node = root.first_child();

    // 4. pugi::xml_node::hash_value (1st call)
    size_t hash1 = root.hash_value();

    // 5. pugi::xml_attribute::hash_value (1st call)
    size_t attr_hash1 = first_attr.hash_value();

    // 6. pugi::xml_node::hash_value (2nd call)
    size_t hash2 = child_node.hash_value();

    // 7. pugi::xml_node::hash_value (3rd call)
    size_t hash3 = first_child_node.hash_value();

    // 8. pugi::xml_node::hash_value (4th call)
    size_t hash4 = root.hash_value();

    // 9. pugi::xml_attribute::hash_value (2nd call)
    size_t attr_hash2 = first_attr.hash_value();

    // 10. pugi::xml_node::hash_value (5th call)
    size_t hash5 = child_node.hash_value();

    // 11. pugi::xml_attribute::hash_value (3rd call)
    size_t attr_hash3 = first_attr.hash_value();

    // Use all computed values to prevent compiler optimizations
    volatile size_t sink = hash1 + attr_hash1 + hash2 + hash3 + hash4 + attr_hash2 + hash5 + attr_hash3;
    (void)sink;

    // Cleanup is handled by RAII (pugi::xml_document destructor)
    return 0;
}