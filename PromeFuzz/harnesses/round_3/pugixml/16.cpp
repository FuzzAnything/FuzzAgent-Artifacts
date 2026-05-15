// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::data at pugixml.cpp:7212:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
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
#include <string_view>

static void initialize_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        doc.load_string("<root><child>text</child><child2>text2</child2></root>");
    } else {
        std::string xml_content(reinterpret_cast<const char*>(data), size);
        doc.load_buffer(xml_content.c_str(), xml_content.size());
    }
}

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "";
    size_t remaining = size - offset;
    const char* str_start = reinterpret_cast<const char*>(data + offset);
    size_t str_len = 0;
    while (str_len < remaining && str_start[str_len] != '\0') {
        str_len++;
    }
    return std::string_view(str_start, str_len);
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* Data, size_t Size) {
    static pugi::xml_document doc;
    doc.reset();
    
    initialize_document(doc, Data, Size);
    
    pugi::xml_node root = doc.root();
    if (!root) return 0;
    
    std::string_view child_name = get_string_view(Data, Size, 0);
    std::string child_name_nullterm(child_name);
    if (!child_name_nullterm.empty()) {
        pugi::xml_node child_node = root.child(child_name_nullterm.c_str());
        
        if (child_node) {
            pugi::xml_text text_obj = child_node.text();
            
            const char* text_value = text_obj.get();
            (void)text_value;
            
            std::string_view set_value1 = get_string_view(Data, Size, child_name.size() + 1);
            if (!set_value1.empty()) {
                text_obj.set(set_value1);
            }
            
            pugi::xml_node data_node = text_obj.data();
            (void)data_node;
            
            std::string_view set_value2 = get_string_view(Data, Size, child_name.size() + set_value1.size() + 2);
            if (!set_value2.empty()) {
                text_obj.set(set_value2);
            }
            
            pugi::xml_node last_child = child_node.last_child();
            (void)last_child;
        }
    }
    
    return 0;
}