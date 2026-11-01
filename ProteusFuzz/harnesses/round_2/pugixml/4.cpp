// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_node::attributes_begin at pugixml.cpp:5628:54 in pugixml.hpp
// pugi::xml_node::attributes_end at pugixml.cpp:5633:54 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Attempt to parse the fuzzed input as XML
    // We use a copy to ensure null termination for safety
    char* buffer = new char[Size + 1];
    std::memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    pugi::xml_parse_result result = doc.load_string(buffer);
    delete[] buffer;

    // Even if parsing fails, we can still test some API behaviors on the document
    // or the root node (which might be null or empty)
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // If no root, try the document node itself
        root = doc;
    }

    // 1. pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_node_iterator> children_range = root.children();

    // 2. pugi::xml_node::begin
    pugi::xml_node_iterator begin_it = root.begin();

    // 3. pugi::xml_node::end
    pugi::xml_node_iterator end_it = root.end();

    // 4. pugi::xml_node::children (again)
    pugi::xml_object_range<pugi::xml_node_iterator> children_range_2 = root.children();

    // 5. pugi::xml_object_range::begin
    pugi::xml_node_iterator range_begin_it = children_range.begin();

    // 6. pugi::xml_object_range::end
    pugi::xml_node_iterator range_end_it = children_range.end();

    // 7. pugi::xml_node::child
    // Use a string_view derived from input if possible, otherwise a safe default
    const char* child_name = nullptr;
    if (Size > 4) {
        // Take a substring from the input as the name to search for
        // Ensure we don't go out of bounds and handle potential nulls inside
        std::string_view sv(reinterpret_cast<const char*>(Data), Size);
        // Find a null terminator or use the whole thing if none found (pugixml handles nulls in names gracefully usually, but let's be safe)
        size_t null_pos = sv.find('\0');
        if (null_pos != std::string_view::npos) {
            sv = sv.substr(0, null_pos);
        }
        if (!sv.empty()) {
            child_name = sv.data();
        }
    }
    // Fallback if no valid name found or input too small
    if (!child_name) {
        child_name = "test";
    }
    
    pugi::xml_node child_node = root.child(child_name);

    // 8. pugi::xml_node::attributes
    pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range = root.attributes();

    // 9. pugi::xml_node::attributes_begin
    pugi::xml_attribute_iterator attr_begin_it = root.attributes_begin();

    // 10. pugi::xml_node::attributes_end
    pugi::xml_attribute_iterator attr_end_it = root.attributes_end();

    // 11. pugi::xml_node::attributes (again)
    pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range_2 = root.attributes();

    // 12. pugi::xml_object_range::begin (for attributes)
    pugi::xml_attribute_iterator attr_range_begin = attr_range.begin();

    // 13. pugi::xml_object_range::end (for attributes)
    pugi::xml_attribute_iterator attr_range_end = attr_range.end();

    // Additional exploration: iterate if ranges are valid
    // This helps trigger more code paths inside the library
    for (auto it = children_range.begin(); it != children_range.end(); ++it) {
        // Access node properties to ensure state is exercised
        (void)it->name();
    }

    for (auto it = attr_range.begin(); it != attr_range.end(); ++it) {
        (void)it->name();
        (void)it->value();
    }

    return 0;
}