// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Create XML document and parse input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // 1. pugi::xml_node::child
    // Create null-terminated string for child name
    if (Size > 0) {
        std::string child_name(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)100));
        if (!child_name.empty() && child_name.back() != '\0') {
            child_name.push_back('\0');
        }
        pugi::xml_node child_node = root.child(child_name.c_str());
        
        // 2. pugi::xml_node::text
        pugi::xml_text text_obj = child_node.text();
        
        // 3. pugi::xml_text::get
        const char* text_content = text_obj.get();
        (void)text_content; // Prevent unused variable warning
        
        // 4. pugi::xml_text::set (first call)
        if (Size > 0) {
            std::string_view set_value(reinterpret_cast<const char*>(Data), 
                                      std::min(Size, (size_t)200));
            bool set_result = text_obj.set(set_value);
            (void)set_result; // Prevent unused variable warning
            
            // 5. pugi::xml_text::data
            pugi::xml_node data_node = text_obj.data();
            (void)data_node; // Prevent unused variable warning
            
            // 6. pugi::xml_text::set (second call)
            // Use different data for second set
            std::string_view set_value2("fuzz_test", 9);
            bool set_result2 = text_obj.set(set_value2);
            (void)set_result2; // Prevent unused variable warning
        }
        
        // 7. pugi::xml_node::last_child
        pugi::xml_node last_child = root.last_child();
        (void)last_child; // Prevent unused variable warning
    }
    
    return 0;
}