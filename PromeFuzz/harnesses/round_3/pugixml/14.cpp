// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
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
#include <string>
#include <string_view>

static void initialize_node(pugi::xml_node& node) {
    // Create a dummy document to get a valid node
    static pugi::xml_document doc;
    static bool initialized = false;
    
    if (!initialized) {
        // Create a root node
        node = doc.append_child("root");
        initialized = true;
    }
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize XML document and node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    
    // Extract strings from fuzzing data for attribute names and values
    // We'll use different portions of the input for different parameters
    size_t quarter = Size / 4;
    if (quarter == 0) quarter = 1;
    
    // Create string views from the input data
    std::string_view attr_name1(reinterpret_cast<const char*>(Data), 
                                std::min(quarter, Size));
    std::string_view attr_value1(reinterpret_cast<const char*>(Data + quarter), 
                                 std::min(quarter, Size - quarter));
    
    size_t third_quarter = 3 * quarter;
    if (third_quarter >= Size) third_quarter = Size - 1;
    
    std::string_view attr_name2(reinterpret_cast<const char*>(Data + 2*quarter), 
                                std::min(quarter, Size - 2*quarter));
    std::string_view attr_value2(reinterpret_cast<const char*>(Data + third_quarter), 
                                 std::min(quarter, Size - third_quarter));
    
    // Ensure null-terminated strings for const char* parameters
    std::string safe_name1(attr_name1.data(), attr_name1.length());
    std::string safe_value1(attr_value1.data(), attr_value1.length());
    std::string safe_name2(attr_name2.data(), attr_name2.length());
    std::string safe_value2(attr_value2.data(), attr_value2.length());
    
    // Extract numeric values from the beginning of data
    float float_val = 0.0f;
    double double_val = 0.0;
    bool bool_val = false;
    
    if (Size >= sizeof(float)) {
        float_val = *reinterpret_cast<const float*>(Data);
    }
    if (Size >= sizeof(double)) {
        double_val = *reinterpret_cast<const double*>(Data);
    }
    if (Size >= 1) {
        bool_val = Data[0] != 0;
    }
    
    try {
        // First pair: string_view append_attribute -> bool set_value
        pugi::xml_attribute attr1 = node.append_attribute(attr_name1);
        if (attr1) {
            attr1.set_value(bool_val);
        }
        
        // Second pair: const char* append_attribute -> string_view set_value
        pugi::xml_attribute attr2 = node.append_attribute(safe_name1.c_str());
        if (attr2) {
            attr2.set_value(attr_value1);
        }
        
        // Third pair: string_view append_attribute -> const char* set_value
        pugi::xml_attribute attr3 = node.append_attribute(attr_name2);
        if (attr3) {
            attr3.set_value(safe_value1.c_str());
        }
        
        // Fourth pair: const char* append_attribute -> float set_value
        pugi::xml_attribute attr4 = node.append_attribute(safe_name2.c_str());
        if (attr4) {
            attr4.set_value(float_val);
        }
        
        // Fifth pair: string_view append_attribute -> double set_value
        pugi::xml_attribute attr5 = node.append_attribute(attr_name1);
        if (attr5) {
            attr5.set_value(double_val);
        }
        
        // Sixth pair: const char* append_attribute -> bool set_value
        pugi::xml_attribute attr6 = node.append_attribute(safe_value2.c_str());
        if (attr6) {
            attr6.set_value(bool_val);
        }
        
        // Repeat the pattern for more coverage
        // Seventh pair: string_view append_attribute -> string_view set_value
        pugi::xml_attribute attr7 = node.append_attribute(attr_value1);
        if (attr7) {
            attr7.set_value(attr_value2);
        }
        
        // Eighth pair: const char* append_attribute -> const char* set_value
        pugi::xml_attribute attr8 = node.append_attribute(safe_value1.c_str());
        if (attr8) {
            attr8.set_value(safe_value2.c_str());
        }
        
        // Ninth pair: string_view append_attribute -> float set_value
        pugi::xml_attribute attr9 = node.append_attribute(attr_name2);
        if (attr9) {
            attr9.set_value(float_val);
        }
        
        // Tenth pair: const char* append_attribute -> double set_value
        pugi::xml_attribute attr10 = node.append_attribute(safe_name1.c_str());
        if (attr10) {
            attr10.set_value(double_val);
        }
        
        // Eleventh pair: string_view append_attribute -> bool set_value
        pugi::xml_attribute attr11 = node.append_attribute(attr_value2);
        if (attr11) {
            attr11.set_value(bool_val);
        }
        
        // Twelfth pair: const char* append_attribute -> string_view set_value
        pugi::xml_attribute attr12 = node.append_attribute(safe_name2.c_str());
        if (attr12) {
            attr12.set_value(attr_name1);
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}