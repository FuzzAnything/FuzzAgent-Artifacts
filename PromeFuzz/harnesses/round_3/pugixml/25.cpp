// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <string>
#include <string_view>

static void writeDummyFile(const uint8_t* data, size_t size) {
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        fwrite(data, 1, size, file);
        fclose(file);
    }
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file-based operations
    writeDummyFile(Data, Size);
    
    // Create XML document and parse from dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare string views for function calls
    // Use first 100 bytes or available size for string data
    size_t str_len = Size > 100 ? 100 : Size;
    std::string str_data(reinterpret_cast<const char*>(Data), str_len);
    
    // Ensure null termination for C-string usage
    if (!str_data.empty()) {
        str_data += '\0';
    }
    
    std::string_view sv1, sv2, sv3, sv4, sv5, sv6, sv7, sv8;
    
    // Split data into 8 parts for the 8 string parameters needed
    if (Size >= 8) {
        size_t part_size = Size / 8;
        sv1 = std::string_view(reinterpret_cast<const char*>(Data), 
                              part_size > 0 ? part_size : 1);
        sv2 = std::string_view(reinterpret_cast<const char*>(Data + part_size), 
                              part_size > 0 ? part_size : 1);
        sv3 = std::string_view(reinterpret_cast<const char*>(Data + 2*part_size), 
                              part_size > 0 ? part_size : 1);
        sv4 = std::string_view(reinterpret_cast<const char*>(Data + 3*part_size), 
                              part_size > 0 ? part_size : 1);
        sv5 = std::string_view(reinterpret_cast<const char*>(Data + 4*part_size), 
                              part_size > 0 ? part_size : 1);
        sv6 = std::string_view(reinterpret_cast<const char*>(Data + 5*part_size), 
                              part_size > 0 ? part_size : 1);
        sv7 = std::string_view(reinterpret_cast<const char*>(Data + 6*part_size), 
                              part_size > 0 ? part_size : 1);
        sv8 = std::string_view(reinterpret_cast<const char*>(Data + 7*part_size), 
                              Size - 7*part_size > 0 ? Size - 7*part_size : 1);
    } else {
        // If not enough data, use the same string view for all
        sv1 = sv2 = sv3 = sv4 = sv5 = sv6 = sv7 = sv8 = str_data;
    }
    
    try {
        // Call functions in specified order
        // 1. pugi::xml_node::attribute
        pugi::xml_attribute attr1 = root.attribute(sv1);
        
        // 2. pugi::xml_node::child
        pugi::xml_node child1 = root.child(sv2);
        
        // 3. pugi::xml_node::attribute
        pugi::xml_attribute attr2 = root.attribute(sv3);
        
        // 4. pugi::xml_node::attribute
        pugi::xml_attribute attr3 = root.attribute(sv4);
        
        // 5. pugi::xml_node::attribute
        pugi::xml_attribute attr4 = root.attribute(sv5);
        
        // 6. pugi::xml_node::last_attribute
        pugi::xml_attribute last_attr = root.last_attribute();
        
        // 7. pugi::xml_node::attribute
        pugi::xml_attribute attr5 = root.attribute(sv7);
        
        // 8. pugi::xml_node::attribute
        pugi::xml_attribute attr6 = root.attribute(sv8);
        
        // Also try calling on child node if it exists
        if (child1) {
            child1.attribute(sv1);
            child1.child(sv2);
            child1.last_attribute();
        }
        
        // Try calling on attributes if they exist
        if (attr1) {
            // Access some attribute properties to explore more states
            const char* name = attr1.name();
            const char* value = attr1.value();
            bool empty = attr1.empty();
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Cleanup
    remove("./dummy_file");
    
    return 0;
}