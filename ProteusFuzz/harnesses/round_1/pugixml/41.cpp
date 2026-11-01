// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
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
#include <string>
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    pugi::xml_document doc;
    if (!doc.load_string("<root/>")) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // Convert input data to string for potential use in names
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for C-string usage
    std::string safe_str = input_str;
    safe_str.push_back('\0');
    const char* c_str = safe_str.c_str();

    // 1. pugi::xml_node::prepend_child (overload 1: string_view_t)
    std::string_view sv_name("xml");
    pugi::xml_node node1 = root.prepend_child(sv_name);

    // 2. pugi::xml_node::child
    pugi::xml_node child1 = root.child("xml");

    // 3. pugi::xml_node::prepend_child (overload 2: const char_t*)
    pugi::xml_node node2 = child1.prepend_child(c_str);
    if (!node2) {
        // If prepend failed, try on root again to explore state
        node2 = root.prepend_child(c_str);
    }

    // 4. pugi::xml_node::child
    pugi::xml_node child2 = root.child(c_str);

    // 5. pugi::xml_node::child
    pugi::xml_node child3 = child2.child("xml");

    // 6. pugi::xml_node::prepend_child (overload 1: string_view_t)
    std::string_view sv_name2("xml");
    pugi::xml_node node3 = child3.prepend_child(sv_name2);

    // 7. pugi::xml_node::child
    pugi::xml_node child4 = node3.child("xml");

    // 8. pugi::xml_node::prepend_child (overload 2: const char_t*)
    pugi::xml_node node4 = child4.prepend_child(c_str);

    // 9. pugi::xml_node::prepend_child (overload 1: string_view_t)
    std::string_view sv_name3("xml");
    pugi::xml_node node5 = node4.prepend_child(sv_name3);

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}