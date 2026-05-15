// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>

static void* custom_allocate(size_t size) {
    return malloc(size);
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Set custom memory management functions first as required
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
    
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        return 0;
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Call functions in the specified order
    pugi::xml_node child1 = root.first_child();
    pugi::xml_node sibling1 = root.next_sibling();
    const char* value1 = root.value();
    
    // For set_value, we need a null-terminated string
    // Create a dummy string from fuzzer data if available, otherwise use default
    const char* dummy_value = "fuzz_value";
    if (Size > 0) {
        // Use first byte to determine value length (max 255)
        size_t val_len = Data[0] % 256;
        if (val_len > 0 && val_len <= Size - 1) {
            // Create null-terminated string from fuzzer data
            char* temp_val = new char[val_len + 1];
            memcpy(temp_val, Data + 1, val_len);
            temp_val[val_len] = '\0';
            root.set_value(temp_val);
            delete[] temp_val;
        } else {
            root.set_value(dummy_value);
        }
    } else {
        root.set_value(dummy_value);
    }
    
    pugi::xml_node child2 = root.first_child();
    pugi::xml_node sibling2 = root.next_sibling();
    pugi::xml_node sibling3 = root.next_sibling();
    pugi::xml_node parent = root.parent();
    
    // For remove_child, need a child node to remove
    // Try to remove first child if it exists
    if (!child1.empty()) {
        root.remove_child(child1);
    }
    
    return 0;
}