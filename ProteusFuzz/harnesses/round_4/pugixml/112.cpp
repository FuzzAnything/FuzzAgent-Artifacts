// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <string_view>
#include <vector>

static bool predicate_name(const pugi::xml_node& node, const std::string& name) {
    return node.name() && std::string(node.name()) == name;
}

static bool predicate_any(const pugi::xml_node& node) {
    return !node.empty();
}

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    // Ensure input is valid UTF-8 or at least valid for parsing
    // We let pugixml handle parsing errors gracefully
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    
    // Even if parsing fails, doc.root() might still be accessible but empty
    // We proceed to test navigation on whatever structure was created
    
    pugi::xml_node root = doc.root();
    
    // Test 1: first_child on root
    pugi::xml_node first = root.first_child();
    
    // Test 2: last_child on root
    pugi::xml_node last = root.last_child();
    
    // Test 3: parent on root (should be null)
    pugi::xml_node parent = root.parent();
    
    // Test 4: child with name from input
    if (Size >= 2) {
        std::string_view name_view(reinterpret_cast<const char*>(Data), std::min(Size - 1, static_cast<size_t>(10)));
        pugi::xml_node child = root.child(name_view);
        (void)child; // Suppress unused warning
    }
    
    // Test 5: find_child with predicate
    if (Size >= 4) {
        std::string name_str(reinterpret_cast<const char*>(Data + 2), std::min(Size - 2, static_cast<size_t>(8)));
        auto pred = [name_str](const pugi::xml_node& node) {
            return node.name() && std::string(node.name()) == name_str;
        };
        pugi::xml_node found_child = root.find_child(pred);
        (void)found_child;
    }
    
    // Test 6: find_node with predicate (depth-first)
    if (Size >= 4) {
        std::string name_str(reinterpret_cast<const char*>(Data + 2), std::min(Size - 2, static_cast<size_t>(8)));
        auto pred = [name_str](const pugi::xml_node& node) {
            return node.name() && std::string(node.name()) == name_str;
        };
        pugi::xml_node found_node = root.find_node(pred);
        (void)found_node;
    }
    
    // Test navigation on children if they exist
    if (!first.empty()) {
        pugi::xml_node first_parent = first.parent();
        pugi::xml_node first_first = first.first_child();
        pugi::xml_node first_last = first.last_child();
        (void)first_parent;
        (void)first_first;
        (void)first_last;
    }
    
    if (!last.empty()) {
        pugi::xml_node last_parent = last.parent();
        pugi::xml_node last_first = last.first_child();
        pugi::xml_node last_last = last.last_child();
        (void)last_parent;
        (void)last_first;
        (void)last_last;
    }
    
    // Test on empty/null nodes
    pugi::xml_node null_node;
    pugi::xml_node null_first = null_node.first_child();
    pugi::xml_node null_last = null_node.last_child();
    pugi::xml_node null_parent = null_node.parent();
    (void)null_first;
    (void)null_last;
    (void)null_parent;
    
    return 0;
}