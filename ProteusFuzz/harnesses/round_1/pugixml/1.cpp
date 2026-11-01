// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <string>
#include <string_view>

static bool parse_xml_document(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    // Ensure null termination for safety
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';
    
    // Use load_buffer instead of parse_buffer
    pugi::xml_parse_result result = doc.load_buffer(buffer.data(), size);
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    
    // Prepare environment: Parse the input data into an XML document
    if (!parse_xml_document(Data, Size, doc)) {
        // If parsing fails, try to parse a minimal valid XML to test API behavior on empty/valid nodes
        doc.load_string("<root><child attr=\"value\">text</child></root>");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        // If root is empty (e.g., empty document), create a dummy root
        root = doc.append_child("root");
    }

    // 1. pugi::xml_node::children
    auto children_range = root.children();
    
    // 2. pugi::xml_node::begin
    auto begin_it = root.begin();
    
    // 3. pugi::xml_node::end
    auto end_it = root.end();
    
    // 4. pugi::xml_node::children (again)
    auto children_range_2 = root.children();
    
    // 5. pugi::xml_object_range::begin
    auto range_begin = children_range.begin();
    
    // 6. pugi::xml_object_range::end
    auto range_end = children_range.end();
    
    // 7. pugi::xml_node::child
    // Using a string_view_t compatible approach as hinted
    std::string_view a_str = "child";
    pugi::xml_node child_node = root.child(a_str.data());
    
    // 8. pugi::xml_node::attributes
    auto attrs_range = root.attributes();
    
    // 9. pugi::xml_node::attributes_begin
    auto attrs_begin = root.attributes_begin();
    
    // 10. pugi::xml_node::attributes_end
    auto attrs_end = root.attributes_end();
    
    // 11. pugi::xml_node::attributes (again)
    auto attrs_range_2 = root.attributes();
    
    // 12. pugi::xml_object_range::begin
    auto attrs_range_begin = attrs_range.begin();
    
    // 13. pugi::xml_object_range::end
    auto attrs_range_end = attrs_range.end();

    // Cleanup: pugixml uses RAII, so the document destructor handles cleanup automatically.
    // No explicit cleanup needed for iterators or ranges.
    
    return 0;
}