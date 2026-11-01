// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <fstream>

static void fuzz_child_value(pugi::xml_node& node) {
    // Test child_value() - get text of first PCDATA/CDATA child
    const pugi::char_t* val1 = node.child_value();
    (void)val1;

    // Test child_value(name) - get text of specific child
    // Use a static buffer to avoid stack issues with fuzzing data
    static char name_buf[256];
    if (node.attribute("test_name")) {
        const char* name_str = node.attribute("test_name").value();
        if (name_str && std::strlen(name_str) < sizeof(name_buf)) {
            std::strcpy(name_buf, name_str);
            const pugi::char_t* val2 = node.child_value(name_buf);
            (void)val2;
        }
    }
}

static void fuzz_name(pugi::xml_node& node) {
    // Test name() - get node name
    const pugi::char_t* name = node.name();
    (void)name;
}

static void fuzz_append_buffer(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return;

    // Try different encodings and options
    pugi::xml_encoding encodings[] = {
        pugi::encoding_auto,
        pugi::encoding_utf8,
        pugi::encoding_utf16_le,
        pugi::encoding_utf16_be,
        pugi::encoding_utf32_le,
        pugi::encoding_utf32_be,
        pugi::encoding_latin1
    };

    unsigned int options[] = {
        pugi::parse_default,
        pugi::parse_escapes,
        pugi::parse_minimal,
        pugi::parse_escapes | pugi::parse_minimal
    };

    for (auto enc : encodings) {
        for (auto opt : options) {
            pugi::xml_parse_result result = node.append_buffer(data, size, opt, enc);
            (void)result;
        }
    }
}

static void fuzz_print(pugi::xml_node& node) {
    // Test print to stream
    std::ostringstream oss;
    node.print(oss);

    // Test print with different flags
    node.print(oss, PUGIXML_TEXT("\t"), pugi::format_default);
    node.print(oss, PUGIXML_TEXT("  "), pugi::format_indent);
    node.print(oss, PUGIXML_TEXT(""), pugi::format_raw);

    // Test print to writer using FILE*
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        pugi::xml_writer_file writer(f);
        node.print(writer);
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_default);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    try {
        pugi::xml_document doc;
        
        // Parse the input data as XML
        pugi::xml_parse_result result = doc.load_buffer(Data, Size);
        
        // If parsing failed, we can still test with an empty document
        // or try to parse as fragment
        
        // Get root node or create one
        pugi::xml_node root = doc.root();
        if (!root) {
            root = doc.append_child(pugi::node_element);
        }

        // Fuzz child_value
        fuzz_child_value(root);
        
        // Fuzz name
        fuzz_name(root);

        // Fuzz append_buffer - try to append more data
        fuzz_append_buffer(root, Data, Size);

        // After appending, fuzz again
        pugi::xml_node new_root = doc.root();
        if (new_root) {
            fuzz_child_value(new_root);
            fuzz_name(new_root);
        }

        // Fuzz print
        fuzz_print(doc);
        
        // Also try printing individual nodes
        for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling()) {
            fuzz_print(child);
        }

        return 0;
    } catch (...) {
        // Catch any exceptions to prevent fuzzer crashes
        return 0;
    }
}