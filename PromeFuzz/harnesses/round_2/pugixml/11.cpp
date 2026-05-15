// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
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

static void invoke_functions(const uint8_t* data, size_t size) {
    // Create a document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // Use the fuzz data to generate parameter values
    // We'll use different portions of the input for different parameters
    size_t offset = 0;
    
    // Helper to get string_view from data
    auto get_string_view = [&](size_t len) -> std::string_view {
        if (offset + len > size) len = size - offset;
        const char* str_data = reinterpret_cast<const char*>(data + offset);
        offset += len;
        return std::string_view(str_data, len);
    };
    
    // Helper to get null-terminated string
    auto get_cstring = [&](size_t len) -> std::string {
        if (offset + len > size) len = size - offset;
        const char* str_data = reinterpret_cast<const char*>(data + offset);
        offset += len;
        return std::string(str_data, len);
    };
    
    // Helper to get numeric values
    auto get_numeric = [&]() -> float {
        if (offset + sizeof(float) > size) {
            offset = size;
            return 0.0f;
        }
        float value;
        memcpy(&value, data + offset, sizeof(float));
        offset += sizeof(float);
        return value;
    };
    
    // Reset offset for each function call to use fresh data
    size_t original_offset = 0;
    
    // Call sequence: append_attribute -> set_value (6 times)
    for (int i = 0; i < 6; i++) {
        // Reset offset for this iteration
        offset = original_offset;
        
        // First append_attribute
        pugi::xml_attribute attr1;
        if (i % 2 == 0) {
            // Use string_view_t overload
            std::string_view attr_name1 = get_string_view(16);
            attr1 = node.append_attribute(attr_name1);
        } else {
            // Use const char_t* overload
            std::string attr_name1_str = get_cstring(16);
            attr1 = node.append_attribute(attr_name1_str.c_str());
        }
        
        // First set_value (call all 5 overloads in sequence)
        if (attr1) {
            switch (i % 5) {
                case 0:
                    attr1.set_value(true);
                    break;
                case 1: {
                    std::string value_str = get_cstring(16);
                    attr1.set_value(value_str.c_str());
                    break;
                }
                case 2:
                    attr1.set_value(get_numeric());
                    break;
                case 3:
                    attr1.set_value(static_cast<double>(get_numeric()));
                    break;
                case 4: {
                    std::string_view value_sv = get_string_view(16);
                    attr1.set_value(value_sv);
                    break;
                }
            }
        }
        
        // Update original_offset for next iteration
        original_offset = offset;
        if (original_offset >= size) {
            original_offset = 0;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    invoke_functions(Data, Size);
    
    return 0;
}