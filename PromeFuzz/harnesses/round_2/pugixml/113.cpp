// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

static pugi::xml_document create_test_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    // If parsing failed, create a minimal document to work with
    if (!result) {
        doc.append_child("root");
        pugi::xml_node root = doc.child("root");
        root.append_attribute("attr1") = "value1";
        root.append_attribute("attr2") = "value2";
        root.append_attribute("attr3") = "value3";
    }
    
    return doc;
}

static std::string generate_string(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "default";
    
    size_t length = size - offset;
    if (length > 100) length = 100; // Limit string length
    
    return std::string(reinterpret_cast<const char*>(data + offset), length);
}

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t* data, size_t size) {
    if (size < 2) return 0;
    
    // Create test document
    pugi::xml_document doc = create_test_document(data, size);
    pugi::xml_node root = doc.child("root");
    if (!root) root = doc.first_child();
    if (!root) return 0;
    
    // Get existing attributes for reference
    pugi::xml_attribute attr1 = root.attribute("attr1");
    pugi::xml_attribute attr2 = root.attribute("attr2");
    pugi::xml_attribute attr3 = root.attribute("attr3");
    
    // Generate strings for attribute names
    std::string name1 = generate_string(data, size, 0);
    std::string name2 = generate_string(data, size, size/3);
    std::string name3 = generate_string(data, size, 2*size/3);
    
    // Test 1: insert_copy_before
    if (attr1 && attr2) {
        try {
            pugi::xml_attribute new_attr = root.insert_copy_before(attr1, attr2);
            (void)new_attr; // Use result to avoid unused variable warning
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 2: insert_attribute_before
    if (attr2) {
        try {
            pugi::xml_attribute new_attr = root.insert_attribute_before(name1.c_str(), attr2);
            (void)new_attr;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 3: insert_copy_after
    if (attr2 && attr3) {
        try {
            pugi::xml_attribute new_attr = root.insert_copy_after(attr2, attr3);
            (void)new_attr;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 4: insert_attribute_after
    if (attr3) {
        try {
            std::string_view name_sv(name2);
            pugi::xml_attribute new_attr = root.insert_attribute_after(name_sv, attr3);
            (void)new_attr;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 5: prepend_attribute
    try {
        pugi::xml_attribute new_attr = root.prepend_attribute(name3.c_str());
        (void)new_attr;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 6: attribute retrieval
    try {
        pugi::xml_attribute retrieved1 = root.attribute(name1.c_str());
        pugi::xml_attribute retrieved2 = root.attribute("attr1");
        pugi::xml_attribute retrieved3 = root.attribute("non_existent");
        (void)retrieved1;
        (void)retrieved2;
        (void)retrieved3;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Additional tests with different node types
    pugi::xml_node child = root.append_child("child");
    child.append_attribute("child_attr") = "child_value";
    
    // Test on child node
    pugi::xml_attribute child_attr = child.attribute("child_attr");
    if (child_attr) {
        try {
            std::string_view child_name_sv("new_child_attr");
            pugi::xml_attribute new_child_attr = child.insert_attribute_after(child_name_sv, child_attr);
            (void)new_child_attr;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test with empty/null attributes
    pugi::xml_attribute empty_attr;
    try {
        pugi::xml_attribute test = root.insert_copy_before(empty_attr, empty_attr);
        (void)test;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test with different string views
    std::string_view empty_sv;
    try {
        pugi::xml_attribute test = root.insert_attribute_after(empty_sv, attr1);
        (void)test;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Clean up
    doc.reset();
    
    return 0;
}