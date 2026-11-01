// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string_view>

static const char* find_null_terminated_string(const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == '\0') {
            return reinterpret_cast<const char*>(data);
        }
    }
    return nullptr;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    if (!root) return 0;

    // 1. pugi::xml_node::child
    const char* name_str = find_null_terminated_string(Data, Size);
    if (!name_str) {
        pugi::xml_node child_node = root.child("test");
        (void)child_node;
    } else {
        pugi::xml_node child_node = root.child(name_str);
        (void)child_node;
    }

    // 2. pugi::xml_node::append_child (first call)
    const char* append_name1 = find_null_terminated_string(Data, Size);
    if (!append_name1) {
        append_name1 = "child1";
    }
    pugi::xml_node child1 = root.append_child(append_name1);
    if (!child1) return 0;

    // 3. pugi::xml_node::append_child (second call)
    const char* append_name2 = find_null_terminated_string(Data, Size);
    if (!append_name2) {
        append_name2 = "child2";
    }
    pugi::xml_node child2 = root.append_child(append_name2);
    if (!child2) return 0;

    // 4. pugi::xml_text::set (first overload: unsigned long long)
    pugi::xml_node text_node1 = child1.append_child(pugi::node_pcdata);
    if (text_node1) {
        unsigned long long num1 = 0;
        if (Size >= sizeof(unsigned long long)) {
            num1 = *reinterpret_cast<const unsigned long long*>(Data);
        } else if (Size > 0) {
            num1 = Data[0];
        }
        bool result1 = text_node1.text().set(num1);
        (void)result1;
    }

    // 5. pugi::xml_node::append_child (third call)
    const char* append_name3 = find_null_terminated_string(Data, Size);
    if (!append_name3) {
        append_name3 = "child3";
    }
    pugi::xml_node child3 = root.append_child(append_name3);
    if (!child3) return 0;

    // 6. pugi::xml_node::append_child (fourth call)
    const char* append_name4 = find_null_terminated_string(Data, Size);
    if (!append_name4) {
        append_name4 = "child4";
    }
    pugi::xml_node child4 = root.append_child(append_name4);
    if (!child4) return 0;

    // 7. pugi::xml_text::set (second overload: long long)
    pugi::xml_node text_node2 = child4.append_child(pugi::node_pcdata);
    if (text_node2) {
        long long num2 = 0;
        if (Size >= sizeof(long long)) {
            num2 = *reinterpret_cast<const long long*>(Data);
        } else if (Size > 0) {
            num2 = static_cast<long long>(Data[0]);
        }
        bool result2 = text_node2.text().set(num2);
        (void)result2;
    }

    return 0;
}