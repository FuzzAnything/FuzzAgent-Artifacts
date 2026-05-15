// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
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

static void fuzz_sequence(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size < 100) return; // Ensure enough data for operations
    
    // Create strings from fuzzer data for node/attribute names
    std::string_view child_name_data(reinterpret_cast<const char*>(data), 20);
    std::string child_name(child_name_data.data(), child_name_data.length());
    
    std::string_view attr_name_data(reinterpret_cast<const char*>(data + 20), 20);
    std::string attr_name(attr_name_data.data(), attr_name_data.length());
    
    // Extract numeric values from fuzzer data
    float float_val1, float_val2;
    double double_val1, double_val2, double_val3;
    int precision1, precision2, precision3, precision4, precision5;
    
    if (size >= 100 + sizeof(float) + sizeof(double) + 5*sizeof(int)) {
        memcpy(&float_val1, data + 40, sizeof(float));
        memcpy(&double_val1, data + 44, sizeof(double));
        memcpy(&precision1, data + 52, sizeof(int));
        memcpy(&double_val2, data + 56, sizeof(double));
        memcpy(&precision2, data + 64, sizeof(int));
        memcpy(&double_val3, data + 68, sizeof(double));
        memcpy(&precision3, data + 76, sizeof(int));
        memcpy(&float_val2, data + 80, sizeof(float));
        memcpy(&precision4, data + 84, sizeof(int));
        memcpy(&precision5, data + 88, sizeof(int));
    } else {
        // Default values if not enough data
        float_val1 = 1.0f; float_val2 = 2.0f;
        double_val1 = 1.0; double_val2 = 2.0; double_val3 = 3.0;
        precision1 = precision2 = precision3 = precision4 = precision5 = 2;
    }
    
    // 1. append_child
    pugi::xml_node child = node.append_child(child_name.c_str());
    if (!child) return;
    
    // 2. append_attribute
    pugi::xml_attribute attr = child.append_attribute(attr_name.c_str());
    if (!attr) return;
    
    // 3. set_value (first - double)
    attr.set_value(double_val1, precision1);
    
    // 4. as_double
    attr.as_double();
    
    // 5. set_value (second - float)
    attr.set_value(float_val1, precision2);
    
    // 6. as_double
    attr.as_double();
    
    // 7. set_value (third - double)
    attr.set_value(double_val2, precision3);
    
    // 8. as_double
    attr.as_double();
    
    // 9. text()
    pugi::xml_text text1 = child.text();
    
    // 10. set() - double overload
    text1.set(double_val3, precision4);
    
    // 11. text()
    child.text();
    
    // 12. text()
    child.text();
    
    // 13. set() - float overload
    text1.set(float_val2, precision5);
    
    // 14. text()
    child.text();
    
    // 15. text()
    child.text();
    
    // 16. set() - double overload again
    text1.set(double_val1, precision1);
    
    // 17. text()
    child.text();
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
    static pugi::xml_document doc;
    
    // Reset document for each fuzzing iteration
    doc.reset();
    
    // Create a root node
    pugi::xml_node root = doc.append_child("root");
    if (!root) return 0;
    
    // Execute the fuzzing sequence
    fuzz_sequence(root, data, size);
    
    return 0;
}