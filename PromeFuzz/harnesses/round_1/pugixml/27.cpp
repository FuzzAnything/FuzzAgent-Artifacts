// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <vector>

static std::vector<std::string> split_by_null(const uint8_t* data, size_t size) {
    std::vector<std::string> strings;
    const char* start = reinterpret_cast<const char*>(data);
    const char* end = start + size;
    
    while (start < end) {
        const char* null_pos = static_cast<const char*>(std::memchr(start, '\0', end - start));
        if (null_pos == nullptr) {
            strings.emplace_back(start, end - start);
            break;
        }
        strings.emplace_back(start, null_pos - start);
        start = null_pos + 1;
    }
    
    return strings;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    std::vector<std::string> strings = split_by_null(Data, Size);
    if (strings.empty()) {
        return 0;
    }
    
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    size_t str_idx = 0;
    
    // First append_child (with name)
    if (str_idx < strings.size()) {
        root.append_child(strings[str_idx++].c_str());
    } else {
        root.append_child("child1");
    }
    
    // Second append_child (with name)
    if (str_idx < strings.size()) {
        root.append_child(strings[str_idx++].c_str());
    } else {
        root.append_child("child2");
    }
    
    // Third append_child (with type) - use node_element
    root.append_child(pugi::node_element);
    
    // Fourth append_child (with name)
    if (str_idx < strings.size()) {
        root.append_child(strings[str_idx++].c_str());
    } else {
        root.append_child("child4");
    }
    
    // First append_attribute
    if (str_idx < strings.size()) {
        root.append_attribute(strings[str_idx++].c_str());
    } else {
        root.append_attribute("attr1");
    }
    
    // Second append_attribute
    if (str_idx < strings.size()) {
        root.append_attribute(strings[str_idx++].c_str());
    } else {
        root.append_attribute("attr2");
    }
    
    // Third append_attribute
    if (str_idx < strings.size()) {
        root.append_attribute(strings[str_idx++].c_str());
    } else {
        root.append_attribute("attr3");
    }
    
    // Now find_child_by_attribute calls
    // First overload (2 arguments): find_child_by_attribute(attr_name, attr_value)
    if (str_idx + 1 < strings.size()) {
        root.find_child_by_attribute(strings[str_idx].c_str(), strings[str_idx + 1].c_str());
        str_idx += 2;
    } else {
        root.find_child_by_attribute("attr1", "value1");
    }
    
    if (str_idx + 1 < strings.size()) {
        root.find_child_by_attribute(strings[str_idx].c_str(), strings[str_idx + 1].c_str());
        str_idx += 2;
    } else {
        root.find_child_by_attribute("attr2", "value2");
    }
    
    if (str_idx + 1 < strings.size()) {
        root.find_child_by_attribute(strings[str_idx].c_str(), strings[str_idx + 1].c_str());
        str_idx += 2;
    } else {
        root.find_child_by_attribute("attr3", "value3");
    }
    
    if (str_idx + 1 < strings.size()) {
        root.find_child_by_attribute(strings[str_idx].c_str(), strings[str_idx + 1].c_str());
        str_idx += 2;
    } else {
        root.find_child_by_attribute("attr4", "value4");
    }
    
    // Second overload (3 arguments): find_child_by_attribute(name, attr_name, attr_value)
    if (str_idx + 2 < strings.size()) {
        root.find_child_by_attribute(strings[str_idx].c_str(), strings[str_idx + 1].c_str(), strings[str_idx + 2].c_str());
        str_idx += 3;
    } else {
        root.find_child_by_attribute("child1", "attr1", "value1");
    }
    
    if (str_idx + 2 < strings.size()) {
        root.find_child_by_attribute(strings[str_idx].c_str(), strings[str_idx + 1].c_str(), strings[str_idx + 2].c_str());
        str_idx += 3;
    } else {
        root.find_child_by_attribute("child2", "attr2", "value2");
    }
    
    if (str_idx + 2 < strings.size()) {
        root.find_child_by_attribute(strings[str_idx].c_str(), strings[str_idx + 1].c_str(), strings[str_idx + 2].c_str());
        str_idx += 3;
    } else {
        root.find_child_by_attribute("child3", "attr3", "value3");
    }
    
    if (str_idx + 2 < strings.size()) {
        root.find_child_by_attribute(strings[str_idx].c_str(), strings[str_idx + 1].c_str(), strings[str_idx + 2].c_str());
        str_idx += 3;
    } else {
        root.find_child_by_attribute("child4", "attr4", "value4");
    }
    
    return 0;
}