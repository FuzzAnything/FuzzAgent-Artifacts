// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
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

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        return std::string_view();
    }
    return std::string_view(reinterpret_cast<const char*>(data + offset), size - offset);
}

static const char* get_cstring(const uint8_t* data, size_t size, size_t offset) {
    static thread_local char buffer[4096];
    if (offset >= size) {
        return "";
    }
    size_t copy_size = std::min(size - offset, sizeof(buffer) - 1);
    memcpy(buffer, data + offset, copy_size);
    buffer[copy_size] = '\0';
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* Data, size_t Size) {
    // Initialize document
    pugi::xml_document doc;
    
    // Create root node
    pugi::xml_node root = doc.append_child("root");
    if (!root) {
        return 0;
    }
    
    // Use different offsets for string parameters to get diverse inputs
    size_t offset1 = 0;
    size_t offset2 = Size > 0 ? Size / 4 : 0;
    size_t offset3 = Size > 0 ? Size / 2 : 0;
    size_t offset4 = Size > 0 ? (3 * Size) / 4 : 0;
    
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(get_cstring(Data, Size, offset1));
    
    // 2. pugi::xml_node::append_child
    pugi::xml_node child1 = root.append_child(get_cstring(Data, Size, offset2));
    
    // 3. pugi::xml_node::append_child
    pugi::xml_node child2 = child1.append_child(get_cstring(Data, Size, offset3));
    
    // 4. pugi::xml_text::set (both overloads)
    if (child2) {
        pugi::xml_text text2 = child2.text();
        if (Size >= sizeof(long long)) {
            long long ll_value;
            memcpy(&ll_value, Data, sizeof(long long));
            text2.set(ll_value);
        }
        if (Size >= sizeof(unsigned long long)) {
            unsigned long long ull_value;
            memcpy(&ull_value, Data, sizeof(unsigned long long));
            text2.set(ull_value);
        }
    }
    
    // 5. pugi::xml_node::append_child
    pugi::xml_node child3 = child2.append_child(get_cstring(Data, Size, offset4));
    
    // 6. pugi::xml_node::append_child
    pugi::xml_node child4 = child3.append_child(get_cstring(Data, Size, offset1));
    
    // 7. pugi::xml_text::set (both overloads)
    if (child4) {
        pugi::xml_text text4 = child4.text();
        if (Size >= sizeof(long long)) {
            long long ll_value2;
            memcpy(&ll_value2, Data + (Size > sizeof(long long) ? sizeof(long long) : 0), 
                   sizeof(long long));
            text4.set(ll_value2);
        }
        if (Size >= sizeof(unsigned long long)) {
            unsigned long long ull_value2;
            memcpy(&ull_value2, Data + (Size > sizeof(unsigned long long) ? sizeof(unsigned long long) : 0), 
                   sizeof(unsigned long long));
            text4.set(ull_value2);
        }
    }
    
    return 0;
}