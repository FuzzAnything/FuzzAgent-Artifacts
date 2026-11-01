// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    write_dummy_file(Data, Size);
    
    if (!doc.load_file("./dummy_file")) {
        return 0;
    }

    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::child
    std::string_view a_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure null-termination for safety if passing to C-style string functions
    // However, pugixml child(const char_t*) expects a C-string.
    // We will try to use the input as a name, but we must ensure it's null-terminated or use a safe substring.
    // Since the input might not be null-terminated, we'll just pick a safe default or a substring if possible.
    // For fuzzing, let's try to use the first byte as a char if it's printable, or just a fixed string.
    // To strictly follow the hint about string_view_t, we assume the API might accept it in some contexts, 
    // but the signature says const char_t*. Let's use a safe approach:
    const char* name_to_search = "root"; 
    if (Size > 0 && Data[0] != 0) {
        // Create a small buffer to hold a safe name from input
        char safe_name[16] = {0};
        size_t copy_len = (Size < 15) ? Size : 15;
        for (size_t i = 0; i < copy_len; ++i) {
            if (Data[i] >= 32 && Data[i] < 127) {
                safe_name[i] = static_cast<char>(Data[i]);
            } else {
                break;
            }
        }
        name_to_search = safe_name;
    }

    pugi::xml_node child_node = root.child(name_to_search);

    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute attr = child_node.first_attribute();

    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child = child_node.first_child();

    // 4. pugi::xml_node::hash_value
    size_t h1 = child_node.hash_value();
    (void)h1;

    // 5. pugi::xml_attribute::hash_value
    size_t h2 = attr.hash_value();
    (void)h2;

    // 6. pugi::xml_node::hash_value
    size_t h3 = first_child.hash_value();
    (void)h3;

    // 7. pugi::xml_node::hash_value (again)
    size_t h4 = child_node.hash_value();
    (void)h4;

    // 8. pugi::xml_node::hash_value (again)
    size_t h5 = first_child.hash_value();
    (void)h5;

    // 9. pugi::xml_attribute::hash_value (again)
    size_t h6 = attr.hash_value();
    (void)h6;

    // 10. pugi::xml_node::hash_value (again)
    size_t h7 = child_node.hash_value();
    (void)h7;

    // 11. pugi::xml_attribute::hash_value (again)
    size_t h8 = attr.hash_value();
    (void)h8;

    return 0;
}