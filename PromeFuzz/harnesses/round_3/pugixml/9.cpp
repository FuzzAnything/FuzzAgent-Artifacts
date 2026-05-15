// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstddef>
#include <string>

static void initialize_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    // Create a simple XML structure with a root node and some attributes
    auto root = doc.append_child("root");
    
    // Add initial attributes using data from fuzzer input
    if (size > 0) {
        // Use first byte to determine number of initial attributes (max 5)
        size_t num_attrs = (data[0] % 5) + 1;
        size_t data_idx = 1;
        
        for (size_t i = 0; i < num_attrs && data_idx + 1 < size; i++) {
            // Create attribute name from data
            char name[2] = {static_cast<char>('a' + (data[data_idx] % 26)), '\0'};
            char value[2] = {static_cast<char>('0' + (data[data_idx + 1] % 10)), '\0'};
            
            root.append_attribute(name).set_value(value);
            data_idx += 2;
        }
    } else {
        // Default attributes if no data
        root.append_attribute("attr1").set_value("val1");
        root.append_attribute("attr2").set_value("val2");
    }
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* data, size_t size) {
    // Initialize document with fuzzer data
    pugi::xml_document doc;
    initialize_document(doc, data, size);
    
    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }
    
    // Get first attribute to use as reference
    pugi::xml_attribute ref_attr = root.first_attribute();
    if (!ref_attr) {
        return 0;
    }
    
    // Use std::string instead of string_view to ensure data persists
    std::string name1, name2;
    if (size > 0) {
        char name1_char = static_cast<char>('x' + (data[0] % 10));
        name1 = std::string(1, name1_char);
        
        char name2_char = size > 1 ? static_cast<char>('y' + (data[1] % 10)) : 'y';
        name2 = std::string(1, name2_char);
    } else {
        name1 = "new1";
        name2 = "new2";
    }
    
    try {
        // 1. insert_attribute_after
        pugi::xml_attribute attr1 = root.insert_attribute_after(name1.c_str(), ref_attr);
        
        // 2. insert_attribute_before
        pugi::xml_attribute attr2 = root.insert_attribute_before(name2.c_str(), ref_attr);
        
        // 3-6. first_child (called 4 times)
        pugi::xml_node child1 = root.first_child();
        pugi::xml_node child2 = root.first_child();
        pugi::xml_node child3 = root.first_child();
        pugi::xml_node child4 = root.first_child();
        
        // 7. append_copy
        if (attr1) {
            pugi::xml_attribute copied1 = root.append_copy(attr1);
        }
        
        // 8. prepend_copy
        if (ref_attr) {
            pugi::xml_attribute copied2 = root.prepend_copy(ref_attr);
        }
        
        // 9. insert_copy_after
        if (attr1 && attr2) {
            pugi::xml_attribute copied3 = root.insert_copy_after(attr1, attr2);
        }
        
        // 10. insert_copy_before
        if (ref_attr && attr1) {
            pugi::xml_attribute copied4 = root.insert_copy_before(ref_attr, attr1);
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}