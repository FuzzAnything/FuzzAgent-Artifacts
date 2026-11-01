// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstring>
#include <vector>
#include <functional>

// Static helper to parse input data into an xml_document
static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;
    
    // Create a null-terminated string from the input data
    std::vector<char> buffer(size + 1, 0);
    std::memcpy(buffer.data(), data, size);
    
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load_string(buffer.data());
    
    if (!result) {
        delete doc;
        return nullptr;
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Parse the input as XML
    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) return 0;
    
    try {
        pugi::xml_node root = doc->document_element();
        if (!root) {
            // If no document element, try to create one for testing
            root = doc->append_child("root");
        }
        
        // Test 1: first_attribute()
        pugi::xml_attribute first_attr = root.first_attribute();
        
        // Test 2: last_attribute()
        pugi::xml_attribute last_attr = root.last_attribute();
        
        // Test 3: attribute(string_view_t name)
        // Create a name from input data if possible
        std::string_view name_view(reinterpret_cast<const char*>(Data), 
                                  std::min(Size, static_cast<size_t>(100)));
        // Ensure null termination for safety
        std::string name_str(name_view);
        pugi::xml_attribute found_attr = root.attribute(name_str.c_str());
        
        // Test 4: find_attribute with predicate
        auto predicate = [](pugi::xml_attribute attr) {
            return attr.name() != nullptr;
        };
        pugi::xml_attribute pred_attr = root.find_attribute(predicate);
        
        // Test 5: insert_attribute_after
        if (!first_attr.empty()) {
            // Use a simple name for the new attribute
            pugi::xml_attribute new_attr = root.insert_attribute_after("test_attr", first_attr);
            if (new_attr) {
                new_attr.set_value("test_value");
            }
        }
        
        // Test 6: insert_copy_before
        if (!first_attr.empty() && !last_attr.empty()) {
            pugi::xml_attribute copied_attr = root.insert_copy_before(first_attr, last_attr);
            if (copied_attr) {
                // Try to modify the copied attribute
                copied_attr.set_value("copied_value");
            }
        }
        
        // Additional stress test: try with empty node
        pugi::xml_node empty_node;
        pugi::xml_attribute empty_first = empty_node.first_attribute();
        pugi::xml_attribute empty_last = empty_node.last_attribute();
        pugi::xml_attribute empty_attr = empty_node.attribute("test");
        pugi::xml_attribute empty_pred = empty_node.find_attribute(predicate);
        
        // Try insert operations on empty node (should return empty attributes)
        pugi::xml_attribute dummy_attr;
        pugi::xml_attribute inserted_after = empty_node.insert_attribute_after("dummy", dummy_attr);
        pugi::xml_attribute copied_before = empty_node.insert_copy_before(dummy_attr, dummy_attr);
        
        // Cleanup is handled by RAII when doc goes out of scope
    } catch (...) {
        // Catch any unexpected exceptions
        delete doc;
        return 0;
    }
    
    delete doc;
    return 0;
}