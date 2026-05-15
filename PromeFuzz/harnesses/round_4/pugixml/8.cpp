// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Create document and node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // If we have no data, just return
    if (Size == 0) {
        return 0;
    }
    
    // Create attribute names and values from data
    // We'll use small fixed-size chunks to avoid memory issues
    const size_t MAX_NAME_LEN = 256;
    const size_t MAX_VALUE_LEN = 256;
    
    // Create 6 attribute names and values
    std::string attr_name_strings[6];
    std::string attr_value_strings[6];
    
    // Extract names and values safely
    for (int i = 0; i < 6; i++) {
        // Calculate safe offsets
        size_t name_offset = (i * 17) % Size;  // Use prime multiplication for dispersion
        size_t name_size = std::min(MAX_NAME_LEN, Size - name_offset);
        if (name_size > 0) {
            attr_name_strings[i] = std::string(
                reinterpret_cast<const char*>(Data + name_offset), 
                name_size
            );
        } else {
            attr_name_strings[i] = "attr";
        }
        
        // Calculate value offsets
        size_t value_offset = ((i + 7) * 23) % Size;  // Different prime for values
        size_t value_size = std::min(MAX_VALUE_LEN, Size - value_offset);
        if (value_size > 0) {
            attr_value_strings[i] = std::string(
                reinterpret_cast<const char*>(Data + value_offset), 
                value_size
            );
        } else {
            attr_value_strings[i] = "value";
        }
    }
    
    // Create string views
    std::string_view attr_name_views[6];
    std::string_view attr_value_views[6];
    
    for (int i = 0; i < 6; i++) {
        attr_name_views[i] = std::string_view(attr_name_strings[i]);
        attr_value_views[i] = std::string_view(attr_value_strings[i]);
    }
    
    // Get C-string pointers
    const char* attr_names[6];
    const char* attr_values[6];
    
    for (int i = 0; i < 6; i++) {
        attr_names[i] = attr_name_strings[i].c_str();
        attr_values[i] = attr_value_strings[i].c_str();
    }
    
    // Create numeric values safely
    bool bool_val = false;
    float float_val = 1.0f;
    double double_val = 2.0;
    
    // Safely extract numeric values if we have enough data
    if (Size >= sizeof(float)) {
        // Use memcpy to avoid alignment issues
        float temp_float;
        memcpy(&temp_float, Data, sizeof(float));
        float_val = temp_float;
    }
    
    if (Size >= sizeof(double)) {
        double temp_double;
        memcpy(&temp_double, Data, sizeof(double));
        double_val = temp_double;
    }
    
    if (Size >= 1) {
        bool_val = (Data[0] & 1) != 0;
    }
    
    // Now call the functions in the specified order
    // Each pair: append_attribute -> set_value
    
    // Pair 1
    pugi::xml_attribute attr1 = node.append_attribute(attr_names[0]);  // const char* overload
    if (attr1) {
        attr1.set_value(bool_val);  // bool overload
    }
    
    // Pair 2
    pugi::xml_attribute attr2 = node.append_attribute(attr_name_views[1]);  // string_view overload
    if (attr2) {
        attr2.set_value(float_val);  // float overload
    }
    
    // Pair 3
    pugi::xml_attribute attr3 = node.append_attribute(attr_names[2]);  // const char* overload
    if (attr3) {
        attr3.set_value(attr_value_views[0]);  // string_view overload
    }
    
    // Pair 4
    pugi::xml_attribute attr4 = node.append_attribute(attr_name_views[3]);  // string_view overload
    if (attr4) {
        attr4.set_value(double_val);  // double overload
    }
    
    // Pair 5
    pugi::xml_attribute attr5 = node.append_attribute(attr_names[4]);  // const char* overload
    if (attr5) {
        attr5.set_value(attr_values[1]);  // const char* overload
    }
    
    // Pair 6
    pugi::xml_attribute attr6 = node.append_attribute(attr_name_views[5]);  // string_view overload
    if (attr6) {
        // Use bool for the 6th set_value
        attr6.set_value(bool_val);  // bool overload
    }
    
    // Cleanup happens automatically when doc goes out of scope
    
    return 0;
}