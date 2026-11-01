// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string>

static bool fuzz_append_attribute_and_set_value(pugi::xml_node& node, const uint8_t* data, size_t size, size_t& offset, bool use_string_view) {
    if (offset + 2 > size) return false;
    
    size_t name_len = data[offset] % (size > 10 ? 10 : size);
    size_t value_len = data[offset + 1] % (size > 10 ? 10 : size);
    offset += 2;
    
    if (offset + name_len + value_len > size) return false;
    
    std::string name_str(reinterpret_cast<const char*>(data + offset), name_len);
    std::string value_str(reinterpret_cast<const char*>(data + offset + name_len), value_len);
    offset += name_len + value_len;
    
    pugi::xml_attribute attr;
    
    if (use_string_view) {
        std::string_view name_sv(name_str);
        attr = node.append_attribute(name_sv);
    } else {
        attr = node.append_attribute(name_str.c_str());
    }
    
    if (!attr) return false;
    
    // Try different set_value overloads
    bool result = true;
    
    // set_value(const char_t*)
    result &= attr.set_value(value_str.c_str());
    
    // set_value(string_view_t)
    std::string_view value_sv(value_str);
    result &= attr.set_value(value_sv);
    
    // set_value(float)
    float f_val = static_cast<float>(data[offset % size]) / 10.0f;
    result &= attr.set_value(f_val);
    
    // set_value(double)
    double d_val = static_cast<double>(data[(offset + 1) % size]) / 10.0;
    result &= attr.set_value(d_val);
    
    // set_value(bool)
    bool b_val = (data[(offset + 2) % size] % 2) == 1;
    result &= attr.set_value(b_val);
    
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    if (!root) return 0;
    
    size_t offset = 0;
    
    // Perform 6 iterations of append_attribute + set_value as requested
    for (int i = 0; i < 6; ++i) {
        bool use_string_view = (Data[offset % Size] % 2) == 1;
        if (!fuzz_append_attribute_and_set_value(root, Data, Size, offset, use_string_view)) {
            break;
        }
    }
    
    return 0;
}