// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a buffer with a null terminator to ensure load_string safety
    // We allocate Size + 1 to hold the input plus the null terminator.
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    // Create an XML document and load the fuzzer input
    pugi::xml_document doc;
    if (!doc.load_string(buffer.data())) {
        // If parsing fails, try to create a minimal valid document to test default behaviors
        doc.append_child("root");
    }

    // Select a text node to test. We try to find the first text node in the document.
    // If none exists, we create one or use an empty xml_text object.
    pugi::xml_text text_node;
    pugi::xml_node root = doc.root();
    
    if (root) {
        pugi::xml_node first_child = root.first_child();
        if (first_child) {
            text_node = first_child.text();
        } else {
            // No children, create a text node on root
            text_node = root.text();
        }
    } else {
        // Fallback to default constructed xml_text (empty/null)
        text_node = pugi::xml_text();
    }

    // Invoke the target functions in the specified order
    // 1. as_string
    const char* str_val = text_node.as_string();
    (void)str_val; // Suppress unused variable warning

    // 2. as_int
    int int_val = text_node.as_int();
    (void)int_val;

    // 3. as_uint
    unsigned int uint_val = text_node.as_uint();
    (void)uint_val;

    // 4. as_double
    double double_val = text_node.as_double();
    (void)double_val;

    // 5. as_float
    float float_val = text_node.as_float();
    (void)float_val;

    // 6. as_bool
    bool bool_val = text_node.as_bool();
    (void)bool_val;

    // 7. as_llong
    long long llong_val = text_node.as_llong();
    (void)llong_val;

    // 8. as_ullong
    unsigned long long ullong_val = text_node.as_ullong();
    (void)ullong_val;

    return 0;
}