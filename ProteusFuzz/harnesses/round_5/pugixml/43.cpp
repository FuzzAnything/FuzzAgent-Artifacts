// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <vector>

static bool parse_xml_from_fuzz_data(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) return false;
    
    // Create a copy of the data with a null terminator to ensure safety for load_string
    // which expects a null-terminated C-string.
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';
    
    return doc.load_string(buffer.data());
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Attempt to parse the input data as XML
    if (!parse_xml_from_fuzz_data(Data, Size, doc)) {
        // If parsing fails, try to create a minimal valid XML structure to test the API logic
        // even if the input is garbage. This ensures we exercise the API functions.
        if (!doc.load_string("<root><child/></root>")) {
            return 0;
        }
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root found, try to create one
        root = doc.append_child("root");
        if (!root) return 0;
        // Add a dummy child to have something to insert before
        root.append_child("dummy");
    }

    // Ensure we have at least one child to insert before
    pugi::xml_node target_sibling = root.child("child");
    if (!target_sibling) {
        target_sibling = root.child("dummy");
        if (!target_sibling) {
            // Fallback: create a child if none exist
            target_sibling = root.append_child("fallback");
            if (!target_sibling) return 0;
        }
    }

    // 1. pugi::xml_node::insert_child_before
    // Using const char_t* overload
    pugi::xml_node node1 = root.insert_child_before("node1", target_sibling);
    
    // 2. pugi::xml_node::child
    pugi::xml_node found1 = root.child("node1");
    
    // 3. pugi::xml_node::child
    pugi::xml_node found2 = root.child("child");
    
    // 4. pugi::xml_node::child
    pugi::xml_node found3 = root.child("dummy");

    // 5. pugi::xml_node::insert_child_before
    // Using string_view_t overload
    std::string_view sv_name1 = "node2";
    pugi::xml_node node2 = root.insert_child_before(sv_name1, target_sibling);

    // 6. pugi::xml_node::insert_child_before
    // Using const char_t* overload
    pugi::xml_node node3 = root.insert_child_before("node3", target_sibling);

    // 7. pugi::xml_node::insert_child_before
    // Using string_view_t overload
    std::string_view sv_name2 = "node4";
    pugi::xml_node node4 = root.insert_child_before(sv_name2, target_sibling);

    // 8. pugi::xml_node::insert_child_before
    // Using const char_t* overload
    pugi::xml_node node5 = root.insert_child_before("node5", target_sibling);

    // 9. pugi::xml_node::insert_child_before
    // Using string_view_t overload
    std::string_view sv_name3 = "node6";
    pugi::xml_node node6 = root.insert_child_before(sv_name3, target_sibling);

    // 10. pugi::xml_node::insert_child_before
    // Using const char_t* overload
    pugi::xml_node node7 = root.insert_child_before("node7", target_sibling);

    // Cleanup is handled automatically by RAII (xml_document destructor)
    return 0;
}