// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
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
#include <string>
#include <functional>

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document with some attributes for testing
    const char* xml_content = 
        "<root attr1=\"value1\" attr2=\"value2\" attr3=\"value3\">"
        "<child attr4=\"value4\" attr5=\"value5\"/>"
        "</root>";
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content);
    if (!result) {
        return 0;
    }
    
    // Get the root node
    pugi::xml_node root = doc.root();
    if (root.empty()) {
        return 0;
    }
    
    // Create a string from the fuzzing input for attribute names
    std::string attr_name;
    if (Size > 0) {
        attr_name.assign(reinterpret_cast<const char*>(Data), 
                         std::min(Size, static_cast<size_t>(100)));
    } else {
        attr_name = "default";
    }
    
    // Test 1: xml_node::attribute
    pugi::xml_attribute attr = root.attribute(attr_name.c_str());
    
    // Test 2: xml_node::remove_attribute (if attribute exists)
    if (attr) {
        root.remove_attribute(attr);
    }
    
    // Test 3: xml_node::insert_attribute_before
    // Get a reference attribute to insert before
    pugi::xml_attribute first_attr = root.first_attribute();
    if (first_attr) {
        pugi::xml_attribute new_attr = root.insert_attribute_before(
            attr_name.c_str(), first_attr);
        if (new_attr) {
            new_attr.set_value("fuzz_value");
        }
    }
    
    // Test 4: xml_node::last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    
    // Test 5: xml_node::first_attribute
    pugi::xml_attribute first_attr_again = root.first_attribute();
    
    // Test 6: xml_node::find_attribute with a predicate
    static auto predicate = [](const pugi::xml_attribute& a) -> bool {
        return a && a.value() && std::strstr(a.value(), "value") != nullptr;
    };
    pugi::xml_attribute found_attr = root.find_attribute(predicate);
    
    // Also test on child node
    pugi::xml_node child = root.child("child");
    if (!child.empty()) {
        // Test attribute retrieval on child
        pugi::xml_attribute child_attr = child.attribute(attr_name.c_str());
        
        // Test remove on child if attribute exists
        if (child_attr) {
            child.remove_attribute(child_attr);
        }
        
        // Test insert on child
        pugi::xml_attribute child_first = child.first_attribute();
        if (child_first) {
            child.insert_attribute_before(attr_name.c_str(), child_first);
        }
        
        // Test last_attribute on child
        pugi::xml_attribute child_last = child.last_attribute();
        
        // Test first_attribute on child
        pugi::xml_attribute child_first_again = child.first_attribute();
        
        // Test find_attribute on child
        pugi::xml_attribute child_found = child.find_attribute(predicate);
    }
    
    // Create a new empty node and test functions on it
    pugi::xml_node empty_node;
    pugi::xml_attribute empty_attr = empty_node.attribute("test");
    pugi::xml_attribute empty_first = empty_node.first_attribute();
    pugi::xml_attribute empty_last = empty_node.last_attribute();
    pugi::xml_attribute empty_found = empty_node.find_attribute(predicate);
    
    // Test remove on empty node (should return false)
    pugi::xml_attribute dummy_attr;
    bool removed = empty_node.remove_attribute(dummy_attr);
    
    // Test insert on empty node (should return empty attribute)
    pugi::xml_attribute inserted = empty_node.insert_attribute_before(
        "test", dummy_attr);
    
    return 0;
}