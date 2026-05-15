// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
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
#include <sstream>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create XML document and node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // Use first byte to determine node type for testing
    uint8_t node_type_selector = Data[0] % 9; // 9 possible node types
    
    // Create different node types based on input
    switch (node_type_selector) {
        case 0: node = doc.append_child(pugi::node_null); break;
        case 1: node = doc.append_child(pugi::node_document); break;
        case 2: node = doc.append_child(pugi::node_element); break;
        case 3: node = doc.append_child(pugi::node_pcdata); break;
        case 4: node = doc.append_child(pugi::node_cdata); break;
        case 5: node = doc.append_child(pugi::node_comment); break;
        case 6: node = doc.append_child(pugi::node_pi); break;
        case 7: node = doc.append_child(pugi::node_declaration); break;
        case 8: node = doc.append_child(pugi::node_doctype); break;
    }
    
    // Prepare string data from fuzzer input
    const char* str_data = reinterpret_cast<const char*>(Data);
    size_t str_len = Size;
    
    // Ensure we have a null-terminated string for some operations
    std::string temp_str;
    if (str_len > 0 && str_data[str_len - 1] != '\0') {
        temp_str.assign(str_data, str_len);
        str_data = temp_str.c_str();
    }
    
    // Test xml_node::set_value
    if (str_len > 0) {
        // Try both overloads
        node.set_value(str_data);
        if (str_len > 1) {
            node.set_value(str_data, str_len - 1);
        }
    }
    
    // Test xml_node::type
    pugi::xml_node_type node_type = node.type();
    (void)node_type; // Avoid unused variable warning
    
    // Test xml_node::print
    std::ostringstream oss;
    node.print(oss);
    
    // Test with different encoding options
    pugi::xml_encoding encoding_flags[] = {
        pugi::encoding_auto,
        pugi::encoding_utf8,
        pugi::encoding_utf16_le,
        pugi::encoding_utf16_be,
        pugi::encoding_utf16,
        pugi::encoding_utf32_le,
        pugi::encoding_utf32_be,
        pugi::encoding_utf32,
        pugi::encoding_wchar,
        pugi::encoding_latin1
    };
    
    for (auto encoding : encoding_flags) {
        std::ostringstream oss2;
        node.print(oss2, "\t", pugi::format_default, encoding);
    }
    
    // Test xml_node::name
    const char* node_name = node.name();
    (void)node_name;
    
    // Test xml_node::value
    const char* node_value = node.value();
    (void)node_value;
    
    // Test xml_node::set_name
    if (str_len > 0) {
        node.set_name(str_data);
    }
    
    // Create additional nodes to test more states
    pugi::xml_node child = node.append_child(pugi::node_element);
    if (str_len > 1) {
        child.set_name("child");
        child.set_value("child_value");
        
        // Test print on child node
        std::ostringstream oss3;
        child.print(oss3, "  ", pugi::format_indent);
    }
    
    // Test with attribute
    pugi::xml_attribute attr = node.append_attribute("attr");
    if (attr) {
        attr.set_value(str_data);
    }
    
    return 0;
}