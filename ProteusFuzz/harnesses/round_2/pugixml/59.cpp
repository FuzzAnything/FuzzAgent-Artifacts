// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
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

// Helper to create a document from fuzz input
static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;
    
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load_buffer(data, size, pugi::parse_default, pugi::encoding_auto);
    
    if (!result) {
        delete doc;
        return nullptr;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a document from input
    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    // Get root node: child() requires a name, so we iterate or use first_child()
    pugi::xml_node root = doc->first_child();
    
    // If no child element found, use the document node itself for testing
    if (root.empty()) {
        root = *doc;
    }

    // Test pugi::xml_node::name
    const pugi::char_t* name = root.name();
    (void)name; // Use the result to avoid unused variable warning

    // Test pugi::xml_node::child_value (both overloads)
    const pugi::char_t* childValue1 = root.child_value();
    (void)childValue1;
    
    const pugi::char_t* childValue2 = root.child_value(PUGIXML_TEXT("test"));
    (void)childValue2;

    // Test pugi::xml_node::append_buffer
    // We'll try to append the input data as a fragment
    pugi::xml_parse_result appendResult = root.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    (void)appendResult;

    // Test pugi::xml_node::print with stream
    {
        std::ostringstream oss;
        root.print(oss, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
        root.print(oss, PUGIXML_TEXT("  "), pugi::format_raw, pugi::encoding_utf8, 0);
        root.print(oss, PUGIXML_TEXT(""), pugi::format_indent_attributes, pugi::encoding_utf16_le, 0);
    }

    // Test pugi::xml_node::print with xml_writer_file
    {
        FILE* file = fopen("./dummy_file", "wb");
        if (file) {
            pugi::xml_writer_file writer(file);
            root.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
            root.print(writer, PUGIXML_TEXT("  "), pugi::format_raw, pugi::encoding_utf8, 0);
            fclose(file);
        }
    }

    // Test pugi::xml_node::print with xml_writer_stream
    {
        std::wostringstream woss;
        pugi::xml_writer_stream writer(woss);
        root.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
        root.print(writer, PUGIXML_TEXT("  "), pugi::format_raw, pugi::encoding_utf8, 0);
    }

    // Cleanup
    delete doc;
    return 0;
}