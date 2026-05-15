// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

static void initialize_node(pugi::xml_node& node) {
    // Create a document and append a comment node (which supports set_value)
    static pugi::xml_document doc;
    node = doc.append_child(pugi::node_comment);
}

static void initialize_attribute(pugi::xml_attribute& attr) {
    // Create a document with an element and append an attribute
    static pugi::xml_document doc;
    static bool initialized = false;
    if (!initialized) {
        doc.append_child(pugi::node_element);
        initialized = true;
    }
    attr = doc.first_child().append_attribute("initial");
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize xml_node and test set_value/set_name
    {
        pugi::xml_node node;
        initialize_node(node);
        
        // Test set_value with size parameter
        if (Size > 0) {
            node.set_value(reinterpret_cast<const pugi::char_t*>(Data), 
                          Size > 1024 ? 1024 : Size); // Limit size for safety
        }
        
        // Test set_name with size parameter
        if (Size > 0) {
            node.set_name(reinterpret_cast<const pugi::char_t*>(Data), 
                         Size > 1024 ? 1024 : Size);
        }
    }

    // Initialize xml_attribute and test set_value overloads
    {
        pugi::xml_attribute attr;
        initialize_attribute(attr);
        
        // Convert first 8 bytes to numeric values for set_value tests
        unsigned long ul_val = 0;
        long l_val = 0;
        
        if (Size >= sizeof(unsigned long)) {
            std::memcpy(&ul_val, Data, sizeof(unsigned long));
        } else {
            std::memcpy(&ul_val, Data, Size);
        }
        
        if (Size >= sizeof(long)) {
            std::memcpy(&l_val, Data, sizeof(long));
        } else {
            std::memcpy(&l_val, Data, Size);
        }
        
        // Test both set_value overloads
        attr.set_value(ul_val);
        attr.set_value(l_val);
        
        // Test set_name overloads
        if (Size > 0) {
            // Test set_name with size parameter
            attr.set_name(reinterpret_cast<const pugi::char_t*>(Data), 
                         Size > 1024 ? 1024 : Size);
            
            // Test set_name without size parameter (requires null-terminated string)
            // Create a null-terminated copy of the data
            size_t copy_size = Size > 1024 ? 1024 : Size;
            char* null_term_data = new char[copy_size + 1];
            std::memcpy(null_term_data, Data, copy_size);
            null_term_data[copy_size] = '\0';
            
            attr.set_name(reinterpret_cast<const pugi::char_t*>(null_term_data));
            delete[] null_term_data;
        }
    }

    // Test with empty/default constructed objects
    {
        pugi::xml_node empty_node;
        pugi::xml_attribute empty_attr;
        
        // These should return false for empty objects
        if (Size > 0) {
            empty_node.set_value(reinterpret_cast<const pugi::char_t*>(Data), 
                                Size > 1024 ? 1024 : Size);
            empty_node.set_name(reinterpret_cast<const pugi::char_t*>(Data), 
                               Size > 1024 ? 1024 : Size);
            
            // Create null-terminated string for set_name without size
            size_t copy_size = Size > 1024 ? 1024 : Size;
            char* null_term_data = new char[copy_size + 1];
            std::memcpy(null_term_data, Data, copy_size);
            null_term_data[copy_size] = '\0';
            
            empty_attr.set_name(reinterpret_cast<const pugi::char_t*>(null_term_data));
            delete[] null_term_data;
            
            empty_attr.set_name(reinterpret_cast<const pugi::char_t*>(Data), 
                               Size > 1024 ? 1024 : Size);
            
            // Convert to numeric values
            unsigned long ul_val = 0;
            long l_val = 0;
            
            if (Size >= sizeof(unsigned long)) {
                std::memcpy(&ul_val, Data, sizeof(unsigned long));
            } else {
                std::memcpy(&ul_val, Data, Size);
            }
            
            if (Size >= sizeof(long)) {
                std::memcpy(&l_val, Data, sizeof(long));
            } else {
                std::memcpy(&l_val, Data, Size);
            }
            
            empty_attr.set_value(ul_val);
            empty_attr.set_value(l_val);
        }
    }

    return 0;
}