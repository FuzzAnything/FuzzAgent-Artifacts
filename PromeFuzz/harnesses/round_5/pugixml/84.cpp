// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
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

static void initialize_test_node(pugi::xml_document& doc, pugi::xml_node& node) {
    // Create a simple XML structure for testing
    const char* xml_content = R"(<root attr1="value1" attr2="value2" attr3="value3"><child/></root>)";
    doc.load_string(xml_content);
    node = doc.child("root");
}

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        return "default";
    }
    // Ensure we don't read past the buffer
    size_t remaining = size - offset;
    const char* str_start = reinterpret_cast<const char*>(data + offset);
    // Find null terminator or use remaining bytes
    size_t len = 0;
    while (len < remaining && str_start[len] != '\0') {
        ++len;
    }
    return std::string_view(str_start, len);
}

static pugi::xml_attribute get_random_attribute(pugi::xml_node& node, const uint8_t* data, size_t size, size_t offset) {
    if (!node || offset >= size) {
        return node.first_attribute();
    }
    
    // Use data to select an attribute
    size_t attr_count = 0;
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        ++attr_count;
    }
    
    if (attr_count == 0) {
        return pugi::xml_attribute();
    }
    
    size_t index = data[offset] % attr_count;
    pugi::xml_attribute attr = node.first_attribute();
    for (size_t i = 0; i < index && attr; ++i) {
        attr = attr.next_attribute();
    }
    return attr;
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* data, size_t size) {
    static pugi::xml_document doc;
    static pugi::xml_node test_node;
    
    // Initialize test node once
    static bool initialized = false;
    if (!initialized) {
        initialize_test_node(doc, test_node);
        initialized = true;
    }
    
    if (size == 0) {
        return 0;
    }
    
    // Use first byte to determine which function to test
    uint8_t function_selector = data[0] % 6;
    
    // Use remaining data for string parameters
    size_t data_offset = 1;
    
    try {
        switch (function_selector) {
            case 0: {
                // Test xml_node::attribute
                std::string_view attr_name = get_string_view(data, size, data_offset);
                pugi::xml_attribute attr = test_node.attribute(attr_name.data());
                (void)attr; // Use attribute to avoid unused variable warning
                break;
            }
            case 1: {
                // Test xml_node::prepend_attribute
                std::string_view attr_name = get_string_view(data, size, data_offset);
                pugi::xml_attribute new_attr = test_node.prepend_attribute(attr_name.data());
                (void)new_attr;
                break;
            }
            case 2: {
                // Test xml_node::insert_copy_before
                pugi::xml_attribute proto = get_random_attribute(test_node, data, size, data_offset);
                data_offset += 1;
                pugi::xml_attribute ref_attr = get_random_attribute(test_node, data, size, data_offset);
                
                if (proto && ref_attr) {
                    pugi::xml_attribute copied_attr = test_node.insert_copy_before(proto, ref_attr);
                    (void)copied_attr;
                }
                break;
            }
            case 3: {
                // Test xml_node::insert_attribute_before
                std::string_view attr_name = get_string_view(data, size, data_offset);
                // Skip past the string view data
                data_offset += attr_name.length() + 1;
                if (data_offset > size) data_offset = 1;
                
                pugi::xml_attribute ref_attr = get_random_attribute(test_node, data, size, data_offset);
                
                if (ref_attr) {
                    pugi::xml_attribute new_attr = test_node.insert_attribute_before(attr_name, ref_attr);
                    (void)new_attr;
                }
                break;
            }
            case 4: {
                // Test xml_node::insert_copy_after
                pugi::xml_attribute proto = get_random_attribute(test_node, data, size, data_offset);
                data_offset += 1;
                pugi::xml_attribute ref_attr = get_random_attribute(test_node, data, size, data_offset);
                
                if (proto && ref_attr) {
                    pugi::xml_attribute copied_attr = test_node.insert_copy_after(proto, ref_attr);
                    (void)copied_attr;
                }
                break;
            }
            case 5: {
                // Test xml_node::insert_attribute_after
                std::string_view attr_name = get_string_view(data, size, data_offset);
                // Skip past the string view data
                data_offset += attr_name.length() + 1;
                if (data_offset > size) data_offset = 1;
                
                pugi::xml_attribute ref_attr = get_random_attribute(test_node, data, size, data_offset);
                
                if (ref_attr) {
                    pugi::xml_attribute new_attr = test_node.insert_attribute_after(attr_name, ref_attr);
                    (void)new_attr;
                }
                break;
            }
        }
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Reset document to clean state for next iteration
    doc.reset();
    initialize_test_node(doc, test_node);
    
    return 0;
}