// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
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
#include <fstream>

static void create_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static pugi::xml_document create_test_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size > 0) {
        create_dummy_file(data, size);
        doc.load_file("./dummy_file");
    }
    
    if (doc.empty()) {
        pugi::xml_node root = doc.append_child("root");
        for (int i = 0; i < 3 && i * 10 < static_cast<int>(size); ++i) {
            std::string attr_name = "attr" + std::to_string(i);
            std::string attr_value = "value" + std::to_string(i);
            root.append_attribute(attr_name.c_str()) = attr_value.c_str();
        }
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t* data, size_t size) {
    if (size == 0) {
        return 0;
    }

    pugi::xml_document doc = create_test_document(data, size);
    pugi::xml_node root = doc.root();
    
    if (root) {
        // Test attribute retrieval
        std::string attr_name1_str(reinterpret_cast<const char*>(data), size % 100);
        pugi::xml_attribute attr1 = root.attribute(attr_name1_str.c_str());
        
        // Test append_attribute
        size_t offset2 = size % 50;
        size_t len2 = (size / 2) % 100;
        if (offset2 + len2 > size) len2 = size - offset2;
        std::string attr_name2_str(reinterpret_cast<const char*>(data + offset2), len2);
        pugi::xml_attribute attr2 = root.append_attribute(attr_name2_str.c_str());
        
        if (attr2) {
            // Test insert_attribute_after
            size_t offset3 = size % 30;
            size_t len3 = (size / 3) % 100;
            if (offset3 + len3 > size) len3 = size - offset3;
            std::string attr_name3_str(reinterpret_cast<const char*>(data + offset3), len3);
            pugi::xml_attribute attr3 = root.insert_attribute_after(attr_name3_str.c_str(), attr2);
            
            // Test insert_attribute_before
            size_t offset4 = size % 20;
            size_t len4 = (size / 4) % 100;
            if (offset4 + len4 > size) len4 = size - offset4;
            std::string attr_name4_str(reinterpret_cast<const char*>(data + offset4), len4);
            pugi::xml_attribute attr4 = root.insert_attribute_before(attr_name4_str.c_str(), attr2);
            
            // Test insert_copy_before and insert_copy_after if we have valid attributes
            if (attr3) {
                pugi::xml_attribute copy1 = root.insert_copy_before(attr3, attr2);
                pugi::xml_attribute copy2 = root.insert_copy_after(attr3, attr2);
            }
        }
    }
    
    return 0;
}