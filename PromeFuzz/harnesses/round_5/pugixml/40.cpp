// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create document and initial structure
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create initial child nodes to have something to work with
    pugi::xml_node node1 = root.append_child("child1");
    pugi::xml_node node2 = root.append_child("child2");
    pugi::xml_node node3 = root.append_child("child3");
    
    // Use the fuzz data to create strings for node names
    // We'll create multiple strings from the input data
    size_t str_len = Size / 10;
    if (str_len == 0) str_len = 1;
    
    // Create strings for the 10 function calls
    const char* strings[10];
    char* buffers[10];
    
    for (int i = 0; i < 10; i++) {
        size_t start = (i * str_len) % Size;
        size_t length = str_len;
        if (start + length > Size) {
            length = Size - start;
        }
        
        // Allocate buffer with space for null terminator
        buffers[i] = new char[length + 1];
        if (length > 0) {
            memcpy(buffers[i], Data + start, length);
        }
        buffers[i][length] = '\0';
        strings[i] = buffers[i];
    }
    
    try {
        // 1. insert_child_after (string_view overload)
        std::string_view sv1(strings[0]);
        root.insert_child_after(sv1, node1);
        
        // 2. child
        root.child(strings[1]);
        
        // 3. child
        root.child(strings[2]);
        
        // 4. child
        root.child(strings[3]);
        
        // 5. insert_child_after (const char* overload)
        root.insert_child_after(strings[4], node2);
        
        // 6. insert_child_after (string_view overload)
        std::string_view sv6(strings[5]);
        root.insert_child_after(sv6, node3);
        
        // 7. insert_child_after (const char* overload)
        root.insert_child_after(strings[6], node1);
        
        // 8. insert_child_after (string_view overload)
        std::string_view sv8(strings[7]);
        root.insert_child_after(sv8, node2);
        
        // 9. insert_child_after (const char* overload)
        root.insert_child_after(strings[8], node3);
        
        // 10. insert_child_after (string_view overload)
        std::string_view sv10(strings[9]);
        root.insert_child_after(sv10, node1);
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Cleanup
    for (int i = 0; i < 10; i++) {
        delete[] buffers[i];
    }
    
    return 0;
}