// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Create XML document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create string views from fuzz data
    std::string_view data_view(reinterpret_cast<const char*>(Data), Size);
    
    // Split data into parts for different parameters
    // We'll use simple splitting to get different strings for different calls
    size_t part_size = Size / 10;
    if (part_size == 0) part_size = 1;
    
    // Create strings for node names and attributes
    std::string node_name1(data_view.substr(0, std::min(part_size, Size)).data(), 
                          std::min(part_size, Size));
    std::string node_name2(Size > part_size ? data_view.substr(part_size, std::min(part_size, Size - part_size)).data() : "node2",
                          Size > part_size ? std::min(part_size, Size - part_size) : 5);
    std::string attr_name1(Size > 2*part_size ? data_view.substr(2*part_size, std::min(part_size, Size - 2*part_size)).data() : "attr1",
                          Size > 2*part_size ? std::min(part_size, Size - 2*part_size) : 5);
    std::string attr_name2(Size > 3*part_size ? data_view.substr(3*part_size, std::min(part_size, Size - 3*part_size)).data() : "attr2",
                          Size > 3*part_size ? std::min(part_size, Size - 3*part_size) : 5);
    std::string attr_value1(Size > 4*part_size ? data_view.substr(4*part_size, std::min(part_size, Size - 4*part_size)).data() : "value1",
                           Size > 4*part_size ? std::min(part_size, Size - 4*part_size) : 6);
    std::string attr_value2(Size > 5*part_size ? data_view.substr(5*part_size, std::min(part_size, Size - 5*part_size)).data() : "value2",
                           Size > 5*part_size ? std::min(part_size, Size - 5*part_size) : 6);
    
    try {
        // First sequence: 4 append_child calls
        // 1. First overload with node_type
        pugi::xml_node child1 = root.append_child(pugi::node_element);
        
        // 2. Second overload with name
        pugi::xml_node child2 = root.append_child(node_name1.c_str());
        
        // 3. First overload with different node_type
        pugi::xml_node child3 = root.append_child(pugi::node_comment);
        
        // 4. Second overload with different name
        pugi::xml_node child4 = root.append_child(node_name2.c_str());
        
        // Second sequence: 3 append_attribute calls
        // 5. First append_attribute
        pugi::xml_attribute attr1 = root.append_attribute(attr_name1.c_str());
        if (attr1) {
            attr1.set_value(attr_value1.c_str());
        }
        
        // 6. Second append_attribute
        pugi::xml_attribute attr2 = root.append_attribute(attr_name2.c_str());
        if (attr2) {
            attr2.set_value(attr_value2.c_str());
        }
        
        // 7. Third append_attribute
        std::string attr_name3 = "attr3";
        pugi::xml_attribute attr3 = root.append_attribute(attr_name3.c_str());
        if (attr3) {
            attr3.set_value("static_value");
        }
        
        // Add attributes to children for find_child_by_attribute to potentially find
        if (child1) {
            pugi::xml_attribute child_attr1 = child1.append_attribute("find_attr");
            if (child_attr1) {
                child_attr1.set_value("find_value1");
            }
        }
        
        if (child2) {
            pugi::xml_attribute child_attr2 = child2.append_attribute("find_attr");
            if (child_attr2) {
                child_attr2.set_value("find_value2");
            }
        }
        
        // Third sequence: 8 find_child_by_attribute calls
        // 8-11. First overload (3 parameters) - 4 calls
        pugi::xml_node found1 = root.find_child_by_attribute("node", "find_attr", "find_value1");
        pugi::xml_node found2 = root.find_child_by_attribute(node_name1.c_str(), attr_name1.c_str(), attr_value1.c_str());
        pugi::xml_node found3 = root.find_child_by_attribute("", "find_attr", "find_value2");
        pugi::xml_node found4 = root.find_child_by_attribute(nullptr, "nonexistent", "value");
        
        // 12-15. Second overload (2 parameters) - 4 calls
        pugi::xml_node found5 = root.find_child_by_attribute("find_attr", "find_value1");
        pugi::xml_node found6 = root.find_child_by_attribute(attr_name1.c_str(), attr_value1.c_str());
        pugi::xml_node found7 = root.find_child_by_attribute("nonexistent", "value");
        pugi::xml_node found8 = root.find_child_by_attribute("", "");
        
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    return 0;
}