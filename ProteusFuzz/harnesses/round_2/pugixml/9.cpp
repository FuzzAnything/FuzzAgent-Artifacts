// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

static bool parse_xml_from_fuzz_data(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return false;
    }
    
    // Create a null-terminated string from the fuzz data
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    
    // Try to parse the XML string
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    return result.status == pugi::status_ok;
}

static bool setup_initial_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    // First try to parse the fuzz data
    if (parse_xml_from_fuzz_data(doc, data, size)) {
        return true;
    }
    
    // If parsing fails, create a simple valid XML document
    doc.load_string("<root><child attr1=\"value1\" attr2=\"value2\"><nested attr3=\"value3\"/></child></root>");
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Create a new XML document for each fuzz iteration
    pugi::xml_document doc;
    
    // Setup the document with fuzz data or default structure
    if (!setup_initial_document(doc, Data, Size)) {
        return 0;
    }
    
    // Get the root node
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }
    
    // Get a child node to work with
    pugi::xml_node child = root.first_child();
    if (!child) {
        // If no child exists, try to create one
        child = root.append_child("test_child");
        if (!child) {
            return 0;
        }
        child.append_attribute("test_attr").set_value("test_value");
    }
    
    // Get attributes to use as references
    pugi::xml_attribute attr1 = child.first_attribute();
    
    // If we don't have enough attributes, create them
    if (!attr1) {
        attr1 = child.append_attribute("attr1");
        attr1.set_value("value1");
    }
    
    pugi::xml_attribute attr2 = attr1.next_attribute();
    if (!attr2) {
        attr2 = child.append_attribute("attr2");
        attr2.set_value("value2");
    }
    
    // Step 1: insert_attribute_after
    pugi::xml_attribute new_attr_after = child.insert_attribute_after("new_attr_after", attr1);
    (void)new_attr_after;
    
    // Step 2: insert_attribute_before
    pugi::xml_attribute new_attr_before = child.insert_attribute_before("new_attr_before", attr2);
    (void)new_attr_before;
    
    // Step 3-6: first_child (called 4 times as specified)
    pugi::xml_node first_child_1 = child.first_child();
    pugi::xml_node first_child_2 = child.first_child();
    pugi::xml_node first_child_3 = child.first_child();
    pugi::xml_node first_child_4 = child.first_child();
    (void)first_child_1;
    (void)first_child_2;
    (void)first_child_3;
    (void)first_child_4;
    
    // Step 7: append_copy
    if (attr1) {
        pugi::xml_attribute copied_attr_append = child.append_copy(attr1);
        (void)copied_attr_append;
    }
    
    // Step 8: prepend_copy
    if (attr2) {
        pugi::xml_attribute copied_attr_prepend = child.prepend_copy(attr2);
        (void)copied_attr_prepend;
    }
    
    // Step 9: insert_copy_after
    if (attr1 && attr2) {
        pugi::xml_attribute copied_attr_after = child.insert_copy_after(attr1, attr2);
        (void)copied_attr_after;
    }
    
    // Step 10: insert_copy_before
    if (attr1 && attr2) {
        pugi::xml_attribute copied_attr_before = child.insert_copy_before(attr1, attr2);
        (void)copied_attr_before;
    }
    
    // Cleanup is automatic with pugi::xml_document destructor
    return 0;
}