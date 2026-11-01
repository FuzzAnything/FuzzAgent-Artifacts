// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <pugixml.hpp>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static std::string data_to_string(const uint8_t* data, size_t size) {
    return std::string(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to dummy file for file-based parsing
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // Parse from file
    result = doc.load_file("./dummy_file");
    if (!result) {
        // Try parsing from string if file load fails (e.g., binary data)
        std::string xml_str = data_to_string(Data, Size);
        result = doc.load_string(xml_str.c_str());
        if (!result) {
            // Try with a minimal valid XML wrapper if parsing fails completely
            std::string safe_xml = "<root>" + xml_str + "</root>";
            result = doc.load_string(safe_xml.c_str());
            if (!result) {
                return 0;
            }
        }
    }

    // Get root node
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // Traverse all nodes to test target functions
    std::vector<pugi::xml_node> nodes_to_test;
    for (pugi::xml_node node = root; node; node = node.next_sibling()) {
        nodes_to_test.push_back(node);
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            nodes_to_test.push_back(child);
        }
    }

    // Test each node with target functions
    for (const auto& node : nodes_to_test) {
        if (!node) continue;

        // 1. Test previous_sibling()
        pugi::xml_node prev = node.previous_sibling();
        (void)prev; // Suppress unused variable warning

        // 2. Test parent()
        pugi::xml_node parent = node.parent();
        (void)parent;

        // 3. Test last_child()
        pugi::xml_node last_child = node.last_child();
        (void)last_child;

        // 4. Test insert_child_before()
        // Create a new child name from input data if possible
        const char* name = "new_child";
        if (Size > 0 && Data[0] != 0) {
            // Use first byte as part of name, ensure it's safe
            char safe_name[32];
            snprintf(safe_name, sizeof(safe_name), "child_%c", (char)(Data[0] & 0x7F));
            name = safe_name;
        }
        
        // Try inserting before the node itself (if it has siblings)
        // Note: insert_child_before is not const, so we need a non-const reference
        // We cast away constness from the vector element
        pugi::xml_node& mutable_node = const_cast<pugi::xml_node&>(node);
        pugi::xml_node inserted_before = mutable_node.insert_child_before(name, mutable_node);
        (void)inserted_before;

        // Try inserting before a sibling if exists
        if (mutable_node.next_sibling()) {
            pugi::xml_node inserted_before_sibling = mutable_node.insert_child_before(name, mutable_node.next_sibling());
            (void)inserted_before_sibling;
        }

        // 5. Test path()
        std::string path_str = mutable_node.path('/');
        (void)path_str;

        // 6. Test insert_move_after()
        // Need two nodes to move
        if (!nodes_to_test.empty()) {
            // Try to move the current node after another node
            // We need a valid 'moved' node and a valid 'node' to move after
            // Use the root's first child if available
            pugi::xml_node first_child = root.first_child();
            if (first_child && first_child != mutable_node) {
                pugi::xml_node moved = mutable_node;
                pugi::xml_node target = first_child;
                // insert_move_after is also not const
                pugi::xml_node moved_after = target.insert_move_after(moved, target);
                (void)moved_after;
            }
        }
    }

    return 0;
}