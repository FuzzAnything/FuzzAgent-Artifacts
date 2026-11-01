// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include <cstring>
#include <string>

static void fuzz_set_name(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size < 2) return;

    // Overload 1: set_name(const char_t* rhs)
    // Ensure null termination for safety
    std::string str(reinterpret_cast<const char*>(data), size);
    str += '\0';
    node.set_name(str.c_str());

    // Overload 2: set_name(const char_t* rhs, size_t size)
    // Use size - 1 to avoid including the null terminator if it exists, or just size if we trust the input
    if (size > 0) {
        node.set_name(str.c_str(), size);
    }
}

static void fuzz_set_value(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size < 2) return;

    std::string str(reinterpret_cast<const char*>(data), size);
    str += '\0';

    // Overload 1: set_value(const char_t* rhs)
    node.set_value(str.c_str());

    // Overload 2: set_value(const char_t* rhs, size_t size)
    if (size > 0) {
        node.set_value(str.c_str(), size);
    }
}

static void fuzz_xml_text_set(pugi::xml_text& text, const uint8_t* data, size_t size) {
    if (size < 2) return;

    std::string str(reinterpret_cast<const char*>(data), size);
    str += '\0';

    // Overload 1: set(const char_t* rhs)
    text.set(str.c_str());

    // Overload 2: set(const char_t* rhs, size_t size)
    if (size > 0) {
        text.set(str.c_str(), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment: Create a document and a root node
    pugi::xml_document doc;
    if (!doc.load_string("<root><child>text</child></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    pugi::xml_node child = root.child("child");
    pugi::xml_text text = child.text();

    // Split input data to test different functions
    size_t chunk_size = Size / 3;
    if (chunk_size == 0) chunk_size = Size;

    // Fuzz set_name on 'child' node
    // We use the first chunk for name setting
    fuzz_set_name(child, Data, chunk_size);

    // Fuzz set_value on 'child' node
    // We use the second chunk for value setting
    size_t offset = chunk_size;
    size_t remaining = Size - offset;
    if (remaining > 0) {
        fuzz_set_value(child, Data + offset, remaining);
    }

    // Fuzz xml_text::set on 'text' object
    // We use the third chunk (or remaining data)
    offset = (chunk_size * 2);
    if (offset < Size) {
        fuzz_xml_text_set(text, Data + offset, Size - offset);
    }

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}