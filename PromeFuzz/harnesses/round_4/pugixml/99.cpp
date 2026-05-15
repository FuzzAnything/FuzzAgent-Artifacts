// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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

static pugi::xml_document create_test_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Create a root node
    pugi::xml_node root = doc.append_child("root");
    
    // Add some initial attributes to work with
    root.append_attribute("attr1");
    root.append_attribute("attr2");
    root.append_attribute("attr3");
    
    // Try to parse the fuzzer input as XML content
    if (size > 0) {
        // Use a buffer for XML parsing
        char* buffer = new char[size + 1];
        memcpy(buffer, data, size);
        buffer[size] = '\0';
        
        // Try to load as XML
        pugi::xml_document temp_doc;
        pugi::xml_parse_result result = temp_doc.load_buffer(buffer, size);
        
        if (result) {
            // Append parsed content as child
            root.append_copy(temp_doc.first_child());
        }
        
        delete[] buffer;
    }
    
    return doc;
}

static std::string get_safe_string(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        return "";
    }
    
    // Limit string length to prevent buffer overflows
    size_t max_len = 256;
    size_t available = size - offset;
    size_t len = available < max_len ? available : max_len;
    
    // Ensure we don't read past buffer boundaries
    std::string result;
    result.reserve(len + 1);
    for (size_t i = 0; i < len; i++) {
        char c = static_cast<char>(data[offset + i]);
        // Only use printable characters to avoid issues
        if (c >= 32 && c <= 126) {
            result.push_back(c);
        } else {
            result.push_back('a' + (c % 26));
        }
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t* data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    try {
        // Create test document with initial structure
        pugi::xml_document doc = create_test_document(data, size);
        pugi::xml_node root = doc.child("root");
        
        if (!root) {
            return 0;
        }
        
        // Get existing attributes for reference
        pugi::xml_attribute attr1 = root.attribute("attr1");
        pugi::xml_attribute attr2 = root.attribute("attr2");
        pugi::xml_attribute attr3 = root.attribute("attr3");
        
        // Use first byte to determine which operations to perform
        uint8_t operation_mask = data[0];
        size_t data_offset = 1;
        
        // Test pugi::xml_node::attribute
        if (operation_mask & 0x01) {
            std::string name = get_safe_string(data, size, data_offset);
            if (!name.empty()) {
                pugi::xml_attribute attr = root.attribute(name.c_str());
                (void)attr;
            }
        }
        
        // Test pugi::xml_node::insert_attribute_before
        if (operation_mask & 0x02 && attr2) {
            std::string name = get_safe_string(data, size, data_offset);
            if (!name.empty()) {
                pugi::xml_attribute new_attr = root.insert_attribute_before(name.c_str(), attr2);
                (void)new_attr;
            }
        }
        
        // Test pugi::xml_node::insert_copy_after
        if (operation_mask & 0x04 && attr1 && attr3) {
            pugi::xml_attribute copy = root.insert_copy_after(attr1, attr3);
            (void)copy;
        }
        
        // Test pugi::xml_node::prepend_attribute
        if (operation_mask & 0x08) {
            std::string name = get_safe_string(data, size, data_offset);
            if (!name.empty()) {
                pugi::xml_attribute new_attr = root.prepend_attribute(name.c_str());
                (void)new_attr;
            }
        }
        
        // Test pugi::xml_node::insert_attribute_after
        if (operation_mask & 0x10 && attr1) {
            std::string name = get_safe_string(data, size, data_offset);
            if (!name.empty()) {
                pugi::xml_attribute new_attr = root.insert_attribute_after(name.c_str(), attr1);
                (void)new_attr;
            }
        }
        
        // Test pugi::xml_node::insert_copy_before
        if (operation_mask & 0x20 && attr2 && attr3) {
            pugi::xml_attribute copy = root.insert_copy_before(attr2, attr3);
            (void)copy;
        }
        
        // Try to access attributes with different names
        for (size_t i = 0; i < 10 && data_offset + i < size; i++) {
            char name[2] = {static_cast<char>('a' + (data[data_offset + i] % 26)), '\0'};
            pugi::xml_attribute attr = root.attribute(name);
            (void)attr;
        }
        
    } catch (...) {
        // Catch all exceptions to prevent crashes
    }
    
    return 0;
}