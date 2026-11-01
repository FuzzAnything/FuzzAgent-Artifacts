// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
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
#include <string>
#include <string_view>

static void cleanup_document(pugi::xml_document& doc) {
    // pugixml xml_document manages memory automatically.
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Prepare the environment: load XML from fuzz input
    // Ensure the input is null-terminated for load_string to work safely with strlen
    // We create a temporary buffer with a null terminator
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    // If parsing fails, we still want to test some functions on an empty/invalid state
    // but we need a valid document structure to attach nodes to if we want to test insertion
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }

    // Fuzz last_child
    if (root) {
        pugi::xml_node last = root.last_child();
        (void)last; // Use the result to avoid unused variable warnings
    }

    // Fuzz parent
    if (root) {
        pugi::xml_node parent = root.parent();
        (void)parent;
    }
    
    // Fuzz path
    if (root) {
        pugi::string_t path = root.path('/');
        (void)path;
        
        // Try with a different delimiter if possible (char_t might be wchar_t)
        pugi::char_t delimiter = '|';
        pugi::string_t path2 = root.path(delimiter);
        (void)path2;
    }

    // Fuzz insert_move_after
    // We need at least two children to move
    if (root) {
        pugi::xml_node child1 = root.append_child("child1");
        pugi::xml_node child2 = root.append_child("child2");
        pugi::xml_node child3 = root.append_child("child3");
        
        if (child1 && child2 && child3) {
            // Move child3 after child1
            pugi::xml_node moved = child1.insert_move_after(child3, child2);
            (void)moved;
            
            // Try moving a node to itself (should fail)
            pugi::xml_node self_move = child1.insert_move_after(child1, child1);
            (void)self_move;
            
            // Try moving a node from a different document (create a new doc)
            pugi::xml_document doc2;
            doc2.append_child("root2");
            pugi::xml_node other_child = doc2.root().append_child("other");
            pugi::xml_node cross_doc = child1.insert_move_after(other_child, child2);
            (void)cross_doc;
            // doc2 will be cleaned up automatically when it goes out of scope
        }
    }

    // Fuzz insert_child_before
    if (root) {
        pugi::xml_node sibling = root.append_child("sibling");
        if (sibling) {
            // Insert before sibling
            pugi::xml_node new_node = root.insert_child_before("new_node", sibling);
            (void)new_node;
            
            // Insert before a null node (should append)
            pugi::xml_node null_node;
            pugi::xml_node new_node2 = root.insert_child_before("new_node2", null_node);
            (void)new_node2;
            
            // Try with empty name
            pugi::xml_node empty_name = root.insert_child_before("", sibling);
            (void)empty_name;
        }
    }

    // Fuzz previous_sibling
    if (root) {
        pugi::xml_node child = root.first_child();
        if (child) {
            pugi::xml_node prev = child.previous_sibling();
            (void)prev;
            
            // Traverse siblings
            pugi::xml_node curr = child;
            while (curr) {
                pugi::xml_node p = curr.previous_sibling();
                curr = p;
            }
        }
    }

    // Cleanup
    cleanup_document(doc);

    return 0;
}