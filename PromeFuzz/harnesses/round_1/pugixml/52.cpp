// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    if (!result) {
        return 0;
    }

    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Prepare child name - ensure null termination
    static char child_name[101];
    size_t name_len = Size < 100 ? Size : 100;
    if (name_len > 0) {
        std::memcpy(child_name, Data, name_len);
        child_name[name_len] = '\0';
    } else {
        child_name[0] = '\0';
    }

    // Prepare text data for set operations
    const char* text_data = reinterpret_cast<const char*>(Data);
    size_t text_size = Size;

    // Sequence of API calls
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name);
    
    // 2. pugi::xml_node::text
    pugi::xml_text text_obj = child_node.text();
    
    // 3. pugi::xml_text::set
    text_obj.set(text_data, text_size);
    
    // 4. pugi::xml_node::first_child
    pugi::xml_node first_child = child_node.first_child();
    
    // 5. pugi::xml_text::set (on first_child's text)
    if (first_child) {
        pugi::xml_text first_child_text = first_child.text();
        first_child_text.set(text_data, text_size);
    }
    
    // 6. pugi::xml_node::first_child (on first_child)
    pugi::xml_node first_child2 = first_child.first_child();
    
    // 7. pugi::xml_node::last_child (on first_child)
    pugi::xml_node last_child = first_child.last_child();
    
    // 8. pugi::xml_text::set (on last_child's text)
    if (last_child) {
        pugi::xml_text last_child_text = last_child.text();
        last_child_text.set(text_data, text_size);
    }
    
    // 9. pugi::xml_node::first_child (on last_child)
    pugi::xml_node first_child3 = last_child.first_child();
    
    // 10. pugi::xml_node::last_child (on first_child3)
    pugi::xml_node last_child2 = first_child3.last_child();
    
    // 11. pugi::xml_text::set (on last_child2's text)
    if (last_child2) {
        pugi::xml_text last_child2_text = last_child2.text();
        last_child2_text.set(text_data, text_size);
    }
    
    // 12. pugi::xml_node::first_child (on last_child2)
    pugi::xml_node first_child4 = last_child2.first_child();

    return 0;
}