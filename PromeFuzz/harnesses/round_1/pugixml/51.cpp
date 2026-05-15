// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create mutable buffer for load_buffer_inplace
    std::vector<char> buffer(Data, Data + Size);
    
    // Create document and parse XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.data(), buffer.size());
    
    // Get the document node (root)
    pugi::xml_node root = doc;
    
    // First append_child
    pugi::xml_node child1 = root.append_child("child1");
    
    // First append_copy - need an attribute to copy
    // Create an attribute on child1 first
    pugi::xml_attribute attr1 = child1.append_attribute("attr1");
    attr1.set_value("value1");
    
    // Now append_copy of this attribute to root
    pugi::xml_attribute copied_attr1 = root.append_copy(attr1);
    
    // first_child
    pugi::xml_node first_child = root.first_child();
    
    // first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    
    // Second append_child
    pugi::xml_node child2 = root.append_child("child2");
    
    // Second append_copy - try to copy the first attribute if it exists
    if (first_attr) {
        pugi::xml_attribute copied_attr2 = root.append_copy(first_attr);
    }
    
    // Second first_attribute call
    pugi::xml_attribute first_attr2 = root.first_attribute();
    
    // Additional operations to explore more states
    if (first_child) {
        // Try to append child to first_child
        pugi::xml_node grandchild = first_child.append_child("grandchild");
        
        // Try to copy attributes between nodes
        if (first_attr) {
            pugi::xml_attribute copied_to_child = first_child.append_copy(first_attr);
        }
        
        // Get first child of first_child
        pugi::xml_node first_grandchild = first_child.first_child();
        
        // Get first attribute of first_child
        pugi::xml_attribute first_child_attr = first_child.first_attribute();
    }
    
    // Try to create a new document and copy nodes between documents
    pugi::xml_document doc2;
    pugi::xml_node new_root = doc2.append_child("newroot");
    
    if (first_child) {
        pugi::xml_node copied_node = new_root.append_copy(first_child);
    }
    
    // Try different node names based on input data
    if (Size > 10) {
        std::string node_name(reinterpret_cast<const char*>(Data), std::min(Size, size_t(10)));
        pugi::xml_node dynamic_child = root.append_child(node_name.c_str());
    }
    
    return 0;
}