// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
#include <algorithm>
#include <pugixml.hpp>
#include <string_view>
#include <functional>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Ensure the input is null-terminated for safe string parsing
    // We create a temporary buffer with a null terminator
    std::vector<char> buffer(size + 1);
    if (size > 0) {
        std::memcpy(buffer.data(), data, size);
    }
    buffer[size] = '\0';

    // Try to parse the input as XML
    pugi::xml_parse_result result = doc->load_string(buffer.data());
    
    // If parsing fails or document is empty, create a minimal valid XML structure
    if (!result || doc->document_element().empty()) {
        doc->reset();
        doc->append_child("root");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    // Ensure we have a root node to work with
    pugi::xml_node root = doc->document_element();
    if (root.empty()) {
        // If document is empty, create a dummy root
        root = doc->append_child("dummy_root");
    }

    // 1. Test child(string_view_t name)
    // Use a portion of the input as the search string if possible
    if (Size > 0) {
        // Create a safe string from input, ensuring null termination
        std::string search_str(reinterpret_cast<const char*>(Data), Size);
        // Remove any null bytes that might appear in the middle to ensure valid string_view
        search_str.erase(std::remove(search_str.begin(), search_str.end(), '\0'), search_str.end());
        
        if (!search_str.empty()) {
            pugi::xml_node found_child = root.child(search_str);
            (void)found_child; // Suppress unused warning
        }

        // Also test with empty string
        pugi::xml_node empty_child = root.child("");
        (void)empty_child;
    }

    // 2. Test last_child()
    pugi::xml_node last = root.last_child();
    (void)last;

    // 3. Test first_child()
    pugi::xml_node first = root.first_child();
    (void)first;

    // 4. Test parent()
    pugi::xml_node parent = root.parent();
    (void)parent;

    // Test parent on a child if it exists
    if (!first.empty()) {
        pugi::xml_node child_parent = first.parent();
        (void)child_parent;
    }

    // 5. Test find_child(Predicate pred)
    // Predicate 1: Check if name starts with 'a' (case insensitive logic simplified)
    auto pred1 = [](pugi::xml_node n) {
        const char* name = n.name();
        if (!name) return false;
        return name[0] == 'a' || name[0] == 'A';
    };
    pugi::xml_node found_child_pred = root.find_child(pred1);
    (void)found_child_pred;

    // Predicate 2: Always true (to find first child)
    auto pred2 = [](pugi::xml_node n) { return !n.empty(); };
    pugi::xml_node found_child_pred2 = root.find_child(pred2);
    (void)found_child_pred2;

    // Predicate 3: Always false
    auto pred3 = [](pugi::xml_node n) { return false; };
    pugi::xml_node found_child_pred3 = root.find_child(pred3);
    (void)found_child_pred3;

    // 6. Test find_node(Predicate pred)
    // Predicate 1: Depth first search for 'a'
    auto pred4 = [](pugi::xml_node n) {
        const char* name = n.name();
        if (!name) return false;
        return name[0] == 'a' || name[0] == 'A';
    };
    pugi::xml_node found_node_pred = root.find_node(pred4);
    (void)found_node_pred;

    // Predicate 2: Find any node
    auto pred5 = [](pugi::xml_node n) { return !n.empty(); };
    pugi::xml_node found_node_pred2 = root.find_node(pred5);
    (void)found_node_pred2;

    // Predicate 3: Never match
    auto pred6 = [](pugi::xml_node n) { return false; };
    pugi::xml_node found_node_pred3 = root.find_node(pred6);
    (void)found_node_pred3;

    // Cleanup
    delete doc;

    return 0;
}