// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
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
#include <functional>

static bool predicate_attribute_exists(pugi::xml_node node) {
    if (!node) return false;
    return node.attribute("test") != pugi::xml_attribute();
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Convert input data to string for parsing
    std::string xml_data(reinterpret_cast<const char*>(Data), Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

    // Even if parsing fails, we can still test with an empty document or partial tree
    pugi::xml_node root = doc;
    if (!root) {
        // Test with null node
        pugi::xml_node empty = root.last_child();
        (void)empty;
        pugi::xml_node empty2 = root.first_child();
        (void)empty2;
        pugi::xml_node empty3 = root.child("test");
        (void)empty3;
        pugi::xml_node empty4 = root.parent();
        (void)empty4;
        
        // Test find_child with null node
        pugi::xml_node empty5 = root.find_child([](pugi::xml_node n){ return true; });
        (void)empty5;
        
        // Test find_node with null node
        pugi::xml_node empty6 = root.find_node([](pugi::xml_node n){ return true; });
        (void)empty6;
        return 0;
    }

    // 1. Test last_child
    pugi::xml_node last = root.last_child();
    (void)last;

    // 2. Test child with various names derived from input
    // Use pugi::string_view_t as specified in the library
    pugi::string_view_t name_view = "root";
    pugi::xml_node child1 = root.child(name_view);
    (void)child1;

    // Try to extract a potential name from the input data if it looks like a tag
    if (Size > 4) {
        std::string potential_name(reinterpret_cast<const char*>(Data), Size > 16 ? 16 : Size);
        // Clean up potential name to be a valid identifier (simple heuristic)
        for (auto &c : potential_name) {
            if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') c = '_';
        }
        if (!potential_name.empty()) {
            pugi::string_view_t sv = potential_name;
            pugi::xml_node child2 = root.child(sv);
            (void)child2;
        }
    }

    // 3. Test first_child
    pugi::xml_node first = root.first_child();
    (void)first;

    // 4. Test find_child with different predicates
    // Predicate 1: Match a specific name
    std::string target_name = "item";
    pugi::xml_node found1 = root.find_child([&target_name](pugi::xml_node n) {
        return n && n.name() == target_name.c_str();
    });
    (void)found1;

    // Predicate 2: Always true (gets first child)
    pugi::xml_node found2 = root.find_child([](pugi::xml_node n) {
        return n;
    });
    (void)found2;

    // Predicate 3: Check for attribute
    pugi::xml_node found3 = root.find_child(predicate_attribute_exists);
    (void)found3;

    // 5. Test find_node (depth-first search)
    // Predicate 1: Match name
    pugi::xml_node found_node1 = root.find_node([&target_name](pugi::xml_node n) {
        return n && n.name() == target_name.c_str();
    });
    (void)found_node1;

    // Predicate 2: Always true (gets first node in subtree)
    pugi::xml_node found_node2 = root.find_node([](pugi::xml_node n) {
        return n;
    });
    (void)found_node2;

    // Predicate 3: Check attribute
    pugi::xml_node found_node3 = root.find_node(predicate_attribute_exists);
    (void)found_node3;

    // 6. Test parent
    pugi::xml_node parent = root.parent();
    (void)parent;

    // Traverse children to test parent on them
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        pugi::xml_node child_parent = child.parent();
        (void)child_parent;
        
        // Test child methods on the child node itself
        pugi::xml_node child_first = child.first_child();
        (void)child_first;
        pugi::xml_node child_last = child.last_child();
        (void)child_last;
        pugi::xml_node child_of_child = child.child("test");
        (void)child_of_child;
        
        // Test find_child on child
        pugi::xml_node found_in_child = child.find_child([](pugi::xml_node n){ return n; });
        (void)found_in_child;
        
        // Test find_node on child
        pugi::xml_node found_in_child_sub = child.find_node([](pugi::xml_node n){ return n; });
        (void)found_in_child_sub;
    }

    return 0;
}