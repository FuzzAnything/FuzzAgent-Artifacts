// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5554:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Use the fuzzer input to create deterministic but varied inputs
    // We'll use the data to generate strings and numeric values
    
    if (Size < 1) return 0;
    
    // Create document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create a child node using fuzzer data for name
    std::string child_name;
    if (Size >= 10) {
        child_name = std::string(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)10));
    } else {
        child_name = "child";
    }
    
    // 1. append_child
    pugi::xml_node child = root.append_child(child_name.c_str());
    if (!child) return 0;
    
    // Create attribute name from fuzzer data
    std::string attr_name1;
    if (Size >= 20) {
        attr_name1 = std::string(reinterpret_cast<const char*>(Data + 10), std::min(Size - 10, (size_t)10));
    } else {
        attr_name1 = "attr1";
    }
    
    // 2. append_attribute
    pugi::xml_attribute attr1 = child.append_attribute(attr_name1.c_str());
    if (!attr1) return 0;
    
    // Generate numeric values from fuzzer data
    double dbl_val1 = 0.0;
    float flt_val1 = 0.0f;
    int precision1 = 6;
    
    if (Size >= 28) {
        // Use bytes 20-27 for double
        memcpy(&dbl_val1, Data + 20, sizeof(double));
        // Use byte 28 for float (but need 4 bytes)
        if (Size >= 32) {
            memcpy(&flt_val1, Data + 28, sizeof(float));
        }
    }
    
    // 3. set_value (double overload)
    attr1.set_value(dbl_val1, precision1);
    
    // 4. as_double
    double result1 = attr1.as_double();
    (void)result1; // Prevent unused variable warning
    
    // 5. set_value (float overload)
    attr1.set_value(flt_val1, precision1);
    
    // 6. as_double
    double result2 = attr1.as_double();
    (void)result2;
    
    // Generate more values for second set_value call
    double dbl_val2 = 1.0;
    float flt_val2 = 1.0f;
    
    if (Size >= 40) {
        if (Size >= 48) {
            memcpy(&dbl_val2, Data + 32, sizeof(double));
        }
        if (Size >= 52) {
            memcpy(&flt_val2, Data + 40, sizeof(float));
        }
    }
    
    // 7. set_value (double overload again)
    attr1.set_value(dbl_val2, precision1);
    
    // 8. as_double
    double result3 = attr1.as_double();
    (void)result3;
    
    // 9. text
    pugi::xml_text text1 = child.text();
    
    // Generate values for text set calls
    double text_dbl1 = 2.0;
    float text_flt1 = 2.0f;
    int text_precision1 = 4;
    
    if (Size >= 60) {
        if (Size >= 68) {
            memcpy(&text_dbl1, Data + 52, sizeof(double));
        }
        if (Size >= 72) {
            memcpy(&text_flt1, Data + 60, sizeof(float));
        }
    }
    
    // 10. set (double overload)
    text1.set(text_dbl1, text_precision1);
    
    // 11. text
    pugi::xml_text text2 = child.text();
    
    // 12. text
    pugi::xml_text text3 = child.text();
    
    // Generate more values for second text set
    double text_dbl2 = 3.0;
    float text_flt2 = 3.0f;
    int text_precision2 = 2;
    
    if (Size >= 80) {
        if (Size >= 88) {
            memcpy(&text_dbl2, Data + 72, sizeof(double));
        }
        if (Size >= 92) {
            memcpy(&text_flt2, Data + 80, sizeof(float));
        }
    }
    
    // 13. set (float overload)
    text2.set(text_flt2, text_precision2);
    
    // 14. text
    pugi::xml_text text4 = child.text();
    
    // 15. text
    pugi::xml_text text5 = child.text();
    
    // Generate more values for third text set
    double text_dbl3 = 4.0;
    float text_flt3 = 4.0f;
    int text_precision3 = 8;
    
    if (Size >= 100) {
        if (Size >= 108) {
            memcpy(&text_dbl3, Data + 92, sizeof(double));
        }
        if (Size >= 112) {
            memcpy(&text_flt3, Data + 100, sizeof(float));
        }
    }
    
    // 16. set (double overload again)
    text4.set(text_dbl3, text_precision3);
    
    // 17. text
    pugi::xml_text text6 = child.text();
    
    return 0;
}