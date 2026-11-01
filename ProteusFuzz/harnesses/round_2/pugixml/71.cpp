// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
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
#include <vector>

static std::string extract_cstring(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    size_t len = 0;
    while (len < size && data[len] != 0) {
        len++;
    }
    return std::string(reinterpret_cast<const char*>(data), len);
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");

    // Generate attribute names from input data
    std::vector<std::string> names;
    size_t offset = 0;
    while (offset + 1 < Size) {
        std::string name = extract_cstring(Data + offset, Size - offset);
        if (!name.empty() && name.size() < 100) {
            names.push_back(name);
        }
        offset += name.size() + 1;
        if (offset >= Size) break;
    }

    if (names.empty()) {
        names.push_back("default_attr");
    }

    // Test prepend_attribute
    for (const auto& name : names) {
        root.prepend_attribute(name.c_str());
    }

    // Test insert_attribute_before
    pugi::xml_attribute hint_attr;
    if (root.attribute("default_attr")) {
        hint_attr = root.attribute("default_attr");
        for (const auto& name : names) {
            root.insert_attribute_before(name.c_str(), hint_attr);
        }
    }

    // Test attribute with hint
    pugi::xml_attribute search_hint;
    for (const auto& name : names) {
        root.attribute(name.c_str(), search_hint);
    }

    // Test insert_attribute_after
    if (hint_attr) {
        for (const auto& name : names) {
            root.insert_attribute_after(name.c_str(), hint_attr);
        }
    }

    // Prepare proto attribute for copy tests
    pugi::xml_attribute proto_attr = root.append_child("proto_node").append_attribute("proto_name");
    if (proto_attr) {
        proto_attr.set_value("proto_value");
    }

    // Test insert_copy_after
    if (hint_attr && proto_attr) {
        root.insert_copy_after(proto_attr, hint_attr);
    }

    // Test insert_copy_before
    if (hint_attr && proto_attr) {
        root.insert_copy_before(proto_attr, hint_attr);
    }

    return 0;
}