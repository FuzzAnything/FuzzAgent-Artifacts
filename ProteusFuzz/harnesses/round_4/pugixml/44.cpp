// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
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

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    if (size == 0) {
        doc->append_child("root");
        return doc;
    }

    std::string xml_str(reinterpret_cast<const char*>(data), size);
    if (xml_str.empty()) {
        doc->append_child("root");
        return doc;
    }

    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    if (!result) {
        delete doc;
        return nullptr;
    }

    if (doc->first_child().type() == pugi::node_null) {
        doc->append_child("root");
    }

    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->first_child();
    if (root.type() == pugi::node_null) {
        delete doc;
        return 0;
    }

    pugi::xml_node sibling1 = root.append_child("sibling1");
    pugi::xml_node sibling2 = root.append_child("sibling2");
    pugi::xml_node sibling3 = root.append_child("sibling3");

    if (!sibling1 || !sibling2 || !sibling3) {
        delete doc;
        return 0;
    }

    // 1. insert_child_before (const char_t*)
    pugi::xml_node new_node1 = root.insert_child_before("new_node1", sibling1);
    (void)new_node1;

    // 2. child
    pugi::xml_node child1 = root.child("sibling1");
    (void)child1;

    // 3. child
    pugi::xml_node child2 = root.child("sibling2");
    (void)child2;

    // 4. child
    pugi::xml_node child3 = root.child("sibling3");
    (void)child3;

    // 5. insert_child_before (const char_t*)
    pugi::xml_node new_node2 = root.insert_child_before("new_node2", sibling2);
    (void)new_node2;

    // 6. insert_child_before (const char_t*)
    pugi::xml_node new_node3 = root.insert_child_before("new_node3", sibling3);
    (void)new_node3;

    // 7. insert_child_before (const char_t*)
    pugi::xml_node new_node4 = root.insert_child_before("new_node4", sibling1);
    (void)new_node4;

    // 8. insert_child_before (const char_t*)
    pugi::xml_node new_node5 = root.insert_child_before("new_node5", sibling2);
    (void)new_node5;

    // 9. insert_child_before (const char_t*)
    pugi::xml_node new_node6 = root.insert_child_before("new_node6", sibling3);
    (void)new_node6;

    // 10. insert_child_before (string_view_t)
    std::string_view str_view = "new_node7";
    pugi::xml_node new_node7 = root.insert_child_before(str_view, sibling1);
    (void)new_node7;

    delete doc;
    return 0;
}