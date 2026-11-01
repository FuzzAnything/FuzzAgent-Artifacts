// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <string_view>

static bool try_parse_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return false;
    }
    // Create a string from the fuzz input, ensuring null termination for safety
    // We construct a std::string which manages its own memory and null-termination
    std::string input(reinterpret_cast<const char*>(data), size);
    // Try parsing as text
    return doc.load_string(input.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Step 1: Prepare environment
    // Attempt to parse the input data into an XML document
    if (!try_parse_document(doc, Data, Size)) {
        // If parsing fails, try to create a minimal valid document to test API behavior on empty/valid structures
        doc.append_child("root");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        // If no root exists (should be rare after append above), create one
        root = doc.append_child("root");
    }

    // Helper to get a safe string for XML tag names from input data
    // We avoid creating a string_view that points to the raw fuzz buffer directly 
    // if we are going to extract a substring, to avoid potential buffer overflows 
    // if the underlying buffer is not null-terminated or if logic is flawed.
    // Instead, we create a temporary std::string which is safe.
    std::string safe_input(reinterpret_cast<const char*>(Data), Size);
    
    // Create a safe name for XML nodes. Ensure it doesn't contain nulls or invalid chars if possible,
    // though pugixml handles most. We limit length.
    std::string safe_name = "child";
    if (safe_input.size() > 0) {
        // Take up to 10 characters, ensuring we don't go out of bounds
        size_t len = safe_input.size() < 10 ? safe_input.size() : 10;
        safe_name = safe_input.substr(0, len);
        // Ensure no null bytes in the middle which might truncate unexpectedly in some contexts,
        // though std::string handles them. For XML names, nulls are invalid anyway.
        // We just use it as is, pugixml will handle invalid names gracefully or ignore.
    }

    // Step 3: Explore program states by invoking target functions diversely

    // 1. Test append_child
    pugi::xml_node new_child = root.append_child(safe_name.c_str());

    // 2. Test child
    pugi::xml_node found_child = root.child(safe_name.c_str());
    
    // 3. Test last_child
    pugi::xml_node last = root.last_child();

    // 4. Test append_move
    // We need a node to move. If we have at least two children, move the second to the first.
    if (new_child) {
        pugi::xml_node sibling = new_child.append_child("sibling");
        if (sibling) {
            // Try moving sibling back to root (it's already a child of new_child, so we move it to root)
            // Note: append_move moves the node to be a child of the caller
            pugi::xml_node moved = root.append_move(sibling);
            (void)moved; // Suppress unused warning
        }
    }

    // 5. Test insert_move_before
    // Requires two valid nodes in the same document.
    if (new_child && found_child) {
        // Try to move new_child before found_child
        pugi::xml_node moved_before = root.insert_move_before(new_child, found_child);
        (void)moved_before;
    }

    // 6. Test insert_copy_after
    // Requires attributes. Let's add an attribute first.
    if (new_child) {
        pugi::xml_attribute attr = new_child.append_attribute("test_attr");
        attr.set_value("value");
        
        pugi::xml_attribute proto = new_child.append_attribute("proto_attr");
        proto.set_value("proto_value");

        // The prompt signature for #6 was: xml_node insert_copy_after(const xml_node& proto, const xml_node& node);
        // This corresponds to the node copying version in pugixml.
        // We need two nodes to copy.
        
        pugi::xml_node proto_node = new_child.append_child("proto_node");
        pugi::xml_node target_node = new_child.append_child("target_node");
        
        if (proto_node && target_node) {
            // Copy proto_node after target_node
            pugi::xml_node copied_node = new_child.insert_copy_after(proto_node, target_node);
            (void)copied_node;
        }
    }

    // Step 2: Cleanup
    // xml_document destructor handles memory cleanup automatically when it goes out of scope.
    // No explicit cleanup needed for pugi::xml_node as they are lightweight wrappers.
    
    return 0;
}