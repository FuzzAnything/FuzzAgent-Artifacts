// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
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
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get root node to start operations
    pugi::xml_node root = doc.root();
    
    // Prepare child name string (null-terminated)
    static char child_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    if (name_len > 0) {
        std::memcpy(child_name, Data, name_len);
        child_name[name_len] = '\0';
    } else {
        child_name[0] = '\0';
    }
    
    // Prepare text data for set operations
    std::string_view text_data(reinterpret_cast<const char*>(Data), Size);
    
    // Sequence of API calls as specified
    pugi::xml_node child_node = root.child(child_name);
    pugi::xml_text text_obj = child_node.text();
    text_obj.set(text_data.data(), text_data.size());
    
    pugi::xml_node first_child1 = child_node.first_child();
    text_obj.set(text_data.data(), text_data.size());
    
    pugi::xml_node first_child2 = child_node.first_child();
    pugi::xml_node last_child1 = child_node.last_child();
    text_obj.set(text_data.data(), text_data.size());
    
    pugi::xml_node first_child3 = child_node.first_child();
    pugi::xml_node last_child2 = child_node.last_child();
    text_obj.set(text_data.data(), text_data.size());
    
    pugi::xml_node first_child4 = child_node.first_child();
    
    // Cleanup happens automatically when objects go out of scope
    return 0;
}