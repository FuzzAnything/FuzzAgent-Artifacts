// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    if (!doc.load_string("<root/>")) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::append_child (name overload)
    pugi::xml_node node1 = root.append_child("child1");

    // 2. pugi::xml_node::append_child (type overload)
    pugi::xml_node node2 = root.append_child(pugi::node_element);

    // 3. pugi::xml_node::child
    std::string_view sv = "child1";
    pugi::xml_node found1 = root.child(sv.data());

    // 4. pugi::xml_node::append_buffer
    pugi::xml_parse_result res1 = root.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);

    // 5. pugi::xml_node::child
    std::string_view sv2 = "child2";
    pugi::xml_node found2 = root.child(sv2.data());

    // 6. pugi::xml_node::child
    std::string_view sv3 = "child3";
    pugi::xml_node found3 = root.child(sv3.data());

    // 7. pugi::xml_node::remove_children
    bool removed = root.remove_children();

    // 8. pugi::xml_node::child
    std::string_view sv4 = "child4";
    pugi::xml_node found4 = root.child(sv4.data());

    // 9. pugi::xml_node::append_buffer
    write_dummy_file(Data, Size);
    pugi::xml_parse_result res2 = root.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_utf8);

    // 10. pugi::xml_node::child
    std::string_view sv5 = "child5";
    pugi::xml_node found5 = root.child(sv5.data());

    (void)res1;
    (void)res2;
    (void)removed;
    (void)found1;
    (void)found2;
    (void)found3;
    (void)found4;
    (void)found5;
    (void)node1;
    (void)node2;

    return 0;
}