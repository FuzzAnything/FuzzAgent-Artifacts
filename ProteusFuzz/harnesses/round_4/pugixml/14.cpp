// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include <string_view>

static void fuzz_xml_text_set(pugi::xml_text& text, const uint8_t* data, size_t size) {
    if (size >= sizeof(int)) {
        int val_int = *reinterpret_cast<const int*>(data);
        text.set(val_int);
    }
    if (size >= sizeof(unsigned int) + 4) {
        unsigned int val_uint = *reinterpret_cast<const unsigned int*>(data + 4);
        text.set(val_uint);
    }
    if (size >= sizeof(float)) {
        float val_float = *reinterpret_cast<const float*>(data);
        text.set(val_float);
    }
    if (size >= sizeof(double)) {
        double val_double = *reinterpret_cast<const double*>(data);
        text.set(val_double);
    }
    if (size >= 1) {
        bool val_bool = data[0] != 0;
        text.set(val_bool);
    }
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    if (!root) return 0;

    // 1. append_child
    pugi::xml_node node1 = root.append_child("child1");
    if (!node1) return 0;

    // 2. append_child
    pugi::xml_node node2 = root.append_child("child2");
    if (!node2) return 0;

    // 3. xml_text::set (all 5 overloads)
    pugi::xml_text text1 = node1.text();
    fuzz_xml_text_set(text1, Data, Size);

    // 4. append_child
    pugi::xml_node node3 = root.append_child("child3");
    if (!node3) return 0;

    // 5. append_child
    pugi::xml_node node4 = root.append_child("child4");
    if (!node4) return 0;

    // 6. xml_text::set (all 5 overloads)
    pugi::xml_text text2 = node2.text();
    fuzz_xml_text_set(text2, Data, Size);

    // 7. append_child
    pugi::xml_node node5 = root.append_child("child5");
    if (!node5) return 0;

    // 8. xml_text::set (all 5 overloads)
    pugi::xml_text text3 = node3.text();
    fuzz_xml_text_set(text3, Data, Size);

    // 9. append_child
    pugi::xml_node node6 = root.append_child("child6");
    if (!node6) return 0;

    // 10. xml_text::set (all 5 overloads)
    pugi::xml_text text4 = node4.text();
    fuzz_xml_text_set(text4, Data, Size);

    // 11. append_child
    pugi::xml_node node7 = root.append_child("child7");
    if (!node7) return 0;

    // 12. xml_text::set (all 5 overloads)
    pugi::xml_text text5 = node5.text();
    fuzz_xml_text_set(text5, Data, Size);

    return 0;
}