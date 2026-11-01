// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <vector>

static pugi::xml_document* create_document_from_fuzz(const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;
    
    // Ensure null termination for safety
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';
    
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load_string(buffer.data());
    
    if (!result) {
        delete doc;
        return nullptr;
    }
    
    return doc;
}

static void cleanup_document(pugi::xml_document* doc) {
    if (doc) {
        delete doc;
    }
}

extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a document from fuzz input
    pugi::xml_document* doc = create_document_from_fuzz(Data, Size);
    if (!doc) return 0;
    
    pugi::xml_node root = doc->document_element();
    if (!root) {
        // If no root element, try to create one
        pugi::xml_node new_root = doc->append_child("root");
        if (!new_root) {
            cleanup_document(doc);
            return 0;
        }
        root = new_root;
    }
    
    // Ensure we have some child nodes to work with
    if (!root.first_child()) {
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
    }
    
    pugi::xml_node child1 = root.first_child();
    pugi::xml_node child2 = child1.next_sibling();
    pugi::xml_node child3 = child2.next_sibling();
    
    // Test 1: insert_move_after
    if (child1 && child2 && child3) {
        pugi::xml_node moved = child3;
        pugi::xml_node target = child1;
        child2.insert_move_after(moved, target);
    }
    
    // Test 2: last_child
    if (root) {
        pugi::xml_node last = root.last_child();
        (void)last; // Use the result
    }
    
    // Test 3: insert_child_before
    if (child1 && child2) {
        std::string_view new_name = "new_child";
        child1.insert_child_before(new_name.data(), child2);
    }
    
    // Test 4: prepend_move
    if (child1 && child2 && child3) {
        pugi::xml_node to_move = child3;
        child1.prepend_move(to_move);
    }
    
    // Test 5: previous_sibling
    if (child2) {
        pugi::xml_node prev = child2.previous_sibling();
        (void)prev; // Use the result
    }
    
    // Test 6: path
    if (root) {
        std::string path_str = root.path();
        (void)path_str; // Use the result
    }
    
    // Additional diverse testing
    if (child1) {
        // Test with different nodes
        pugi::xml_node temp = child1.append_child("temp");
        if (temp && child2) {
            child1.insert_move_after(temp, child1);
            child1.prepend_move(temp);
        }
        
        // Test path on various nodes
        std::string p1 = child1.path();
        std::string p2 = child2.path();
        (void)p1;
        (void)p2;
    }
    
    cleanup_document(doc);
    return 0;
}