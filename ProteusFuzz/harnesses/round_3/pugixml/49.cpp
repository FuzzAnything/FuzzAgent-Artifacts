// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstring>
#include <string>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");

    // If parsing fails, we still try to exercise the API on empty nodes to test error handling
    pugi::xml_node root = result ? doc.root() : pugi::xml_node();

    // Helper lambda to safely get a child by name from fuzzed data
    auto get_child_name = [Data, Size](size_t offset) -> const char* {
        if (offset >= Size) return "";
        // Find a null terminator or use a fixed length to avoid reading out of bounds
        size_t len = 0;
        while (offset + len < Size && Data[offset + len] != 0 && len < 256) {
            len++;
        }
        static char buf[257];
        if (len == 0) {
            buf[0] = 'a';
            buf[1] = 0;
            return buf;
        }
        std::memcpy(buf, Data + offset, len);
        buf[len] = 0;
        return buf;
    };

    // Sequence 1: 8 calls to child()
    pugi::xml_node n1 = root.child(get_child_name(0));
    pugi::xml_node n2 = n1.child(get_child_name(1));
    pugi::xml_node n3 = n2.child(get_child_name(2));
    pugi::xml_node n4 = n3.child(get_child_name(3));
    pugi::xml_node n5 = n4.child(get_child_name(4));
    pugi::xml_node n6 = n5.child(get_child_name(5));
    pugi::xml_node n7 = n6.child(get_child_name(6));
    pugi::xml_node n8 = n7.child(get_child_name(7));

    // Sequence 2: 4 calls to children()
    pugi::xml_object_range<pugi::xml_named_node_iterator> r1 = n8.children(get_child_name(8));
    pugi::xml_object_range<pugi::xml_named_node_iterator> r2 = n8.children(get_child_name(9));
    pugi::xml_object_range<pugi::xml_named_node_iterator> r3 = n8.children(get_child_name(10));
    pugi::xml_object_range<pugi::xml_named_node_iterator> r4 = n8.children(get_child_name(11));

    // Sequence 3: 4 calls to end()
    auto e1 = r1.end();
    auto e2 = r2.end();
    auto e3 = r3.end();
    auto e4 = r4.end();

    // Sequence 4: 1 call to first_child()
    pugi::xml_node fc1 = n8.first_child();

    // Sequence 5: 3 calls to end()
    auto e5 = r1.end();
    auto e6 = r2.end();
    auto e7 = r3.end();

    // Sequence 6: 1 call to first_child()
    pugi::xml_node fc2 = n8.first_child();

    // Sequence 7: 1 call to last_child()
    pugi::xml_node lc1 = n8.last_child();

    // Sequence 8: 6 calls to end()
    auto e8 = r1.end();
    auto e9 = r2.end();
    auto e10 = r3.end();
    auto e11 = r4.end();
    auto e12 = r1.end();
    auto e13 = r2.end();

    // Sequence 9: 1 call to first_child()
    pugi::xml_node fc3 = n8.first_child();

    // Sequence 10: 2 calls to end()
    auto e14 = r1.end();
    auto e15 = r2.end();

    // Sequence 11: 1 call to begin()
    auto b1 = r1.begin();

    // Sequence 12: 2 calls to end()
    auto e16 = r1.end();
    auto e17 = r2.end();

    // Sequence 13: 1 call to begin()
    auto b2 = r2.begin();

    // Suppress unused variable warnings
    (void)e1; (void)e2; (void)e3; (void)e4; (void)e5; (void)e6; (void)e7;
    (void)e8; (void)e9; (void)e10; (void)e11; (void)e12; (void)e13;
    (void)e14; (void)e15; (void)e16; (void)e17;
    (void)b1; (void)b2;
    (void)n1; (void)n2; (void)n3; (void)n4; (void)n5; (void)n6; (void)n7; (void)n8;
    (void)fc1; (void)fc2; (void)fc3; (void)lc1;
    (void)r1; (void)r2; (void)r3; (void)r4;

    return 0;
}