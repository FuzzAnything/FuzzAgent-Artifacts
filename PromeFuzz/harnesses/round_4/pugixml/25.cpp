// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

static void extract_strings(const uint8_t *data, size_t size, 
                           std::string& str1, std::string& str2,
                           std::string& str3, std::string& str4,
                           std::string& str5, std::string& str6,
                           std::string& str7, std::string& str8,
                           std::string& str9, std::string& str10) {
    // Helper to extract null-terminated strings from fuzzing input
    const char* ptr = reinterpret_cast<const char*>(data);
    const char* end = ptr + size;
    
    auto extract = [&](std::string& out) {
        if (ptr >= end) {
            out = "default";
            return;
        }
        const char* start = ptr;
        while (ptr < end && *ptr != '\0') {
            ++ptr;
        }
        out.assign(start, ptr - start);
        if (ptr < end) ++ptr; // Skip null terminator
    };
    
    extract(str1);
    extract(str2);
    extract(str3);
    extract(str4);
    extract(str5);
    extract(str6);
    extract(str7);
    extract(str8);
    extract(str9);
    extract(str10);
}

static int extract_int(const uint8_t *data, size_t size, size_t offset) {
    if (offset + sizeof(int) <= size) {
        int value;
        memcpy(&value, data + offset, sizeof(int));
        return value;
    }
    return 42; // Default value
}

static unsigned int extract_uint(const uint8_t *data, size_t size, size_t offset) {
    if (offset + sizeof(unsigned int) <= size) {
        unsigned int value;
        memcpy(&value, data + offset, sizeof(unsigned int));
        return value;
    }
    return 42u; // Default value
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Extract strings for node/attribute names and values
    std::string child_name, attr_name1, attr_value1_str, attr_name2, 
                attr_name3, attr_value3_str, attr_name4, attr_name5,
                dummy1, dummy2;
    
    extract_strings(Data, Size, child_name, attr_name1, attr_value1_str,
                   attr_name2, attr_name3, attr_value3_str, attr_name4,
                   attr_name5, dummy1, dummy2);
    
    // Extract integers for set_value overloads
    int int_val1 = extract_int(Data, Size, 0);
    unsigned int uint_val1 = extract_uint(Data, Size, sizeof(int));
    int int_val3 = extract_int(Data, Size, sizeof(int) + sizeof(unsigned int));
    unsigned int uint_val3 = extract_uint(Data, Size, 2 * (sizeof(int) + sizeof(unsigned int)));
    
    // Create XML document and nodes
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create a child node to search for
    pugi::xml_node child_node = root.append_child(child_name.c_str());
    
    // 1. pugi::xml_node::child
    pugi::xml_node found_node = root.child(child_name.c_str());
    
    // Use found_node for subsequent operations (or root if empty)
    pugi::xml_node target_node = found_node ? found_node : root;
    
    // 2. pugi::xml_node::append_attribute
    pugi::xml_attribute attr1 = target_node.append_attribute(attr_name1.c_str());
    
    // 3. pugi::xml_attribute::set_value (all 3 overloads)
    if (attr1) {
        attr1.set_value(attr_value1_str.c_str());
        attr1.set_value(int_val1);
        attr1.set_value(uint_val1);
    }
    
    // 4. pugi::xml_node::append_attribute
    pugi::xml_attribute attr2 = target_node.append_attribute(attr_name2.c_str());
    
    // 5. pugi::xml_node::append_attribute
    pugi::xml_attribute attr3 = target_node.append_attribute(attr_name3.c_str());
    
    // 6. pugi::xml_attribute::set_value (all 3 overloads)
    if (attr3) {
        attr3.set_value(attr_value3_str.c_str());
        attr3.set_value(int_val3);
        attr3.set_value(uint_val3);
    }
    
    // 7. pugi::xml_node::append_attribute
    pugi::xml_attribute attr4 = target_node.append_attribute(attr_name4.c_str());
    
    // 8. pugi::xml_node::append_attribute
    pugi::xml_attribute attr5 = target_node.append_attribute(attr_name5.c_str());
    
    return 0;
}