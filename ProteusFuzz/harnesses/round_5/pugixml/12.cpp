// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
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
#include <cstring>

static std::string generate_xml_from_fuzz_data(const uint8_t *Data, size_t Size) {
    // Convert fuzz data to a valid XML string, escaping special characters
    std::string result = "<root>";
    for (size_t i = 0; i < Size; ++i) {
        uint8_t byte = Data[i];
        if (byte == '<' || byte == '>' || byte == '&' || byte == '"' || byte == '\'') {
            result += "\\x";
            char hex[4];
            snprintf(hex, sizeof(hex), "%02x", byte);
            result += hex;
        } else if (byte >= 32 && byte < 127) {
            result += static_cast<char>(byte);
        } else {
            // Skip control characters to avoid invalid XML
            continue;
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    std::string xml_str = generate_xml_from_fuzz_data(Data, Size);
    
    // Parse the XML string
    if (!doc.load_string(xml_str.c_str())) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // Ensure we have some children to work with for move operations
    // Create some dummy nodes if the parsed XML is too simple
    pugi::xml_node dummy1 = root.append_child("dummy1");
    pugi::xml_node dummy2 = root.append_child("dummy2");
    pugi::xml_node dummy3 = root.append_child("dummy3");
    pugi::xml_node dummy4 = root.append_child("dummy4");
    pugi::xml_node dummy5 = root.append_child("dummy5");
    pugi::xml_node dummy6 = root.append_child("dummy6");

    // 1. pugi::xml_node::child
    std::string_view a_str = "dummy1";
    pugi::xml_node node1 = root.child(a_str.data());

    // 2. pugi::xml_node::child (again)
    std::string_view b_str = "dummy2";
    pugi::xml_node node2 = root.child(b_str.data());

    // 3. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();

    // 4. pugi::xml_node::last_child
    pugi::xml_node last = root.last_child();

    // 5. pugi::xml_node::prepend_move
    // Move dummy3 to be the first child of root (if it exists)
    if (dummy3) {
        root.prepend_move(dummy3);
    }

    // 6. pugi::xml_node::prepend_move (again)
    // Move dummy4 to be the first child of root
    if (dummy4) {
        root.prepend_move(dummy4);
    }

    // 7. pugi::xml_node::append_move
    // Move dummy5 to be the last child of root
    if (dummy5) {
        root.append_move(dummy5);
    }

    // 8. pugi::xml_node::append_move (again)
    // Move dummy6 to be the last child of root
    if (dummy6) {
        root.append_move(dummy6);
    }

    // 9. pugi::xml_node::insert_move_before
    // Try to insert dummy3 before dummy2 (if both exist)
    if (dummy3 && dummy2) {
        root.insert_move_before(dummy3, dummy2);
    }

    // 10. pugi::xml_node::insert_move_before (again)
    // Try to insert dummy4 before dummy5 (if both exist)
    if (dummy4 && dummy5) {
        root.insert_move_before(dummy4, dummy5);
    }

    // 11. pugi::xml_node::insert_move_after
    // Try to insert dummy3 after dummy1 (if both exist)
    if (dummy3 && dummy1) {
        root.insert_move_after(dummy3, dummy1);
    }

    // 12. pugi::xml_node::insert_move_after (again)
    // Try to insert dummy4 after dummy2 (if both exist)
    if (dummy4 && dummy2) {
        root.insert_move_after(dummy4, dummy2);
    }

    // 13. pugi::xml_node::append_move (final)
    // Try to append dummy1 again if it still exists
    if (dummy1) {
        root.append_move(dummy1);
    }

    return 0;
}