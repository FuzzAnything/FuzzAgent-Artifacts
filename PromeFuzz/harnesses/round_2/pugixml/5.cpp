// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data to work with
    if (Size == 0) {
        return 0;
    }
    
    // Create a null-terminated string from the fuzzer input
    std::string xml_string(reinterpret_cast<const char*>(Data), Size);
    
    // Step 1: Load XML document from string
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_string.c_str());
    
    // If parsing failed, we can still continue with an empty document
    // to test other API functions
    
    // Step 2: Get first child with name "child1"
    pugi::xml_node node1 = doc.child("child1");
    
    // Step 3: Get child of node1 with name "child2"
    pugi::xml_node node2 = node1.child("child2");
    
    // Step 4: Set name of node2
    // Use a portion of the fuzzer data for the new name
    size_t name_len = Size > 10 ? 10 : Size;
    std::string new_name(xml_string.substr(0, name_len));
    node2.set_name(new_name.c_str());
    
    // Step 5: Get attribute "attr1" from node2
    pugi::xml_attribute attr1 = node2.attribute("attr1");
    
    // Step 6: Set name of attr1
    std::string attr_name(xml_string.substr(0, std::min(name_len, Size)));
    attr1.set_name(attr_name.c_str());
    
    // Step 7: Get attribute "attr2" from node2
    pugi::xml_attribute attr2 = node2.attribute("attr2");
    
    // Step 8: Set value of attr2
    std::string attr_value(xml_string.substr(0, std::min(name_len, Size)));
    attr2.set_value(attr_value.c_str());
    
    // Step 9: Get child of node2 with name "child3"
    pugi::xml_node node3 = node2.child("child3");
    
    // Step 10: Get text object for node3
    pugi::xml_text text_obj = node3.text();
    
    // Step 11: Set text value
    std::string text_value(xml_string.substr(0, std::min(name_len, Size)));
    text_obj.set(text_value.c_str());
    
    // Step 12: Append copy of node3 to node2
    pugi::xml_node copied_node = node2.append_copy(node3);
    
    // Step 13: Get child of node2 with name "child4"
    pugi::xml_node node4 = node2.child("child4");
    
    // Step 14: Append copy of node4 to node2
    pugi::xml_node copied_node2 = node2.append_copy(node4);
    
    return 0;
}