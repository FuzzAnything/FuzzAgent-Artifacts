// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
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
#include <vector>
#include <cstring>
#include <cstddef>

static std::string generate_string(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        return "";
    }
    size_t len = 0;
    while (offset + len < size && data[offset + len] != 0 && len < 1024) {
        len++;
    }
    return std::string(reinterpret_cast<const char*>(data + offset), len);
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    pugi::xml_document doc;
    if (!doc.load_string("<root/>")) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // Helper to safely get string from fuzz data
    auto get_str = [&](size_t idx, size_t max_len = 256) -> std::string {
        if (idx >= Size) return "";
        size_t len = 0;
        while (idx + len < Size && len < max_len) {
            if (Data[idx + len] == 0) break;
            len++;
        }
        return std::string(reinterpret_cast<const char*>(Data + idx), len);
    };

    // 1. append_child (4 times)
    // Overload 1: append_child(xml_node_type)
    pugi::xml_node n1 = root.append_child(pugi::node_element);
    if (!n1) return 0;
    n1.append_attribute("id").set_value("1");

    // Overload 2: append_child(const char_t* name)
    std::string name2 = get_str(1, 50);
    pugi::xml_node n2 = root.append_child(name2.c_str());
    if (!n2) return 0;
    n2.append_attribute("id").set_value("2");

    // Overload 1: append_child(xml_node_type)
    pugi::xml_node n3 = root.append_child(pugi::node_element);
    if (!n3) return 0;
    n3.append_attribute("id").set_value("3");

    // Overload 2: append_child(const char_t* name)
    std::string name4 = get_str(51, 50);
    pugi::xml_node n4 = root.append_child(name4.c_str());
    if (!n4) return 0;
    n4.append_attribute("id").set_value("4");

    // 2. append_attribute (3 times)
    std::string attr_name1 = get_str(101, 30);
    std::string attr_val1 = get_str(131, 30);
    pugi::xml_attribute a1 = root.append_attribute(attr_name1.c_str());
    if (a1) a1.set_value(attr_val1.c_str());

    std::string attr_name2 = get_str(161, 30);
    std::string attr_val2 = get_str(191, 30);
    pugi::xml_attribute a2 = root.append_attribute(attr_name2.c_str());
    if (a2) a2.set_value(attr_val2.c_str());

    std::string attr_name3 = get_str(221, 30);
    std::string attr_val3 = get_str(251, 30);
    pugi::xml_attribute a3 = root.append_attribute(attr_name3.c_str());
    if (a3) a3.set_value(attr_val3.c_str());

    // 3. find_child_by_attribute (8 times)
    // Overload 1: find_child_by_attribute(attr_name, attr_value)
    std::string fa1_name = get_str(281, 20);
    std::string fa1_val = get_str(301, 20);
    pugi::xml_node res1 = root.find_child_by_attribute(fa1_name.c_str(), fa1_val.c_str());
    (void)res1;

    std::string fa2_name = get_str(321, 20);
    std::string fa2_val = get_str(341, 20);
    pugi::xml_node res2 = root.find_child_by_attribute(fa2_name.c_str(), fa2_val.c_str());
    (void)res2;

    std::string fa3_name = get_str(361, 20);
    std::string fa3_val = get_str(381, 20);
    pugi::xml_node res3 = root.find_child_by_attribute(fa3_name.c_str(), fa3_val.c_str());
    (void)res3;

    std::string fa4_name = get_str(401, 20);
    std::string fa4_val = get_str(421, 20);
    pugi::xml_node res4 = root.find_child_by_attribute(fa4_name.c_str(), fa4_val.c_str());
    (void)res4;

    // Overload 2: find_child_by_attribute(name, attr_name, attr_value)
    std::string fa5_tag = get_str(441, 20);
    std::string fa5_name = get_str(461, 20);
    std::string fa5_val = get_str(481, 20);
    pugi::xml_node res5 = root.find_child_by_attribute(fa5_tag.c_str(), fa5_name.c_str(), fa5_val.c_str());
    (void)res5;

    std::string fa6_tag = get_str(501, 20);
    std::string fa6_name = get_str(521, 20);
    std::string fa6_val = get_str(541, 20);
    pugi::xml_node res6 = root.find_child_by_attribute(fa6_tag.c_str(), fa6_name.c_str(), fa6_val.c_str());
    (void)res6;

    std::string fa7_tag = get_str(561, 20);
    std::string fa7_name = get_str(581, 20);
    std::string fa7_val = get_str(601, 20);
    pugi::xml_node res7 = root.find_child_by_attribute(fa7_tag.c_str(), fa7_name.c_str(), fa7_val.c_str());
    (void)res7;

    std::string fa8_tag = get_str(621, 20);
    std::string fa8_name = get_str(641, 20);
    std::string fa8_val = get_str(661, 20);
    pugi::xml_node res8 = root.find_child_by_attribute(fa8_tag.c_str(), fa8_name.c_str(), fa8_val.c_str());
    (void)res8;

    return 0;
}