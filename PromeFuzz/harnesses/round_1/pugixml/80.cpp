// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
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

static void traverse_and_test(pugi::xml_node node, const std::string& attr_name, const std::string& new_attr_name) {
    // Test first_attribute
    pugi::xml_attribute first_attr = node.first_attribute();
    
    // Test last_attribute
    pugi::xml_attribute last_attr = node.last_attribute();
    
    // Test attribute by name
    pugi::xml_attribute found_attr = node.attribute(attr_name.c_str());
    
    // Test insert_attribute_after and insert_attribute_before if we have a reference attribute
    if (first_attr) {
        // Insert after first attribute
        pugi::xml_attribute inserted_after = node.insert_attribute_after(new_attr_name.c_str(), first_attr);
        
        // If insertion succeeded, test remove_attribute
        if (inserted_after) {
            node.remove_attribute(inserted_after);
        }
        
        // Insert before first attribute
        pugi::xml_attribute inserted_before = node.insert_attribute_before(new_attr_name.c_str(), first_attr);
        
        // If insertion succeeded, test remove_attribute
        if (inserted_before) {
            node.remove_attribute(inserted_before);
        }
    }
    
    // Recursively traverse child nodes
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
        traverse_and_test(child, attr_name, new_attr_name);
    }
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_trim_pcdata);
    
    // If parsing failed, create a simple test document
    if (!result) {
        static const char* dummy_xml = 
            "<root attr1=\"value1\" attr2=\"value2\">"
            "<child attr3=\"value3\"/>"
            "</root>";
        doc.load_string(dummy_xml);
    }
    
    // Extract strings for attribute names from fuzz data
    std::string attr_name_str;
    std::string new_attr_name_str;
    
    if (Size > 0) {
        // Use first half of data for attr_name (or all if small)
        size_t split = Size / 2;
        if (split > 0) {
            // Create a properly terminated string
            attr_name_str.assign(reinterpret_cast<const char*>(Data), std::min(split, Size));
        }
        
        if (Size - split > 0) {
            // Create a properly terminated string
            new_attr_name_str.assign(reinterpret_cast<const char*>(Data + split), Size - split);
        }
    }
    
    // Use default names if no data available or strings are empty
    if (attr_name_str.empty()) {
        attr_name_str = "test_attr";
    }
    if (new_attr_name_str.empty()) {
        new_attr_name_str = "new_attr";
    }
    
    // Traverse document and test API functions
    traverse_and_test(doc, attr_name_str, new_attr_name_str);
    
    // Also test on empty nodes
    pugi::xml_node empty_node;
    empty_node.first_attribute();
    empty_node.last_attribute();
    empty_node.attribute(attr_name_str.c_str());
    
    // Test with various node types
    pugi::xml_node root = doc.root();
    if (root) {
        // Test first_attribute on root
        pugi::xml_attribute root_first = root.first_attribute();
        
        // Test attribute lookup with different names
        root.attribute("nonexistent");
        root.attribute("");
        
        // Test last_attribute
        pugi::xml_attribute root_last = root.last_attribute();
        
        // Test insert/remove operations if we have attributes
        if (root_first) {
            // Try to insert before and after existing attribute
            pugi::xml_attribute inserted = root.insert_attribute_before("fuzz_attr", root_first);
            if (inserted) {
                root.remove_attribute(inserted);
            }
            
            inserted = root.insert_attribute_after("fuzz_attr2", root_first);
            if (inserted) {
                root.remove_attribute(inserted);
            }
            
            // Try to remove the first attribute and re-add it
            if (root_first) {
                // Store name and value before removal
                const char* name = root_first.name();
                const char* value = root_first.value();
                
                if (name && value) {
                    // Remove and re-add
                    root.remove_attribute(root_first);
                    
                    // Add new attribute with same name
                    pugi::xml_attribute new_attr = root.append_attribute(name);
                    if (new_attr) {
                        new_attr.set_value(value);
                    }
                }
            }
        }
    }
    
    return 0;
}