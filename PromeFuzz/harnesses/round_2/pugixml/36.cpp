// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document with some structure
    pugi::xml_document doc;
    const char* xml_content = 
        "<root attr1='val1' attr2='val2' attr3='val3'>"
        "<child attr4='val4' attr5='val5'/>"
        "</root>";
    
    pugi::xml_parse_result result = doc.load_string(xml_content);
    if (!result) {
        return 0;
    }
    
    pugi::xml_node root = doc.root();
    if (root.empty()) {
        return 0;
    }
    
    // 1. Call first_child
    pugi::xml_node child = root.first_child();
    
    // Prepare some string data from fuzzer input
    std::string str_data;
    if (Size > 0) {
        str_data.assign(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(100)));
    } else {
        str_data = "default";
    }
    
    std::string_view str_view = str_data;
    
    // 2-8. Call attribute multiple times with both overloads
    // First overload (const char*)
    pugi::xml_attribute attr1 = child.attribute("attr4");
    pugi::xml_attribute attr2 = child.attribute("attr5");
    pugi::xml_attribute attr3 = child.attribute(str_data.c_str());
    
    // Second overload (string_view with hint)
    pugi::xml_attribute hint;
    pugi::xml_attribute attr4 = child.attribute(str_view, hint);
    pugi::xml_attribute attr5 = child.attribute("attr4", hint);
    pugi::xml_attribute attr6 = child.attribute("attr5", hint);
    pugi::xml_attribute attr7 = child.attribute(str_view, hint);
    pugi::xml_attribute attr8 = child.attribute(str_data.c_str(), hint);
    
    // 9. Call set_name on an attribute
    if (!attr1.empty()) {
        attr1.set_name(str_view);
    }
    
    // 10-13. Call attribute again multiple times
    pugi::xml_attribute attr9 = child.attribute(str_data.c_str());
    pugi::xml_attribute attr10 = child.attribute("attr4");
    pugi::xml_attribute attr11 = child.attribute(str_view, hint);
    pugi::xml_attribute attr12 = child.attribute("attr5", hint);
    
    return 0;
}