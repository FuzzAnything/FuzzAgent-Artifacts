// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <vector>

static std::string generate_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) return "default";
    size_t len = size % max_len;
    if (len == 0) len = max_len;
    return std::string(reinterpret_cast<const char*>(data), len);
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* Data, size_t Size) {
    // Initialize document and nodes
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some initial structure
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = child1.append_child("child3");
    
    // Add initial attributes
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    attr1.set_value("value1");
    pugi::xml_attribute attr2 = root.append_attribute("attr2");
    attr2.set_value("value2");
    
    // Generate strings from fuzzer input
    std::string name1 = generate_string(Data, Size, 20);
    std::string name2 = generate_string(Data + 10, Size > 10 ? Size - 10 : 0, 20);
    
    // 1. insert_attribute_after
    try {
        root.insert_attribute_after(name1.c_str(), attr1);
    } catch (...) {}
    
    // 2. insert_attribute_before
    try {
        root.insert_attribute_before(name2.c_str(), attr2);
    } catch (...) {}
    
    // 3-6. first_child (4 times)
    pugi::xml_node current = root;
    for (int i = 0; i < 4; ++i) {
        try {
            current = current.first_child();
            if (!current) break;
        } catch (...) {
            break;
        }
    }
    
    // 7. append_copy
    try {
        root.append_copy(attr1);
    } catch (...) {}
    
    // 8. prepend_copy
    try {
        root.prepend_copy(attr2);
    } catch (...) {}
    
    // 9. insert_copy_after
    try {
        root.insert_copy_after(attr1, attr2);
    } catch (...) {}
    
    // 10. insert_copy_before
    try {
        root.insert_copy_before(attr2, attr1);
    } catch (...) {}
    
    return 0;
}