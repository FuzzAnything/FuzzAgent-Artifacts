// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // Prepare input: ensure null termination for parsing
    std::string input(reinterpret_cast<const char*>(Data), Size);
    // Append a null terminator if not present to be safe for C-style parsing
    if (Size > 0 && input.back() != '\0') {
        input += '\0';
    }

    result = doc.load_string(input.c_str());
    if (!result) {
        // If parsing fails, try loading from file as an alternative path
        write_dummy_file(Data, Size);
        result = doc.load_file("./dummy_file");
        if (!result) {
            return 0;
        }
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Use the correct type pugi::char_t which is 'char' in default mode
    const pugi::char_t* fuzz_data = reinterpret_cast<const pugi::char_t*>(Data);
    size_t fuzz_size = Size;

    // Traverse the document to find nodes to fuzz
    for (pugi::xml_node node = root; node; node = node.next_sibling()) {
        if (Size > 0) {
            // Test set_name overloads
            node.set_name(fuzz_data);
            node.set_name(fuzz_data, fuzz_size);

            // Test set_value overloads
            node.set_value(fuzz_data);
            node.set_value(fuzz_data, fuzz_size);
        }

        // Test offset_debug
        ptrdiff_t offset = node.offset_debug();
        (void)offset;

        // Recurse into children
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            if (Size > 0) {
                child.set_name(fuzz_data);
                child.set_name(fuzz_data, fuzz_size);
                child.set_value(fuzz_data);
                child.set_value(fuzz_data, fuzz_size);
            }
            ptrdiff_t child_offset = child.offset_debug();
            (void)child_offset;
        }
    }

    // Fuzz xml_text::set
    for (pugi::xml_node node = root; node; node = node.next_sibling()) {
        pugi::xml_text text_node = node.text();
        if (text_node) {
            if (Size > 0) {
                text_node.set(fuzz_data);
            }
        }
        
        // Also try text() on children
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            pugi::xml_text child_text = child.text();
            if (child_text) {
                if (Size > 0) {
                    child_text.set(fuzz_data);
                }
            }
        }
    }

    return 0;
}