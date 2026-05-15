// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    // Create XML document and nodes for testing
    pugi::xml_document doc;
    
    // Test pugi::xml_node::set_value
    {
        pugi::xml_node node = doc.append_child(pugi::node_pcdata);
        if (Size > 0) {
            // Ensure null termination for safety
            std::string value(reinterpret_cast<const char*>(Data), Size);
            node.set_value(value.c_str(), value.size());
        } else {
            node.set_value("", 0);
        }
    }
    
    // Test pugi::xml_attribute::set_value overloads
    {
        pugi::xml_attribute attr = doc.append_attribute("test_attr");
        if (Size >= sizeof(unsigned long)) {
            unsigned long ul_val = *reinterpret_cast<const unsigned long*>(Data);
            attr.set_value(ul_val);
        }
        if (Size >= sizeof(long)) {
            long l_val = *reinterpret_cast<const long*>(Data);
            attr.set_value(l_val);
        }
    }
    
    // Test pugi::xml_node::set_name
    {
        pugi::xml_node node = doc.append_child(pugi::node_element);
        if (Size > 0) {
            std::string name(reinterpret_cast<const char*>(Data), Size);
            node.set_name(name.c_str(), name.size());
        } else {
            node.set_name("", 0);
        }
    }
    
    // Test pugi::xml_attribute::set_name overloads
    {
        pugi::xml_attribute attr = doc.append_attribute("initial_name");
        if (Size > 0) {
            std::string name(reinterpret_cast<const char*>(Data), Size);
            // Test overload with size parameter
            attr.set_name(name.c_str(), name.size());
            
            // Test overload without size parameter
            attr.set_name(name.c_str());
        } else {
            attr.set_name("", 0);
            attr.set_name("");
        }
    }
    
    // Additional test with different node types for set_value
    {
        pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
        if (Size > 0) {
            std::string comment_value(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)100));
            comment_node.set_value(comment_value.c_str(), comment_value.size());
        }
        
        pugi::xml_node pi_node = doc.append_child(pugi::node_pi);
        if (Size > 0) {
            std::string pi_value(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)50));
            pi_node.set_value(pi_value.c_str(), pi_value.size());
        }
    }
    
    // Test with empty attribute
    {
        pugi::xml_attribute empty_attr;
        if (Size >= sizeof(unsigned long)) {
            unsigned long ul_val = *reinterpret_cast<const unsigned long*>(Data);
            empty_attr.set_value(ul_val); // Should fail
        }
    }
    
    // Test with empty node
    {
        pugi::xml_node empty_node;
        if (Size > 0) {
            std::string value(reinterpret_cast<const char*>(Data), Size);
            empty_node.set_value(value.c_str(), value.size()); // Should fail
            empty_node.set_name(value.c_str(), value.size()); // Should fail
        }
    }
    
    return 0;
}