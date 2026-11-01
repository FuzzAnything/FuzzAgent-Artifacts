// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    if (!doc.load_string("<root/>")) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::append_child
    pugi::xml_node node1 = root.append_child("child1");
    if (!node1) return 0;

    // 2. pugi::xml_node::append_child
    pugi::xml_node node2 = node1.append_child("child2");
    if (!node2) return 0;

    // 3. pugi::xml_text::set (invoke all 5 overloads)
    pugi::xml_text text1 = node2.text();
    // set(double)
    text1.set(3.14);
    // set(int)
    text1.set(42);
    // set(float)
    text1.set(2.71f);
    // set(bool)
    text1.set(true);
    // set(unsigned int)
    text1.set(static_cast<unsigned int>(100));

    // 4. pugi::xml_node::append_child
    pugi::xml_node node3 = root.append_child("child3");
    if (!node3) return 0;

    // 5. pugi::xml_node::append_child
    pugi::xml_node node4 = node3.append_child("child4");
    if (!node4) return 0;

    // 6. pugi::xml_text::set (invoke all 5 overloads)
    pugi::xml_text text2 = node4.text();
    text2.set(1.5);
    text2.set(-10);
    text2.set(0.0f);
    text2.set(false);
    text2.set(static_cast<unsigned int>(0));

    // 7. pugi::xml_node::append_child
    pugi::xml_node node5 = root.append_child("child5");
    if (!node5) return 0;

    // 8. pugi::xml_text::set (invoke all 5 overloads)
    pugi::xml_text text3 = node5.text();
    text3.set(999.99);
    text3.set(123);
    text3.set(45.6f);
    text3.set(true);
    text3.set(static_cast<unsigned int>(789));

    // 9. pugi::xml_node::append_child
    pugi::xml_node node6 = root.append_child("child6");
    if (!node6) return 0;

    // 10. pugi::xml_text::set (invoke all 5 overloads)
    pugi::xml_text text4 = node6.text();
    text4.set(-0.5);
    text4.set(-1);
    text4.set(-1.0f);
    text4.set(false);
    text4.set(static_cast<unsigned int>(255));

    // 11. pugi::xml_node::append_child
    pugi::xml_node node7 = root.append_child("child7");
    if (!node7) return 0;

    // 12. pugi::xml_text::set (invoke all 5 overloads)
    pugi::xml_text text5 = node7.text();
    text5.set(0.0);
    text5.set(0);
    text5.set(0.0f);
    text5.set(true);
    text5.set(static_cast<unsigned int>(0));

    // Optional: Use input data to write to dummy file if needed for other tests
    write_dummy_file(Data, Size);

    return 0;
}