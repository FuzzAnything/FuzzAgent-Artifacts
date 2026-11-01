// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <pugixml.hpp>

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Limit size to prevent excessive allocation
    std::size_t safe_size = Size;
    if (safe_size > 1024 * 1024) {
        safe_size = 1024 * 1024;
    }
    
    // Create a temporary buffer to hold the XML string with a null terminator
    std::string xml_str(reinterpret_cast<const char*>(Data), safe_size);
    xml_str.push_back('\0');

    // 1. pugi::xml_document::load_string
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    // Proceed regardless of parse result to test API behavior on potentially invalid states

    // 2. pugi::xml_node::child (First call)
    pugi::xml_node node1 = doc.child("root");
    if (node1.empty()) {
        // Fallback to first child if "root" not found
        node1 = doc.first_child();
    }

    // 3. pugi::xml_node::child (Second call)
    pugi::xml_node node2 = node1.child("child");
    if (node2.empty()) {
        // Fallback to first child
        node2 = node1.first_child();
    }

    // 4. pugi::xml_node::set_name
    const char* new_name = "new_name";
    node2.set_name(new_name);

    // 5. pugi::xml_node::attribute
    pugi::xml_attribute attr1 = node2.attribute("attr1");

    // 6. pugi::xml_attribute::set_name
    const char* new_attr_name = "new_attr";
    attr1.set_name(new_attr_name);

    // 7. pugi::xml_node::attribute (Second call)
    pugi::xml_attribute attr2 = node2.attribute("new_attr");
    if (attr2.empty()) {
        attr2 = node2.attribute("id");
    }

    // 8. pugi::xml_attribute::set_value
    const char* attr_value = "test_value";
    attr2.set_value(attr_value);

    // 9. pugi::xml_node::child (Third call)
    pugi::xml_node node3 = node2.child("item");
    if (node3.empty()) {
        node3 = node2.first_child();
    }

    // 10. pugi::xml_node::text
    pugi::xml_text text_node = node3.text();

    // 11. pugi::xml_text::set
    const char* text_content = "text_content";
    text_node.set(text_content);

    // 12. pugi::xml_node::append_copy
    pugi::xml_node copied1 = node2.append_copy(node3);

    // 13. pugi::xml_node::child (Fourth call)
    pugi::xml_node node4 = node2.child("item");
    if (node4.empty()) {
        node4 = node2.first_child();
    }

    // 14. pugi::xml_node::append_copy (Second call)
    pugi::xml_node copied2 = node2.append_copy(node4);

    return 0;
}