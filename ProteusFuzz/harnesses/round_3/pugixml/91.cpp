// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
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
#include <functional>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    if (size == 0) {
        return nullptr;
    }
    
    // Try to parse as XML
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load_buffer(data, size);
    
    if (!result) {
        delete doc;
        return nullptr;
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create document from input
    pugi::xml_document* doc = create_document_from_input(Data, Size);
    
    if (!doc) {
        return 0;
    }
    
    // Get root node or create one if document is empty
    pugi::xml_node root = doc->document_element();
    if (!root) {
        root = doc->append_child("root");
    }
    
    // Test 1: first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr; // Suppress unused variable warning
    
    // Test 2: find_attribute with various predicates
    // Predicate: attribute name starts with 'a'
    root.find_attribute([](const pugi::xml_attribute& attr) {
        if (!attr) return false;
        const char* name = attr.name();
        return name && name[0] == 'a';
    });
    
    // Predicate: attribute value contains 'test'
    root.find_attribute([](const pugi::xml_attribute& attr) {
        if (!attr) return false;
        const char* value = attr.value();
        return value && std::string(value).find("test") != std::string::npos;
    });
    
    // Predicate: always true (to get first attribute)
    root.find_attribute([](const pugi::xml_attribute& attr) {
        return !!attr;
    });
    
    // Test 3: attribute by name
    pugi::xml_attribute attr_by_name = root.attribute("id");
    (void)attr_by_name;
    
    // Test with different attribute names derived from input
    if (Size > 0) {
        char name[64];
        size_t name_len = std::min(Size, 63UL);
        std::memcpy(name, Data, name_len);
        name[name_len] = '\0';
        root.attribute(name);
    }
    
    // Test 4: insert_attribute_after
    if (root.first_attribute()) {
        pugi::xml_attribute ref_attr = root.first_attribute();
        char new_name[32];
        size_t name_len = std::min(Size > 0 ? Size : 1, 31UL);
        std::memcpy(new_name, Data, name_len);
        new_name[name_len] = '\0';
        
        root.insert_attribute_after(new_name, ref_attr);
    }
    
    // Test 5: insert_attribute_before
    if (root.first_attribute()) {
        pugi::xml_attribute ref_attr = root.first_attribute();
        char new_name[32];
        size_t name_len = std::min(Size > 0 ? Size : 1, 31UL);
        std::memcpy(new_name, Data, name_len);
        new_name[name_len] = '\0';
        
        root.insert_attribute_before(new_name, ref_attr);
    }
    
    // Test 6: last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    (void)last_attr;
    
    // Additional stress testing: multiple insertions
    for (int i = 0; i < 10 && Size > 0; ++i) {
        char attr_name[16];
        snprintf(attr_name, sizeof(attr_name), "attr%d", i);
        root.append_attribute(attr_name);
        
        // Try to insert after the newly created attribute
        pugi::xml_attribute new_attr = root.first_attribute();
        if (new_attr) {
            char insert_name[16];
            snprintf(insert_name, sizeof(insert_name), "insert%d", i);
            root.insert_attribute_after(insert_name, new_attr);
        }
    }
    
    // Cleanup
    delete doc;
    
    return 0;
}