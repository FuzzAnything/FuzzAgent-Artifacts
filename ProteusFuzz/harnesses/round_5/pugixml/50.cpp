// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <cstddef>
#include <string_view>
#include <fstream>
#include <sstream>

static std::string generate_xml_from_input(const uint8_t* data, size_t size) {
    std::ostringstream oss;
    oss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root>";
    for (size_t i = 0; i < size; ++i) {
        uint8_t byte = data[i];
        if (byte >= 32 && byte < 127 && byte != '<' && byte != '>' && byte != '&' && byte != '"' && byte != '\'') {
            oss << static_cast<char>(byte);
        } else if (byte == '\n' || byte == '\r' || byte == '\t') {
            oss << static_cast<char>(byte);
        } else {
            oss << "&#";
            oss << static_cast<int>(byte);
            oss << ";";
        }
    }
    oss << "</root>";
    return oss.str();
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    std::string xml_content = generate_xml_from_input(Data, Size);

    // Load XML from string
    if (!doc.load_string(xml_content.c_str())) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // Sequence of calls as specified
    // 1-8: pugi::xml_node::child
    pugi::xml_node n1 = root.child("a");
    pugi::xml_node n2 = n1.child("b");
    pugi::xml_node n3 = n2.child("c");
    pugi::xml_node n4 = n3.child("d");
    pugi::xml_node n5 = n4.child("e");
    pugi::xml_node n6 = n5.child("f");
    pugi::xml_node n7 = n6.child("g");
    pugi::xml_node n8 = n7.child("h");

    // 9-12: pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_named_node_iterator> r1 = root.children("item");
    pugi::xml_object_range<pugi::xml_named_node_iterator> r2 = n1.children("item");
    pugi::xml_object_range<pugi::xml_named_node_iterator> r3 = n2.children("item");
    pugi::xml_object_range<pugi::xml_named_node_iterator> r4 = n3.children("item");

    // 13: pugi::xml_object_range::end
    auto e1 = r1.end();

    // 14: pugi::xml_node::first_child
    pugi::xml_node fc1 = root.first_child();

    // 15-17: pugi::xml_object_range::end
    auto e2 = r2.end();
    auto e3 = r3.end();
    auto e4 = r4.end();

    // 18: pugi::xml_node::first_child
    pugi::xml_node fc2 = n1.first_child();

    // 19: pugi::xml_node::last_child
    pugi::xml_node lc1 = root.last_child();

    // 20-25: pugi::xml_object_range::end
    auto e5 = r1.end();
    auto e6 = r2.end();
    auto e7 = r3.end();
    auto e8 = r4.end();
    auto e9 = r1.end();
    auto e10 = r2.end();

    // 26: pugi::xml_node::first_child
    pugi::xml_node fc3 = n2.first_child();

    // 27-28: pugi::xml_object_range::end
    auto e11 = r3.end();
    auto e12 = r4.end();

    // 29: pugi::xml_object_range::begin
    auto b1 = r1.begin();

    // 30-31: pugi::xml_object_range::end
    auto e13 = r2.end();
    auto e14 = r3.end();

    // 32: pugi::xml_object_range::begin
    auto b2 = r4.begin();

    // Suppress unused variable warnings
    (void)e1; (void)e2; (void)e3; (void)e4; (void)e5; (void)e6; (void)e7; (void)e8;
    (void)e9; (void)e10; (void)e11; (void)e12; (void)e13; (void)e14;
    (void)b1; (void)b2;
    (void)fc1; (void)fc2; (void)fc3;
    (void)lc1;
    (void)n1; (void)n2; (void)n3; (void)n4; (void)n5; (void)n6; (void)n7; (void)n8;
    (void)r1; (void)r2; (void)r3; (void)r4;

    return 0;
}