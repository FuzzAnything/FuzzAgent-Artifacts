// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
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
    // Create a dummy XML document with some structure
    static const char* xml_content = 
        "<?xml version=\"1.0\"?>"
        "<root>"
        "  <child1>Text1</child1>"
        "  <child2>Text2</child2>"
        "  <child3>Text3</child3>"
        "  <empty/>"
        "</root>";

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content);
    
    if (!result) {
        // If parsing fails, create a minimal document
        doc.append_child("root").append_child("child").text().set("default");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Use fuzzer data to create parameters for API calls
    // Ensure we have at least some data to work with
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string for child name from fuzzer data
    size_t name_len = Size > 100 ? 100 : Size;
    char child_name[101];
    memcpy(child_name, Data, name_len);
    child_name[name_len] = '\0';

    // Create string for set operations from fuzzer data
    const char* set_value = reinterpret_cast<const char*>(Data);
    size_t set_size = Size > 50 ? 50 : Size;

    // Sequence of API calls as specified
    // 1. child
    pugi::xml_node child_node = root.child(child_name);
    
    // 2. text
    pugi::xml_text child_text = child_node.text();
    
    // 3. set
    if (child_text) {
        child_text.set(set_value, set_size);
    }
    
    // 4. first_child
    pugi::xml_node first_child = child_node.first_child();
    
    // 5. set (on first_child's text)
    if (first_child) {
        pugi::xml_text first_child_text = first_child.text();
        if (first_child_text) {
            first_child_text.set(set_value, set_size);
        }
    }
    
    // 6. first_child (again)
    first_child = child_node.first_child();
    
    // 7. last_child
    pugi::xml_node last_child = child_node.last_child();
    
    // 8. set (on last_child's text)
    if (last_child) {
        pugi::xml_text last_child_text = last_child.text();
        if (last_child_text) {
            last_child_text.set(set_value, set_size);
        }
    }
    
    // 9. first_child (again)
    first_child = child_node.first_child();
    
    // 10. last_child (again)
    last_child = child_node.last_child();
    
    // 11. set (on last_child's text again)
    if (last_child) {
        pugi::xml_text last_child_text2 = last_child.text();
        if (last_child_text2) {
            last_child_text2.set(set_value, set_size);
        }
    }
    
    // 12. first_child (final)
    first_child = child_node.first_child();

    return 0;
}