// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::data at pugixml.cpp:7212:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    if (!result) {
        // If parsing fails, create a minimal document to still exercise APIs
        doc.append_child("root");
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // 1. pugi::xml_node::child
    // Create null-terminated string for child name
    char child_name[256] = {0};
    if (Size > 0) {
        size_t name_len = Size < 255 ? Size : 255;
        memcpy(child_name, Data, name_len);
        child_name[name_len] = '\0';
    } else {
        strcpy(child_name, "child");
    }
    
    pugi::xml_node child_node = root.child(child_name);
    
    // 2. pugi::xml_node::text
    pugi::xml_text text_obj = child_node.text();
    
    // 3. pugi::xml_text::get
    const char* text_content = text_obj.get();
    (void)text_content; // Use variable to avoid unused warning
    
    // 4. pugi::xml_text::set (first call)
    std::string_view set_value1 = "test_value";
    bool set_result1 = text_obj.set(set_value1);
    (void)set_result1; // Use variable to avoid unused warning
    
    // 5. pugi::xml_text::data
    pugi::xml_node data_node = text_obj.data();
    (void)data_node; // Use variable to avoid unused warning
    
    // 6. pugi::xml_text::set (second call)
    // Create string view from fuzzer data for more diverse testing
    std::string_view set_value2;
    if (Size > 0) {
        set_value2 = std::string_view(reinterpret_cast<const char*>(Data), 
                                     Size > 1024 ? 1024 : Size);
    } else {
        set_value2 = "another_value";
    }
    bool set_result2 = text_obj.set(set_value2);
    (void)set_result2; // Use variable to avoid unused warning
    
    // 7. pugi::xml_node::last_child
    pugi::xml_node last_child = child_node.last_child();
    (void)last_child; // Use variable to avoid unused warning
    
    return 0;
}