// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <string>
#include <string_view>
#include <fstream>
#include "pugixml.hpp"

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file-based parsing
    writeDummyFile(Data, Size);
    
    // Parse XML from memory buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing succeeded, test the API functions
    if (result) {
        // Get the root node
        pugi::xml_node root = doc.root();
        
        // 1. Test last_attribute
        pugi::xml_attribute last_attr = root.last_attribute();
        
        // 2. Test insert_attribute_before
        if (!root.attributes().empty()) {
            // Get the first attribute as reference
            pugi::xml_attribute ref_attr = root.first_attribute();
            std::string_view name_str = "fuzz_attr";
            pugi::xml_attribute inserted = root.insert_attribute_before(name_str.data(), ref_attr);
            if (inserted) {
                inserted.set_value("fuzz_value");
            }
        }
        
        // 3. Test prepend_attribute
        std::string_view prepend_name = "prepend_attr";
        pugi::xml_attribute prepended = root.prepend_attribute(prepend_name.data());
        if (prepended) {
            prepended.set_value("prepend_value");
        }
        
        // 4. Test find_attribute with lambda predicate
        pugi::xml_attribute found = root.find_attribute([](pugi::xml_attribute attr) {
            return std::strlen(attr.name()) > 0;
        });
        
        // 5. Test attribute lookup by name
        std::string_view lookup_name = "test";
        pugi::xml_attribute looked_up = root.attribute(lookup_name.data());
        
        // 6. Test first_attribute
        pugi::xml_attribute first_attr = root.first_attribute();
        
        // Also test on child nodes
        for (pugi::xml_node child : root.children()) {
            // Test all functions on child nodes as well
            pugi::xml_attribute child_last = child.last_attribute();
            pugi::xml_attribute child_first = child.first_attribute();
            
            if (!child.attributes().empty()) {
                pugi::xml_attribute child_ref = child.first_attribute();
                std::string_view child_name = "child_attr";
                pugi::xml_attribute child_inserted = child.insert_attribute_before(child_name.data(), child_ref);
                
                pugi::xml_attribute child_prepended = child.prepend_attribute("child_prepend");
                
                pugi::xml_attribute child_found = child.find_attribute([](pugi::xml_attribute attr) {
                    return attr.as_string() != nullptr;
                });
                
                pugi::xml_attribute child_lookup = child.attribute("child_lookup");
            }
        }
    }
    
    // Test with empty node
    pugi::xml_node empty_node;
    pugi::xml_attribute empty_last = empty_node.last_attribute();
    pugi::xml_attribute empty_first = empty_node.first_attribute();
    pugi::xml_attribute empty_lookup = empty_node.attribute("nonexistent");
    pugi::xml_attribute empty_found = empty_node.find_attribute([](pugi::xml_attribute attr) { return true; });
    
    // Test prepend on empty node (should return empty attribute)
    pugi::xml_attribute empty_prepended = empty_node.prepend_attribute("empty_attr");
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}