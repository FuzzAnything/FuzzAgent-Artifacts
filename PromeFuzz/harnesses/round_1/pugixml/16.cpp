// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::data at pugixml.cpp:7212:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from fuzzer input for child name
    static char child_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    if (name_len > 0) {
        memcpy(child_name, Data, name_len);
    }
    child_name[name_len] = '\0';
    
    // Create a string view for set operations
    std::string_view set_value(reinterpret_cast<const char*>(Data), Size);
    
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing failed, create a simple document to work with
    if (!result) {
        const char* simple_xml = "<root><child>text</child></root>";
        doc.load_string(simple_xml);
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // 1. Call child() with the fuzzer-provided name
    pugi::xml_node child_node = root.child(child_name);
    
    // 2. Call text() on the child node
    pugi::xml_text text_obj = child_node.text();
    
    // 3. Call get() on the text object
    const char* text_content = text_obj.get();
    
    // 4. Call set() on the text object with fuzzer data
    bool set_result1 = text_obj.set(set_value);
    
    // 5. Call data() on the text object
    pugi::xml_node data_node = text_obj.data();
    
    // 6. Call set() again with a modified value (append "modified")
    std::string modified_value = std::string(set_value) + "modified";
    bool set_result2 = text_obj.set(modified_value.c_str());
    
    // 7. Call last_child() on the root node
    pugi::xml_node last_child = root.last_child();
    
    // Clean up - nothing specific needed for pugixml
    
    return 0;
}