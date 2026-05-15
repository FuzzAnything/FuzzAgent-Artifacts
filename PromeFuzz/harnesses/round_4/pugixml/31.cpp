// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>

static std::string make_null_terminated(std::string_view sv) {
    return std::string(sv.data(), sv.length());
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some child nodes for testing
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    
    // Extract strings from fuzzer input
    // We'll use the input data to create various strings for testing
    size_t offset = 0;
    
    // First string for child name lookup
    std::string child_name;
    if (offset < Size) {
        size_t len = Data[offset] % (Size - offset);
        if (len > 0 && offset + len <= Size) {
            child_name = make_null_terminated(
                std::string_view(reinterpret_cast<const char*>(Data + offset), len));
        }
        offset += len + 1;
    }
    
    // Strings for attribute names and values
    std::string attr_name1, attr_name2, attr_name3, attr_name4;
    std::string attr_value1, attr_value2;
    
    // Read attribute names
    for (int i = 0; i < 4; i++) {
        if (offset < Size) {
            size_t len = Data[offset] % (Size - offset);
            if (len > 0 && offset + len <= Size) {
                std::string name = make_null_terminated(
                    std::string_view(reinterpret_cast<const char*>(Data + offset), len));
                switch(i) {
                    case 0: attr_name1 = name; break;
                    case 1: attr_name2 = name; break;
                    case 2: attr_name3 = name; break;
                    case 3: attr_name4 = name; break;
                }
            }
            offset += len + 1;
        }
    }
    
    // Read attribute values
    for (int i = 0; i < 2; i++) {
        if (offset < Size) {
            size_t len = Data[offset] % (Size - offset);
            if (len > 0 && offset + len <= Size) {
                std::string value = make_null_terminated(
                    std::string_view(reinterpret_cast<const char*>(Data + offset), len));
                switch(i) {
                    case 0: attr_value1 = value; break;
                    case 1: attr_value2 = value; break;
                }
            }
            offset += len + 1;
        }
    }
    
    // Extract integer values from remaining data
    unsigned long long ull_val = 0;
    long long ll_val = 0;
    unsigned long long ull_val2 = 0;
    long long ll_val2 = 0;
    
    if (Size - offset >= sizeof(unsigned long long)) {
        memcpy(&ull_val, Data + offset, sizeof(unsigned long long));
        offset += sizeof(unsigned long long);
    }
    if (Size - offset >= sizeof(long long)) {
        memcpy(&ll_val, Data + offset, sizeof(long long));
        offset += sizeof(long long);
    }
    if (Size - offset >= sizeof(unsigned long long)) {
        memcpy(&ull_val2, Data + offset, sizeof(unsigned long long));
        offset += sizeof(unsigned long long);
    }
    if (Size - offset >= sizeof(long long)) {
        memcpy(&ll_val2, Data + offset, sizeof(long long));
    }
    
    // 1. pugi::xml_node::child
    pugi::xml_node found_child = root.child(child_name.empty() ? "child1" : child_name.c_str());
    
    // 2. pugi::xml_node::append_attribute
    pugi::xml_attribute attr1 = found_child.append_attribute(
        attr_name1.empty() ? "attr1" : attr_name1.c_str());
    
    // 3. pugi::xml_node::append_attribute
    pugi::xml_attribute attr2 = found_child.append_attribute(
        attr_name2.empty() ? "attr2" : attr_name2.c_str());
    
    // 4. pugi::xml_attribute::set_value (both overloads)
    if (attr1) {
        attr1.set_value(ull_val);
        attr1.set_value(ll_val);
    }
    
    // 5. pugi::xml_node::append_attribute
    pugi::xml_attribute attr3 = found_child.append_attribute(
        attr_name3.empty() ? "attr3" : attr_name3.c_str());
    
    // 6. pugi::xml_node::append_attribute
    pugi::xml_attribute attr4 = found_child.append_attribute(
        attr_name4.empty() ? "attr4" : attr_name4.c_str());
    
    // 7. pugi::xml_attribute::set_value (both overloads)
    if (attr3) {
        attr3.set_value(ull_val2);
        attr3.set_value(ll_val2);
    }
    
    return 0;
}