// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
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
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>

static bool parse_document_from_fuzz_data(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    // Ensure the input is null-terminated for safe parsing if needed, though load_buffer handles size
    // We use load_buffer to parse directly from the fuzz input
    pugi::xml_parse_result result = doc.load_buffer(data, size, pugi::parse_default);
    return result.status == pugi::status_ok;
}

static void fuzz_set_name(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (node.empty()) return;

    // Overload 1: set_name(const char_t* rhs)
    // We use a portion of the fuzz data as the new name
    if (size > 0) {
        // Create a temporary buffer with a null terminator to be safe
        // We limit the length to avoid huge allocations if fuzz input is massive
        size_t max_len = (size < 4096) ? size : 4096;
        std::string temp_str(reinterpret_cast<const char*>(data), max_len);
        node.set_name(temp_str.c_str());
    }

    // Overload 2: set_name(const char_t* rhs, size_t size)
    if (size > 0) {
        size_t len = (size < 4096) ? size : 4096;
        node.set_name(reinterpret_cast<const pugi::char_t*>(data), len);
    }
}

static void fuzz_set_value(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (node.empty()) return;

    // Overload 1: set_value(const char_t* rhs)
    if (size > 0) {
        size_t max_len = (size < 4096) ? size : 4096;
        std::string temp_str(reinterpret_cast<const char*>(data), max_len);
        node.set_value(temp_str.c_str());
    }

    // Overload 2: set_value(const char_t* rhs, size_t size)
    if (size > 0) {
        size_t len = (size < 4096) ? size : 4096;
        node.set_value(reinterpret_cast<const pugi::char_t*>(data), len);
    }
}

static void fuzz_offset_debug(pugi::xml_node node) {
    if (node.empty()) return;
    ptrdiff_t offset = node.offset_debug();
    (void)offset; // Use the value to prevent unused variable warnings
}

static void fuzz_xml_text_set(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (node.empty()) return;

    pugi::xml_text text = node.text();
    if (text.empty()) return;

    // Overload: set(const char_t* rhs)
    if (size > 0) {
        size_t max_len = (size < 4096) ? size : 4096;
        std::string temp_str(reinterpret_cast<const char*>(data), max_len);
        text.set(temp_str.c_str());
    }
}

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;

    // Attempt to parse the fuzz input
    // We use a dummy file approach if the data looks like it should be a file, 
    // but load_buffer is more direct for fuzzing raw bytes.
    // However, to ensure we test the "single buffer" constraint for offset_debug,
    // we load directly from buffer.
    bool parsed = parse_document_from_fuzz_data(Data, Size, doc);
    
    if (!parsed) {
        // Even if parsing fails, we might have a valid document structure if partial
        // or we can try to create a minimal document to test setters on empty nodes
        // But the instructions say "Use it on valid nodes". 
        // If parsing fails completely, doc might be empty.
        // Let's try to create a minimal valid document if parsing fails to ensure coverage
        if (doc.empty()) {
            doc.append_child("root");
        }
    }

    // Traverse the document and invoke target functions
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        // Fuzz set_name
        fuzz_set_name(node, Data, Size);

        // Fuzz set_value
        fuzz_set_value(node, Data, Size);

        // Fuzz offset_debug
        fuzz_offset_debug(node);

        // Fuzz xml_text::set
        fuzz_xml_text_set(node, Data, Size);

        // Recursively process children
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            fuzz_set_name(child, Data, Size);
            fuzz_set_value(child, Data, Size);
            fuzz_offset_debug(child);
            fuzz_xml_text_set(child, Data, Size);
        }
    }

    // Also test on the document node itself if applicable (though set_name/set_value usually fail on root element if not set up right)
    fuzz_set_name(doc, Data, Size);
    fuzz_set_value(doc, Data, Size);
    fuzz_offset_debug(doc);
    
    // Test xml_text on the document
    fuzz_xml_text_set(doc, Data, Size);

    return 0;
}