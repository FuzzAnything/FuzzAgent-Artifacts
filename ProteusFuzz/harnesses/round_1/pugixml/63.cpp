// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
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

static bool try_set_text_string(pugi::xml_text& text, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    std::string_view sv(reinterpret_cast<const char*>(data), size);
    return text.set(sv);
}

static bool try_set_text_bool(pugi::xml_text& text, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    bool val = (data[0] % 2 == 0);
    return text.set(val);
}

static bool try_set_attr_value(pugi::xml_attribute& attr, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    std::string_view sv(reinterpret_cast<const char*>(data), size);
    return attr.set_value(sv);
}

static bool try_set_node_name(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    std::string_view sv(reinterpret_cast<const char*>(data), size);
    return node.set_name(sv);
}

static bool try_set_node_value_sv(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    std::string_view sv(reinterpret_cast<const char*>(data), size);
    return node.set_value(sv);
}

static bool try_set_node_value_cstr(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    // Ensure null-termination for C-string overload
    std::string str(reinterpret_cast<const char*>(data), size);
    return node.set_value(str.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    if (!doc.load_string("<root><child>text</child></root>")) {
        return 0;
    }

    // Get nodes and attributes for testing
    pugi::xml_node root = doc.child("root");
    pugi::xml_node child = root.child("child");
    pugi::xml_attribute attr = child.attribute("test");
    
    // Create attribute if it doesn't exist to ensure valid target for set_value
    if (!attr) {
        attr = child.append_attribute("test");
    }

    // Prepare text object
    pugi::xml_text text = child.text();

    // Fuzz xml_text::set (string_view)
    if (child.type() == pugi::node_pcdata) {
        try_set_text_string(text, Data, Size);
    }

    // Fuzz xml_text::set (bool)
    try_set_text_bool(text, Data, Size);

    // Fuzz xml_attribute::set_value
    if (attr) {
        try_set_attr_value(attr, Data, Size);
    }

    // Fuzz xml_node::set_name
    if (root) {
        try_set_node_name(root, Data, Size);
    }
    if (child) {
        try_set_node_name(child, Data, Size);
    }

    // Fuzz xml_node::set_value (string_view)
    // Only valid for text nodes, comments, CDATA, etc.
    if (child.type() == pugi::node_pcdata || child.type() == pugi::node_comment || 
        child.type() == pugi::node_cdata || child.type() == pugi::node_pi || child.type() == pugi::node_doctype) {
        try_set_node_value_sv(child, Data, Size);
    }

    // Fuzz xml_node::set_value (const char_t*)
    if (child.type() == pugi::node_pcdata || child.type() == pugi::node_comment || 
        child.type() == pugi::node_cdata || child.type() == pugi::node_pi || child.type() == pugi::node_doctype) {
        try_set_node_value_cstr(child, Data, Size);
    }

    // Additional state exploration: try setting on root if it's a valid type
    if (root.type() == pugi::node_pcdata || root.type() == pugi::node_comment || 
        root.type() == pugi::node_cdata || root.type() == pugi::node_pi || root.type() == pugi::node_doctype) {
        try_set_node_value_sv(root, Data, Size);
        try_set_node_value_cstr(root, Data, Size);
    }

    return 0;
}