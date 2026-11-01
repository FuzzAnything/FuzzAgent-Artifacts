// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string>
#include <cstring>

static std::string generate_xml_from_input(const uint8_t* data, size_t size) {
    // Create a safe XML string from input, ensuring it's well-formed enough for parsing
    std::string xml_str = "<root>";
    
    // Limit the amount of data to include to prevent excessive memory usage
    size_t max_len = std::min(size, static_cast<size_t>(10000));
    
    for (size_t i = 0; i < max_len; ++i) {
        unsigned char c = data[i];
        // Escape special XML characters
        if (c == '<') {
            xml_str += "&lt;";
        } else if (c == '>') {
            xml_str += "&gt;";
        } else if (c == '&') {
            xml_str += "&amp;";
        } else if (c == '"') {
            xml_str += "&quot;";
        } else if (c == '\'') {
            xml_str += "&apos;";
        } else if (c >= 32 && c < 127) {
            xml_str += static_cast<char>(c);
        } else {
            // Replace non-printable characters with a placeholder
            xml_str += "_";
        }
    }
    
    xml_str += "</root>";
    return xml_str;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Parse the generated XML
    pugi::xml_document doc;
    std::string xml_str = generate_xml_from_input(Data, Size);
    
    if (!doc.load_string(xml_str.c_str())) {
        // If parsing fails, try a minimal valid XML
        if (!doc.load_string("<root/>")) {
            return 0;
        }
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Ensure we have at least one attribute to work with
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    if (!attr1) {
        return 0;
    }
    attr1.set_value("value1");

    pugi::xml_attribute attr2 = root.append_attribute("attr2");
    if (!attr2) {
        return 0;
    }
    attr2.set_value("value2");

    // 1. insert_attribute_after
    pugi::xml_attribute new_attr_after = root.insert_attribute_after("new_attr_after", attr1);
    (void)new_attr_after; // Suppress unused variable warning

    // 2. insert_attribute_before
    pugi::xml_attribute new_attr_before = root.insert_attribute_before("new_attr_before", attr2);
    (void)new_attr_before;

    // 3. first_child (4 times as per requirement)
    pugi::xml_node child1 = root.first_child();
    (void)child1;
    
    pugi::xml_node child2 = root.first_child();
    (void)child2;
    
    pugi::xml_node child3 = root.first_child();
    (void)child3;
    
    pugi::xml_node child4 = root.first_child();
    (void)child4;

    // 4. append_copy
    pugi::xml_attribute appended_copy = root.append_copy(attr1);
    (void)appended_copy;

    // 5. prepend_copy
    pugi::xml_attribute prepended_copy = root.prepend_copy(attr2);
    (void)prepended_copy;

    // 6. insert_copy_after
    pugi::xml_attribute inserted_copy_after = root.insert_copy_after(attr1, attr2);
    (void)inserted_copy_after;

    // 7. insert_copy_before
    pugi::xml_attribute inserted_copy_before = root.insert_copy_before(attr2, attr1);
    (void)inserted_copy_before;

    return 0;
}