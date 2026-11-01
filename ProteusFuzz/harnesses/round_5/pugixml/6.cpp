// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy XML document with text content derived from input
    pugi::xml_document doc;
    
    // Construct a valid XML string from input data, ensuring it's safe
    // We wrap the raw data in a text node to ensure we have a valid text node to test
    // We escape potential problematic characters by treating input as raw bytes in a CDATA or just text
    // To be safe and simple, we create a minimal XML structure
    
    // Limit size to avoid huge allocations
    if (Size > 10000) Size = 10000;
    
    // Create a buffer for the XML content
    // We'll format it as <root><text>...</text></root>
    // We need to handle the input bytes carefully. We will just use them as the text content.
    // To ensure valid XML, we might need to escape, but for fuzzing, we want to test parsing failures too.
    // However, pugixml requires well-formed XML. So we will wrap the input in a way that is always valid XML.
    // We can use CDATA or just escape. Let's try to escape < and & and " to be safe, or just use a simple approach.
    // Actually, the easiest way to fuzz text nodes is to load a document that contains the data.
    // Let's construct a string: "<r><t>" + escaped_data + "</t></r>"
    
    // Simple escaping for XML special characters to ensure the document loads
    size_t max_len = Size * 6 + 20; // generous buffer
    char* buffer = new char[max_len];
    size_t offset = 0;
    
    // Start tag
    const char* start = "<r><t>";
    size_t start_len = 6;
    for (size_t i = 0; i < start_len; ++i) buffer[offset++] = start[i];
    
    // Escape input data
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        if (c == '<') {
            buffer[offset++] = '&'; buffer[offset++] = 'l'; buffer[offset++] = 't'; buffer[offset++] = ';';
        } else if (c == '>') {
            buffer[offset++] = '&'; buffer[offset++] = 'g'; buffer[offset++] = 't'; buffer[offset++] = ';';
        } else if (c == '&') {
            buffer[offset++] = '&'; buffer[offset++] = 'a'; buffer[offset++] = 'm'; buffer[offset++] = 'p'; buffer[offset++] = ';';
        } else if (c == '"') {
            buffer[offset++] = '&'; buffer[offset++] = 'q'; buffer[offset++] = 'u'; buffer[offset++] = 'o'; buffer[offset++] = 't'; buffer[offset++] = ';';
        } else if (c == '\'') {
            buffer[offset++] = '&'; buffer[offset++] = 'a'; buffer[offset++] = 'p'; buffer[offset++] = 'o'; buffer[offset++] = 's'; buffer[offset++] = ';';
        } else if (c >= 32 && c < 127) {
            buffer[offset++] = c;
        } else {
            // Skip non-printable or replace with space to ensure valid UTF-8/XML
            buffer[offset++] = ' ';
        }
    }
    
    // End tag
    const char* end = "</t></r>";
    size_t end_len = 7;
    for (size_t i = 0; i < end_len; ++i) buffer[offset++] = end[i];
    buffer[offset] = '\0';
    
    // Load the document
    pugi::xml_parse_result result = doc.load_string(buffer);
    delete[] buffer;
    
    if (!result) {
        // If parsing failed, we can't test the text node functions effectively on this input
        // But we might still want to test with an empty text node or null node
        // Let's try to get the text node anyway, it might be null
    }
    
    // Get the text node
    pugi::xml_node root = doc.child("r");
    if (!root) {
        // If root is missing, create a dummy text node (empty) to test default behaviors
        pugi::xml_text text; // Default constructor creates empty
        // Invoke the target functions in order
        text.as_string();
        text.as_int();
        text.as_uint();
        text.as_double();
        text.as_float();
        text.as_bool();
        text.as_llong();
        text.as_ullong();
        return 0;
    }
    
    pugi::xml_node text_node = root.child("t");
    if (!text_node) {
        // Fallback to empty text node
        pugi::xml_text text;
        text.as_string();
        text.as_int();
        text.as_uint();
        text.as_double();
        text.as_float();
        text.as_bool();
        text.as_llong();
        text.as_ullong();
        return 0;
    }
    
    pugi::xml_text text = text_node.text();
    
    // Invoke the target functions in the specified order
    // 1. as_string
    const pugi::char_t* s = text.as_string();
    (void)s; // Suppress unused warning
    
    // 2. as_int
    int i = text.as_int();
    (void)i;
    
    // 3. as_uint
    unsigned int ui = text.as_uint();
    (void)ui;
    
    // 4. as_double
    double d = text.as_double();
    (void)d;
    
    // 5. as_float
    float f = text.as_float();
    (void)f;
    
    // 6. as_bool
    bool b = text.as_bool();
    (void)b;
    
    // 7. as_llong
    long long ll = text.as_llong();
    (void)ll;
    
    // 8. as_ullong
    unsigned long long ull = text.as_ullong();
    (void)ull;
    
    return 0;
}