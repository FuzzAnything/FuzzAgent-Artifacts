// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    if (!doc.load_string("<root/>")) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // Helper to extract string from fuzzer data
    auto get_string = [&](size_t offset, size_t max_len) -> std::string {
        if (offset >= Size) return "";
        size_t len = 0;
        while (offset + len < Size && len < max_len && Data[offset + len] != 0) {
            len++;
        }
        return std::string(reinterpret_cast<const char*>(Data) + offset, len);
    };

    // Step 1: append_child (4 times)
    // Use name overload
    std::string name1 = get_string(0, 100);
    pugi::xml_node node1 = root.append_child(name1.empty() ? "child1" : name1.c_str());

    // Use type overload
    pugi::xml_node node2 = root.append_child(pugi::node_element);
    // Fix: node2.name() is a getter, use set_name to set the name
    if (node2) node2.set_name("child2");

    // Use name overload again
    std::string name3 = get_string(100, 100);
    pugi::xml_node node3 = root.append_child(name3.empty() ? "child3" : name3.c_str());

    // Use type overload again
    pugi::xml_node node4 = root.append_child(pugi::node_pcdata);

    // Step 2: append_attribute (3 times)
    std::string attrName1 = get_string(200, 50);
    pugi::xml_attribute attr1 = node1.append_attribute(attrName1.empty() ? "attr1" : attrName1.c_str());
    if (attr1) {
        std::string attrVal1 = get_string(250, 50);
        attr1.set_value(attrVal1.empty() ? "val1" : attrVal1.c_str());
    }

    std::string attrName2 = get_string(300, 50);
    pugi::xml_attribute attr2 = node1.append_attribute(attrName2.empty() ? "attr2" : attrName2.c_str());
    if (attr2) {
        std::string attrVal2 = get_string(350, 50);
        attr2.set_value(attrVal2.empty() ? "val2" : attrVal2.c_str());
    }

    std::string attrName3 = get_string(400, 50);
    pugi::xml_attribute attr3 = node1.append_attribute(attrName3.empty() ? "attr3" : attrName3.c_str());
    if (attr3) {
        std::string attrVal3 = get_string(450, 50);
        attr3.set_value(attrVal3.empty() ? "val3" : attrVal3.c_str());
    }

    // Step 3: find_child_by_attribute (8 times)
    // Overload 1: find_child_by_attribute(name, attr_name, attr_value)
    std::string findName1 = get_string(500, 50);
    std::string findAttr1 = get_string(550, 50);
    std::string findVal1 = get_string(600, 50);
    root.find_child_by_attribute(
        findName1.empty() ? "child1" : findName1.c_str(),
        findAttr1.empty() ? "attr1" : findAttr1.c_str(),
        findVal1.empty() ? "val1" : findVal1.c_str()
    );

    // Overload 2: find_child_by_attribute(attr_name, attr_value)
    std::string findAttr2 = get_string(650, 50);
    std::string findVal2 = get_string(700, 50);
    root.find_child_by_attribute(
        findAttr2.empty() ? "attr1" : findAttr2.c_str(),
        findVal2.empty() ? "val1" : findVal2.c_str()
    );

    // Overload 1
    std::string findName3 = get_string(750, 50);
    std::string findAttr3 = get_string(800, 50);
    std::string findVal3 = get_string(850, 50);
    root.find_child_by_attribute(
        findName3.empty() ? "child3" : findName3.c_str(),
        findAttr3.empty() ? "attr2" : findAttr3.c_str(),
        findVal3.empty() ? "val2" : findVal3.c_str()
    );

    // Overload 2
    std::string findAttr4 = get_string(900, 50);
    std::string findVal4 = get_string(950, 50);
    root.find_child_by_attribute(
        findAttr4.empty() ? "attr2" : findAttr4.c_str(),
        findVal4.empty() ? "val2" : findVal4.c_str()
    );

    // Overload 1
    std::string findName5 = get_string(1000, 50);
    std::string findAttr5 = get_string(1050, 50);
    std::string findVal5 = get_string(1100, 50);
    root.find_child_by_attribute(
        findName5.empty() ? "child1" : findName5.c_str(),
        findAttr5.empty() ? "attr3" : findAttr5.c_str(),
        findVal5.empty() ? "val3" : findVal5.c_str()
    );

    // Overload 2
    std::string findAttr6 = get_string(1150, 50);
    std::string findVal6 = get_string(1200, 50);
    root.find_child_by_attribute(
        findAttr6.empty() ? "attr3" : findAttr6.c_str(),
        findVal6.empty() ? "val3" : findVal6.c_str()
    );

    // Overload 1
    std::string findName7 = get_string(1250, 50);
    std::string findAttr7 = get_string(1300, 50);
    std::string findVal7 = get_string(1350, 50);
    root.find_child_by_attribute(
        findName7.empty() ? "nonexistent" : findName7.c_str(),
        findAttr7.empty() ? "nonexistent" : findAttr7.c_str(),
        findVal7.empty() ? "nonexistent" : findVal7.c_str()
    );

    // Overload 2
    std::string findAttr8 = get_string(1400, 50);
    std::string findVal8 = get_string(1450, 50);
    root.find_child_by_attribute(
        findAttr8.empty() ? "nonexistent" : findAttr8.c_str(),
        findVal8.empty() ? "nonexistent" : findVal8.c_str()
    );

    return 0;
}