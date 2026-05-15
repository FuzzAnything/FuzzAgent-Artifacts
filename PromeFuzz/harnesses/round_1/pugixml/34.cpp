// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

static void initialize_xml_document(pugi::xml_document& doc) {
    // Create a root node to work with
    doc.append_child("root");
}

static std::string_view extract_string(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) {
        return "";
    }
    
    const char* str_start = reinterpret_cast<const char*>(data + offset);
    size_t remaining = size - offset;
    
    // Find null terminator or use remaining bytes
    size_t str_len = 0;
    while (str_len < remaining && str_start[str_len] != '\0') {
        str_len++;
    }
    
    offset += str_len + 1; // Skip string and null terminator
    if (str_len == 0) {
        return "";
    }
    
    return std::string_view(str_start, str_len);
}

static uint64_t extract_uint64(const uint8_t* data, size_t size, size_t& offset) {
    if (offset + sizeof(uint64_t) > size) {
        return 0;
    }
    
    uint64_t value = 0;
    std::memcpy(&value, data + offset, sizeof(uint64_t));
    offset += sizeof(uint64_t);
    return value;
}

static int64_t extract_int64(const uint8_t* data, size_t size, size_t& offset) {
    if (offset + sizeof(int64_t) > size) {
        return 0;
    }
    
    int64_t value = 0;
    std::memcpy(&value, data + offset, sizeof(int64_t));
    offset += sizeof(int64_t);
    return value;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Extract string parameters from fuzzer input
    std::string_view child_name = extract_string(Data, Size, offset);
    std::string_view append_child_name1 = extract_string(Data, Size, offset);
    std::string_view append_child_name2 = extract_string(Data, Size, offset);
    std::string_view append_child_name3 = extract_string(Data, Size, offset);
    std::string_view append_child_name4 = extract_string(Data, Size, offset);
    
    // Extract integer parameters for xml_text::set
    uint64_t set_value_ull1 = extract_uint64(Data, Size, offset);
    int64_t set_value_ll1 = extract_int64(Data, Size, offset);
    uint64_t set_value_ull2 = extract_uint64(Data, Size, offset);
    int64_t set_value_ll2 = extract_int64(Data, Size, offset);
    
    // Initialize XML document
    pugi::xml_document doc;
    initialize_xml_document(doc);
    
    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }
    
    try {
        // 1. pugi::xml_node::child
        pugi::xml_node child_node = root.child(child_name.data());
        
        // 2. pugi::xml_node::append_child
        pugi::xml_node appended_child1 = child_node.append_child(append_child_name1.data());
        
        // 3. pugi::xml_node::append_child
        pugi::xml_node appended_child2 = child_node.append_child(append_child_name2.data());
        
        // 4. pugi::xml_text::set (both overloads)
        if (appended_child1) {
            pugi::xml_text text1 = appended_child1.text();
            text1.set(set_value_ull1);  // unsigned long long overload
            text1.set(set_value_ll1);   // long long overload
        }
        
        // 5. pugi::xml_node::append_child
        pugi::xml_node appended_child3 = child_node.append_child(append_child_name3.data());
        
        // 6. pugi::xml_node::append_child
        pugi::xml_node appended_child4 = child_node.append_child(append_child_name4.data());
        
        // 7. pugi::xml_text::set (both overloads)
        if (appended_child3) {
            pugi::xml_text text3 = appended_child3.text();
            text3.set(set_value_ull2);  // unsigned long long overload
            text3.set(set_value_ll2);   // long long overload
        }
        
        // Additional exploration: try to access the text values
        if (appended_child1) {
            appended_child1.text().get();
        }
        if (appended_child3) {
            appended_child3.text().get();
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crash
    }
    
    return 0;
}