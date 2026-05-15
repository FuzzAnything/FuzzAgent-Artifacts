// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
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

static void* custom_allocate(size_t size) {
    return malloc(size);
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        // If parsing fails, still try to exercise APIs on empty nodes
        pugi::xml_node empty_node;
        empty_node.first_child();
        empty_node.next_sibling();
        empty_node.value();
        
        // Prepare a null-terminated string for set_value
        char buffer[256];
        size_t copy_size = Size < 255 ? Size : 255;
        if (copy_size > 0) {
            memcpy(buffer, Data, copy_size);
        }
        buffer[copy_size] = '\0';
        empty_node.set_value(buffer);
        
        empty_node.first_child();
        empty_node.next_sibling();
        empty_node.next_sibling();
        empty_node.parent();
        empty_node.remove_child(empty_node);
        
        pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
        return 0;
    }
    
    // Get root node to start operations
    pugi::xml_node root = doc.root().first_child();
    if (!root) {
        root = doc.root();
    }
    
    // 1. first_child
    pugi::xml_node child1 = root.first_child();
    
    // 2. next_sibling
    pugi::xml_node sibling1 = child1.next_sibling();
    
    // 3. value
    if (sibling1) {
        const char* val = sibling1.value();
        (void)val; // Use val to avoid unused variable warning
    }
    
    // 4. set_value
    if (sibling1) {
        // Prepare a null-terminated string from fuzzer input
        char buffer[256];
        size_t copy_size = Size < 255 ? Size : 255;
        if (copy_size > 0) {
            memcpy(buffer, Data, copy_size);
        }
        buffer[copy_size] = '\0';
        sibling1.set_value(buffer);
    }
    
    // 5. first_child (again)
    pugi::xml_node child2 = sibling1.first_child();
    
    // 6. next_sibling
    pugi::xml_node sibling2 = child2.next_sibling();
    
    // 7. next_sibling (again)
    pugi::xml_node sibling3 = sibling2.next_sibling();
    
    // 8. parent
    pugi::xml_node parent = sibling3.parent();
    
    // 9. remove_child
    if (parent && child2) {
        parent.remove_child(child2);
    }
    
    // 10. set_memory_management_functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
    
    return 0;
}