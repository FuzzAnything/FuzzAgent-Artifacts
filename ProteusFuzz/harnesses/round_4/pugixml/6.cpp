// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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

static bool try_parse_document(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) return false;
    // Ensure the input is null-terminated for parsing
    char* buffer = new char[size + 1];
    std::memcpy(buffer, data, size);
    buffer[size] = '\0';
    
    pugi::xml_parse_result result = doc.load_string(buffer);
    delete[] buffer;
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    
    // Step 1: Prepare environment - parse the fuzzed input as XML
    if (!try_parse_document(Data, Size, doc)) {
        // If parsing fails, try with a minimal valid XML to test logic on empty/invalid states
        if (!doc.load_string("<root/>")) {
            return 0;
        }
    }

    // Ensure we have a root to work with
    pugi::xml_node root_node = doc.root();
    if (!root_node) {
        return 0;
    }

    // Execute target API functions in the specified order
    
    // 1. pugi::xpath_node_set::type
    // We need an xpath_node_set. Create one by evaluating an XPath expression.
    // If the expression fails, we get an empty set, which is also a valid state to test.
    pugi::xpath_node_set set = root_node.select_nodes("//node[@id]"); 
    pugi::xpath_node_set::type_t set_type = set.type();
    (void)set_type; // Use the value to suppress unused warnings

    // 2. pugi::xpath_node_set::sort
    // Sort the set (ascending by default)
    set.sort();
    // Sort descending to explore another state
    set.sort(true);

    // 3. pugi::xpath_node_set::empty
    bool is_empty = set.empty();
    (void)is_empty;

    // Prepare an xpath_node for the next steps
    // If the set is not empty, pick the first node; otherwise, use a default constructed (empty) node
    pugi::xpath_node xpath_node_obj;
    if (!set.empty()) {
        xpath_node_obj = set.begin()[0];
    }

    // 4. pugi::xpath_node::attribute
    pugi::xml_attribute attr = xpath_node_obj.attribute();
    (void)attr;

    // 5. pugi::xpath_node::parent
    pugi::xml_node parent_node = xpath_node_obj.parent();
    (void)parent_node;

    // 6. pugi::xpath_node::node
    pugi::xml_node contained_node = xpath_node_obj.node();
    (void)contained_node;

    // 7. pugi::xml_node::root
    // We can call root() on the root_node itself, or on any node we found
    pugi::xml_node root_from_node = root_node.root();
    (void)root_from_node;
    
    // Also test root() on the parent_node if it's valid
    if (parent_node) {
        pugi::xml_node root_from_parent = parent_node.root();
        (void)root_from_parent;
    }

    // Cleanup is handled by RAII (pugixml destructors)
    return 0;
}