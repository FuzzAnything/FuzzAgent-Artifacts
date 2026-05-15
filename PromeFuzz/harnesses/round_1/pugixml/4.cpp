// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
#include <vector>

static std::string make_null_terminated(const uint8_t* data, size_t size, size_t max_len = 256) {
    size_t len = size < max_len ? size : max_len;
    return std::string(reinterpret_cast<const char*>(data), len);
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* Data, size_t Size) {
    // Create null-terminated XML string from fuzzer input
    std::string xml_content = make_null_terminated(Data, Size, 4096);
    
    // Create document and load XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    
    // If parsing failed, we can still try some operations on empty document
    // Get root node (might be empty if parsing failed)
    pugi::xml_node root = doc.document_element();
    
    // 1. First child call
    std::string child1_name = make_null_terminated(Data, Size, 16);
    if (!child1_name.empty()) {
        child1_name += '\0'; // Ensure null termination
    }
    pugi::xml_node child1 = root.child(child1_name.c_str());
    
    // 2. Second child call
    std::string child2_name = make_null_terminated(Data + 16, Size > 16 ? Size - 16 : 0, 16);
    if (!child2_name.empty()) {
        child2_name += '\0';
    }
    pugi::xml_node child2 = child1.child(child2_name.c_str());
    
    // 3. Set name on child2
    std::string new_name = make_null_terminated(Data + 32, Size > 32 ? Size - 32 : 0, 32);
    if (!new_name.empty()) {
        new_name += '\0';
        child2.set_name(new_name.c_str());
    }
    
    // 4. Get attribute
    std::string attr1_name = make_null_terminated(Data + 64, Size > 64 ? Size - 64 : 0, 16);
    if (!attr1_name.empty()) {
        attr1_name += '\0';
    }
    pugi::xml_attribute attr1 = child2.attribute(attr1_name.c_str());
    
    // 5. Set attribute name
    std::string new_attr_name = make_null_terminated(Data + 80, Size > 80 ? Size - 80 : 0, 16);
    if (!new_attr_name.empty()) {
        new_attr_name += '\0';
        attr1.set_name(new_attr_name.c_str());
    }
    
    // 6. Get another attribute
    std::string attr2_name = make_null_terminated(Data + 96, Size > 96 ? Size - 96 : 0, 16);
    if (!attr2_name.empty()) {
        attr2_name += '\0';
    }
    pugi::xml_attribute attr2 = child2.attribute(attr2_name.c_str());
    
    // 7. Set attribute value
    std::string attr_value = make_null_terminated(Data + 112, Size > 112 ? Size - 112 : 0, 32);
    if (!attr_value.empty()) {
        attr_value += '\0';
        attr2.set_value(attr_value.c_str());
    }
    
    // 8. Get another child
    std::string child3_name = make_null_terminated(Data + 144, Size > 144 ? Size - 144 : 0, 16);
    if (!child3_name.empty()) {
        child3_name += '\0';
    }
    pugi::xml_node child3 = child2.child(child3_name.c_str());
    
    // 9. Get text and set it
    pugi::xml_text text = child3.text();
    std::string text_value = make_null_terminated(Data + 160, Size > 160 ? Size - 160 : 0, 64);
    if (!text_value.empty()) {
        text_value += '\0';
        text.set(text_value.c_str());
    }
    
    // 10. Append copy of child3 to child2
    if (child3) {
        child2.append_copy(child3);
    }
    
    // 11. Get another child for final operations
    std::string child4_name = make_null_terminated(Data + 224, Size > 224 ? Size - 224 : 0, 16);
    if (!child4_name.empty()) {
        child4_name += '\0';
    }
    pugi::xml_node child4 = child2.child(child4_name.c_str());
    
    // 12. Append copy of child3 to child4 (if both exist)
    if (child3 && child4) {
        child4.append_copy(child3);
    }
    
    return 0;
}