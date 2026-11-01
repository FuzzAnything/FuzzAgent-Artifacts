// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
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
#include <cstring>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to create a minimal document manually
        if (!doc.load_string("<xml/>")) {
            return 0;
        }
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Ensure we have a valid node to work with
    if (root.type() != pugi::node_element) {
        return 0;
    }

    // 1. pugi::xml_node::prepend_child (string_view_t)
    std::string_view a_str = "child1";
    pugi::xml_node n1 = root.prepend_child(a_str);
    if (!n1) return 0;

    // 2. pugi::xml_node::child
    pugi::xml_node c1 = root.child("child1");
    if (!c1) return 0;

    // 3. pugi::xml_node::prepend_child (const char_t*)
    pugi::xml_node n2 = c1.prepend_child("child2");
    if (!n2) return 0;

    // 4. pugi::xml_node::child
    pugi::xml_node c2 = c1.child("child2");
    if (!c2) return 0;

    // 5. pugi::xml_node::child
    pugi::xml_node c3 = root.child("child1");
    if (!c3) return 0;

    // 6. pugi::xml_node::prepend_child (string_view_t)
    std::string_view b_str = "child3";
    pugi::xml_node n3 = c3.prepend_child(b_str);
    if (!n3) return 0;

    // 7. pugi::xml_node::child
    pugi::xml_node c4 = c3.child("child3");
    if (!c4) return 0;

    // 8. pugi::xml_node::prepend_child (const char_t*)
    pugi::xml_node n4 = c4.prepend_child("child4");
    if (!n4) return 0;

    // 9. pugi::xml_node::prepend_child (string_view_t)
    std::string_view c_str = "child5";
    pugi::xml_node n5 = c4.prepend_child(c_str);
    if (!n5) return 0;

    return 0;
}