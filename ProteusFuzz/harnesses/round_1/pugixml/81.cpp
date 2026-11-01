// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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

static pugi::xml_document* create_document_from_fuzz_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Try to parse the input as XML
    pugi::xml_parse_result result = doc->load_buffer(data, size);
    if (!result) {
        // If parsing fails, create a minimal valid XML to ensure we have nodes to work with
        // This helps test edge cases where the document is malformed but we still want to test the API
        if (!doc->load_string("<root><child/></root>")) {
            delete doc;
            return nullptr;
        }
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_fuzz_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->document_element();
    if (root.empty()) {
        delete doc;
        return 0;
    }

    // Create a custom walker for traverse test
    struct test_walker : public pugi::xml_tree_walker {
        bool begin(pugi::xml_node& node) override {
            return true;
        }
        bool for_each(pugi::xml_node& node) override {
            return true;
        }
        bool end(pugi::xml_node& node) override {
            return true;
        }
    };

    // Test 1: insert_child_after
    // We need a sibling to insert after. Let's try to find one or create one.
    pugi::xml_node ref_sibling = root.child("child");
    if (ref_sibling.empty()) {
        // If no child exists, create one first to have a reference
        ref_sibling = root.append_child("child");
    }
    
    // Try inserting after the sibling
    std::string_view new_node_name = "new_node";
    pugi::xml_node inserted = root.insert_child_after(new_node_name, ref_sibling);
    
    // Test 2: previous_sibling
    // Test on the inserted node or the reference sibling
    std::string_view prev_name_filter; // empty string view
    pugi::xml_node prev = inserted.empty() ? ref_sibling : inserted;
    pugi::xml_node prev_sib = prev.previous_sibling(prev_name_filter);
    
    // Test with a name filter
    pugi::xml_node prev_sib_filtered = prev.previous_sibling("child");

    // Test 3: remove_child
    // Try to remove a child by name
    std::string_view remove_name = "new_node";
    bool removed = root.remove_child(remove_name);
    
    // Try removing the original child
    std::string_view remove_child_name = "child";
    bool removed_child = root.remove_child(remove_child_name);

    // Test 4: parent
    // Get parent of root (should be null)
    pugi::xml_node root_parent = root.parent();
    
    // Get parent of a child if it exists
    pugi::xml_node child_node = root.child("child");
    if (!child_node.empty()) {
        pugi::xml_node child_parent = child_node.parent();
        (void)child_parent; // suppress unused warning
    }

    // Test 5: next_sibling
    // Find next sibling
    pugi::xml_node next_sib = child_node.empty() ? root : child_node;
    pugi::xml_node next = next_sib.next_sibling();
    
    // Find next sibling with name filter
    pugi::xml_node next_filtered = next_sib.next_sibling("new_node");

    // Test 6: traverse
    test_walker walker;
    bool traverse_result = root.traverse(walker);
    (void)traverse_result; // suppress unused warning

    delete doc;
    return 0;
}