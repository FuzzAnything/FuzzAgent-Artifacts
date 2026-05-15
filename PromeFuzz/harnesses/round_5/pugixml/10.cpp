// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Create XML document and node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("root");
    
    // Use the fuzzer input for attribute names and values
    // We'll create string views from the input data
    std::string_view input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Cycle through the sequence 6 times (total 12 function calls)
    for (int i = 0; i < 6; ++i) {
        // First append_attribute call
        pugi::xml_attribute attr1;
        if (i % 2 == 0) {
            // Use string_view_t overload
            std::string attr_name1 = "attr" + std::to_string(i*2);
            attr1 = node.append_attribute(std::string_view(attr_name1));
        } else {
            // Use const char_t* overload
            std::string attr_name1 = "attr" + std::to_string(i*2);
            attr1 = node.append_attribute(attr_name1.c_str());
        }
        
        // First set_value call with one of the 5 overloads
        if (attr1) {
            switch (i % 5) {
                case 0:
                    attr1.set_value(std::string_view(input_str));
                    break;
                case 1: {
                    std::string str_val(input_str.data(), input_str.length());
                    attr1.set_value(str_val.c_str());
                    break;
                }
                case 2: {
                    float float_val = 0.0f;
                    if (Size >= sizeof(float)) {
                        memcpy(&float_val, Data, sizeof(float));
                    }
                    attr1.set_value(float_val);
                    break;
                }
                case 3: {
                    bool bool_val = (Size > 0 && Data[0] != 0);
                    attr1.set_value(bool_val);
                    break;
                }
                case 4: {
                    double double_val = 0.0;
                    if (Size >= sizeof(double)) {
                        memcpy(&double_val, Data, sizeof(double));
                    }
                    attr1.set_value(double_val);
                    break;
                }
            }
        }
        
        // Second append_attribute call
        pugi::xml_attribute attr2;
        if ((i + 1) % 2 == 0) {
            // Use string_view_t overload
            std::string attr_name2 = "attr" + std::to_string(i*2 + 1);
            attr2 = node.append_attribute(std::string_view(attr_name2));
        } else {
            // Use const char_t* overload
            std::string attr_name2 = "attr" + std::to_string(i*2 + 1);
            attr2 = node.append_attribute(attr_name2.c_str());
        }
        
        // Second set_value call with a different overload
        if (attr2) {
            switch ((i + 1) % 5) {
                case 0:
                    attr2.set_value(std::string_view(input_str));
                    break;
                case 1: {
                    std::string str_val(input_str.data(), input_str.length());
                    attr2.set_value(str_val.c_str());
                    break;
                }
                case 2: {
                    float float_val = 0.0f;
                    if (Size >= sizeof(float)) {
                        memcpy(&float_val, Data, sizeof(float));
                    }
                    attr2.set_value(float_val);
                    break;
                }
                case 3: {
                    bool bool_val = (Size > 0 && Data[0] != 0);
                    attr2.set_value(bool_val);
                    break;
                }
                case 4: {
                    double double_val = 0.0;
                    if (Size >= sizeof(double)) {
                        memcpy(&double_val, Data, sizeof(double));
                    }
                    attr2.set_value(double_val);
                    break;
                }
            }
        }
    }
    
    return 0;
}