// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;

    // Ensure we have a null-terminated string for parsing if we try to load from buffer
    // We will primarily build the tree programmatically to test the specific APIs robustly
    // but also try parsing if the data looks like text.
    
    // Strategy: Use input data to generate names and values for nodes/attributes
    // to exercise append_child, set_name, child, last_child, previous_sibling, insert_copy_after
    
    // 1. Initialize document and root
    if (!doc.load_string("<root></root>")) {
        // If load fails (unlikely for this string), try to parse from input if valid XML
        // We need to ensure null termination for load_string
        if (Size > 0 && Data[Size - 1] != 0) {
            // Create a safe buffer with null terminator
            char* safe_buf = new char[Size + 1];
            memcpy(safe_buf, Data, Size);
            safe_buf[Size] = '\0';
            if (!doc.load_string(safe_buf)) {
                // If parsing fails, start fresh
                doc.reset();
                pugi::xml_node root = doc.append_child("root");
            }
            delete[] safe_buf;
        } else {
            doc.reset();
            pugi::xml_node root = doc.append_child("root");
        }
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Helper to get a null-terminated string from fuzz input
    auto get_safe_string = [&](size_t start, size_t max_len) -> std::string_view {
        if (start >= Size) return std::string_view();
        size_t len = 0;
        while (start + len < Size && len < max_len && Data[start + len] != 0) {
            len++;
        }
        return std::string_view(reinterpret_cast<const char*>(Data) + start, len);
    };

    // Generate some nodes to test traversal
    for (size_t i = 0; i < Size && i < 100; ++i) {
        // Ensure we have enough data for a name
        if (i + 1 >= Size) break;
        
        std::string_view name_sv = get_safe_string(i, 50);
        if (name_sv.empty()) continue;

        // Test append_child
        pugi::xml_node new_node = root.append_child(name_sv);
        
        // Test set_name on the new node
        std::string_view new_name_sv = get_safe_string(i + 1, 50);
        if (!new_name_sv.empty()) {
            new_node.set_name(new_name_sv);
        }

        // Add an attribute to test insert_copy_after later
        if (i + 2 < Size) {
            std::string_view attr_name = get_safe_string(i + 2, 30);
            if (!attr_name.empty()) {
                pugi::xml_attribute attr = new_node.append_attribute(attr_name);
                if (attr) {
                    attr.set_value("test");
                }
            }
        }
    }

    // Test last_child
    pugi::xml_node last = root.last_child();
    (void)last; // Use the result to avoid unused warnings, logic is tested by execution

    // Test child
    if (Size > 0) {
        std::string_view search_name = get_safe_string(0, 50);
        pugi::xml_node found = root.child(search_name);
        (void)found;
    }

    // Test previous_sibling
    if (root) {
        pugi::xml_node curr = root.last_child();
        while (curr) {
            pugi::xml_node prev = curr.previous_sibling();
            curr = prev;
        }
    }

    // Test insert_copy_after
    // We need two attributes to test this: one to copy (proto) and one to insert after (node)
    if (root) {
        pugi::xml_attribute proto = root.first_attribute();
        pugi::xml_attribute after = pugi::xml_attribute(); // Default constructed (empty)
        
        if (proto) {
            after = proto.next_attribute();
        }
        
        if (proto && after) {
            // Insert a copy of proto after after
            pugi::xml_attribute inserted = root.insert_copy_after(proto, after);
            (void)inserted;
        }
        
        // Try inserting after a non-existent attribute (should return null)
        pugi::xml_attribute dummy;
        if (proto) {
            pugi::xml_attribute res = root.insert_copy_after(proto, dummy);
            (void)res;
        }
    }

    // Cleanup is handled by doc destructor (RAII)
    return 0;
}