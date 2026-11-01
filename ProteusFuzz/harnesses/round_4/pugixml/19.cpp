// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Convert input data to a C-string for parsing
    // Ensure null termination for safety
    std::string input(reinterpret_cast<const char*>(Data), Size);
    if (input.back() != '\0') {
        input += '\0';
    }

    // Parse the document
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    if (!result) {
        // Parsing failed, but we can still try to operate on empty nodes
        // to test error handling paths
    }

    pugi::xml_node root = doc;

    // 1. pugi::xml_node::child
    // Use a string_view_t compatible approach. 
    // We try to find a child named "test" or use the first byte of input if valid
    const char* child_name = (Size > 0 && Data[0] != 0) ? reinterpret_cast<const char*>(Data) : "test";
    // To avoid parsing the whole input as a name, we just use a fixed safe name or a short prefix
    // Since child takes const char_t*, we need to be careful. Let's use a simple static name for stability
    // or derive a short name from input.
    pugi::xml_node node_child = root.child("child");

    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute attr_first = node_child.first_attribute();

    // 3. pugi::xml_node::first_child
    pugi::xml_node node_first_child = node_child.first_child();

    // 4. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_int_obj_1 = node_child.internal_object();

    // 5. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_int_obj_1 = attr_first.internal_object();

    // 6. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_int_obj_2 = node_first_child.internal_object();

    // 7. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_int_obj_3 = root.internal_object();

    // 8. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_int_obj_4 = node_child.internal_object();

    // 9. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_int_obj_2 = attr_first.internal_object();

    // 10. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_int_obj_5 = node_first_child.internal_object();

    // 11. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_int_obj_3 = attr_first.internal_object();

    // No explicit cleanup needed for pugi::xml_document as it cleans up on destruction
    // The pointers obtained from internal_object() are not owned by us and should not be deleted.
    // We just ensure the code compiles and runs without crashing on valid/invalid inputs.

    return 0;
}