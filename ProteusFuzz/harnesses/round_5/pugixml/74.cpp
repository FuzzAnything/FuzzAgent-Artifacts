// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    static pugi::xml_document doc;
    doc.reset();
    
    if (size == 0) {
        return nullptr;
    }
    
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    if (!result) {
        return nullptr;
    }
    
    return &doc;
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Ensure minimum size for basic XML structure
    if (Size < 4) {
        return 0;
    }
    
    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) {
        return 0;
    }
    
    pugi::xml_node root = doc->document_element();
    if (!root) {
        // Try to create a simple document if parsing failed to produce a root
        doc->reset();
        pugi::xml_parse_result result = doc->load_string("<root/>");
        if (!result) {
            return 0;
        }
        root = doc->document_element();
    }
    
    // Ensure we have at least one attribute to work with
    if (!root.attribute("test")) {
        pugi::xml_attribute attr = root.append_attribute("test");
        if (!attr) {
            return 0;
        }
        attr.set_value("value");
    }
    
    // Get some attributes for testing
    pugi::xml_attribute attr1 = root.attribute("test");
    if (!attr1) {
        return 0;
    }
    
    // Add another attribute for sibling operations
    pugi::xml_attribute attr2 = root.append_attribute("test2");
    if (!attr2) {
        return 0;
    }
    attr2.set_value("value2");
    
    // Test insert_copy_after
    pugi::xml_attribute copied_after = root.insert_copy_after(attr1, attr2);
    (void)copied_after;
    
    // Test insert_attribute_before
    std::string_view name_sv = "new_attr";
    pugi::xml_attribute new_attr_before = root.insert_attribute_before(name_sv, attr1);
    (void)new_attr_before;
    
    // Test insert_copy_before
    pugi::xml_attribute copied_before = root.insert_copy_before(attr2, attr1);
    (void)copied_before;
    
    // Test attribute lookup
    pugi::xml_attribute found_attr = root.attribute("test");
    (void)found_attr;
    
    // Test prepend_copy
    pugi::xml_attribute prepended = root.prepend_copy(attr1);
    (void)prepended;
    
    // Test append_copy
    pugi::xml_attribute appended = root.append_copy(attr2);
    (void)appended;
    
    // Additional edge case: try operations with invalid attributes
    pugi::xml_attribute invalid_attr;
    root.insert_copy_after(invalid_attr, attr1);
    root.insert_copy_after(attr1, invalid_attr);
    root.insert_copy_before(invalid_attr, attr1);
    root.insert_copy_before(attr1, invalid_attr);
    root.prepend_copy(invalid_attr);
    root.append_copy(invalid_attr);
    
    // Test with different string views
    std::string_view empty_sv = "";
    root.insert_attribute_before(empty_sv, attr1);
    
    return 0;
}