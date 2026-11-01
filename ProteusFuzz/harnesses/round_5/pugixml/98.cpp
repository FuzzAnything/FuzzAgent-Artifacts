// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size > 0) {
        doc.load_buffer(data, size);
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a document from the input data
    pugi::xml_document doc = create_document_from_input(Data, Size);
    
    // Get the root node or create one if empty
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }

    // Test pugi::xml_node::name
    const pugi::char_t* name = root.name();
    (void)name; // Use the result to avoid unused variable warning

    // Test pugi::xml_node::child_value (overload 1: no name)
    const pugi::char_t* childVal1 = root.child_value();
    (void)childVal1;

    // Test pugi::xml_node::child_value (overload 2: with name)
    const pugi::char_t* childVal2 = root.child_value("test");
    (void)childVal2;

    // Prepare data for append_buffer
    // We use the input data directly, ensuring it's treated as a buffer
    const void* buffer = Data;
    size_t bufferSize = Size;

    // Test pugi::xml_node::append_buffer
    // We try to append the buffer to the root node
    pugi::xml_parse_result parseResult = root.append_buffer(buffer, bufferSize, pugi::parse_default, pugi::encoding_auto);
    (void)parseResult;

    // If parsing failed or root is not an element, try to create a new element to append to
    if (!root || root.type() != pugi::node_element) {
        pugi::xml_node newRoot = doc.append_child("newRoot");
        newRoot.append_buffer(buffer, bufferSize, pugi::parse_default, pugi::encoding_auto);
    }

    // Re-fetch root for printing tests
    root = doc.root();
    if (!root) {
        root = doc.append_child("fallbackRoot");
    }

    // Test pugi::xml_node::print (overload 1: with std::basic_ostream<wchar_t>)
    std::wstringstream wss;
    root.print(wss);

    // Test pugi::xml_node::print (overload 2: with xml_writer)
    // Create a dummy file for xml_writer_file
    const char* dummyFile = "./dummy_file";
    FILE* file = fopen(dummyFile, "wb");
    if (file) {
        pugi::xml_writer_file writerFile(file);
        root.print(writerFile);
        fclose(file);
    }

    // Clean up dummy file if it exists
    std::remove(dummyFile);

    return 0;
}