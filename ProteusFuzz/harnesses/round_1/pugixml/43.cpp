// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <string_view>

static bool try_parse_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    // Ensure the input is null-terminated for parsing
    std::string buffer(reinterpret_cast<const char*>(data), size);
    if (buffer.back() != '\0') {
        buffer += '\0';
    }
    pugi::xml_parse_result result = doc.load_string(buffer.c_str());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Prepare environment: parse the input as XML
    if (!try_parse_document(doc, Data, Size)) {
        // If parsing fails, try to create a minimal valid document manually
        doc.append_child("root");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root found, create one
        root = doc.append_child("root");
    }

    // Ensure root is an element node
    if (root.type() != pugi::node_element) {
        return 0;
    }

    // Helper to get attribute name from fuzz data safely
    auto get_attr_name = [&](size_t offset, size_t len) -> const char* {
        if (offset >= Size) return "attr";
        size_t actual_len = (offset + len > Size) ? (Size - offset) : len;
        if (actual_len == 0) return "attr";
        // Ensure null-termination if not present
        std::string s(reinterpret_cast<const char*>(Data + offset), actual_len);
        if (s.back() != '\0') s += '\0';
        static std::string static_buf;
        static_buf = s;
        return static_buf.c_str();
    };

    // 1. pugi::xml_node::prepend_attribute (char_t* overload)
    {
        const char* name1 = get_attr_name(0, 10);
        root.prepend_attribute(name1);
    }

    // 2. pugi::xml_node::prepend_attribute (char_t* overload)
    {
        const char* name2 = get_attr_name(10, 10);
        root.prepend_attribute(name2);
    }

    // 3. pugi::xml_node::child
    {
        const char* child_name1 = get_attr_name(20, 10);
        pugi::xml_node child1 = root.child(child_name1);
        (void)child1; // Use the node to avoid unused warnings
    }

    // 4. pugi::xml_node::prepend_attribute (string_view_t overload)
    {
        size_t offset = 30;
        size_t len = 10;
        if (offset >= Size) len = 0;
        else len = (offset + len > Size) ? (Size - offset) : len;
        
        std::string_view sv;
        if (len > 0) {
            sv = std::string_view(reinterpret_cast<const char*>(Data + offset), len);
        } else {
            sv = std::string_view("attr");
        }
        root.prepend_attribute(sv);
    }

    // 5. pugi::xml_node::child
    {
        const char* child_name2 = get_attr_name(40, 10);
        pugi::xml_node child2 = root.child(child_name2);
        (void)child2;
    }

    // 6. pugi::xml_node::prepend_attribute (char_t* overload)
    {
        const char* name3 = get_attr_name(50, 10);
        root.prepend_attribute(name3);
    }

    // 7. pugi::xml_node::child
    {
        const char* child_name3 = get_attr_name(60, 10);
        pugi::xml_node child3 = root.child(child_name3);
        (void)child3;
    }

    // 8. pugi::xml_node::child
    {
        const char* child_name4 = get_attr_name(70, 10);
        pugi::xml_node child4 = root.child(child_name4);
        (void)child4;
    }

    // 9. pugi::xml_node::prepend_attribute (string_view_t overload)
    {
        size_t offset = 80;
        size_t len = 10;
        if (offset >= Size) len = 0;
        else len = (offset + len > Size) ? (Size - offset) : len;
        
        std::string_view sv;
        if (len > 0) {
            sv = std::string_view(reinterpret_cast<const char*>(Data + offset), len);
        } else {
            sv = std::string_view("attr");
        }
        root.prepend_attribute(sv);
    }

    // 10. pugi::xml_node::child
    {
        const char* child_name5 = get_attr_name(90, 10);
        pugi::xml_node child5 = root.child(child_name5);
        (void)child5;
    }

    // 11. pugi::xml_node::child
    {
        const char* child_name6 = get_attr_name(100, 10);
        pugi::xml_node child6 = root.child(child_name6);
        (void)child6;
    }

    // 12. pugi::xml_node::prepend_attribute (char_t* overload)
    {
        const char* name4 = get_attr_name(110, 10);
        root.prepend_attribute(name4);
    }

    return 0;
}