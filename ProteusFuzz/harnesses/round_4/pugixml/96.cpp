// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
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
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to dummy file for loading
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");

    // Helper lambda to safely get a node
    auto get_node = [&](pugi::xml_node n) -> pugi::xml_node {
        return n;
    };

    // Test first_child and last_child on root and various nodes
    pugi::xml_node root = doc;
    pugi::xml_node first = root.first_child();
    pugi::xml_node last = root.last_child();

    // Traverse children
    for (pugi::xml_node cur = first; cur; cur = cur.next_sibling()) {
        pugi::xml_node fc = cur.first_child();
        pugi::xml_node lc = cur.last_child();
        (void)fc;
        (void)lc;
    }

    // Test child() with various names derived from input
    std::string_view sv(reinterpret_cast<const char*>(Data), Size);
    // Use a safe substring to avoid out-of-bounds if string is not null-terminated
    std::string_view name_view = sv.substr(0, std::min(sv.size(), static_cast<size_t>(10)));
    pugi::xml_node found_child = root.child(name_view);
    (void)found_child;

    // Test find_node with a simple predicate
    // Predicate: node name starts with first byte of input as char
    char search_char = static_cast<char>(Data[0]);
    auto pred = [search_char](pugi::xml_node n) {
        const char* name = n.name();
        if (!name) return false;
        // Simple check: first character matches
        return name[0] == search_char;
    };
    pugi::xml_node found_node = root.find_node(pred);
    (void)found_node;

    // Test append_move: move a node to another node within same document
    // We need two valid nodes: source and target
    pugi::xml_node target = root;
    pugi::xml_node source = first; // first child of root

    // Only move if source is valid and not the root itself
    if (source && source != root) {
        // Move source to target
        pugi::xml_node moved = target.append_move(source);
        (void)moved;
    }

    // Test insert_copy_after: insert a copy of a node after another node
    // We need a node with children to act as parent
    for (pugi::xml_node n = root; n; n = n.next_sibling()) {
        pugi::xml_node proto_node = n;
        pugi::xml_node after_node = n; // same node, but we need a sibling?
        // The function inserts a copy of proto after after_node.
        // So after_node must be a child of the current node (the one calling the function).
        // Let's try to find a node that has children
        pugi::xml_node child = n.first_child();
        if (child) {
            // Try to insert a copy of child after itself (might be invalid, but safe to try)
            pugi::xml_node inserted = n.insert_copy_after(child, child);
            (void)inserted;
        }
        break;
    }

    // Cleanup: the document is automatically cleaned up when it goes out of scope.
    return 0;
}