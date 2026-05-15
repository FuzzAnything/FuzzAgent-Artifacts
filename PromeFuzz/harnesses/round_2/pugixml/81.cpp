// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

static void test_node_functions(pugi::xml_node& node, const char* data, size_t size) {
    if (size > 0) {
        // Test set_value with size parameter
        node.set_value(data, size);
        
        // Test set_name with size parameter
        node.set_name(data, size);
        
        // Test set_value with null-terminated string (if data contains null byte)
        for (size_t i = 0; i < size; ++i) {
            if (data[i] == '\0') {
                node.set_value(data);
                break;
            }
        }
    }
}

static void test_attribute_functions(pugi::xml_attribute& attr, const char* data, size_t size) {
    if (size > 0) {
        // Test set_name with size parameter
        attr.set_name(data, size);
        
        // Test set_name with null-terminated string (if data contains null byte)
        for (size_t i = 0; i < size; ++i) {
            if (data[i] == '\0') {
                attr.set_name(data);
                break;
            }
        }
        
        // Test set_value with numeric overloads
        // Use data to derive numeric values
        unsigned long ul_val = 0;
        long l_val = 0;
        
        // Create numeric values from first few bytes of data
        if (size >= sizeof(unsigned long)) {
            memcpy(&ul_val, data, sizeof(unsigned long));
        } else if (size > 0) {
            // Use available bytes
            memcpy(&ul_val, data, size);
        }
        
        if (size >= sizeof(long)) {
            memcpy(&l_val, data, sizeof(long));
        } else if (size > 0) {
            // Use available bytes
            memcpy(&l_val, data, size);
        }
        
        attr.set_value(ul_val);
        attr.set_value(l_val);
    }
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a document and nodes for testing
    pugi::xml_document doc;
    
    // Create different node types to test set_value on supported nodes
    pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
    pugi::xml_node pi_node = doc.append_child(pugi::node_pi);
    pugi::xml_node cdata_node = doc.append_child(pugi::node_cdata);
    pugi::xml_node doctype_node = doc.append_child(pugi::node_doctype);
    
    // Test set_value on different node types
    test_node_functions(comment_node, reinterpret_cast<const char*>(Data), Size);
    test_node_functions(pi_node, reinterpret_cast<const char*>(Data), Size);
    test_node_functions(cdata_node, reinterpret_cast<const char*>(Data), Size);
    test_node_functions(doctype_node, reinterpret_cast<const char*>(Data), Size);
    
    // Create an element node to test set_name (elements support set_name)
    pugi::xml_node element_node = doc.append_child(pugi::node_element);
    test_node_functions(element_node, reinterpret_cast<const char*>(Data), Size);
    
    // Create attributes for testing
    pugi::xml_attribute attr1 = element_node.append_attribute("attr1");
    pugi::xml_attribute attr2 = element_node.append_attribute("attr2");
    
    // Test attribute functions
    test_attribute_functions(attr1, reinterpret_cast<const char*>(Data), Size);
    test_attribute_functions(attr2, reinterpret_cast<const char*>(Data), Size);
    
    // Also test on empty attribute (should return false)
    pugi::xml_attribute empty_attr;
    test_attribute_functions(empty_attr, reinterpret_cast<const char*>(Data), Size);
    
    // Test on empty node (should return false)
    pugi::xml_node empty_node;
    test_node_functions(empty_node, reinterpret_cast<const char*>(Data), Size);
    
    // Write to dummy file if needed for additional testing
    const char* dummy_file = "./dummy_file";
    doc.save_file(dummy_file);
    
    return 0;
}