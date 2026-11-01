// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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

static void fuzz_xml_node_name(pugi::xml_node& node) {
    const pugi::char_t* name = node.name();
    (void)name;
}

static void fuzz_xml_node_print(pugi::xml_node& node, const uint8_t* data, size_t size) {
    // Overload 1: print to std::basic_ostream<wchar_t>
    // We use a wstringstream to capture the output
    std::wstringstream wss;
    node.print(wss, PUGIXML_TEXT("\t"), pugi::format_default, 0);

    // Overload 2: print to xml_writer (using file)
    // xml_writer_file expects a FILE* (void*), so we open a file with fopen
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        pugi::xml_writer_file writer_file(f);
        node.print(writer_file, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
        fclose(f);
    }
}

static void fuzz_xml_node_child_value(pugi::xml_node& node, const pugi::char_t* name) {
    const pugi::char_t* val1 = node.child_value();
    const pugi::char_t* val2 = node.child_value(name);
    (void)val1;
    (void)val2;
}

static void fuzz_xml_node_append_buffer(pugi::xml_node& node, const uint8_t* data, size_t size) {
    pugi::xml_parse_result result = node.append_buffer(data, size, pugi::parse_default, pugi::encoding_auto);
    (void)result;
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    // Attempt to parse the input as an XML document
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get the root node. If parsing failed or document is empty, we still have a valid node object (empty)
    pugi::xml_node root = doc;
    
    // If the document has no root element, create one to allow append_buffer and other operations
    if (root.empty() && result.status == pugi::status_ok) {
        root = doc.append_child(pugi::node_element);
    }

    // Fuzz name()
    fuzz_xml_node_name(root);

    // Fuzz print() overloads
    fuzz_xml_node_print(root, Data, Size);

    // Fuzz child_value() overloads
    // Cast input data to char_t*. Note: if PUGIXML_WCHAR_MODE is on, this might be a type mismatch,
    // but for fuzzing, we pass the raw buffer. The function handles null/empty gracefully.
    const pugi::char_t* name_ptr = reinterpret_cast<const pugi::char_t*>(Data);
    fuzz_xml_node_child_value(root, name_ptr);

    // Fuzz append_buffer()
    // This will attempt to parse the input as a fragment and append it.
    fuzz_xml_node_append_buffer(root, Data, Size);

    // Cleanup is handled by xml_document destructor
    return 0;
}