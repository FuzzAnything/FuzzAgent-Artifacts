// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

static pugi::xml_document parse_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    if (!result) {
        return doc;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* Data, size_t Size) {
    pugi::xml_document doc = parse_document(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc;
    }

    // Helper to safely traverse
    auto safe_next_sibling = [](pugi::xml_node node, pugi::string_view_t name) {
        return node.next_sibling(name);
    };
    auto safe_next_sibling_char = [](pugi::xml_node node, const pugi::char_t* name) {
        return node.next_sibling(name);
    };
    auto safe_next_sibling_void = [](pugi::xml_node node) {
        return node.next_sibling();
    };

    auto safe_prev_sibling = [](pugi::xml_node node, pugi::string_view_t name) {
        return node.previous_sibling(name);
    };
    auto safe_prev_sibling_char = [](pugi::xml_node node, const pugi::char_t* name) {
        return node.previous_sibling(name);
    };
    auto safe_prev_sibling_void = [](pugi::xml_node node) {
        return node.previous_sibling();
    };

    auto safe_child = [](pugi::xml_node node, const pugi::char_t* name) {
        return node.child(name);
    };

    // Sequence of calls as specified
    pugi::xml_node current = root;

    // 1. next_sibling()
    current = safe_next_sibling_void(current);
    // 2. next_sibling(name)
    std::string_view a_str = "test";
    current = safe_next_sibling(current, a_str);
    // 3. next_sibling(const char_t*)
    const pugi::char_t* c_str = "test";
    current = safe_next_sibling_char(current, c_str);

    // 4. previous_sibling()
    current = safe_prev_sibling_void(current);
    // 5. previous_sibling(name)
    current = safe_prev_sibling(current, a_str);
    // 6. previous_sibling(const char_t*)
    current = safe_prev_sibling_char(current, c_str);

    // 7-12. child() x6
    current = safe_child(current, "child1");
    current = safe_child(current, "child2");
    current = safe_child(current, "child3");
    current = safe_child(current, "child4");
    current = safe_child(current, "child5");
    current = safe_child(current, "child6");

    // 13. next_sibling()
    current = safe_next_sibling_void(current);
    // 14. next_sibling(name)
    current = safe_next_sibling(current, a_str);

    // 15. previous_sibling()
    current = safe_prev_sibling_void(current);
    // 16. previous_sibling(name)
    current = safe_prev_sibling(current, a_str);

    // 17-20. next_sibling() x4
    current = safe_next_sibling_void(current);
    current = safe_next_sibling_void(current);
    current = safe_next_sibling_void(current);
    current = safe_next_sibling_void(current);

    // 21-24. previous_sibling() x4
    current = safe_prev_sibling_void(current);
    current = safe_prev_sibling_void(current);
    current = safe_prev_sibling_void(current);
    current = safe_prev_sibling_void(current);

    return 0;
}