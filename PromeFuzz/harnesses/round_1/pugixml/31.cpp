// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    if (!result) {
        return 0;
    }

    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Create string_view for child name (ensure null-terminated)
    std::string_view child_name_sv(reinterpret_cast<const char*>(Data), Size > 100 ? 100 : Size);
    std::string child_name_str(child_name_sv.data(), child_name_sv.length());
    
    // 1. Call child()
    pugi::xml_node child_node = root.child(child_name_str.c_str());
    
    // Use child_node if valid, otherwise use root
    pugi::xml_node target_node = child_node ? child_node : root;

    // Create attribute names from fuzzer data
    size_t offset = 0;
    const char* attr_name1 = "attr1";
    const char* attr_name2 = "attr2";
    const char* attr_name3 = "attr3";
    const char* attr_name4 = "attr4";
    
    // Use fuzzer data for attribute names if we have enough data
    if (Size >= 4) {
        // Create small strings from fuzzer data for attribute names
        char name1[10], name2[10], name3[10], name4[10];
        
        // First attribute name
        size_t len1 = (Data[0] % 8) + 1;
        for (size_t i = 0; i < len1 && i < 9 && offset < Size; i++) {
            name1[i] = static_cast<char>((Data[offset++] % 26) + 'a');
        }
        name1[len1 < 9 ? len1 : 8] = '\0';
        attr_name1 = name1;
        
        // Second attribute name
        size_t len2 = (Data[1] % 8) + 1;
        for (size_t i = 0; i < len2 && i < 9 && offset < Size; i++) {
            name2[i] = static_cast<char>((Data[offset++] % 26) + 'a');
        }
        name2[len2 < 9 ? len2 : 8] = '\0';
        attr_name2 = name2;
        
        // Third attribute name
        size_t len3 = (Data[2] % 8) + 1;
        for (size_t i = 0; i < len3 && i < 9 && offset < Size; i++) {
            name3[i] = static_cast<char>((Data[offset++] % 26) + 'a');
        }
        name3[len3 < 9 ? len3 : 8] = '\0';
        attr_name3 = name3;
        
        // Fourth attribute name
        size_t len4 = (Data[3] % 8) + 1;
        for (size_t i = 0; i < len4 && i < 9 && offset < Size; i++) {
            name4[i] = static_cast<char>((Data[offset++] % 26) + 'a');
        }
        name4[len4 < 9 ? len4 : 8] = '\0';
        attr_name4 = name4;
    }

    // 2. First append_attribute
    pugi::xml_attribute attr1 = target_node.append_attribute(attr_name1);
    
    // 3. Second append_attribute
    pugi::xml_attribute attr2 = target_node.append_attribute(attr_name2);
    
    // 4. First set_value calls (both overloads)
    if (attr1) {
        // Extract numeric values from fuzzer data
        long long ll_value = 0;
        unsigned long long ull_value = 0;
        
        if (Size >= 8) {
            memcpy(&ll_value, Data, sizeof(ll_value));
            memcpy(&ull_value, Data, sizeof(ull_value));
        } else if (Size > 0) {
            ll_value = static_cast<long long>(Data[0]);
            ull_value = static_cast<unsigned long long>(Data[0]);
        }
        
        attr1.set_value(ll_value);
        attr1.set_value(ull_value);
    }
    
    // 5. Third append_attribute
    pugi::xml_attribute attr3 = target_node.append_attribute(attr_name3);
    
    // 6. Fourth append_attribute
    pugi::xml_attribute attr4 = target_node.append_attribute(attr_name4);
    
    // 7. Second set_value calls (both overloads)
    if (attr3) {
        // Extract different numeric values from fuzzer data
        long long ll_value2 = 0;
        unsigned long long ull_value2 = 0;
        
        if (Size >= 16) {
            memcpy(&ll_value2, Data + 8, sizeof(ll_value2));
            memcpy(&ull_value2, Data + 8, sizeof(ull_value2));
        } else if (Size > 4) {
            ll_value2 = static_cast<long long>(Data[4]);
            ull_value2 = static_cast<unsigned long long>(Data[4]);
        }
        
        attr3.set_value(ll_value2);
        attr3.set_value(ull_value2);
    }

    return 0;
}