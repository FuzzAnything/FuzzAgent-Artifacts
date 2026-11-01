// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6305:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6270:34 in pugixml.hpp
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
#include <stdint.h>
#include <stdlib.h>

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    if (!root) {
        return 0;
    }

    // 1. append_child
    pugi::xml_node node1 = root.append_child(pugi::node_element);
    if (!node1) {
        return 0;
    }

    // 2. first_child
    pugi::xml_node child1 = root.first_child();

    // 3. first_child
    pugi::xml_node child2 = root.first_child();

    // 4. insert_child_after
    pugi::xml_node node2 = root.insert_child_after(pugi::node_element, child1);

    // 5. first_child
    pugi::xml_node child3 = root.first_child();

    // 6. insert_child_before
    pugi::xml_node node3 = root.insert_child_before(pugi::node_element, child2);

    // 7. first_child
    pugi::xml_node child4 = root.first_child();

    // 8. prepend_child
    pugi::xml_node node4 = root.prepend_child(pugi::node_element);

    // Cleanup is handled by RAII (xml_document destructor)
    (void)node1;
    (void)node2;
    (void)node3;
    (void)node4;
    (void)child1;
    (void)child2;
    (void)child3;
    (void)child4;

    return 0;
}