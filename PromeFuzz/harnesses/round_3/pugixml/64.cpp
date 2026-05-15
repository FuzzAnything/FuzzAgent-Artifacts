// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
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

static pugi::xml_document create_test_document() {
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    node.append_attribute("test_attr");
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create test document with valid nodes and attributes
    pugi::xml_document doc = create_test_document();
    pugi::xml_node node = doc.first_child();
    pugi::xml_attribute attr = node.first_attribute();
    
    // Create empty/default-constructed objects
    pugi::xml_attribute empty_attr;
    pugi::xml_node empty_node;

    // Use input data for string parameters
    std::string_view sv(reinterpret_cast<const char*>(Data), Size);
    
    // Test 1: xml_attribute::set_value
    if (Size > 0) {
        attr.set_value(sv.data(), sv.size());
        empty_attr.set_value(sv.data(), sv.size());
    }

    // Test 2: xml_attribute::name
    const char* attr_name = attr.name();
    const char* empty_attr_name = empty_attr.name();
    (void)attr_name;
    (void)empty_attr_name;

    // Test 3: xml_node::set_name
    if (Size > 0) {
        node.set_name(sv.data(), sv.size());
        empty_node.set_name(sv.data(), sv.size());
    }

    // Test 4: xml_attribute::empty
    bool attr_empty = attr.empty();
    bool empty_attr_empty = empty_attr.empty();
    (void)attr_empty;
    (void)empty_attr_empty;

    // Test 5: xml_attribute::set_name (both overloads)
    if (Size > 0) {
        // Size-based overload
        attr.set_name(sv.data(), sv.size());
        empty_attr.set_name(sv.data(), sv.size());
        
        // Null-terminated string overload
        // Create null-terminated string from input data
        size_t len = Size < 100 ? Size : 100;
        char buffer[101];
        std::memcpy(buffer, Data, len);
        buffer[len] = '\0';
        
        attr.set_name(buffer);
        empty_attr.set_name(buffer);
    }

    return 0;
}