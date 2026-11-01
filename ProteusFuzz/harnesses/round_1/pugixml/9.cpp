// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
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

static pugi::xml_document create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    // Ensure null termination for parsing
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Append a null terminator if not present to be safe for C-string parsers
    if (xml_str.back() != '\0') {
        xml_str += '\0';
    }
    doc.load_string(xml_str.c_str());
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc = create_document_from_fuzz_data(Data, Size);
    if (!doc) return 0;

    // Start traversal from the document root
    pugi::xml_node node = doc;
    if (!node) return 0;

    // 1. pugi::xml_node::next_sibling (3 overloads)
    // Overload: next_sibling()
    node = node.next_sibling();
    // Overload: next_sibling(const char_t* name)
    if (node) node = node.next_sibling("test");
    // Overload: next_sibling(string_view_t name)
    if (node) {
        std::string_view sv = "test";
        node = node.next_sibling(sv);
    }

    // 2. pugi::xml_node::next_sibling (3 overloads)
    if (node) node = node.next_sibling();
    if (node) node = node.next_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.next_sibling(sv);
    }

    // 3. pugi::xml_node::previous_sibling (3 overloads)
    if (node) node = node.previous_sibling();
    if (node) node = node.previous_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.previous_sibling(sv);
    }

    // 4. pugi::xml_node::previous_sibling (3 overloads)
    if (node) node = node.previous_sibling();
    if (node) node = node.previous_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.previous_sibling(sv);
    }

    // 5. pugi::xml_node::child (1 overload provided in task)
    if (node) node = node.child("test");
    // 6. pugi::xml_node::child
    if (node) node = node.child("test");
    // 7. pugi::xml_node::child
    if (node) node = node.child("test");
    // 8. pugi::xml_node::child
    if (node) node = node.child("test");
    // 9. pugi::xml_node::child
    if (node) node = node.child("test");
    // 10. pugi::xml_node::child
    if (node) node = node.child("test");

    // 11. pugi::xml_node::next_sibling (3 overloads)
    if (node) node = node.next_sibling();
    if (node) node = node.next_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.next_sibling(sv);
    }

    // 12. pugi::xml_node::next_sibling (3 overloads)
    if (node) node = node.next_sibling();
    if (node) node = node.next_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.next_sibling(sv);
    }

    // 13. pugi::xml_node::previous_sibling (3 overloads)
    if (node) node = node.previous_sibling();
    if (node) node = node.previous_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.previous_sibling(sv);
    }

    // 14. pugi::xml_node::previous_sibling (3 overloads)
    if (node) node = node.previous_sibling();
    if (node) node = node.previous_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.previous_sibling(sv);
    }

    // 15. pugi::xml_node::next_sibling (3 overloads)
    if (node) node = node.next_sibling();
    if (node) node = node.next_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.next_sibling(sv);
    }

    // 16. pugi::xml_node::next_sibling (3 overloads)
    if (node) node = node.next_sibling();
    if (node) node = node.next_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.next_sibling(sv);
    }

    // 17. pugi::xml_node::next_sibling (3 overloads)
    if (node) node = node.next_sibling();
    if (node) node = node.next_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.next_sibling(sv);
    }

    // 18. pugi::xml_node::next_sibling (3 overloads)
    if (node) node = node.next_sibling();
    if (node) node = node.next_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.next_sibling(sv);
    }

    // 19. pugi::xml_node::previous_sibling (3 overloads)
    if (node) node = node.previous_sibling();
    if (node) node = node.previous_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.previous_sibling(sv);
    }

    // 20. pugi::xml_node::previous_sibling (3 overloads)
    if (node) node = node.previous_sibling();
    if (node) node = node.previous_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.previous_sibling(sv);
    }

    // 21. pugi::xml_node::previous_sibling (3 overloads)
    if (node) node = node.previous_sibling();
    if (node) node = node.previous_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.previous_sibling(sv);
    }

    // 22. pugi::xml_node::previous_sibling (3 overloads)
    if (node) node = node.previous_sibling();
    if (node) node = node.previous_sibling("test");
    if (node) {
        std::string_view sv = "test";
        node = node.previous_sibling(sv);
    }

    return 0;
}