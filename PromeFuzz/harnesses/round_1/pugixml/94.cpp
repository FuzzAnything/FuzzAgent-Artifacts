// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a document with a root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some initial attributes for testing
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    attr1.set_value("value1");
    
    pugi::xml_attribute attr2 = root.append_attribute("attr2");
    attr2.set_value("value2");
    
    pugi::xml_attribute attr3 = root.append_attribute("attr3");
    attr3.set_value("value3");

    // Use fuzzer input safely - ensure we don't go out of bounds
    const char* data_ptr = reinterpret_cast<const char*>(Data);
    size_t usable_size = Size;
    
    // Safely create string views with bounds checking
    std::string_view input_data(data_ptr, usable_size);
    
    // Create attribute names with safe bounds checking
    std::string attr_name1, attr_name2, attr_name3, attr_name4;
    
    if (usable_size >= 1) {
        size_t len1 = std::min(static_cast<size_t>(1), usable_size);
        attr_name1 = std::string(data_ptr, len1);
    } else {
        attr_name1 = "a";
    }
    
    if (usable_size >= 2) {
        size_t len2 = std::min(static_cast<size_t>(1), usable_size - 1);
        attr_name2 = std::string(data_ptr + 1, len2);
    } else {
        attr_name2 = "b";
    }
    
    if (usable_size >= 3) {
        size_t len3 = std::min(static_cast<size_t>(1), usable_size - 2);
        attr_name3 = std::string(data_ptr + 2, len3);
    } else {
        attr_name3 = "c";
    }
    
    if (usable_size >= 4) {
        size_t len4 = std::min(static_cast<size_t>(1), usable_size - 3);
        attr_name4 = std::string(data_ptr + 3, len4);
    } else {
        attr_name4 = "d";
    }

    // Test 1: attribute() function
    {
        pugi::xml_attribute found_attr = root.attribute(attr_name1.c_str());
        (void)found_attr;
    }

    // Test 2: insert_attribute_after()
    {
        pugi::xml_attribute new_attr = root.insert_attribute_after(attr_name2, attr1);
        if (new_attr) {
            new_attr.set_value("inserted_after");
        }
    }

    // Test 3: append_attribute()
    {
        pugi::xml_attribute new_attr = root.append_attribute(attr_name3.c_str());
        if (new_attr) {
            new_attr.set_value("appended");
        }
    }

    // Test 4: insert_attribute_before()
    {
        pugi::xml_attribute new_attr = root.insert_attribute_before(attr_name4, attr2);
        if (new_attr) {
            new_attr.set_value("inserted_before");
        }
    }

    // Test 5: insert_copy_before()
    {
        pugi::xml_attribute copied_attr = root.insert_copy_before(attr1, attr3);
        (void)copied_attr;
    }

    // Test 6: insert_copy_after()
    {
        pugi::xml_attribute copied_attr = root.insert_copy_after(attr2, attr1);
        (void)copied_attr;
    }

    // Additional test: Try to access attributes with various names
    {
        // Use small chunks of input data for attribute names
        for (size_t i = 0; i < usable_size && i < 20; i++) {
            size_t chunk_size = std::min(static_cast<size_t>(10), usable_size - i);
            std::string test_name(data_ptr + i, chunk_size);
            pugi::xml_attribute test_attr = root.attribute(test_name.c_str());
            (void)test_attr;
        }
    }

    return 0;
}