// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
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
#include <string>
#include <string_view>
#include <cstring>

static std::string create_xml_string(const uint8_t* data, size_t size) {
    std::string result;
    result.reserve(size + 100);
    result += "<root>";
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == '<' || data[i] == '>' || data[i] == '&' || data[i] == '"') {
            result += " ";
        } else {
            result += static_cast<char>(data[i]);
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    std::string xml_content = create_xml_string(Data, Size);

    if (!doc.load_string(xml_content.c_str())) {
        return 0;
    }

    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::prepend_child (const char_t*)
    pugi::xml_node node1 = root.prepend_child("child1");
    if (!node1) {
        return 0;
    }

    // 2. pugi::xml_node::child
    pugi::xml_node child1 = root.child("child1");
    if (!child1) {
        return 0;
    }

    // 3. pugi::xml_node::prepend_child (string_view_t)
    std::string_view sv_name = "child2";
    pugi::xml_node node2 = root.prepend_child(sv_name);
    if (!node2) {
        return 0;
    }

    // 4. pugi::xml_node::child
    pugi::xml_node child2 = root.child("child2");
    if (!child2) {
        return 0;
    }

    // 5. pugi::xml_node::child
    pugi::xml_node child3 = root.child("child1");
    if (!child3) {
        return 0;
    }

    // 6. pugi::xml_node::prepend_child (const char_t*)
    pugi::xml_node node3 = child3.prepend_child("grandchild1");
    if (!node3) {
        return 0;
    }

    // 7. pugi::xml_node::child
    pugi::xml_node grandchild1 = child3.child("grandchild1");
    if (!grandchild1) {
        return 0;
    }

    // 8. pugi::xml_node::prepend_child (string_view_t)
    std::string_view sv_name2 = "grandchild2";
    pugi::xml_node node4 = child3.prepend_child(sv_name2);
    if (!node4) {
        return 0;
    }

    // 9. pugi::xml_node::prepend_child (const char_t*)
    pugi::xml_node node5 = child3.prepend_child("grandchild3");
    if (!node5) {
        return 0;
    }

    return 0;
}