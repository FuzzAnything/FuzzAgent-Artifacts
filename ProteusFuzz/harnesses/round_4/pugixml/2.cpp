// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
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

static void invoke_target_functions(const uint8_t *Data, size_t Size) {
    // Create a temporary buffer to hold the fuzzed data as an XML string
    // Ensure null termination for safety
    std::string xml_buffer(reinterpret_cast<const char*>(Data), Size);
    xml_buffer += '\0';

    pugi::xml_document doc;
    
    // Parse the fuzzed input as XML
    pugi::xml_parse_result result = doc.load_string(xml_buffer.c_str());
    
    // Even if parsing fails, we can still test the functions on the root or empty nodes
    // to trigger different code paths (null checks, empty checks)
    
    // Get the root node (or first child if root is null)
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.child(""); // Try to get any child
    }
    
    // If no children, try to get text from document itself or create a text node scenario
    pugi::xml_text text_node = root.text();
    
    // If text_node is empty/null, we can still call the functions to test default behavior
    // We also try to set content to fuzzed data to test parsing logic
    if (root) {
        root.text().set(xml_buffer.c_str());
        text_node = root.text();
    } else {
        // If no node exists, text_node remains default constructed (empty)
        // This tests the null/empty handling paths
    }

    // Invoke the target functions in the specified order
    
    // 1. pugi::xml_text::as_string
    // Fix: Pass a string literal or const char_t* directly, not a cast of a string to char_t
    const pugi::char_t* str_result = text_node.as_string(PUGIXML_TEXT("default"));
    (void)str_result; // Suppress unused variable warning
    
    // 2. pugi::xml_text::as_int
    int int_result = text_node.as_int(42);
    (void)int_result;
    
    // 3. pugi::xml_text::as_uint
    unsigned int uint_result = text_node.as_uint(43);
    (void)uint_result;
    
    // 4. pugi::xml_text::as_double
    double double_result = text_node.as_double(3.14);
    (void)double_result;
    
    // 5. pugi::xml_text::as_float
    float float_result = text_node.as_float(2.71f);
    (void)float_result;
    
    // 6. pugi::xml_text::as_bool
    bool bool_result = text_node.as_bool(true);
    (void)bool_result;
    
    // 7. pugi::xml_text::as_llong
    long long llong_result = text_node.as_llong(10000000000LL);
    (void)llong_result;
    
    // 8. pugi::xml_text::as_ullong
    unsigned long long ullong_result = text_node.as_ullong(20000000000ULL);
    (void)ullong_result;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Invoke the target functions with the fuzzed data
    invoke_target_functions(Data, Size);
    
    return 0;
}