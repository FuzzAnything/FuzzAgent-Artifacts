// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) return "";
    size_t length = data[offset] % (size - offset);
    offset++;
    if (offset + length > size) length = size - offset;
    const char* str = reinterpret_cast<const char*>(data + offset);
    offset += length;
    return std::string_view(str, length);
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;

    // Create document with some structure
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Add some initial attributes
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    attr1.set_value("value1");
    pugi::xml_attribute attr2 = root.append_attribute("attr2");
    attr2.set_value("value2");
    pugi::xml_attribute attr3 = root.append_attribute("attr3");
    attr3.set_value("value3");

    size_t offset = 0;
    
    // Use first byte to determine which function to test
    uint8_t func_choice = Data[0] % 6;
    offset = 1;

    switch (func_choice) {
        case 0: { // insert_copy_before
            if (Size < 3) break;
            std::string_view name = get_string_view(Data, Size, offset);
            pugi::xml_attribute new_attr = root.append_attribute(name.data());
            new_attr.set_value("copy_value");
            root.insert_copy_before(new_attr, attr2);
            break;
        }
        case 1: { // insert_attribute_before
            if (Size < 3) break;
            std::string_view name = get_string_view(Data, Size, offset);
            root.insert_attribute_before(name.data(), attr2);
            break;
        }
        case 2: { // insert_copy_after
            if (Size < 3) break;
            std::string_view name = get_string_view(Data, Size, offset);
            pugi::xml_attribute new_attr = root.append_attribute(name.data());
            new_attr.set_value("copy_value");
            root.insert_copy_after(new_attr, attr2);
            break;
        }
        case 3: { // attribute with hint
            if (Size < 3) break;
            std::string_view name = get_string_view(Data, Size, offset);
            pugi::xml_attribute hint = attr1;
            root.attribute(name.data(), hint);
            break;
        }
        case 4: { // insert_attribute_after
            if (Size < 3) break;
            std::string_view name = get_string_view(Data, Size, offset);
            root.insert_attribute_after(name.data(), attr2);
            break;
        }
        case 5: { // prepend_attribute
            if (Size < 2) break;
            std::string_view name = get_string_view(Data, Size, offset);
            root.prepend_attribute(name.data());
            break;
        }
    }

    // Try to trigger different code paths by using different combinations
    if (Size > 10) {
        // Create another document and try cross-document operations
        pugi::xml_document doc2;
        pugi::xml_node root2 = doc2.append_child("root2");
        pugi::xml_attribute other_attr = root2.append_attribute("other");
        other_attr.set_value("other_value");
        
        // This should fail but we want to test the failure path
        root.insert_copy_before(other_attr, attr1);
    }

    // Test with empty/null attributes
    pugi::xml_attribute empty_attr;
    root.insert_copy_before(empty_attr, attr1);
    root.insert_copy_after(empty_attr, attr1);

    // Test with self-reference (should fail)
    root.insert_copy_before(attr1, attr1);
    root.insert_copy_after(attr1, attr1);

    // Clean up - let RAII handle it
    return 0;
}