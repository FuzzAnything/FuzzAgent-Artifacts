// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
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
#include <pugixml.hpp>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;

    // Ensure the input is null-terminated before passing to load_string
    // We create a temporary buffer to safely handle arbitrary binary input
    std::vector<char> safe_input(Size + 1);
    if (Size > 0) {
        std::memcpy(safe_input.data(), Data, Size);
    }
    safe_input[Size] = '\0';

    // Try to load the raw input as XML
    if (!doc.load_string(safe_input.data())) {
        // If parsing fails, construct a valid XML document wrapping the raw data
        // We escape special characters minimally or just wrap in CDATA to avoid parse errors
        std::ostringstream oss;
        oss << "<?xml version=\"1.0\"?><root><![CDATA[";
        // Write raw bytes, but ensure no unescaped ]]> sequences break CDATA if we were strict
        // For fuzzing, simply appending is usually sufficient as CDATA handles most binary
        oss.write(safe_input.data(), Size);
        oss << "]]></root>";
        
        std::string xml_str = oss.str();
        if (!doc.load_string(xml_str.c_str())) {
            // Fallback: create a minimal document if everything else fails
            doc.append_child("root");
        }
    }

    pugi::xml_node root = doc.root();
    if (root.empty()) {
        root = doc.append_child("root");
    }

    // Create some initial children to work with
    pugi::xml_node sibling1 = root.append_child("sibling1");
    pugi::xml_node sibling2 = root.append_child("sibling2");
    pugi::xml_node sibling3 = root.append_child("sibling3");

    // 1. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node1 = root.insert_child_after("new1", sibling1);

    // 2. pugi::xml_node::child (const char_t* name)
    pugi::xml_node child1 = root.child("sibling1");

    // 3. pugi::xml_node::child (const char_t* name)
    pugi::xml_node child2 = root.child("sibling2");

    // 4. pugi::xml_node::child (const char_t* name)
    pugi::xml_node child3 = root.child("sibling3");

    // 5. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node2 = root.insert_child_after("new2", sibling2);

    // 6. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node3 = root.insert_child_after("new3", sibling3);

    // 7. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node4 = root.insert_child_after("new4", new_node1);

    // 8. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node5 = root.insert_child_after("new5", new_node2);

    // 9. pugi::xml_node::insert_child_after (const char_t* name, const xml_node& node)
    pugi::xml_node new_node6 = root.insert_child_after("new6", new_node3);

    // 10. pugi::xml_node::insert_child_after (string_view_t name, const xml_node& node)
    // Ensure the string_view points to valid memory that outlives the call
    const char* sv_data = "new7";
    std::string_view sv_name(sv_data, 4);
    pugi::xml_node new_node7 = root.insert_child_after(sv_name, new_node4);

    return 0;
}