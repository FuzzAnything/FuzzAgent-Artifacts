// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: write input to dummy file and load into document
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // If parsing fails, we can still test with an empty document to exercise null handling
    if (!result) {
        // Continue with empty doc to test robustness
    }

    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        root = doc; // Fallback to document root if no 'root' child
    }

    // 1. pugi::xml_node::child
    // Use a string derived from input or a fixed name to find a child
    // Ensure null termination for safety if the input is not null-terminated
    const char* safe_name = (Size > 0 && Data[Size - 1] == 0) ? reinterpret_cast<const char*>(Data) : "node";
    
    pugi::xml_node child_node = root.child(safe_name);

    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute attr = child_node.first_attribute();

    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child = child_node.first_child();

    // 4. pugi::xml_node::internal_object (on child_node)
    pugi::xml_node_struct* node_struct_1 = child_node.internal_object();
    (void)node_struct_1; // Suppress unused warning

    // 5. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_struct = attr.internal_object();
    (void)attr_struct; // Suppress unused warning

    // 6. pugi::xml_node::internal_object (on first_child)
    pugi::xml_node_struct* node_struct_2 = first_child.internal_object();
    (void)node_struct_2; // Suppress unused warning

    // 7. pugi::xml_node::internal_object (on child_node again)
    pugi::xml_node_struct* node_struct_3 = child_node.internal_object();
    (void)node_struct_3; // Suppress unused warning

    // 8. pugi::xml_node::internal_object (on root)
    pugi::xml_node_struct* node_struct_4 = root.internal_object();
    (void)node_struct_4; // Suppress unused warning

    // 9. pugi::xml_attribute::internal_object (on attr again)
    pugi::xml_attribute_struct* attr_struct_2 = attr.internal_object();
    (void)attr_struct_2; // Suppress unused warning

    // 10. pugi::xml_node::internal_object (on first_child again)
    pugi::xml_node_struct* node_struct_5 = first_child.internal_object();
    (void)node_struct_5; // Suppress unused warning

    // 11. pugi::xml_attribute::internal_object (on attr again)
    pugi::xml_attribute_struct* attr_struct_3 = attr.internal_object();
    (void)attr_struct_3; // Suppress unused warning

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}