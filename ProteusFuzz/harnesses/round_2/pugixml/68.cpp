// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
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
#include <functional>
#include <string_view>

static pugi::xml_document parse_document(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    if (Size == 0) {
        return doc;
    }
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc = parse_document(Data, Size);
    
    // Get the root node (or empty if parsing failed completely)
    pugi::xml_node root = doc.first_child();
    
    // If no root, try to test on the document node itself (which is also an xml_node)
    // In pugixml, an empty node is checked by !node or node.is_null() is not a member, 
    // but node.is_null() is actually valid in newer versions, however the error says it's not.
    // The standard way to check if a node is empty/null in pugixml is !node or node == nullptr.
    // Actually, xml_node has an implicit conversion to void* (or similar) or we can check name() == nullptr.
    // But the most robust way without is_null() is checking if the node is empty via !node operator.
    // Wait, pugixml xml_node has an operator void*() or similar? 
    // Actually, in pugixml, you check validity by !node or node.is_null() if available.
    // The error says 'is_null' is not a member. Let's use !root.
    if (!root) {
        root = doc;
    }

    // 1. Test first_child
    pugi::xml_node first = root.first_child();
    (void)first;

    // 2. Test last_child
    pugi::xml_node last = root.last_child();
    (void)last;

    // 3. Test parent
    pugi::xml_node parent = root.parent();
    (void)parent;

    // 4. Test child with string_view
    std::string_view name_str;
    if (Size > 0) {
        size_t len = (Size > 1024) ? 1024 : Size;
        name_str = std::string_view(reinterpret_cast<const char*>(Data), len);
    }
    pugi::xml_node child_node = root.child(name_str);
    (void)child_node;

    // 5. Test find_child with a predicate
    auto pred_find_child = [](pugi::xml_node n) {
        if (!n) return false;
        const char* name = n.name();
        if (!name) return false;
        return name[0] != '\0';
    };
    pugi::xml_node found_child = root.find_child(pred_find_child);
    (void)found_child;

    // 6. Test find_node with a predicate
    // attribute_count() is not a member, use first_attribute() != nullptr
    auto pred_find_node = [](pugi::xml_node n) {
        if (!n) return false;
        if (n.type() == pugi::node_element) {
            return n.first_attribute() != nullptr;
        }
        return false;
    };
    pugi::xml_node found_node = root.find_node(pred_find_node);
    (void)found_node;

    // Additional exploration
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        (void)child.first_child();
        (void)child.last_child();
        (void)child.parent();
        
        if (Size > 2) {
            std::string_view sub_str(reinterpret_cast<const char*>(Data) + 1, Size - 2);
            (void)child.child(sub_str);
        }

        (void)child.find_child(pred_find_child);
        (void)child.find_node(pred_find_node);
    }

    return 0;
}