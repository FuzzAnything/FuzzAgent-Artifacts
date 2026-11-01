// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_126(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a document and parse input or use dummy file
    pugi::xml_document doc;

    // Try parsing the input data directly as XML
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try loading from a dummy file written with input data
        write_dummy_file(Data, Size);
        if (!doc.load_file("./dummy_file")) {
            // If still fails, create a minimal valid XML structure to test APIs
            doc.append_child("root");
        }
    }

    // Ensure we have at least a root node to work with
    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        root = doc.append_child("root");
    }

    // Create some child nodes to manipulate
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");

    // Fuzz insert_move_after
    // Move child2 after child1
    pugi::xml_node moved_after = child1.insert_move_after(child2, child1);
    (void)moved_after; // Suppress unused warning

    // Fuzz last_child
    pugi::xml_node last = root.last_child();
    if (!last.empty()) {
        // Try to get path of last child
        pugi::string_t path = last.path('/');
        (void)path;
    }

    // Fuzz insert_child_before
    // Insert a new node before child3 (if it exists)
    if (!child3.empty()) {
        pugi::xml_node new_node = root.insert_child_before("new_child", child3);
        (void)new_node;
    }

    // Fuzz prepend_move
    // Move child1 to the beginning of root's children
    pugi::xml_node prepended = root.prepend_move(child1);
    (void)prepended;

    // Fuzz path
    if (!root.empty()) {
        pugi::string_t root_path = root.path('/');
        (void)root_path;
    }

    // Fuzz previous_sibling
    if (!child2.empty()) {
        pugi::xml_node prev = child2.previous_sibling();
        (void)prev;
    }

    // Cleanup: doc destructor handles memory automatically
    return 0;
}