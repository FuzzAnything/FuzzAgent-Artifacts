// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
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
#include <vector>
#include <algorithm>

static bool try_parse_document(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    
    // Ensure the input is valid UTF-8 or at least doesn't contain nulls in critical places
    // We'll try to parse it directly; if it fails, we return false
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    return result.status == pugi::status_ok;
}

static void safe_fuzz_operations(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (doc.empty()) {
        return;
    }

    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        return;
    }

    // Collect all nodes for testing
    std::vector<pugi::xml_node> nodes;
    for (pugi::xml_node n = root; !n.empty(); n = n.next_sibling()) {
        nodes.push_back(n);
        for (pugi::xml_node c = n.first_child(); !c.empty(); c = c.next_sibling()) {
            nodes.push_back(c);
        }
    }

    if (nodes.empty()) {
        return;
    }

    // Fuzz insert_move_after
    // Requires two nodes from the same document
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = 0; j < nodes.size(); ++j) {
            if (i == j) continue;
            // Attempt to move nodes[j] after nodes[i]
            // This might fail if nodes[j] is already a child of nodes[i] or if they are not siblings
            nodes[i].insert_move_after(nodes[j], nodes[i]);
        }
    }

    // Fuzz last_child
    for (const auto& node : nodes) {
        pugi::xml_node last = node.last_child();
        (void)last; // Use the result to prevent unused warnings
    }

    // Fuzz insert_child_before
    // We need a parent node and a sibling node
    for (size_t i = 0; i < nodes.size(); ++i) {
        pugi::xml_node parent = nodes[i];
        if (parent.type() != pugi::node_element) continue;
        
        for (size_t j = 0; j < nodes.size(); ++j) {
            pugi::xml_node sibling = nodes[j];
            if (sibling.type() != pugi::node_element) continue;
            
            // Try to insert a new child before the sibling
            // Use a simple name derived from data if available
            // CRITICAL FIX: Ensure the string persists for the duration of the call
            // We use a static buffer or a string object that lives long enough
            std::string name_str;
            name_str.reserve(20);
            
            if (size > 0 && data[0] != 0) {
                // Create a temporary string for the name
                size_t len = 0;
                while (len < size && len < 10 && data[len] != 0) {
                    if (data[len] >= 32 && data[len] <= 126) {
                        name_str += static_cast<char>(data[len]);
                    } else {
                        break;
                    }
                    len++;
                }
            }
            
            if (name_str.empty()) {
                name_str = "test";
            }
            
            // Pass the C-string from the string object, which is valid as long as name_str exists
            parent.insert_child_before(name_str.c_str(), sibling);
        }
    }

    // Fuzz prepend_move
    for (size_t i = 0; i < nodes.size(); ++i) {
        pugi::xml_node parent = nodes[i];
        if (parent.type() != pugi::node_element) continue;
        
        for (size_t j = 0; j < nodes.size(); ++j) {
            if (i == j) continue;
            pugi::xml_node moved = nodes[j];
            if (moved.type() != pugi::node_element) continue;
            
            // Attempt to prepend move
            parent.prepend_move(moved);
        }
    }

    // Fuzz path
    for (const auto& node : nodes) {
        std::string path = node.path('/');
        (void)path;
        
        // Try with different delimiters
        if (size > 0) {
            char delimiter = static_cast<char>(data[0] & 0x7F);
            if (delimiter == 0) delimiter = '/';
            path = node.path(delimiter);
            (void)path;
        }
    }

    // Fuzz previous_sibling
    for (const auto& node : nodes) {
        pugi::xml_node prev = node.previous_sibling();
        (void)prev;
    }
}

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Try to parse the input as XML
    if (!try_parse_document(Data, Size, doc)) {
        // If parsing fails, try to create a minimal valid XML document
        // to ensure we can still test the functions
        std::string fallback_xml = "<root><child/></root>";
        if (!doc.load_string(fallback_xml.c_str())) {
            return 0;
        }
    }

    // Perform fuzzing operations
    safe_fuzz_operations(doc, Data, Size);

    return 0;
}