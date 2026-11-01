// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

static void invoke_text_conversion_sequence(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Create a text node from the input data
    // We construct a minimal XML structure to ensure the text node is valid
    char buffer[4096];
    size_t copy_size = (size < sizeof(buffer) - 4) ? size : sizeof(buffer) - 4;
    
    // Create a safe XML string from fuzz input
    // Wrap input in a tag to ensure valid XML structure for parsing
    std::memcpy(buffer, "<root>", 6);
    size_t offset = 6;
    
    if (copy_size > 0) {
        // Escape potential closing tags to prevent premature end of element
        for (size_t i = 0; i < copy_size; ++i) {
            if (data[i] == '<' || data[i] == '>') {
                if (offset + 4 >= sizeof(buffer)) break;
                buffer[offset++] = '&';
                buffer[offset++] = 'l';
                buffer[offset++] = 't';
                buffer[offset++] = ';';
            } else if (data[i] == '&') {
                if (offset + 4 >= sizeof(buffer)) break;
                buffer[offset++] = '&';
                buffer[offset++] = 'a';
                buffer[offset++] = 'm';
                buffer[offset++] = 'p';
                buffer[offset++] = ';';
            } else {
                if (offset >= sizeof(buffer)) break;
                buffer[offset++] = data[i];
            }
        }
    }
    
    if (offset + 7 < sizeof(buffer)) {
        std::memcpy(buffer + offset, "</root>", 7);
        offset += 7;
    }
    buffer[offset] = '\0';
    
    if (!doc.load_string(buffer)) {
        // If parsing fails, try with empty document
        return;
    }
    
    pugi::xml_node root = doc.child("root");
    if (!root) return;
    
    // Get text node
    pugi::xml_text text_node = root.text();
    
    // Invoke the target functions in the specified order
    // 1. as_string
    const pugi::char_t* str_val = text_node.as_string(PUGIXML_TEXT("default"));
    (void)str_val;
    
    // 2. as_int
    int int_val = text_node.as_int(42);
    (void)int_val;
    
    // 3. as_uint
    unsigned int uint_val = text_node.as_uint(100u);
    (void)uint_val;
    
    // 4. as_double
    double double_val = text_node.as_double(3.14);
    (void)double_val;
    
    // 5. as_float
    float float_val = text_node.as_float(2.71f);
    (void)float_val;
    
    // 6. as_bool
    bool bool_val = text_node.as_bool(true);
    (void)bool_val;
    
    // 7. as_llong
    long long llong_val = text_node.as_llong(-1000LL);
    (void)llong_val;
    
    // 8. as_ullong
    unsigned long long ullong_val = text_node.as_ullong(9999ULL);
    (void)ullong_val;
    
    // Also test with empty text node
    pugi::xml_text empty_text;
    (void)empty_text.as_string();
    (void)empty_text.as_int();
    (void)empty_text.as_uint();
    (void)empty_text.as_double();
    (void)empty_text.as_float();
    (void)empty_text.as_bool();
    (void)empty_text.as_llong();
    (void)empty_text.as_ullong();
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    invoke_text_conversion_sequence(Data, Size);
    
    return 0;
}