// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

static std::string create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len = 100) {
    size_t len = size < max_len ? size : max_len;
    std::string str(reinterpret_cast<const char*>(data), len);
    if (len > 0 && str.back() != '\0') {
        str += '\0';
    }
    return str;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Create null-terminated XML string from fuzzer input
    std::string xml_string = create_null_terminated_string(Data, Size, 1000);
    
    // 1. load_string
    pugi::xml_parse_result result = doc.load_string(xml_string.c_str());
    
    // Only proceed with operations if parsing was successful
    if (result.status == pugi::status_ok) {
        // Get root node
        pugi::xml_node root = doc.first_child();
        
        // 2. child
        std::string child1_name = create_null_terminated_string(Data, Size, 50);
        pugi::xml_node child1 = root.child(child1_name.c_str());
        
        // 3. child (on child1)
        std::string child2_name = create_null_terminated_string(Data + 10, Size > 10 ? Size - 10 : 0, 50);
        pugi::xml_node child2 = child1.child(child2_name.c_str());
        
        // 4. set_name (on child2 if valid)
        if (child2) {
            std::string new_name = create_null_terminated_string(Data + 20, Size > 20 ? Size - 20 : 0, 50);
            child2.set_name(new_name.c_str());
        }
        
        // 5. attribute
        std::string attr1_name = create_null_terminated_string(Data + 30, Size > 30 ? Size - 30 : 0, 50);
        pugi::xml_attribute attr1 = child2.attribute(attr1_name.c_str());
        
        // 6. set_name (on attr1 if valid)
        if (attr1) {
            std::string new_attr_name = create_null_terminated_string(Data + 40, Size > 40 ? Size - 40 : 0, 50);
            attr1.set_name(new_attr_name.c_str());
        }
        
        // 7. attribute (another attribute)
        std::string attr2_name = create_null_terminated_string(Data + 50, Size > 50 ? Size - 50 : 0, 50);
        pugi::xml_attribute attr2 = child2.attribute(attr2_name.c_str());
        
        // 8. set_value (on attr2 if valid)
        if (attr2) {
            std::string attr_value = create_null_terminated_string(Data + 60, Size > 60 ? Size - 60 : 0, 100);
            attr2.set_value(attr_value.c_str());
        }
        
        // 9. child (on child2)
        std::string child3_name = create_null_terminated_string(Data + 70, Size > 70 ? Size - 70 : 0, 50);
        pugi::xml_node child3 = child2.child(child3_name.c_str());
        
        // 10. text
        pugi::xml_text text = child3.text();
        
        // 11. set (on text if valid)
        if (text) {
            std::string text_value = create_null_terminated_string(Data + 80, Size > 80 ? Size - 80 : 0, 100);
            text.set(text_value.c_str());
        }
        
        // 12. append_copy (copy child2 to child3 if both valid)
        if (child2 && child3) {
            child3.append_copy(child2);
        }
        
        // 13. child (on child3)
        std::string child4_name = create_null_terminated_string(Data + 90, Size > 90 ? Size - 90 : 0, 50);
        pugi::xml_node child4 = child3.child(child4_name.c_str());
        
        // 14. append_copy (copy child3 to child4 if both valid)
        if (child3 && child4) {
            child4.append_copy(child3);
        }
    }
    
    return 0;
}