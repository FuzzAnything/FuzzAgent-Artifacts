// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
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

static void fuzz_node(pugi::xml_node& node, const uint8_t* data, size_t size) {
    // Ensure we have enough data for names
    if (size < 2) return;

    // Create a name from the input data
    std::string_view name_view(reinterpret_cast<const char*>(data), size > 10 ? 10 : size);
    std::string name_str(name_view);
    if (name_str.empty()) name_str = "test";
    const char* name = name_str.c_str();

    // append_child 1
    pugi::xml_node child1 = node.append_child(name);
    if (!child1) return;

    // append_child 2
    pugi::xml_node child2 = node.append_child(name);
    if (!child2) return;

    // Create text node for child2
    pugi::xml_text text2 = child2.text();
    if (!text2) return;

    // Invoke all 5 overloads of set on text2
    float f_val = 0.0f;
    if (size > 0) f_val = static_cast<float>(data[0]);
    text2.set(f_val);

    int i_val = 0;
    if (size > 1) i_val = static_cast<int>(data[1]);
    text2.set(i_val);

    double d_val = 0.0;
    if (size > 2) d_val = static_cast<double>(data[2]);
    text2.set(d_val);

    bool b_val = false;
    if (size > 3) b_val = (data[3] != 0);
    text2.set(b_val);

    unsigned int ui_val = 0;
    if (size > 4) ui_val = static_cast<unsigned int>(data[4]);
    text2.set(ui_val);

    // append_child 3
    pugi::xml_node child3 = node.append_child(name);
    if (!child3) return;

    // append_child 4
    pugi::xml_node child4 = node.append_child(name);
    if (!child4) return;

    // Create text node for child4
    pugi::xml_text text4 = child4.text();
    if (!text4) return;

    // Invoke all 5 overloads of set on text4
    text4.set(f_val);
    text4.set(i_val);
    text4.set(d_val);
    text4.set(b_val);
    text4.set(ui_val);

    // append_child 5
    pugi::xml_node child5 = node.append_child(name);
    if (!child5) return;

    // Create text node for child5
    pugi::xml_text text5 = child5.text();
    if (!text5) return;

    // Invoke all 5 overloads of set on text5
    text5.set(f_val);
    text5.set(i_val);
    text5.set(d_val);
    text5.set(b_val);
    text5.set(ui_val);

    // append_child 6
    pugi::xml_node child6 = node.append_child(name);
    if (!child6) return;

    // Create text node for child6
    pugi::xml_text text6 = child6.text();
    if (!text6) return;

    // Invoke all 5 overloads of set on text6
    text6.set(f_val);
    text6.set(i_val);
    text6.set(d_val);
    text6.set(b_val);
    text6.set(ui_val);

    // append_child 7
    pugi::xml_node child7 = node.append_child(name);
    if (!child7) return;

    // Create text node for child7
    pugi::xml_text text7 = child7.text();
    if (!text7) return;

    // Invoke all 5 overloads of set on text7
    text7.set(f_val);
    text7.set(i_val);
    text7.set(d_val);
    text7.set(b_val);
    text7.set(ui_val);
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    if (!root) return 0;

    fuzz_node(root, Data, Size);

    return 0;
}