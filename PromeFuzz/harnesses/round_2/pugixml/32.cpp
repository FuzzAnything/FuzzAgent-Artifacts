// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

static std::string extract_null_terminated_string(const uint8_t* data, size_t size, size_t& offset, size_t max_len = 100) {
    if (offset >= size) return "";
    
    size_t len = 0;
    while (offset + len < size && len < max_len && data[offset + len] != '\0') {
        ++len;
    }
    
    std::string result(reinterpret_cast<const char*>(data + offset), len);
    offset += len;
    
    // Skip the null terminator if present
    if (offset < size && data[offset] == '\0') {
        ++offset;
    }
    
    return result;
}

static long long extract_int64(const uint8_t* data, size_t size, size_t& offset) {
    if (offset + sizeof(long long) > size) return 0;
    
    long long value;
    std::memcpy(&value, data + offset, sizeof(long long));
    offset += sizeof(long long);
    return value;
}

static unsigned long long extract_uint64(const uint8_t* data, size_t size, size_t& offset) {
    if (offset + sizeof(unsigned long long) > size) return 0;
    
    unsigned long long value;
    std::memcpy(&value, data + offset, sizeof(unsigned long long));
    offset += sizeof(unsigned long long);
    return value;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Extract strings for node and attribute names
    std::string child_name = extract_null_terminated_string(Data, Size, offset);
    std::string attr_name1 = extract_null_terminated_string(Data, Size, offset);
    std::string attr_name2 = extract_null_terminated_string(Data, Size, offset);
    std::string attr_name3 = extract_null_terminated_string(Data, Size, offset);
    std::string attr_name4 = extract_null_terminated_string(Data, Size, offset);
    
    // Extract values for set_value calls
    long long value1 = extract_int64(Data, Size, offset);
    unsigned long long value2 = extract_uint64(Data, Size, offset);
    long long value3 = extract_int64(Data, Size, offset);
    unsigned long long value4 = extract_uint64(Data, Size, offset);
    
    // Create XML document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Add a child node to ensure we have something to work with
    pugi::xml_node child_node = root.append_child(child_name.empty() ? "default" : child_name.c_str());
    
    // 1. pugi::xml_node::child
    pugi::xml_node found_child = root.child(child_name.empty() ? "default" : child_name.c_str());
    
    // 2. pugi::xml_node::append_attribute (first call)
    pugi::xml_attribute attr1 = found_child.append_attribute(attr_name1.empty() ? "attr1" : attr_name1.c_str());
    
    // 3. pugi::xml_node::append_attribute (second call)
    pugi::xml_attribute attr2 = found_child.append_attribute(attr_name2.empty() ? "attr2" : attr_name2.c_str());
    
    // 4. pugi::xml_attribute::set_value (first attribute, both overloads)
    if (attr1) {
        attr1.set_value(value1);           // long long overload
        attr1.set_value(value2);           // unsigned long long overload
    }
    
    // 5. pugi::xml_node::append_attribute (third call)
    pugi::xml_attribute attr3 = found_child.append_attribute(attr_name3.empty() ? "attr3" : attr_name3.c_str());
    
    // 6. pugi::xml_node::append_attribute (fourth call)
    pugi::xml_attribute attr4 = found_child.append_attribute(attr_name4.empty() ? "attr4" : attr_name4.c_str());
    
    // 7. pugi::xml_attribute::set_value (fourth attribute, both overloads)
    if (attr4) {
        attr4.set_value(value3);           // long long overload
        attr4.set_value(value4);           // unsigned long long overload
    }
    
    return 0;
}