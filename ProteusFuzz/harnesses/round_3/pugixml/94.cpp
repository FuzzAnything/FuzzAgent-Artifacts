// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
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

static void safe_invoke_child_value(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (node) {
        // child_value()
        const pugi::char_t* val1 = node.child_value();
        (void)val1; // suppress unused warning

        // child_value(name)
        if (size > 0) {
            // Create a temporary string from fuzz data to use as name
            // Ensure it's null-terminated for safety
            std::vector<pugi::char_t> name_buf(size + 1);
            std::memcpy(name_buf.data(), data, std::min(size, size_t(size)));
            name_buf[size] = 0;
            const pugi::char_t* val2 = node.child_value(name_buf.data());
            (void)val2;
        }
    }
}

static void safe_invoke_name(pugi::xml_node node) {
    if (node) {
        const pugi::char_t* name = node.name();
        (void)name;
    }
}

static void safe_invoke_append_buffer(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (node) {
        // Try different encodings and options
        pugi::xml_encoding encodings[] = {
            pugi::encoding_auto, pugi::encoding_utf8, pugi::encoding_utf16_le,
            pugi::encoding_utf16_be, pugi::encoding_utf16, pugi::encoding_utf32_le,
            pugi::encoding_utf32_be, pugi::encoding_utf32, pugi::encoding_wchar,
            pugi::encoding_latin1
        };
        unsigned int options[] = {
            pugi::parse_default, pugi::parse_escapes, pugi::parse_minimal,
            pugi::parse_full, pugi::parse_embed_pcdata
        };

        for (auto enc : encodings) {
            for (auto opt : options) {
                pugi::xml_parse_result res = node.append_buffer(data, size, opt, enc);
                (void)res;
            }
        }
    }
}

static void safe_invoke_print(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (!node) return;

    // Prepare indent string from fuzz data if possible
    std::vector<pugi::char_t> indent_buf(64, 0);
    if (size > 0) {
        size_t copy_len = std::min(size, size_t(63));
        std::memcpy(indent_buf.data(), data, copy_len);
    }

    // 1. Print to std::basic_ostream<char>
    std::ostringstream oss;
    pugi::xml_encoding encodings[] = {
        pugi::encoding_auto, pugi::encoding_utf8, pugi::encoding_utf16_le,
        pugi::encoding_utf16_be, pugi::encoding_utf16, pugi::encoding_utf32_le,
        pugi::encoding_utf32_be, pugi::encoding_utf32, pugi::encoding_wchar,
        pugi::encoding_latin1
    };
    unsigned int flags[] = {
        pugi::format_default, pugi::format_raw, pugi::format_no_escapes,
        pugi::format_indent_attributes, pugi::format_no_escapes
    };

    for (auto enc : encodings) {
        for (auto flg : flags) {
            node.print(oss, indent_buf.data(), flg, enc, 0);
            node.print(oss, indent_buf.data(), flg, enc, 5); // different depth
        }
    }

    // 2. Print to xml_writer
    // Create a dummy file for xml_writer_file
    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        pugi::xml_writer_file writer(f);
        for (auto enc : encodings) {
            for (auto flg : flags) {
                node.print(writer, indent_buf.data(), flg, enc, 0);
                node.print(writer, indent_buf.data(), flg, enc, 5);
            }
        }
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize document
    pugi::xml_document doc;
    
    // Parse the input data as XML
    pugi::xml_parse_result res = doc.load_buffer(Data, Size);
    if (res.status != pugi::status_ok) {
        // Even if parsing fails, we might have a partial document or empty doc
        // We still want to test the APIs on whatever state we have
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        // If root is empty, try to create a document node manually or just test on doc
        root = doc; 
    }

    // Invoke child_value
    safe_invoke_child_value(root, Data, Size);
    // Also try on children if they exist
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        safe_invoke_child_value(child, Data, Size);
    }

    // Invoke name
    safe_invoke_name(root);
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        safe_invoke_name(child);
    }

    // Invoke append_buffer
    // We need a valid element node to append to. If root is document, try to append to it.
    // If root is an element, append to it.
    safe_invoke_append_buffer(root, Data, Size);
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        safe_invoke_append_buffer(child, Data, Size);
    }

    // Invoke print
    safe_invoke_print(root, Data, Size);
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        safe_invoke_print(child, Data, Size);
    }

    return 0;
}