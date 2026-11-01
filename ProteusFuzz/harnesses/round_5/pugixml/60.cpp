// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
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
#include <string>
#include <string_view>

static std::string generate_string_from_fuzz_data(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        return "";
    }
    size_t len = 0;
    // Find a null terminator or limit to size
    while (offset + len < size && data[offset + len] != 0 && len < 1024) {
        len++;
    }
    return std::string(reinterpret_cast<const char*>(data + offset), len);
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Parse the fuzzed data as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    // We proceed regardless of parse result to test API behavior on potentially empty or partial docs

    pugi::xml_node root = doc.root();
    if (!root) {
        // If no root exists, create one to ensure we have a valid node to work with
        root = doc.append_child("root");
        if (!root) return 0;
    }

    // Helper to get a string from data
    auto get_str = [&](size_t idx) -> std::string {
        if (idx >= Size) return "";
        return generate_string_from_fuzz_data(Data, Size, idx);
    };

    // 1. Test append_child
    std::string child_name = get_str(0);
    pugi::xml_node child_node = root.append_child(child_name);
    
    if (child_node) {
        // Add some attributes to the child to test insert_copy_after later (if we were testing attributes)
        // But the target API is xml_node::insert_copy_after(const xml_node&, const xml_node&)
        // So we need child nodes, not attributes, for the proto and target.
        
        // Add a child to act as 'proto' for node copy
        std::string proto_child_name = get_str(1);
        pugi::xml_node proto_node = child_node.append_child(proto_child_name);
        
        // Add another child to act as the 'after' target
        std::string target_child_name = get_str(2);
        pugi::xml_node target_node = child_node.append_child(target_child_name);

        // 2. Test last_child
        pugi::xml_node last = child_node.last_child();
        (void)last;

        // 3. Test child
        std::string search_name = get_str(3);
        pugi::xml_node found_child = child_node.child(search_name);
        (void)found_child;

        // 4. Test insert_copy_after (Node version)
        // Insert a copy of proto_node after target_node
        if (proto_node && target_node) {
            pugi::xml_node inserted = child_node.insert_copy_after(proto_node, target_node);
            (void)inserted;
        }

        // 5. Test append_move
        // Create a node to move
        std::string move_src_name = get_str(4);
        pugi::xml_node move_src = child_node.append_child(move_src_name);
        if (move_src) {
            // Move it to root
            pugi::xml_node moved = root.append_move(move_src);
            (void)moved;
        }
    }

    // 6. Test insert_move_before
    // Create nodes for move before at root level
    std::string move_before_src = get_str(5);
    std::string move_before_target = get_str(6);
    
    pugi::xml_node src_node = root.append_child(move_before_src);
    pugi::xml_node target_node = root.append_child(move_before_target);
    
    if (src_node && target_node) {
        pugi::xml_node moved_before = root.insert_move_before(src_node, target_node);
        (void)moved_before;
    }

    return 0;
}