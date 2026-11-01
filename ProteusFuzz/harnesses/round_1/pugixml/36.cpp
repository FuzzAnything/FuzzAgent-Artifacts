// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size > 0) {
        std::string xml_str(reinterpret_cast<const char*>(data), size);
        doc.load_string(xml_str.c_str());
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
        if (!root) return 0;
    }

    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child("child");
    if (!child_node) {
        child_node = root.append_child("child");
        if (!child_node) return 0;
    }

    // 2. pugi::xml_node::append_child (first call)
    pugi::xml_node node1 = child_node.append_child("node1");
    if (!node1) return 0;

    // 3. pugi::xml_node::append_child (second call)
    pugi::xml_node node2 = child_node.append_child("node2");
    if (!node2) return 0;

    // 4. pugi::xml_text::set (unsigned long long)
    pugi::xml_text text1 = node1.text();
    text1.set(static_cast<unsigned long long>(Data[0] * 1000));

    // 5. pugi::xml_node::append_child (third call)
    pugi::xml_node node3 = child_node.append_child("node3");
    if (!node3) return 0;

    // 6. pugi::xml_node::append_child (fourth call)
    pugi::xml_node node4 = child_node.append_child("node4");
    if (!node4) return 0;

    // 7. pugi::xml_text::set (long long)
    pugi::xml_text text2 = node2.text();
    text2.set(static_cast<long long>(Data[0] * -1000));

    // Additional diversity: try setting text on node3 and node4 with different overloads if possible
    pugi::xml_text text3 = node3.text();
    text3.set(static_cast<unsigned long long>(Size));

    pugi::xml_text text4 = node4.text();
    text4.set(static_cast<long long>(Size));

    return 0;
}