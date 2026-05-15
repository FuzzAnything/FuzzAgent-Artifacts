// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
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
#include <string>
#include <string_view>
#include <functional>

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create XML document and node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    
    // Add some initial attributes to work with
    for (int i = 0; i < 5 && i * 2 + 1 < Size; i++) {
        std::string name = "attr" + std::to_string(i);
        node.append_attribute(name.c_str()) = "value";
    }

    // Test first_attribute
    pugi::xml_attribute first_attr = node.first_attribute();
    
    // Test last_attribute
    pugi::xml_attribute last_attr = node.last_attribute();

    // Test attribute retrieval by name
    if (Size > 10) {
        std::string_view attr_name(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)50));
        pugi::xml_attribute found_attr = node.attribute(attr_name.data());
    }

    // Test find_attribute with a predicate
    auto pred = [](const pugi::xml_attribute& attr) {
        return std::string_view(attr.name()).find("attr") != std::string_view::npos;
    };
    pugi::xml_attribute found_by_pred = node.find_attribute(pred);

    // Test insert_attribute_before if we have a reference attribute
    if (first_attr) {
        std::string_view new_attr_name("new_attribute");
        pugi::xml_attribute inserted = node.insert_attribute_before(new_attr_name.data(), first_attr);
        
        // Test remove_attribute on the newly inserted attribute
        if (inserted) {
            bool removed = node.remove_attribute(inserted);
        }
    }

    // Test remove_attribute on existing attributes
    if (first_attr) {
        bool removed_first = node.remove_attribute(first_attr);
    }

    // Clean up any remaining attributes
    pugi::xml_attribute attr = node.first_attribute();
    while (attr) {
        pugi::xml_attribute next = attr.next_attribute();
        node.remove_attribute(attr);
        attr = next;
    }

    return 0;
}