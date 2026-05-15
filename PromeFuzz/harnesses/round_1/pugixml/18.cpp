// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
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
#include <algorithm>

static void safe_string_view(std::string_view& sv, const uint8_t* data, size_t size, size_t offset) {
    if (offset < size) {
        size_t max_len = std::min(size - offset, static_cast<size_t>(100));
        sv = std::string_view(reinterpret_cast<const char*>(data + offset), max_len);
    } else {
        sv = std::string_view("default", 7);
    }
}

static int extract_int(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(int) <= size) {
        int value;
        std::memcpy(&value, data + offset, sizeof(int));
        return value;
    }
    return 0;
}

static float extract_float(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(float) <= size) {
        float value;
        std::memcpy(&value, data + offset, sizeof(float));
        return value;
    }
    return 0.0f;
}

static unsigned int extract_uint(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(unsigned int) <= size) {
        unsigned int value;
        std::memcpy(&value, data + offset, sizeof(unsigned int));
        return value;
    }
    return 0;
}

static double extract_double(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(double) <= size) {
        double value;
        std::memcpy(&value, data + offset, sizeof(double));
        return value;
    }
    return 0.0;
}

static bool extract_bool(const uint8_t* data, size_t size, size_t offset) {
    if (offset < size) {
        return data[offset] != 0;
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* Data, size_t Size) {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    if (!root) {
        return 0;
    }
    
    size_t offset = 0;
    
    // First append_child
    std::string_view name1;
    safe_string_view(name1, Data, Size, offset);
    offset = (offset + 1) % (Size + 1);
    std::string name1_str(name1);
    pugi::xml_node node1 = root.append_child(name1_str.c_str());
    
    // Second append_child
    std::string_view name2;
    safe_string_view(name2, Data, Size, offset);
    offset = (offset + 1) % (Size + 1);
    std::string name2_str(name2);
    pugi::xml_node node2 = node1.append_child(name2_str.c_str());
    
    if (node2) {
        pugi::xml_text text2 = node2.text();
        // Call all 5 overloads of set
        text2.set(extract_int(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text2.set(extract_float(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text2.set(extract_uint(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text2.set(extract_double(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text2.set(extract_bool(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
    }
    
    // Third append_child
    std::string_view name3;
    safe_string_view(name3, Data, Size, offset);
    offset = (offset + 1) % (Size + 1);
    std::string name3_str(name3);
    pugi::xml_node node3 = node2.append_child(name3_str.c_str());
    
    // Fourth append_child
    std::string_view name4;
    safe_string_view(name4, Data, Size, offset);
    offset = (offset + 1) % (Size + 1);
    std::string name4_str(name4);
    pugi::xml_node node4 = node3.append_child(name4_str.c_str());
    
    if (node4) {
        pugi::xml_text text4 = node4.text();
        // Call all 5 overloads of set
        text4.set(extract_int(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text4.set(extract_float(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text4.set(extract_uint(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text4.set(extract_double(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text4.set(extract_bool(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
    }
    
    // Fifth append_child
    std::string_view name5;
    safe_string_view(name5, Data, Size, offset);
    offset = (offset + 1) % (Size + 1);
    std::string name5_str(name5);
    pugi::xml_node node5 = node4.append_child(name5_str.c_str());
    
    if (node5) {
        pugi::xml_text text5 = node5.text();
        // Call all 5 overloads of set
        text5.set(extract_int(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text5.set(extract_float(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text5.set(extract_uint(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text5.set(extract_double(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text5.set(extract_bool(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
    }
    
    // Sixth append_child
    std::string_view name6;
    safe_string_view(name6, Data, Size, offset);
    offset = (offset + 1) % (Size + 1);
    std::string name6_str(name6);
    pugi::xml_node node6 = node5.append_child(name6_str.c_str());
    
    if (node6) {
        pugi::xml_text text6 = node6.text();
        // Call all 5 overloads of set
        text6.set(extract_int(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text6.set(extract_float(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text6.set(extract_uint(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text6.set(extract_double(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text6.set(extract_bool(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
    }
    
    // Seventh append_child
    std::string_view name7;
    safe_string_view(name7, Data, Size, offset);
    offset = (offset + 1) % (Size + 1);
    std::string name7_str(name7);
    pugi::xml_node node7 = node6.append_child(name7_str.c_str());
    
    if (node7) {
        pugi::xml_text text7 = node7.text();
        // Call all 5 overloads of set
        text7.set(extract_int(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text7.set(extract_float(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text7.set(extract_uint(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text7.set(extract_double(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
        
        text7.set(extract_bool(Data, Size, offset));
        offset = (offset + 1) % (Size + 1);
    }
    
    return 0;
}