// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
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

static pugi::xml_document doc;

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Initialize document with root node
    doc.reset();
    pugi::xml_node root = doc.append_child("root");
    
    // Prepare string parameters from fuzzer input
    // Ensure null-terminated strings for child() and append_child()
    char* buffer = nullptr;
    if (Size > 0) {
        buffer = new char[Size + 1];
        memcpy(buffer, Data, Size);
        buffer[Size] = '\0';
    } else {
        buffer = new char[1];
        buffer[0] = '\0';
    }
    
    std::string_view str_view(buffer, Size);
    
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(buffer);
    
    // 2. pugi::xml_node::append_child
    pugi::xml_node child1 = root.append_child(buffer);
    
    // 3. pugi::xml_node::append_child
    pugi::xml_node child2 = root.append_child(buffer);
    
    // 4. pugi::xml_text::set (both overloads)
    if (Size >= sizeof(unsigned long)) {
        unsigned long ul_val = *reinterpret_cast<const unsigned long*>(Data);
        child1.text().set(ul_val);
    }
    
    if (Size >= sizeof(long)) {
        long l_val = *reinterpret_cast<const long*>(Data);
        child1.text().set(l_val);
    }
    
    // 5. pugi::xml_node::append_child
    pugi::xml_node child3 = root.append_child(buffer);
    
    // 6. pugi::xml_node::append_child
    pugi::xml_node child4 = root.append_child(buffer);
    
    // 7. pugi::xml_text::set (both overloads)
    if (Size >= sizeof(unsigned long)) {
        unsigned long ul_val2 = *reinterpret_cast<const unsigned long*>(Data);
        child3.text().set(ul_val2);
    }
    
    if (Size >= sizeof(long)) {
        long l_val2 = *reinterpret_cast<const long*>(Data);
        child3.text().set(l_val2);
    }
    
    // Cleanup
    delete[] buffer;
    
    return 0;
}