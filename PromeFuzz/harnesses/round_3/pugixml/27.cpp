// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
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
#include <vector>

static std::string make_null_terminated(std::string_view sv) {
    return std::string(sv.data(), sv.length());
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Use data to create strings for node/attribute names and values
    std::string_view data_view(reinterpret_cast<const char*>(Data), Size);
    size_t pos = 0;
    
    auto get_string = [&](size_t len) -> std::string {
        if (pos + len > Size) len = Size - pos;
        std::string result(data_view.substr(pos, len));
        pos += len;
        return result;
    };
    
    // Create some child nodes using append_child
    std::vector<pugi::xml_node> children;
    
    // First append_child - with node type
    children.push_back(root.append_child(pugi::node_element));
    
    // Second append_child - with name
    std::string child_name1 = get_string(8);
    children.push_back(root.append_child(child_name1.c_str()));
    
    // Third append_child - with node type
    children.push_back(root.append_child(pugi::node_comment));
    
    // Fourth append_child - with name
    std::string child_name2 = get_string(8);
    children.push_back(root.append_child(child_name2.c_str()));
    
    // Append attributes to one of the children
    if (!children.empty()) {
        pugi::xml_node& target_child = children[0];
        
        // First append_attribute
        std::string attr_name1 = get_string(8);
        target_child.append_attribute(attr_name1.c_str()) = "value1";
        
        // Second append_attribute
        std::string attr_name2 = get_string(8);
        target_child.append_attribute(attr_name2.c_str()) = "value2";
        
        // Third append_attribute
        std::string attr_name3 = get_string(8);
        target_child.append_attribute(attr_name3.c_str()) = "value3";
        
        // Now find_child_by_attribute calls
        // First overload (3 parameters)
        std::string search_name1 = get_string(8);
        std::string search_attr_name1 = get_string(8);
        std::string search_attr_value1 = get_string(8);
        root.find_child_by_attribute(search_name1.c_str(), 
                                    search_attr_name1.c_str(), 
                                    search_attr_value1.c_str());
        
        // Second overload (2 parameters)
        std::string search_attr_name2 = get_string(8);
        std::string search_attr_value2 = get_string(8);
        root.find_child_by_attribute(search_attr_name2.c_str(), 
                                    search_attr_value2.c_str());
        
        // More find_child_by_attribute calls with various combinations
        for (int i = 0; i < 6; i++) {
            if (i % 2 == 0) {
                // Use 3-parameter version
                std::string name = get_string(4);
                std::string attr_name = get_string(4);
                std::string attr_value = get_string(4);
                root.find_child_by_attribute(name.c_str(), 
                                            attr_name.c_str(), 
                                            attr_value.c_str());
            } else {
                // Use 2-parameter version
                std::string attr_name = get_string(4);
                std::string attr_value = get_string(4);
                root.find_child_by_attribute(attr_name.c_str(), 
                                            attr_value.c_str());
            }
        }
    }
    
    return 0;
}