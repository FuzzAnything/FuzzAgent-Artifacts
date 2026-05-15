// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data to work with
    if (Size < 1) {
        return 0;
    }

    // Create a null-terminated string from the input data for XML content
    std::string xml_content;
    if (Size > 0) {
        xml_content.assign(reinterpret_cast<const char*>(Data), Size);
    } else {
        xml_content = "<root></root>";
    }

    // 1. pugi::xml_document::load_string
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    
    // If loading fails, we can still proceed with an empty document
    // or create a minimal document structure for further operations
    if (!result) {
        // Create a minimal document to continue fuzzing
        doc.load_string("<root><child attr=\"value\">text</child></root>");
    }

    // Get the root node
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.append_child("root");
    }

    // 2. pugi::xml_node::child
    // Use a string from the input data for child name
    std::string child1_name;
    if (Size > 1) {
        child1_name = std::string(1, static_cast<char>(Data[0] % 26 + 'a'));
    } else {
        child1_name = "child1";
    }
    pugi::xml_node child1 = root.child(child1_name.c_str());
    
    // If child doesn't exist, create one
    if (!child1) {
        child1 = root.append_child(child1_name.c_str());
    }

    // 3. pugi::xml_node::child (second call)
    std::string child2_name;
    if (Size > 2) {
        child2_name = std::string(1, static_cast<char>(Data[1] % 26 + 'a'));
    } else {
        child2_name = "child2";
    }
    pugi::xml_node child2 = child1.child(child2_name.c_str());
    
    // If child doesn't exist, create one
    if (!child2) {
        child2 = child1.append_child(child2_name.c_str());
    }

    // 4. pugi::xml_node::set_name
    std::string new_node_name;
    if (Size > 3) {
        new_node_name = std::string(1, static_cast<char>(Data[2] % 26 + 'a'));
    } else {
        new_node_name = "newname";
    }
    if (child2) {
        child2.set_name(new_node_name.c_str());
    }

    // 5. pugi::xml_node::attribute
    std::string attr1_name;
    if (Size > 4) {
        attr1_name = std::string(1, static_cast<char>(Data[3] % 26 + 'a'));
    } else {
        attr1_name = "attr1";
    }
    pugi::xml_attribute attr1 = child2.attribute(attr1_name.c_str());
    
    // If attribute doesn't exist, create one
    if (!attr1) {
        attr1 = child2.append_attribute(attr1_name.c_str());
    }

    // 6. pugi::xml_attribute::set_name
    std::string new_attr_name;
    if (Size > 5) {
        new_attr_name = std::string(1, static_cast<char>(Data[4] % 26 + 'a'));
    } else {
        new_attr_name = "newattr";
    }
    if (attr1) {
        attr1.set_name(new_attr_name.c_str());
    }

    // 7. pugi::xml_node::attribute (second call)
    // Use the new attribute name
    pugi::xml_attribute attr2 = child2.attribute(new_attr_name.c_str());

    // 8. pugi::xml_attribute::set_value
    std::string attr_value;
    if (Size > 6) {
        // Use up to 10 characters from input for attribute value
        size_t value_len = std::min(static_cast<size_t>(10), Size - 6);
        attr_value.assign(reinterpret_cast<const char*>(Data + 6), value_len);
    } else {
        attr_value = "value";
    }
    if (attr2) {
        attr2.set_value(attr_value.c_str());
    }

    // 9. pugi::xml_node::child (third call)
    std::string child3_name;
    if (Size > 7) {
        child3_name = std::string(1, static_cast<char>(Data[7] % 26 + 'a'));
    } else {
        child3_name = "child3";
    }
    pugi::xml_node child3 = child2.child(child3_name.c_str());
    
    // If child doesn't exist, create one
    if (!child3) {
        child3 = child2.append_child(child3_name.c_str());
    }

    // 10. pugi::xml_node::text
    pugi::xml_text text_obj = child3.text();

    // 11. pugi::xml_text::set
    std::string text_value;
    if (Size > 8) {
        // Use up to 10 characters from input for text value
        size_t text_len = std::min(static_cast<size_t>(10), Size - 8);
        text_value.assign(reinterpret_cast<const char*>(Data + 8), text_len);
    } else {
        text_value = "some text";
    }
    if (text_obj) {
        text_obj.set(text_value.c_str());
    }

    // 12. pugi::xml_node::append_copy
    if (child3) {
        pugi::xml_node copied_node = root.append_copy(child3);
        (void)copied_node; // Avoid unused variable warning
    }

    // 13. pugi::xml_node::child (fourth call)
    std::string child4_name;
    if (Size > 9) {
        child4_name = std::string(1, static_cast<char>(Data[9] % 26 + 'a'));
    } else {
        child4_name = "child4";
    }
    pugi::xml_node child4 = root.child(child4_name.c_str());

    // 14. pugi::xml_node::append_copy (second call)
    if (child4) {
        pugi::xml_node copied_node2 = root.append_copy(child4);
        (void)copied_node2; // Avoid unused variable warning
    }

    return 0;
}