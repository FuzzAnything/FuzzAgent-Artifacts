// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_node::attributes_begin at pugixml.cpp:5628:54 in pugixml.hpp
// pugi::xml_node::attributes_end at pugixml.cpp:5633:54 in pugixml.hpp
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
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
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
#include <cstring>
#include <string_view>

static void writeDummyFile(const uint8_t* Data, size_t Size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* Data, size_t Size) {
    // Write input to dummy file for potential file-based parsing
    writeDummyFile(Data, Size);
    
    // Initialize XML document and parse from memory buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get root node (empty if parsing failed)
    pugi::xml_node root = doc.root();
    
    // 1. pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_node_iterator> children_range = root.children();
    
    // 2. pugi::xml_node::begin
    pugi::xml_node_iterator begin_iter = root.begin();
    
    // 3. pugi::xml_node::end
    pugi::xml_node_iterator end_iter = root.end();
    
    // 4. pugi::xml_node::children (again)
    pugi::xml_object_range<pugi::xml_node_iterator> children_range2 = root.children();
    
    // 5. pugi::xml_object_range::begin
    pugi::xml_node_iterator range_begin_iter = children_range2.begin();
    
    // 6. pugi::xml_object_range::end
    pugi::xml_node_iterator range_end_iter = children_range2.end();
    
    // 7. pugi::xml_node::child
    // Create a null-terminated string from input data for child name
    std::string child_name;
    if (Size > 0) {
        size_t name_len = Size > 100 ? 100 : Size;
        child_name.assign(reinterpret_cast<const char*>(Data), name_len);
        // Ensure null termination
        child_name.push_back('\0');
        // Remove the null terminator we added for the string_view
        child_name.pop_back();
    } else {
        child_name = "test";
    }
    std::string_view child_name_view(child_name);
    pugi::xml_node child_node = root.child(child_name_view.data());
    
    // 8. pugi::xml_node::attributes
    pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range = root.attributes();
    
    // 9. pugi::xml_node::attributes_begin
    pugi::xml_attribute_iterator attr_begin_iter = root.attributes_begin();
    
    // 10. pugi::xml_node::attributes_end
    pugi::xml_attribute_iterator attr_end_iter = root.attributes_end();
    
    // 11. pugi::xml_node::attributes (again)
    pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range2 = root.attributes();
    
    // 12. pugi::xml_object_range::begin
    pugi::xml_attribute_iterator attr_range_begin_iter = attr_range2.begin();
    
    // 13. pugi::xml_object_range::end
    pugi::xml_attribute_iterator attr_range_end_iter = attr_range2.end();
    
    // Try to explore more states by using the iterators
    for (auto it = root.begin(); it != root.end(); ++it) {
        // Access child nodes
        pugi::xml_node node = *it;
        node.children();
        node.begin();
        node.end();
        
        // Try child with different names
        if (Size > 1) {
            std::string alt_name(reinterpret_cast<const char*>(Data + 1), 
                                Size > 50 ? 50 : Size - 1);
            alt_name.push_back('\0');
            alt_name.pop_back();
            node.child(alt_name.c_str());
        }
        
        // Try attributes
        node.attributes();
        node.attributes_begin();
        node.attributes_end();
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}