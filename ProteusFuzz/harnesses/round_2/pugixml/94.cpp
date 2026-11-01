// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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

static bool parse_xml_from_fuzz_data(const uint8_t *Data, size_t Size, pugi::xml_document &doc) {
    if (Size == 0) {
        return false;
    }
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    return result.status == pugi::status_ok;
}

static class CustomWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node &node) override {
        (void)node;
        return true;
    }

    bool end(pugi::xml_node &node) override {
        (void)node;
        return true;
    }

    bool for_each(pugi::xml_node &node) override {
        (void)node;
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Attempt to parse the fuzzed input
    if (!parse_xml_from_fuzz_data(Data, Size, doc)) {
        // Proceed to test behavior on potentially empty/invalid docs
    }

    // Get root node
    pugi::xml_node root = doc.root();
    
    // Test pugi::xml_node::root
    pugi::xml_node root_from_root = root.root();
    (void)root_from_root;

    // Test pugi::xml_node::parent
    pugi::xml_node parent_of_root = root.parent();
    (void)parent_of_root;

    // Traverse children to test sibling navigation
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        // Test pugi::xml_node::parent on a child
        pugi::xml_node child_parent = child.parent();
        (void)child_parent;

        // Test pugi::xml_node::next_sibling() (no args)
        pugi::xml_node next_sibling_no_arg = child.next_sibling();
        (void)next_sibling_no_arg;

        // Test pugi::xml_node::next_sibling(name)
        const pugi::char_t *current_name = child.name();
        if (current_name) {
            pugi::xml_node next_sibling_named = child.next_sibling(current_name);
            (void)next_sibling_named;
        } else {
            pugi::xml_node next_sibling_named = child.next_sibling("dummy");
            (void)next_sibling_named;
        }
        
        // Try searching for a name derived from the fuzz buffer
        if (Size > 0) {
            size_t name_len = (Size < 20) ? Size : 20;
            std::string search_name(reinterpret_cast<const char*>(Data), name_len);
            pugi::xml_node next_sibling_fuzz_name = child.next_sibling(search_name.c_str());
            (void)next_sibling_fuzz_name;
        }
    }

    // Test xml_tree_walker::begin and xml_tree_walker::end
    CustomWalker walker;
    root.traverse(walker);

    // Test with a specific node if we can find one
    pugi::xml_node test_node = root.first_child();
    if (!test_node.empty()) {
        // Test root() on a child
        pugi::xml_node child_root = test_node.root();
        (void)child_root;

        // Test parent() on a child
        pugi::xml_node child_parent_check = test_node.parent();
        (void)child_parent_check;

        // Test next_sibling on a child
        pugi::xml_node child_next = test_node.next_sibling();
        (void)child_next;
        
        // Test next_sibling with name on a child
        const pugi::char_t *child_name = test_node.name();
        if (child_name) {
            pugi::xml_node child_next_named = test_node.next_sibling(child_name);
            (void)child_next_named;
        }
    }

    return 0;
}