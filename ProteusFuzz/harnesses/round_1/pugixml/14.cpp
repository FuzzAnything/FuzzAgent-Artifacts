// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6305:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, create a minimal document to proceed with API testing
        doc.append_child(pugi::node_element);
    }

    pugi::xml_node root = doc;
    if (!root) return 0;

    // 1. pugi::xml_node::append_child
    pugi::xml_node node1 = root.append_child(pugi::node_element);
    if (!node1) return 0;

    // 2. pugi::xml_node::first_child
    pugi::xml_node first1 = node1.first_child();

    // 3. pugi::xml_node::first_child (again on the same node)
    pugi::xml_node first2 = node1.first_child();

    // Prepare a sibling for insert_child_after
    pugi::xml_node sibling = root.append_child(pugi::node_element);
    if (!sibling) return 0;

    // 4. pugi::xml_node::insert_child_after
    pugi::xml_node nodeAfter = root.insert_child_after(pugi::node_element, sibling);
    if (!nodeAfter) return 0;

    // 5. pugi::xml_node::first_child
    pugi::xml_node first3 = nodeAfter.first_child();

    // Prepare a sibling for insert_child_before
    pugi::xml_node siblingBefore = root.append_child(pugi::node_element);
    if (!siblingBefore) return 0;

    // 6. pugi::xml_node::insert_child_before
    pugi::xml_node nodeBefore = root.insert_child_before(pugi::node_element, siblingBefore);
    if (!nodeBefore) return 0;

    // 7. pugi::xml_node::first_child
    pugi::xml_node first4 = nodeBefore.first_child();

    // 8. pugi::xml_node::prepend_child
    pugi::xml_node nodePrepend = root.prepend_child(pugi::node_element);
    if (!nodePrepend) return 0;

    return 0;
}