// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input
    static const char* dummy_file = "./dummy_file";
    std::ofstream out(dummy_file, std::ios::binary);
    if (out.is_open()) {
        out.write(reinterpret_cast<const char*>(Data), Size);
        out.close();
    }

    // Load XML document from the dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(dummy_file);
    
    // If parsing fails, create a minimal document to test API functions
    if (!result) {
        doc.append_child("root");
    }
    
    // Get the root node (or create one if empty)
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }
    
    // Prepare some attributes for testing
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    attr1.set_value("value1");
    
    pugi::xml_attribute attr2 = root.append_attribute("attr2");
    attr2.set_value("value2");
    
    // Test 1: pugi::xml_node::attribute
    if (Size > 0) {
        // Use first byte as length for attribute name
        size_t name_len = Data[0] % 256;
        if (name_len > 0 && name_len <= Size - 1) {
            std::string attr_name(reinterpret_cast<const char*>(Data + 1), name_len);
            pugi::xml_attribute found_attr = root.attribute(attr_name.c_str());
            // Do something with the result to avoid dead code elimination
            if (found_attr) {
                const char* val = found_attr.value();
            }
        }
    }
    
    // Test 2: pugi::xml_node::insert_attribute_after
    if (Size > 0) {
        size_t name_len = Data[0] % 256;
        if (name_len > 0 && name_len <= Size - 1) {
            std::string new_attr_name(reinterpret_cast<const char*>(Data + 1), name_len);
            pugi::xml_attribute new_attr = root.insert_attribute_after(new_attr_name, attr1);
            if (new_attr) {
                new_attr.set_value("inserted_after");
            }
        }
    }
    
    // Test 3: pugi::xml_node::append_attribute
    if (Size > 0) {
        size_t name_len = Data[0] % 256;
        if (name_len > 0 && name_len <= Size - 1) {
            std::string attr_name(reinterpret_cast<const char*>(Data + 1), name_len);
            // Ensure null termination
            std::string safe_name(attr_name.data(), attr_name.length());
            pugi::xml_attribute new_attr = root.append_attribute(safe_name.c_str());
            if (new_attr) {
                new_attr.set_value("appended");
            }
        }
    }
    
    // Test 4: pugi::xml_node::insert_attribute_before
    if (Size > 0) {
        size_t name_len = Data[0] % 256;
        if (name_len > 0 && name_len <= Size - 1) {
            std::string new_attr_name(reinterpret_cast<const char*>(Data + 1), name_len);
            pugi::xml_attribute new_attr = root.insert_attribute_before(new_attr_name, attr2);
            if (new_attr) {
                new_attr.set_value("inserted_before");
            }
        }
    }
    
    // Test 5: pugi::xml_node::insert_copy_before
    if (attr1 && attr2) {
        pugi::xml_attribute copied_attr = root.insert_copy_before(attr1, attr2);
        if (copied_attr) {
            // Do something with the copied attribute
            const char* val = copied_attr.value();
        }
    }
    
    // Test 6: pugi::xml_node::insert_copy_after
    if (attr1 && attr2) {
        pugi::xml_attribute copied_attr = root.insert_copy_after(attr2, attr1);
        if (copied_attr) {
            // Do something with the copied attribute
            const char* val = copied_attr.value();
        }
    }
    
    // Clean up the dummy file
    std::remove(dummy_file);
    
    return 0;
}