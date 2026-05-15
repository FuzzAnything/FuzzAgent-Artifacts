// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
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
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
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
#include <fstream>
#include <string>
#include <vector>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static std::string extractNullTerminatedString(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "";
    
    size_t length = 0;
    while (offset + length < size && data[offset + length] != 0) {
        ++length;
    }
    
    return std::string(reinterpret_cast<const char*>(data + offset), length);
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* Data, size_t Size) {
    // Write input to dummy file for potential file-based parsing
    writeDummyFile(Data, Size);
    
    // Parse XML from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        // Even if parsing fails, we can still test with empty document
        // Continue with empty document to test error paths
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // 1. pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_node_iterator> children_range1 = root.children();
    
    // 2. pugi::xml_node::begin
    pugi::xml_node_iterator begin_iter = root.begin();
    
    // 3. pugi::xml_node::end
    pugi::xml_node_iterator end_iter = root.end();
    
    // 4. pugi::xml_node::children (again)
    pugi::xml_object_range<pugi::xml_node_iterator> children_range2 = root.children();
    
    // 5. pugi::xml_object_range::begin
    pugi::xml_node_iterator range_begin = children_range2.begin();
    
    // 6. pugi::xml_object_range::end
    pugi::xml_node_iterator range_end = children_range2.end();
    
    // 7. pugi::xml_node::child
    // Extract a null-terminated string from fuzzer input for child name
    std::string child_name = extractNullTerminatedString(Data, Size, 0);
    if (!child_name.empty()) {
        pugi::xml_node child_node = root.child(child_name.c_str());
    }
    
    // 8. pugi::xml_node::attributes
    pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range1 = root.attributes();
    
    // 9. pugi::xml_node::attributes_begin
    pugi::xml_attribute_iterator attr_begin = root.attributes_begin();
    
    // 10. pugi::xml_node::attributes_end
    pugi::xml_attribute_iterator attr_end = root.attributes_end();
    
    // 11. pugi::xml_node::attributes (again)
    pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range2 = root.attributes();
    
    // 12. pugi::xml_object_range::begin
    pugi::xml_attribute_iterator attr_range_begin = attr_range2.begin();
    
    // 13. pugi::xml_object_range::end
    pugi::xml_attribute_iterator attr_range_end = attr_range2.end();
    
    // Additional exploration: iterate through children and attributes
    for (pugi::xml_node child : root.children()) {
        // Test child operations recursively
        pugi::xml_object_range<pugi::xml_node_iterator> child_children = child.children();
        pugi::xml_object_range<pugi::xml_attribute_iterator> child_attrs = child.attributes();
        
        // Extract another string for child name lookup
        std::string nested_child_name = extractNullTerminatedString(Data, Size, 
            child_name.empty() ? 0 : std::min(child_name.length(), Size));
        if (!nested_child_name.empty()) {
            pugi::xml_node nested_child = child.child(nested_child_name.c_str());
        }
    }
    
    // Iterate through attributes
    for (pugi::xml_attribute attr : root.attributes()) {
        // Access attribute properties to explore more code paths
        std::string name = attr.name();
        std::string value = attr.value();
    }
    
    return 0;
}