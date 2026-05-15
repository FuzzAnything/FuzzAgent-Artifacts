// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Parse XML from input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default);
    
    // Only proceed if parsing succeeded
    if (result) {
        // Get the root node
        pugi::xml_node root = doc.root();
        
        // 1. pugi::xml_node::children
        pugi::xml_object_range<pugi::xml_node_iterator> children_range1 = root.children();
        
        // 2. pugi::xml_node::begin
        pugi::xml_node_iterator begin_iter = root.begin();
        
        // 3. pugi::xml_node::end
        pugi::xml_node_iterator end_iter = root.end();
        
        // 4. pugi::xml_node::children (again)
        pugi::xml_object_range<pugi::xml_node_iterator> children_range2 = root.children();
        
        // 5. pugi::xml_object_range::begin (from first children range)
        pugi::xml_node_iterator range_begin_iter = children_range1.begin();
        
        // 6. pugi::xml_object_range::end (from first children range)
        pugi::xml_node_iterator range_end_iter = children_range1.end();
        
        // 7. pugi::xml_node::child
        // Create a null-terminated string for child name
        std::string_view name_view;
        if (Size > 0) {
            // Use first min(Size, 100) bytes as name, ensure null termination
            size_t name_len = Size > 100 ? 100 : Size;
            char name[101];
            memcpy(name, Data, name_len);
            name[name_len] = '\0';
            name_view = std::string_view(name, name_len);
        } else {
            name_view = "default";
        }
        pugi::xml_node child_node = root.child(name_view.data());
        
        // 8. pugi::xml_node::attributes
        pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range1 = root.attributes();
        
        // 9. pugi::xml_node::attributes_begin
        pugi::xml_attribute_iterator attr_begin_iter = root.attributes_begin();
        
        // 10. pugi::xml_node::attributes_end
        pugi::xml_attribute_iterator attr_end_iter = root.attributes_end();
        
        // 11. pugi::xml_node::attributes (again)
        pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range2 = root.attributes();
        
        // 12. pugi::xml_object_range::begin (from first attributes range)
        pugi::xml_attribute_iterator attr_range_begin_iter = attr_range1.begin();
        
        // 13. pugi::xml_object_range::end (from first attributes range)
        pugi::xml_attribute_iterator attr_range_end_iter = attr_range1.end();
        
        // Iterate through children to explore more states
        for (pugi::xml_node child : root.children()) {
            // Call the same sequence on child nodes
            auto child_children = child.children();
            auto child_begin = child.begin();
            auto child_end = child.end();
            auto child_attr_range = child.attributes();
            
            // Try child with different names
            pugi::xml_node grandchild = child.child("test");
            pugi::xml_node grandchild2 = child.child("");
        }
        
        // Iterate through attributes
        for (pugi::xml_attribute attr : root.attributes()) {
            // Access attribute values
            const char* value = attr.value();
            (void)value; // Suppress unused warning
        }
    }
    
    return 0;
}