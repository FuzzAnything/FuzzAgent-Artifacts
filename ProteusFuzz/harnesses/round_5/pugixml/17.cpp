// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
#include <pugixml.hpp>
#include <string_view>

static pugi::xml_document* create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;
    
    // Prepare XML string from fuzz data, ensuring null termination
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    
    // Try to parse the document
    if (!doc->load_string(xml_str.c_str())) {
        // If parsing fails, try to create a minimal valid document with fuzz data as text
        xml_str = "<root><child>";
        xml_str.append(reinterpret_cast<const char*>(data), size);
        xml_str += "</child></root>";
        if (!doc->load_string(xml_str.c_str())) {
            delete doc;
            return nullptr;
        }
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_fuzz_data(Data, Size);
    if (!doc) return 0;

    try {
        pugi::xml_node root = doc->document_element();
        if (!root) {
            delete doc;
            return 0;
        }

        // 1. pugi::xml_node::child
        // Use fuzz data to generate a child name. 
        // Ensure it's a valid C-string by limiting size and null-terminating.
        pugi::char_t name_buf[256];
        size_t copy_len = (Size < 255) ? Size : 255;
        std::memcpy(name_buf, Data, copy_len);
        name_buf[copy_len] = '\0';
        
        pugi::xml_node child_node = root.child(name_buf);

        // 2. pugi::xml_node::text
        pugi::xml_text text_obj = child_node.text();

        // 3. pugi::xml_text::get
        const pugi::char_t* text_content = text_obj.get();
        (void)text_content; // Use the value to avoid unused variable warning

        // 4. pugi::xml_text::set
        // Prepare a string_view_t from fuzz data
        std::string_view fuzz_text(reinterpret_cast<const char*>(Data), Size);
        // Note: set() returns bool, we ignore it as per fuzzing goal of state exploration.
        text_obj.set(fuzz_text);

        // 5. pugi::xml_text::data
        pugi::xml_node data_node = text_obj.data();
        (void)data_node; // Use the value to avoid unused variable warning

        // 6. pugi::xml_text::set (Second invocation)
        // Modify the text again with a different portion or the same data
        std::string_view fuzz_text2(reinterpret_cast<const char*>(Data), Size);
        text_obj.set(fuzz_text2);

        // 7. pugi::xml_node::last_child
        pugi::xml_node last_child = root.last_child();
        (void)last_child; // Use the value to avoid unused variable warning

    } catch (...) {
        // Handle any unexpected exceptions
    }

    delete doc;
    return 0;
}