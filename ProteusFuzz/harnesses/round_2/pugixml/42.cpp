// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
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
#include <cstring>
#include <string>

static std::string create_xml_document(const uint8_t *Data, size_t Size) {
    std::string result = "<root>";
    // Append fuzz data as text content, escaping special XML characters
    for (size_t i = 0; i < Size; ++i) {
        uint8_t c = Data[i];
        if (c == '<') {
            result += "&lt;";
        } else if (c == '>') {
            result += "&gt;";
        } else if (c == '&') {
            result += "&amp;";
        } else if (c >= 32 && c < 127) {
            result += static_cast<char>(c);
        } else {
            // Skip non-printable characters to ensure valid XML
            continue;
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    std::string xml_str = create_xml_document(Data, Size);

    if (!doc.load_string(xml_str.c_str())) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::insert_child_after
    // Insert a node after the first child (if exists) or after root itself if no children?
    // insert_child_after requires a valid sibling node. Let's create a dummy sibling first.
    pugi::xml_node sibling1 = root.append_child("sibling1");
    pugi::xml_node new_node1 = root.insert_child_after("node1", sibling1);

    // 2. pugi::xml_node::child
    pugi::xml_node found1 = root.child("node1");

    // 3. pugi::xml_node::child
    pugi::xml_node found2 = root.child("sibling1");

    // 4. pugi::xml_node::child
    pugi::xml_node found3 = root.child("root"); // Should be empty

    // 5. pugi::xml_node::insert_child_after (const char_t* overload)
    pugi::xml_node new_node2 = root.insert_child_after("node2", found1);

    // 6. pugi::xml_node::insert_child_after (string_view_t overload)
    std::string_view sv_name = "node3";
    pugi::xml_node new_node3 = root.insert_child_after(sv_name, found2);

    // 7. pugi::xml_node::insert_child_after (const char_t* overload)
    pugi::xml_node new_node4 = root.insert_child_after("node4", new_node2);

    // 8. pugi::xml_node::insert_child_after (string_view_t overload)
    std::string_view sv_name2 = "node5";
    pugi::xml_node new_node5 = root.insert_child_after(sv_name2, new_node3);

    // 9. pugi::xml_node::insert_child_after (const char_t* overload)
    pugi::xml_node new_node6 = root.insert_child_after("node6", new_node4);

    // 10. pugi::xml_node::insert_child_after (string_view_t overload)
    std::string_view sv_name3 = "node7";
    pugi::xml_node new_node7 = root.insert_child_after(sv_name3, new_node5);

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}