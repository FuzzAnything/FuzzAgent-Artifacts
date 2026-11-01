// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for load_string
    pugi::char_t *input_str = new pugi::char_t[Size + 1];
    std::memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(input_str);
    delete[] input_str;

    if (!result) {
        // Parsing failed, but we can still try to exercise some paths with an empty doc
        // However, most operations require valid nodes, so we proceed cautiously.
    }

    // 1. pugi::xml_node::child
    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        // Try to get child from document directly if root is not found
        root = doc.child(""); 
    }

    // 2. pugi::xml_node::child (second call)
    pugi::xml_node child1 = root.child("child1");

    // 3. pugi::xml_node::set_name
    // Only set name if node is not empty
    if (!child1.empty()) {
        child1.set_name("new_child_name");
    }

    // 4. pugi::xml_node::attribute
    pugi::xml_attribute attr1 = child1.attribute("attr1");

    // 5. pugi::xml_attribute::set_name
    if (!attr1.empty()) {
        attr1.set_name("new_attr_name");
    }

    // 6. pugi::xml_node::attribute (second call)
    pugi::xml_attribute attr2 = child1.attribute("attr2");

    // 7. pugi::xml_attribute::set_value
    if (!attr2.empty()) {
        attr2.set_value("new_value");
    }

    // 8. pugi::xml_node::child (third call)
    pugi::xml_node child2 = child1.child("child2");

    // 9. pugi::xml_node::text
    pugi::xml_text text_node = child2.text();

    // 10. pugi::xml_text::set
    if (!text_node.empty()) {
        text_node.set("new_text_content");
    }

    // 11. pugi::xml_node::append_copy
    // Try to append a copy of child1 to child2 if valid
    if (!child2.empty() && !child1.empty()) {
        pugi::xml_node copied_node = child2.append_copy(child1);
        (void)copied_node; // Suppress unused variable warning
    }

    // 12. pugi::xml_node::child (fourth call)
    pugi::xml_node child3 = child2.child("child3");

    // 13. pugi::xml_node::append_copy (second call)
    if (!child3.empty() && !child1.empty()) {
        pugi::xml_node copied_node2 = child3.append_copy(child1);
        (void)copied_node2;
    }

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}