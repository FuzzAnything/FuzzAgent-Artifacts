// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string>
#include <string_view>

static bool try_set_name(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    
    // Try null-terminated version
    // Ensure null termination by creating a temporary buffer if needed
    std::string str(reinterpret_cast<const char*>(data), size);
    str.push_back('\0');
    
    bool result1 = node.set_name(str.c_str());
    
    // Try sized version
    bool result2 = node.set_name(str.c_str(), str.size() - 1);
    
    return result1 || result2;
}

static bool try_set_text(pugi::xml_text text, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    
    std::string str(reinterpret_cast<const char*>(data), size);
    str.push_back('\0');
    
    bool result1 = text.set(str.c_str());
    bool result2 = text.set(str.c_str(), str.size() - 1);
    
    return result1 || result2;
}

static bool try_set_value(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    
    std::string str(reinterpret_cast<const char*>(data), size);
    str.push_back('\0');
    
    bool result1 = node.set_value(str.c_str());
    bool result2 = node.set_value(str.c_str(), str.size() - 1);
    
    return result1 || result2;
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a simple XML document to work with
    pugi::xml_document doc;
    
    // Parse a minimal XML structure
    if (!doc.load_string("<root><element>text</element><pi target='value'/><comment>comment</comment><cdata><![CDATA[data]]></cdata></root>")) {
        return 0;
    }
    
    pugi::xml_node root = doc.child("root");
    if (!root) return 0;
    
    // Get various node types for testing
    pugi::xml_node element_node = root.child("element");
    pugi::xml_node pi_node = root.child("pi");
    pugi::xml_node comment_node = root.child("comment");
    pugi::xml_node cdata_node = root.child("cdata");
    
    // Test set_name on element node
    if (element_node) {
        try_set_name(element_node, Data, Size);
    }
    
    // Test set_name on PI node
    if (pi_node) {
        try_set_name(pi_node, Data, Size);
    }
    
    // Test set_value on element node (should fail for elements)
    if (element_node) {
        try_set_value(element_node, Data, Size);
    }
    
    // Test set_value on comment node
    if (comment_node) {
        try_set_value(comment_node, Data, Size);
    }
    
    // Test set_value on cdata node
    if (cdata_node) {
        try_set_value(cdata_node, Data, Size);
    }
    
    // Test set on text node
    pugi::xml_text text_node = element_node.text();
    if (text_node) {
        try_set_text(text_node, Data, Size);
    }
    
    // Also test with child text nodes
    pugi::xml_text pi_text = pi_node.text();
    if (pi_text) {
        try_set_text(pi_text, Data, Size);
    }
    
    return 0;
}