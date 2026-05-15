// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create XML document and parse input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default);
    
    // Get root node to work with
    pugi::xml_node node = doc.root();
    
    // Prepare string views from input data
    // We'll use different portions of the input for different attribute/child names
    size_t chunk_size = Size / 8;
    if (chunk_size == 0) chunk_size = 1;
    
    // Create string views from the input data
    // Ensure null termination by using std::string_view which doesn't require null termination
    std::string_view sv1(reinterpret_cast<const char*>(Data), 
                         (chunk_size < Size) ? chunk_size : Size);
    std::string_view sv2(reinterpret_cast<const char*>(Data) + ((chunk_size < Size) ? chunk_size : 0), 
                         (2*chunk_size < Size) ? chunk_size : ((Size > chunk_size) ? Size - chunk_size : 0));
    std::string_view sv3(reinterpret_cast<const char*>(Data) + ((2*chunk_size < Size) ? 2*chunk_size : ((Size > chunk_size) ? chunk_size : 0)), 
                         (3*chunk_size < Size) ? chunk_size : ((Size > 2*chunk_size) ? Size - 2*chunk_size : 0));
    std::string_view sv4(reinterpret_cast<const char*>(Data) + ((3*chunk_size < Size) ? 3*chunk_size : ((Size > 2*chunk_size) ? 2*chunk_size : 0)), 
                         (4*chunk_size < Size) ? chunk_size : ((Size > 3*chunk_size) ? Size - 3*chunk_size : 0));
    std::string_view sv5(reinterpret_cast<const char*>(Data) + ((4*chunk_size < Size) ? 4*chunk_size : ((Size > 3*chunk_size) ? 3*chunk_size : 0)), 
                         (5*chunk_size < Size) ? chunk_size : ((Size > 4*chunk_size) ? Size - 4*chunk_size : 0));
    std::string_view sv6(reinterpret_cast<const char*>(Data) + ((5*chunk_size < Size) ? 5*chunk_size : ((Size > 4*chunk_size) ? 4*chunk_size : 0)), 
                         (6*chunk_size < Size) ? chunk_size : ((Size > 5*chunk_size) ? Size - 5*chunk_size : 0));
    std::string_view sv7(reinterpret_cast<const char*>(Data) + ((6*chunk_size < Size) ? 6*chunk_size : ((Size > 5*chunk_size) ? 5*chunk_size : 0)), 
                         (7*chunk_size < Size) ? chunk_size : ((Size > 6*chunk_size) ? Size - 6*chunk_size : 0));
    std::string_view sv8(reinterpret_cast<const char*>(Data) + ((7*chunk_size < Size) ? 7*chunk_size : ((Size > 6*chunk_size) ? 6*chunk_size : 0)), 
                         (8*chunk_size < Size) ? chunk_size : ((Size > 7*chunk_size) ? Size - 7*chunk_size : 0));

    // Call functions in specified order
    // 1. pugi::xml_node::attribute
    pugi::xml_attribute attr1 = node.attribute(sv1);
    
    // 2. pugi::xml_node::child
    pugi::xml_node child1 = node.child(sv2);
    
    // 3. pugi::xml_node::attribute
    pugi::xml_attribute attr2 = node.attribute(sv3);
    
    // 4. pugi::xml_node::attribute
    pugi::xml_attribute attr3 = node.attribute(sv4);
    
    // 5. pugi::xml_node::attribute
    pugi::xml_attribute attr4 = node.attribute(sv5);
    
    // 6. pugi::xml_node::last_attribute
    pugi::xml_attribute last_attr = node.last_attribute();
    
    // 7. pugi::xml_node::attribute
    pugi::xml_attribute attr5 = node.attribute(sv6);
    
    // 8. pugi::xml_node::attribute
    pugi::xml_attribute attr6 = node.attribute(sv7);

    // Try to explore more states by using the returned objects
    if (child1) {
        // Call attribute on the child node with the remaining string view
        pugi::xml_attribute child_attr = child1.attribute(sv8);
    }
    
    if (attr1) {
        // Access attribute value
        const char* val = attr1.value();
    }
    
    if (last_attr) {
        // Access last attribute name
        const char* name = last_attr.name();
    }

    return 0;
}