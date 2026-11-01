// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for load_string
    std::string input(reinterpret_cast<const char*>(Data), Size);
    if (input.back() != '\0') {
        input += '\0';
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    
    // Proceed even if parsing fails, to test robustness
    if (doc.empty()) {
        return 0;
    }

    // 1. pugi::xml_node::child (First call in sequence)
    // We try to find a child named "root" or use the document root itself if it's an element
    pugi::xml_node node1 = doc.child("root");
    if (node1.empty()) {
        node1 = doc.first_child();
    }
    if (node1.empty()) {
        return 0;
    }

    // 2. pugi::xml_node::child (Second call in sequence)
    // Try to find a child of node1
    pugi::xml_node node2 = node1.child("child");
    if (node2.empty()) {
        node2 = node1.first_child();
    }
    if (node2.empty()) {
        return 0;
    }

    // 3. pugi::xml_node::set_name
    // Rename node2
    node2.set_name("renamed_node");

    // 4. pugi::xml_node::attribute
    // Get an attribute from node2
    pugi::xml_attribute attr1 = node2.attribute("attr1");
    if (attr1.empty()) {
        // If attribute doesn't exist, use the first attribute
        attr1 = node2.first_attribute();
        if (attr1.empty()) {
            // Create a new attribute to ensure we have one to modify
            attr1 = node2.append_attribute("new_attr");
        }
    }

    // 5. pugi::xml_attribute::set_name
    // Rename the attribute
    attr1.set_name("renamed_attr");

    // 6. pugi::xml_node::attribute (Second call)
    // Retrieve the attribute again by its new name
    pugi::xml_attribute attr2 = node2.attribute("renamed_attr");
    if (attr2.empty()) {
        // Fallback to first attribute if rename lookup failed
        attr2 = node2.first_attribute();
        if (attr2.empty()) {
            return 0;
        }
    }

    // 7. pugi::xml_attribute::set_value
    // Set the value of the attribute
    attr2.set_value("new_value");

    // 8. pugi::xml_node::child (Third call)
    // Get a child of node2 to work with text
    pugi::xml_node node3 = node2.child("text_node");
    if (node3.empty()) {
        node3 = node2.first_child();
    }
    if (node3.empty()) {
        // Create a child if none exists
        node3 = node2.append_child("text_node");
    }

    // 9. pugi::xml_node::text
    // Get the text object
    pugi::xml_text text_obj = node3.text();

    // 10. pugi::xml_text::set
    // Set the text content
    text_obj.set("fuzzed_text_content");

    // 11. pugi::xml_node::append_copy
    // Append a copy of node3 to node2
    pugi::xml_node copy1 = node2.append_copy(node3);

    // 12. pugi::xml_node::child (Fourth call)
    // Try to find the copied node (it might have the same name)
    pugi::xml_node node4 = node2.child("text_node");
    if (node4.empty()) {
        node4 = node2.last_child();
    }
    if (node4.empty()) {
        return 0;
    }

    // 13. pugi::xml_node::append_copy (Second call)
    // Append a copy of node4 to node2
    node2.append_copy(node4);

    return 0;
}