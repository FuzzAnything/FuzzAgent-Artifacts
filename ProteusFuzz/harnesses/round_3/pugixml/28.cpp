// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
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

static void safe_invoke(pugi::xml_node node, const pugi::char_t* name) {
    if (!node) return;
    (void)node.child(name);
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure the input is null-terminated for load_string
    std::string input(reinterpret_cast<const char*>(Data), Size);
    if (input.back() != '\0') {
        input += '\0';
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(input.c_str(), pugi::parse_default);

    // Proceed only if we have a document, even if parsing had errors, to test robustness
    if (!doc) return 0;

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // Try to get the first child regardless of name if root is not found
        root = doc.child(input.c_str());
        if (!root) {
            root = doc.first_child();
        }
    }

    // Chain of child calls as requested
    pugi::xml_node n1 = root.child(input.c_str());
    pugi::xml_node n2 = n1.child(input.c_str());
    pugi::xml_node n3 = n2.child(input.c_str());
    pugi::xml_node n4 = n3.child(input.c_str());
    pugi::xml_node n5 = n4.child(input.c_str());
    pugi::xml_node n6 = n5.child(input.c_str());
    pugi::xml_node n7 = n6.child(input.c_str());

    // Call text()
    if (n7) {
        pugi::xml_text txt = n7.text();
        (void)txt; // Suppress unused warning
    }

    // More child calls
    pugi::xml_node n8 = n7.child(input.c_str());
    
    // Call child_value() overloads
    if (n8) {
        const pugi::char_t* val1 = n8.child_value();
        (void)val1;
        const pugi::char_t* val2 = n8.child_value(input.c_str());
        (void)val2;
    }

    // Additional exploration with safe_invoke to ensure coverage
    safe_invoke(n8, input.c_str());
    safe_invoke(n7, input.c_str());
    safe_invoke(n6, input.c_str());
    safe_invoke(n5, input.c_str());
    safe_invoke(n4, input.c_str());
    safe_invoke(n3, input.c_str());
    safe_invoke(n2, input.c_str());
    safe_invoke(n1, input.c_str());
    safe_invoke(root, input.c_str());

    return 0;
}