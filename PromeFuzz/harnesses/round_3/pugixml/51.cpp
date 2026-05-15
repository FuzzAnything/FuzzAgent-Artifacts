// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
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
#include <cstddef>
#include <sstream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize document and nodes
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Create initial child node to insert before
    pugi::xml_node target_node = root.append_child(pugi::node_element);
    
    // First insert_child_before
    pugi::xml_node_type type1 = static_cast<pugi::xml_node_type>(Data[0] % 9);
    pugi::xml_node new_node1 = root.insert_child_before(type1, target_node);
    
    // First first_child
    pugi::xml_node first_child1 = root.first_child();
    
    // append_attribute
    if (Size > 1) {
        size_t attr_len = Size % 100;
        if (attr_len > Size) attr_len = Size;
        std::string attr_name(reinterpret_cast<const char*>(Data), attr_len);
        if (!attr_name.empty()) {
            pugi::xml_attribute attr = root.append_attribute(attr_name.c_str());
            if (attr) {
                // Set attribute value if we have more data
                if (Size > attr_len + 1) {
                    size_t val_len = (Size - attr_len) % 100;
                    std::string attr_val(reinterpret_cast<const char*>(Data + attr_len), val_len);
                    attr.set_value(attr_val.c_str());
                }
            }
        }
    }
    
    // Second insert_child_before
    if (Size > 2) {
        pugi::xml_node_type type2 = static_cast<pugi::xml_node_type>(Data[1] % 9);
        pugi::xml_node new_node2 = root.insert_child_before(type2, target_node);
        
        // Second first_child
        pugi::xml_node first_child2 = root.first_child();
        
        // set_value
        if (first_child2 && Size > 3) {
            size_t val_len = (Size - 2) % 200;
            if (val_len > Size - 2) val_len = Size - 2;
            std::string node_val(reinterpret_cast<const char*>(Data + 2), val_len);
            if (!node_val.empty()) {
                first_child2.set_value(node_val.c_str());
            }
        }
    }
    
    // save to string stream
    try {
        std::stringstream ss;
        pugi::xml_writer_stream writer(ss);
        doc.save(writer, "  ", pugi::format_default, pugi::encoding_auto);
    } catch (...) {
        // Ignore exceptions during saving
    }
    
    return 0;
}