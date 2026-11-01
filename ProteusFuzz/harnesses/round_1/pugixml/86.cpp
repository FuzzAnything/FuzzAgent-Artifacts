// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Ensure the input buffer is null-terminated for load_string
    // We create a temporary buffer with a null terminator
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Check if the input is valid XML. If not, we construct a minimal valid XML
    // to ensure the API functions are tested on valid structures.
    if (!doc.load_string(input_str.c_str())) {
        // Fallback: create a simple document manually to avoid parsing garbage
        // which might cause heap overflows if the parser expects valid structure
        // but receives truncated or malformed data that it tries to read past.
        // We construct a safe XML string.
        std::string safe_xml = "<root>";
        // Append some safe data from input if possible, ensuring it's valid XML content
        for (size_t i = 0; i < Size && i < 100; ++i) {
            unsigned char c = Data[i];
            if (c >= 32 && c < 127 && c != '<' && c != '>' && c != '&' && c != '"' && c != '\'') {
                safe_xml += static_cast<char>(c);
            } else {
                safe_xml += 'x';
            }
        }
        safe_xml += "</root>";
        
        if (!doc.load_string(safe_xml.c_str())) {
            // If even the safe construction fails (unlikely), return
            return 0;
        }
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Prepare string_view for name parameters
    // We use a sanitized version of the input to avoid potential issues with invalid names
    std::string name_str;
    name_str.reserve(Size);
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        if (c >= 32 && c < 127 && c != '<' && c != '>' && c != '&' && c != '"' && c != '\'') {
            name_str += static_cast<char>(c);
        } else {
            name_str += 'x';
        }
    }
    if (name_str.empty()) name_str = "test";
    
    std::string_view name_view = std::string_view(name_str);

    // 1. Test append_child
    pugi::xml_node new_child = root.append_child(name_view);
    if (new_child) {
        new_child.text().set("fuzz_data");
    }

    // 2. Test child
    pugi::xml_node found_child = root.child(name_view);
    (void)found_child;

    // 3. Test last_child
    pugi::xml_node last = root.last_child();
    (void)last;

    // 4. Test append_move
    if (root.last_child()) {
        pugi::xml_node to_move = root.last_child();
        pugi::xml_node moved_result = root.append_move(to_move);
        (void)moved_result;
    }

    // 5. Test insert_move_before
    pugi::xml_node first_child = root.first_child();
    pugi::xml_node second_child = pugi::xml_node();
    
    if (first_child) {
        second_child = first_child.next_sibling();
    }
    
    if (first_child && second_child) {
        pugi::xml_node inserted = root.insert_move_before(first_child, second_child);
        (void)inserted;
    } else if (first_child) {
        pugi::xml_node inserted = root.insert_move_before(first_child, first_child);
        (void)inserted;
    }

    // 6. Test insert_copy_after
    pugi::xml_node proto = root.first_child();
    pugi::xml_node target = pugi::xml_node();
    
    if (proto) {
        target = proto.next_sibling();
    }
    
    if (proto && target) {
        pugi::xml_node copied = root.insert_copy_after(proto, target);
        (void)copied;
    } else if (proto) {
        pugi::xml_node copied = root.insert_copy_after(proto, proto);
        (void)copied;
    }

    // Additional exploration: Create more nodes to increase state space
    for (size_t i = 0; i < 5 && i < Size; ++i) {
        pugi::xml_node child = root.append_child("child");
        if (child) {
            child.text().set("test");
        }
    }

    // Re-test operations with more nodes
    pugi::xml_node c1 = root.first_child();
    pugi::xml_node c2 = pugi::xml_node();
    if (c1) {
        c2 = c1.next_sibling();
    }

    if (c1 && c2) {
        root.insert_move_before(c1, c2);
        root.insert_copy_after(c1, c2);
    }

    return 0;
}