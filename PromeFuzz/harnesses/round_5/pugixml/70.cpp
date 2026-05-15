// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
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
#include <string_view>
#include <vector>

static std::string_view get_random_string_view(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) {
        return std::string_view();
    }
    size_t length = data[offset] % 32;
    if (offset + length >= size) {
        length = size - offset - 1;
    }
    const char* str = reinterpret_cast<const char*>(data + offset);
    offset += length + 1;
    return std::string_view(str, length);
}

static pugi::xml_document create_test_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    size_t offset = 0;
    
    // Create a root node
    pugi::xml_node root = doc.append_child("root");
    
    // Add some initial attributes
    for (int i = 0; i < 5 && offset < size; i++) {
        std::string_view name = get_random_string_view(data, size, offset);
        std::string_view value = get_random_string_view(data, size, offset);
        if (!name.empty()) {
            root.append_attribute(name.data()).set_value(value.data());
        }
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* data, size_t size) {
    if (size < 1) {
        return 0;
    }
    
    // Create test document with initial attributes
    pugi::xml_document doc = create_test_document(data, size);
    pugi::xml_node root = doc.child("root");
    
    if (!root) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Test 1: attribute with hint
    {
        pugi::xml_attribute hint = root.first_attribute();
        if (hint) {
            std::string_view name = get_random_string_view(data, size, offset);
            pugi::xml_attribute attr = root.attribute(name.data(), hint);
            // Use the attribute to avoid optimization
            if (attr) {
                const char* value = attr.value();
                (void)value;
            }
        }
    }
    
    // Test 2: prepend_attribute
    {
        std::string_view name = get_random_string_view(data, size, offset);
        pugi::xml_attribute attr = root.prepend_attribute(name.data());
        if (attr) {
            std::string_view value = get_random_string_view(data, size, offset);
            attr.set_value(value.data());
        }
    }
    
    // Test 3: insert_copy_before
    {
        pugi::xml_attribute first_attr = root.first_attribute();
        pugi::xml_attribute second_attr = root.last_attribute();
        
        if (first_attr && second_attr && first_attr != second_attr) {
            pugi::xml_attribute new_attr = root.insert_copy_before(second_attr, first_attr);
            if (new_attr) {
                const char* value = new_attr.value();
                (void)value;
            }
        }
    }
    
    // Test 4: insert_attribute_before
    {
        pugi::xml_attribute ref_attr = root.first_attribute();
        if (ref_attr) {
            std::string_view name = get_random_string_view(data, size, offset);
            pugi::xml_attribute new_attr = root.insert_attribute_before(name.data(), ref_attr);
            if (new_attr) {
                std::string_view value = get_random_string_view(data, size, offset);
                new_attr.set_value(value.data());
            }
        }
    }
    
    // Test 5: insert_copy_after
    {
        pugi::xml_attribute first_attr = root.first_attribute();
        pugi::xml_attribute second_attr = root.last_attribute();
        
        if (first_attr && second_attr && first_attr != second_attr) {
            pugi::xml_attribute new_attr = root.insert_copy_after(first_attr, second_attr);
            if (new_attr) {
                const char* value = new_attr.value();
                (void)value;
            }
        }
    }
    
    // Test 6: insert_attribute_after
    {
        pugi::xml_attribute ref_attr = root.last_attribute();
        if (ref_attr) {
            std::string_view name = get_random_string_view(data, size, offset);
            pugi::xml_attribute new_attr = root.insert_attribute_after(name.data(), ref_attr);
            if (new_attr) {
                std::string_view value = get_random_string_view(data, size, offset);
                new_attr.set_value(value.data());
            }
        }
    }
    
    return 0;
}