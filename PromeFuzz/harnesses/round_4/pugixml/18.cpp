// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
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
#include <vector>

static std::string make_null_terminated(const uint8_t* data, size_t size, size_t max_len = 100) {
    size_t len = size < max_len ? size : max_len;
    std::string result(reinterpret_cast<const char*>(data), len);
    if (!result.empty() && result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

static void populate_node(pugi::xml_node& node, const std::vector<std::string>& names) {
    for (size_t i = 0; i < names.size(); ++i) {
        if (!names[i].empty()) {
            node.append_attribute(names[i].c_str()) = "value";
            if (i % 2 == 0) {
                node.append_child(names[i].c_str());
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");

    std::vector<std::string> names;
    const uint8_t* ptr = Data;
    size_t remaining = Size;

    while (remaining > 0 && names.size() < 20) {
        size_t chunk = remaining > 10 ? 10 : remaining;
        names.push_back(make_null_terminated(ptr, chunk));
        ptr += chunk;
        remaining -= chunk;
    }

    populate_node(root, names);

    size_t name_idx = 0;
    auto get_name = [&]() -> const char* {
        if (name_idx >= names.size()) name_idx = 0;
        return names[name_idx++].c_str();
    };

    pugi::xml_attribute attr1, attr2;

    root.remove_attribute(get_name());
    root.remove_attribute(get_name());
    root.child(get_name());
    root.child(get_name());
    root.remove_attribute(get_name());
    root.remove_attribute(get_name());
    attr1 = root.attribute(get_name());
    root.remove_attribute(get_name());
    attr2 = root.attribute(get_name());
    root.remove_attribute(attr1);
    root.remove_attribute(attr2);
    root.remove_attribute(get_name());
    root.remove_attribute(get_name());

    return 0;
}