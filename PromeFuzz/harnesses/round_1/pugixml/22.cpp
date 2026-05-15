// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, we can still test with empty document
    if (!result) {
        // Create minimal valid XML structure to test APIs
        static const char* minimal_xml = "<root><child attr1=\"val1\" attr2=\"val2\"/></root>";
        result = doc.load_string(minimal_xml);
        if (!result) {
            return 0; // Shouldn't happen
        }
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare string views for function calls
    // We'll use parts of the input data to create strings
    // Ensure we don't read beyond bounds
    size_t max_str_len = Size > 0 ? Size : 1;
    
    // Create string views from input data
    // Use modulo to ensure we stay within bounds
    std::string_view str1, str2, str3, str4, str5, str6, str7;
    
    if (Size > 0) {
        // Create null-terminated strings from input data
        // We'll use the first part of input for first string
        size_t str1_len = Size > 10 ? 10 : Size;
        static char buf1[11];
        memcpy(buf1, Data, str1_len);
        buf1[str1_len] = '\0';
        str1 = std::string_view(buf1, str1_len);
        
        // For subsequent strings, use different parts of input
        if (Size > 20) {
            size_t offset = Size / 4;
            size_t str2_len = Size > offset + 10 ? 10 : Size - offset;
            static char buf2[11];
            memcpy(buf2, Data + offset, str2_len);
            buf2[str2_len] = '\0';
            str2 = std::string_view(buf2, str2_len);
            
            offset = Size / 2;
            size_t str3_len = Size > offset + 10 ? 10 : Size - offset;
            static char buf3[11];
            memcpy(buf3, Data + offset, str3_len);
            buf3[str3_len] = '\0';
            str3 = std::string_view(buf3, str3_len);
            
            offset = 3 * Size / 4;
            size_t str4_len = Size > offset + 10 ? 10 : Size - offset;
            static char buf4[11];
            memcpy(buf4, Data + offset, str4_len);
            buf4[str4_len] = '\0';
            str4 = std::string_view(buf4, str4_len);
            
            // Use fixed strings for remaining calls to ensure coverage
            str5 = "attr1";
            str6 = "attr2";
            str7 = "child";
        } else {
            // Not enough data, use fixed strings
            str2 = "attr1";
            str3 = "attr2";
            str4 = "attr3";
            str5 = "attr4";
            str6 = "attr5";
            str7 = "child";
        }
    } else {
        // No input data, use fixed strings
        str1 = "attr1";
        str2 = "attr2";
        str3 = "attr3";
        str4 = "attr4";
        str5 = "attr5";
        str6 = "attr6";
        str7 = "child";
    }
    
    // Call functions in specified order
    try {
        // 1. First attribute call
        pugi::xml_attribute attr1 = root.attribute(str1);
        
        // 2. Child call
        pugi::xml_node child = root.child(str7);
        
        // 3. Second attribute call (on root)
        pugi::xml_attribute attr2 = root.attribute(str2);
        
        // 4. Third attribute call (on root)
        pugi::xml_attribute attr3 = root.attribute(str3);
        
        // 5. Fourth attribute call (on root)
        pugi::xml_attribute attr4 = root.attribute(str4);
        
        // 6. Last attribute call (on root)
        pugi::xml_attribute last_attr = root.last_attribute();
        
        // 7. Fifth attribute call (on root)
        pugi::xml_attribute attr5 = root.attribute(str5);
        
        // 8. Sixth attribute call (on root)
        pugi::xml_attribute attr6 = root.attribute(str6);
        
        // Also test on child node if it exists
        if (child) {
            pugi::xml_attribute child_attr = child.attribute(str1);
            pugi::xml_attribute child_last_attr = child.last_attribute();
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crash
    }
    
    return 0;
}