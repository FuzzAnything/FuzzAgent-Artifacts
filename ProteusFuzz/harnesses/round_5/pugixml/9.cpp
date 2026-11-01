// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
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
#include <string_view>

static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    static pugi::xml_document doc;
    doc.reset();
    if (size == 0) {
        return nullptr;
    }
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    if (!result) {
        return nullptr;
    }
    return &doc;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->root();
    if (!root) {
        return 0;
    }

    // Start traversal from root or a child if root has no siblings
    pugi::xml_node current = root;
    
    // Helper to safely get a node for traversal
    auto get_traversable_node = [&]() -> pugi::xml_node {
        if (current && current.parent()) {
            return current;
        }
        // Fallback to root if current is invalid or root itself
        return root;
    };

    // 1. next_sibling overloads
    current = get_traversable_node();
    (void)current.next_sibling();
    std::string_view sv = "test";
    (void)current.next_sibling(sv);
    (void)current.next_sibling("test");

    // 2. next_sibling overloads (second invocation)
    current = get_traversable_node();
    (void)current.next_sibling();
    (void)current.next_sibling(sv);
    (void)current.next_sibling("test");

    // 3. previous_sibling overloads
    current = get_traversable_node();
    (void)current.previous_sibling();
    (void)current.previous_sibling(sv);
    (void)current.previous_sibling("test");

    // 4. previous_sibling overloads (second invocation)
    current = get_traversable_node();
    (void)current.previous_sibling();
    (void)current.previous_sibling(sv);
    (void)current.previous_sibling("test");

    // 5-10. child (6 times)
    current = get_traversable_node();
    (void)current.child("child1");
    (void)current.child("child2");
    (void)current.child("child3");
    (void)current.child("child4");
    (void)current.child("child5");
    (void)current.child("child6");

    // 11-12. next_sibling overloads
    current = get_traversable_node();
    (void)current.next_sibling();
    (void)current.next_sibling(sv);
    (void)current.next_sibling("test");

    // 13-14. previous_sibling overloads
    current = get_traversable_node();
    (void)current.previous_sibling();
    (void)current.previous_sibling(sv);
    (void)current.previous_sibling("test");

    // 15-18. next_sibling overloads (4 times)
    current = get_traversable_node();
    (void)current.next_sibling();
    (void)current.next_sibling(sv);
    (void)current.next_sibling("test");
    
    current = get_traversable_node();
    (void)current.next_sibling();
    (void)current.next_sibling(sv);
    (void)current.next_sibling("test");

    // 19-22. previous_sibling overloads (4 times)
    current = get_traversable_node();
    (void)current.previous_sibling();
    (void)current.previous_sibling(sv);
    (void)current.previous_sibling("test");
    
    current = get_traversable_node();
    (void)current.previous_sibling();
    (void)current.previous_sibling(sv);
    (void)current.previous_sibling("test");

    return 0;
}