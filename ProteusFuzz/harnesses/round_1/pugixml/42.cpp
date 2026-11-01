// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>

static bool parse_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return doc.load_string("<root/>");
    }
    std::string str(reinterpret_cast<const char*>(data), size);
    return doc.load_string(str.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* Data, size_t Size) {
    pugi::xml_document doc;
    
    if (!parse_document(doc, Data, Size)) {
        return 0;
    }

    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // Create initial children to have siblings to work with
    pugi::xml_node sibling1 = root.append_child("sibling1");
    pugi::xml_node sibling2 = root.append_child("sibling2");
    pugi::xml_node sibling3 = root.append_child("sibling3");

    // 1. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node1 = root.insert_child_after("new_node1", sibling1);

    // 2. pugi::xml_node::child
    pugi::xml_node child1 = root.child("new_node1");

    // 3. pugi::xml_node::child
    pugi::xml_node child2 = root.child("sibling2");

    // 4. pugi::xml_node::child
    pugi::xml_node child3 = root.child("sibling3");

    // 5. pugi::xml_node::insert_child_after (string_view_t, const xml_node& node)
    std::string_view sv_name1 = "new_node2";
    pugi::xml_node new_node2 = root.insert_child_after(sv_name1, sibling2);

    // 6. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node3 = root.insert_child_after("new_node3", sibling3);

    // 7. pugi::xml_node::insert_child_after (string_view_t, const xml_node& node)
    std::string_view sv_name2 = "new_node4";
    pugi::xml_node new_node4 = root.insert_child_after(sv_name2, new_node1);

    // 8. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node5 = root.insert_child_after("new_node5", new_node2);

    // 9. pugi::xml_node::insert_child_after (string_view_t, const xml_node& node)
    std::string_view sv_name3 = "new_node6";
    pugi::xml_node new_node6 = root.insert_child_after(sv_name3, new_node3);

    // 10. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node7 = root.insert_child_after("new_node7", new_node4);

    // Cleanup is handled by the xml_document destructor when it goes out of scope
    return 0;
}