// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Create a document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // Prepare strings from fuzzer input
    // We need multiple strings for attribute/child names
    // Use a simple approach: split input into chunks
    const size_t max_strings = 12;
    const size_t chunk_size = Size / max_strings;
    
    // Store string pointers for later use
    const char* strings[max_strings];
    char* buffers[max_strings];
    
    // Create null-terminated strings from input
    for (size_t i = 0; i < max_strings; i++) {
        size_t start = i * chunk_size;
        size_t length = (i == max_strings - 1) ? (Size - start) : chunk_size;
        
        if (start < Size && length > 0) {
            // Allocate buffer with space for null terminator
            buffers[i] = new char[length + 1];
            size_t copy_len = (start + length <= Size) ? length : (Size - start);
            memcpy(buffers[i], Data + start, copy_len);
            buffers[i][copy_len] = '\0';
            strings[i] = buffers[i];
        } else {
            // Use default string if input is too small
            buffers[i] = new char[2];
            buffers[i][0] = 'a' + (i % 26);
            buffers[i][1] = '\0';
            strings[i] = buffers[i];
        }
    }
    
    try {
        // Call functions in specified order
        // 1. prepend_attribute (string_view overload)
        node.prepend_attribute(std::string_view(strings[0]));
        
        // 2. prepend_attribute (const char* overload)
        node.prepend_attribute(strings[1]);
        
        // 3. child
        node.child(strings[2]);
        
        // 4. prepend_attribute (string_view overload)
        node.prepend_attribute(std::string_view(strings[3]));
        
        // 5. child
        node.child(strings[4]);
        
        // 6. prepend_attribute (const char* overload)
        node.prepend_attribute(strings[5]);
        
        // 7. child
        node.child(strings[6]);
        
        // 8. child
        node.child(strings[7]);
        
        // 9. prepend_attribute (string_view overload)
        node.prepend_attribute(std::string_view(strings[8]));
        
        // 10. child
        node.child(strings[9]);
        
        // 11. child
        node.child(strings[10]);
        
        // 12. prepend_attribute (const char* overload)
        node.prepend_attribute(strings[11]);
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Clean up allocated buffers
    for (size_t i = 0; i < max_strings; i++) {
        delete[] buffers[i];
    }
    
    return 0;
}