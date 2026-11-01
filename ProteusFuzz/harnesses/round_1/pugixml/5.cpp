// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>
#include <cstring>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    pugi::xml_document doc;
    write_dummy_file(Data, Size);
    
    // Load document, ignore errors to allow malformed input for fuzzing
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // If loading fails completely, try loading from string as fallback
    if (!result) {
        // Create a minimal valid XML to ensure we have a document structure
        doc.load_string("<root/>");
    }

    // Evaluate XPath to get a node set
    pugi::xpath_node_set node_set = doc.select_nodes("//*");
    
    // 1. pugi::xpath_node_set::type
    pugi::xpath_node_set::type_t set_type = node_set.type();
    (void)set_type; // Suppress unused variable warning

    // 2. pugi::xpath_node_set::sort
    // Try sorting in both directions to explore states
    node_set.sort(false);
    node_set.sort(true);

    // 3. pugi::xpath_node_set::empty
    bool is_empty = node_set.empty();
    (void)is_empty;

    // Get a specific node from the set to test xpath_node methods
    pugi::xpath_node xpath_node_item;
    if (!node_set.empty()) {
        xpath_node_item = node_set[0];
    } else {
        // If set is empty, create a dummy xpath_node from root
        pugi::xml_node root_node = doc.root();
        if (root_node) {
            xpath_node_item = pugi::xpath_node(root_node);
        }
    }

    // 4. pugi::xpath_node::attribute
    // This returns an xml_attribute; if the node is not an attribute, it returns invalid
    pugi::xml_attribute attr = xpath_node_item.attribute();
    (void)attr;

    // 5. pugi::xpath_node::parent
    pugi::xml_node parent_node = xpath_node_item.parent();
    (void)parent_node;

    // 6. pugi::xpath_node::node
    pugi::xml_node xml_node_item = xpath_node_item.node();
    (void)xml_node_item;

    // 7. pugi::xml_node::root
    // Ensure we have a valid node before calling root()
    if (xml_node_item) {
        pugi::xml_node root = xml_node_item.root();
        (void)root;
    } else {
        // Fallback to document root if the specific node is empty
        pugi::xml_node fallback_root = doc.root();
        if (fallback_root) {
            (void)fallback_root.root();
        }
    }

    // Cleanup is handled by RAII (destructors of doc, node_set, etc.)
    return 0;
}