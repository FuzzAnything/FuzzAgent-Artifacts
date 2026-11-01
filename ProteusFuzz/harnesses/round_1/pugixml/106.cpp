// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a null-terminated copy of the input to ensure safety for load_string
    // This prevents heap-buffer-overflow caused by reading past the buffer end
    // when the input data does not contain a null terminator.
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure the string is null-terminated for C-style functions if needed, 
    // but load_string handles size. However, to be safe with internal strlen calls 
    // if pugixml expects null-terminated strings in certain modes or internal logic,
    // we rely on the fact that load_string takes a string_view or const char* with length logic.
    // Actually, pugixml::load_string(const char_t* buf) expects a null-terminated string.
    // If the input is not null-terminated, we must append a null byte.
    
    std::string safe_input = input_str;
    safe_input.push_back('\0');

    pugi::xml_document doc;
    if (!doc.load_string(safe_input.c_str())) {
        return 0;
    }

    pugi::xml_node root = doc;

    // Prepare safe strings for attribute names and values
    // We use a fixed buffer size and copy safely from the input
    char safe_name[64] = {0};
    char safe_attr[64] = {0};
    char safe_val[64] = {0};
    
    // Copy up to 63 bytes from the input to avoid overflow
    size_t copy_len = (Size < 63) ? Size : 63;
    if (Size > 0) {
        memcpy(safe_name, Data, copy_len);
    }
    if (Size > 1) {
        size_t copy_len2 = (Size - 1 < 63) ? Size - 1 : 63;
        memcpy(safe_attr, Data + 1, copy_len2);
    }
    if (Size > 2) {
        size_t copy_len3 = (Size - 2 < 63) ? Size - 2 : 63;
        memcpy(safe_val, Data + 2, copy_len3);
    }

    // Test pugi::xml_node::find_child_by_attribute (overload 1: with name)
    pugi::xml_node node1 = root.find_child_by_attribute(safe_name, safe_attr, safe_val);
    (void)node1;

    // Test pugi::xml_node::find_child_by_attribute (overload 2: without name)
    pugi::xml_node node2 = root.find_child_by_attribute(safe_attr, safe_val);
    (void)node2;

    // Test pugi::xml_node::append_child
    pugi::xml_node new_child = root.append_child(pugi::node_element);
    if (!new_child.empty()) {
        // Set a name for the new element
        new_child.set_name(safe_name);
        
        // Test pugi::xml_node::insert_child_before
        // Insert a new node before the newly created child
        pugi::xml_node inserted = new_child.parent().insert_child_before(safe_attr, new_child);
        (void)inserted;
        
        // Test pugi::xml_node::parent
        pugi::xml_node parent_node = new_child.parent();
        (void)parent_node;
        
        // Test pugi::xml_node::first_element_by_path
        // Construct a path from input, ensuring it's null-terminated
        char path_buf[128] = {0};
        size_t path_len = (Size < 127) ? Size : 127;
        if (Size > 0) {
            memcpy(path_buf, Data, path_len);
        }
        
        char delimiter = '/';
        if (Size > 0) delimiter = Data[0];
        
        pugi::xml_node path_node = root.first_element_by_path(path_buf, delimiter);
        (void)path_node;
        
        // Try with root path
        pugi::xml_node path_node_root = root.first_element_by_path(path_buf, '/');
        (void)path_node_root;
    }

    // Additional state exploration: try appending different node types
    for (int i = 0; i < 8; ++i) {
        pugi::xml_node_type type = static_cast<pugi::xml_node_type>(i);
        pugi::xml_node appended = root.append_child(type);
        (void)appended;
    }

    // Test parent on root (should be null)
    pugi::xml_node root_parent = root.parent();
    (void)root_parent;

    return 0;
}