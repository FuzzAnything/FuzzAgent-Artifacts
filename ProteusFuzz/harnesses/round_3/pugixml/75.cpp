// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>
#include <string_view>
#include <vector>

static pugi::xml_document create_document_from_fuzz(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.append_child("root");
        return doc;
    }
    
    // Ensure the data is null-terminated for parsing
    std::vector<uint8_t> buffer(data, data + size);
    buffer.push_back(0);
    
    if (!doc.load_string(reinterpret_cast<const char*>(buffer.data()))) {
        // If parsing fails, create a minimal valid document
        doc.append_child("root");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc = create_document_from_fuzz(Data, Size);
    pugi::xml_node root = doc.document_element();
    
    if (!root) {
        root = doc.append_child("root");
    }

    // Fuzz last_child
    {
        pugi::xml_node last = root.last_child();
        (void)last; // Suppress unused variable warning
    }

    // Fuzz child with string_view_t
    {
        std::string_view name_view(reinterpret_cast<const char*>(Data), Size);
        // Truncate to avoid potential issues with very long strings or embedded nulls
        size_t max_len = 1024;
        if (Size > max_len) Size = max_len;
        name_view = std::string_view(reinterpret_cast<const char*>(Data), Size);
        
        pugi::xml_node found = root.child(name_view);
        (void)found;
    }

    // Fuzz first_child
    {
        pugi::xml_node first = root.first_child();
        (void)first;
    }

    // Fuzz find_node with a simple predicate
    {
        // Predicate: check if node name starts with 'a' (case insensitive check not needed for simple fuzz)
        auto predicate = [](pugi::xml_node n) {
            const char* name = n.name();
            if (!name) return false;
            return name[0] == 'a' || name[0] == 'A';
        };
        pugi::xml_node found = root.find_node(predicate);
        (void)found;
    }

    // Fuzz insert_copy_after
    {
        // We need a proto node and a reference node
        // Create some structure if it doesn't exist
        if (!root.child("proto")) {
            root.append_child("proto");
        }
        if (!root.child("ref")) {
            root.append_child("ref");
        }
        
        pugi::xml_node proto = root.child("proto");
        pugi::xml_node ref = root.child("ref");
        
        // Attempt to insert copy after ref
        pugi::xml_node inserted = root.insert_copy_after(proto, ref);
        (void)inserted;
        
        // Also try with invalid nodes to test error handling
        pugi::xml_node invalid_node;
        pugi::xml_node inserted_invalid = root.insert_copy_after(invalid_node, ref);
        (void)inserted_invalid;
    }

    // Fuzz append_move
    {
        // Create a node to move
        if (!root.child("movable")) {
            root.append_child("movable");
        }
        pugi::xml_node movable = root.child("movable");
        
        // Move it to root (it's already a child, but let's try moving it again or to a sibling)
        // First, ensure we have a target
        if (!root.child("target")) {
            root.append_child("target");
        }
        pugi::xml_node target = root.child("target");
        
        // Move movable to target
        pugi::xml_node moved = target.append_move(movable);
        (void)moved;
        
        // Try moving an invalid node
        pugi::xml_node invalid_node;
        pugi::xml_node moved_invalid = target.append_move(invalid_node);
        (void)moved_invalid;
    }

    return 0;
}