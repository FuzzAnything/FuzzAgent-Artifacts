// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
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

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Try to parse the input as XML
    if (size > 0) {
        // Ensure the input is null-terminated for load_string
        std::vector<char> buffer(size + 1);
        std::memcpy(buffer.data(), data, size);
        buffer[size] = '\0';
        
        pugi::xml_parse_result result = doc->load_string(buffer.data());
        if (!result) {
            // If parsing fails, create a minimal valid XML document
            doc->reset();
            if (!doc->append_child("root")) {
                delete doc;
                return nullptr;
            }
        }
    } else {
        doc->reset();
        if (!doc->append_child("root")) {
            delete doc;
            return nullptr;
        }
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* Data, size_t Size) {
    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->root();
    if (!root) {
        delete doc;
        return 0;
    }

    // 1. pugi::xml_node::prepend_child (overload 1: string_view_t)
    std::string_view sv_name = "child1";
    pugi::xml_node node1 = root.prepend_child(sv_name);
    if (!node1) {
        delete doc;
        return 0;
    }

    // 2. pugi::xml_node::child
    pugi::xml_node child1 = node1.child("nonexistent");
    (void)child1; // Suppress unused variable warning

    // 3. pugi::xml_node::prepend_child (overload 2: const char_t*)
    pugi::xml_node node2 = root.prepend_child("child2");
    if (!node2) {
        delete doc;
        return 0;
    }

    // 4. pugi::xml_node::child
    pugi::xml_node child2 = node2.child("nonexistent");
    (void)child2;

    // 5. pugi::xml_node::child
    pugi::xml_node child3 = root.child("child1");
    (void)child3;

    // 6. pugi::xml_node::prepend_child (overload 1: string_view_t)
    std::string_view sv_name2 = "child3";
    pugi::xml_node node3 = root.prepend_child(sv_name2);
    if (!node3) {
        delete doc;
        return 0;
    }

    // 7. pugi::xml_node::child
    pugi::xml_node child4 = node3.child("nonexistent");
    (void)child4;

    // 8. pugi::xml_node::prepend_child (overload 2: const char_t*)
    pugi::xml_node node4 = root.prepend_child("child4");
    if (!node4) {
        delete doc;
        return 0;
    }

    // 9. pugi::xml_node::prepend_child (overload 1: string_view_t)
    std::string_view sv_name3 = "child5";
    pugi::xml_node node5 = root.prepend_child(sv_name3);
    if (!node5) {
        delete doc;
        return 0;
    }

    // Cleanup
    delete doc;
    return 0;
}