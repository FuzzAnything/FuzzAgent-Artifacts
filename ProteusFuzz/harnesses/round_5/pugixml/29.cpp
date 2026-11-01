// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Try to load from dummy file first, if not exists or fails, create a minimal valid XML
    if (!doc.load_file("./dummy_file")) {
        write_dummy_file(Data, Size);
        if (!doc.load_file("./dummy_file")) {
            // If loading raw data fails (not valid XML), create a minimal document structure
            // Use the name overload for append_child to set the name directly
            pugi::xml_node root_node = doc.append_child("root");
            if (root_node.empty()) {
                return 0;
            }
        }
    }

    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        // Fallback if root wasn't created correctly
        root = doc.append_child("root");
        if (root.empty()) {
            return 0;
        }
    }

    // 1. pugi::xml_node::append_child (type overload)
    pugi::xml_node node1 = root.append_child(pugi::node_element);
    if (node1.empty()) {
        return 0;
    }
    // Note: name() is a const getter. We cannot set the name here. 
    // The node will have an empty name.

    // 2. pugi::xml_node::append_child (name overload)
    pugi::xml_node node2 = root.append_child("child2");
    if (node2.empty()) {
        return 0;
    }

    // 3. pugi::xml_node::child
    pugi::xml_node found1 = root.child("child1");

    // 4. pugi::xml_node::append_buffer
    const char* buffer_data = "<inner>test</inner>";
    size_t buffer_size = std::strlen(buffer_data);
    pugi::xml_parse_result result1 = root.append_buffer(buffer_data, buffer_size);
    (void)result1; // Suppress unused warning

    // 5. pugi::xml_node::child
    pugi::xml_node found2 = root.child("inner");

    // 6. pugi::xml_node::child
    pugi::xml_node found3 = root.child("child2");

    // 7. pugi::xml_node::remove_children
    bool removed = root.remove_children();
    (void)removed; // Suppress unused warning

    // 8. pugi::xml_node::child
    pugi::xml_node found4 = root.child("child1");

    // 9. pugi::xml_node::append_buffer
    const char* buffer_data2 = "<new>data</new>";
    size_t buffer_size2 = std::strlen(buffer_data2);
    pugi::xml_parse_result result2 = root.append_buffer(buffer_data2, buffer_size2);
    (void)result2; // Suppress unused warning

    // 10. pugi::xml_node::child
    pugi::xml_node found5 = root.child("new");

    return 0;
}