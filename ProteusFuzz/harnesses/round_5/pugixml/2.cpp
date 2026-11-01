// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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
#include <cstddef>
#include <string_view>

static void test_fuzz_logic(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Convert input data to a string for parsing
    // Ensure null termination
    std::string input(reinterpret_cast<const char*>(data), size);
    input.push_back('\0');
    
    // Parse the document
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    if (!result) {
        // Parsing failed, but we can still test with an empty document
        // or a minimal valid document
        if (!doc.empty()) {
            doc.reset();
        }
        // Load a minimal valid XML if parsing failed completely to avoid null pointer issues
        doc.load_string("<root/>");
    }
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // If no document element, use the document itself or return
        root = doc;
    }

    // 1. pugi::xml_node::children
    auto children_range = root.children();
    
    // 2. pugi::xml_node::begin
    auto it_begin = root.begin();
    
    // 3. pugi::xml_node::end
    auto it_end = root.end();
    
    // Iterate to ensure iterators are valid
    for (auto it = it_begin; it != it_end; ++it) {
        (void)it;
    }
    
    // 4. pugi::xml_node::children (again)
    auto children_range2 = root.children();
    
    // 5. pugi::xml_object_range::begin
    auto range_it_begin = children_range2.begin();
    
    // 6. pugi::xml_object_range::end
    auto range_it_end = children_range2.end();
    
    for (auto it = range_it_begin; it != range_it_end; ++it) {
        (void)it;
    }
    
    // 7. pugi::xml_node::child
    // Use a string_view_t (const char_t*) derived from input or a static string
    const pugi::char_t* child_name = "child";
    pugi::xml_node child_node = root.child(child_name);
    
    // 8. pugi::xml_node::attributes
    auto attrs_range = root.attributes();
    
    // 9. pugi::xml_node::attributes_begin
    auto attr_begin = root.attributes_begin();
    
    // 10. pugi::xml_node::attributes_end
    auto attr_end = root.attributes_end();
    
    for (auto it = attr_begin; it != attr_end; ++it) {
        (void)it;
    }
    
    // 11. pugi::xml_node::attributes (again)
    auto attrs_range2 = root.attributes();
    
    // 12. pugi::xml_object_range::begin
    auto range_attr_begin = attrs_range2.begin();
    
    // 13. pugi::xml_object_range::end
    auto range_attr_end = attrs_range2.end();
    
    for (auto it = range_attr_begin; it != range_attr_end; ++it) {
        (void)it;
    }
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    try {
        test_fuzz_logic(Data, Size);
    } catch (...) {
        // Catch any unexpected exceptions to prevent fuzzer crash
        return 0;
    }
    
    return 0;
}