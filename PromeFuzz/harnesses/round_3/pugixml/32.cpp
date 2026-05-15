// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

static std::string make_null_terminated(std::string_view sv) {
    return std::string(sv.data(), sv.length());
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a document and root node to work with
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Use the fuzzer data to create strings for node/attribute names
    // We'll split the input into chunks for different parameters
    size_t offset = 0;
    
    // Helper to get a string view from the input
    auto get_string_view = [&](size_t len) -> std::string_view {
        if (offset >= Size) return "";
        size_t actual_len = std::min(len, Size - offset);
        std::string_view sv(reinterpret_cast<const char*>(Data + offset), actual_len);
        offset += actual_len;
        return sv;
    };
    
    // 1. pugi::xml_node::child
    // First create a child node so we have something to find
    std::string_view child_name_sv = get_string_view(16);
    std::string child_name = make_null_terminated(child_name_sv);
    if (!child_name.empty()) {
        root.append_child(child_name.c_str());
    }
    
    // Now call child() to find it
    pugi::xml_node found_child = root.child(child_name.c_str());
    
    // Use found_child if valid, otherwise use root
    pugi::xml_node target_node = found_child ? found_child : root;
    
    // 2. First pugi::xml_node::append_attribute
    std::string_view attr_name1_sv = get_string_view(16);
    std::string attr_name1 = make_null_terminated(attr_name1_sv);
    pugi::xml_attribute attr1;
    if (!attr_name1.empty()) {
        attr1 = target_node.append_attribute(attr_name1.c_str());
    }
    
    // 3. Second pugi::xml_node::append_attribute
    std::string_view attr_name2_sv = get_string_view(16);
    std::string attr_name2 = make_null_terminated(attr_name2_sv);
    pugi::xml_attribute attr2;
    if (!attr_name2.empty()) {
        attr2 = target_node.append_attribute(attr_name2.c_str());
    }
    
    // 4. pugi::xml_attribute::set_value (call both overloads)
    if (attr1) {
        // Use remaining bytes to generate numeric values
        if (Size - offset >= sizeof(long long)) {
            long long ll_val;
            memcpy(&ll_val, Data + offset, sizeof(long long));
            offset += sizeof(long long);
            attr1.set_value(ll_val);
        }
        
        if (Size - offset >= sizeof(unsigned long long)) {
            unsigned long long ull_val;
            memcpy(&ull_val, Data + offset, sizeof(unsigned long long));
            offset += sizeof(unsigned long long);
            attr1.set_value(ull_val);
        }
    }
    
    // 5. Third pugi::xml_node::append_attribute
    std::string_view attr_name3_sv = get_string_view(16);
    std::string attr_name3 = make_null_terminated(attr_name3_sv);
    pugi::xml_attribute attr3;
    if (!attr_name3.empty()) {
        attr3 = target_node.append_attribute(attr_name3.c_str());
    }
    
    // 6. Fourth pugi::xml_node::append_attribute
    std::string_view attr_name4_sv = get_string_view(16);
    std::string attr_name4 = make_null_terminated(attr_name4_sv);
    pugi::xml_attribute attr4;
    if (!attr_name4.empty()) {
        attr4 = target_node.append_attribute(attr_name4.c_str());
    }
    
    // 7. pugi::xml_attribute::set_value (call both overloads)
    if (attr3) {
        // Use any remaining bytes for numeric values
        if (Size - offset >= sizeof(long long)) {
            long long ll_val;
            memcpy(&ll_val, Data + offset, sizeof(long long));
            offset += sizeof(long long);
            attr3.set_value(ll_val);
        }
        
        if (Size - offset >= sizeof(unsigned long long)) {
            unsigned long long ull_val;
            memcpy(&ull_val, Data + offset, sizeof(unsigned long long));
            offset += sizeof(unsigned long long);
            attr3.set_value(ull_val);
        }
    }
    
    return 0;
}