// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::data at pugixml.cpp:7212:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::data at pugixml.cpp:7212:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        // If parsing fails, create a minimal valid XML document to test APIs
        static const char* minimal_xml = "<root><child>text</child></root>";
        result = doc.load_buffer(minimal_xml, std::strlen(minimal_xml));
        if (!result) {
            return 0;
        }
    }

    // Get root node
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.append_child("root");
        root.append_child("child").text().set("default");
    }

    // Prepare child name from fuzzer data
    char child_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    std::memcpy(child_name, Data, name_len);
    child_name[name_len] = '\0';

    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name);

    // 2. pugi::xml_node::text
    pugi::xml_text text_obj = child_node.text();

    // 3. pugi::xml_text::get
    const pugi::char_t* text_content = text_obj.get();
    (void)text_content; // Use result to avoid unused variable warning

    // 4. pugi::xml_text::set (first call)
    // Prepare set value from fuzzer data (use different portion if possible)
    char set_value1[256];
    size_t set_len1 = Size < 255 ? Size : 255;
    // Use middle portion of data if available
    size_t offset1 = Size > 256 ? Size - 256 : 0;
    if (offset1 + set_len1 > Size) set_len1 = Size - offset1;
    if (set_len1 > 0) {
        std::memcpy(set_value1, Data + offset1, set_len1);
    }
    set_value1[set_len1] = '\0';
    
    bool set_result1 = text_obj.set(std::string_view(set_value1));
    (void)set_result1;

    // 5. pugi::xml_text::data
    pugi::xml_node data_node = text_obj.data();
    (void)data_node;

    // 6. pugi::xml_text::set (second call)
    // Prepare another set value
    char set_value2[256];
    size_t set_len2 = Size < 255 ? Size : 255;
    // Use beginning portion again but modified
    if (set_len2 > 0) {
        std::memcpy(set_value2, Data, set_len2);
        // Modify slightly to get different value
        if (set_len2 > 0) set_value2[0] = set_value2[0] ^ 0x01;
    }
    set_value2[set_len2] = '\0';
    
    bool set_result2 = text_obj.set(std::string_view(set_value2));
    (void)set_result2;

    // 7. pugi::xml_node::last_child
    pugi::xml_node last_child = root.last_child();
    (void)last_child;

    // Also test with the document itself
    pugi::xml_node doc_child = doc.child("root");
    if (doc_child) {
        pugi::xml_node another_child = doc_child.child("dummy");
        pugi::xml_text another_text = another_child.text();
        const pugi::char_t* another_get = another_text.get();
        (void)another_get;
        
        bool another_set = another_text.set("fuzz");
        (void)another_set;
        
        pugi::xml_node another_data = another_text.data();
        (void)another_data;
        
        pugi::xml_node doc_last_child = doc.last_child();
        (void)doc_last_child;
    }

    return 0;
}