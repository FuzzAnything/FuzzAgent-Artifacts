// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
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

static pugi::xml_document parse_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    if (!result) {
        return pugi::xml_document();
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Parse the input data as XML
    pugi::xml_document doc = parse_document(Data, Size);
    
    // If document is empty (parsing failed), we can't proceed with XPath queries safely
    if (doc.empty()) {
        return 0;
    }

    // Initialize XPath query context
    pugi::xpath_variable_set vars;
    pugi::xpath_query query("//*", &vars);
    
    // Evaluate XPath to get a node set
    pugi::xpath_node_set set = query.evaluate_node_set(doc);

    // 1. Invoke pugi::xpath_node_set::type
    // Determine the collection type (unordered, sorted, etc.)
    pugi::xpath_node_set::type_t type = set.type();
    (void)type; // Suppress unused variable warning

    // 2. Invoke pugi::xpath_node_set::sort
    // Sort the collection. We try both ascending and descending based on input data
    if (Size > 0 && Data[0] % 2 == 0) {
        set.sort(false); // Ascending
    } else {
        set.sort(true);  // Descending
    }

    // 3. Invoke pugi::xpath_node_set::empty
    // Check if the set is empty after sorting
    bool is_empty = set.empty();
    (void)is_empty;

    // 4-6. Iterate through the set to invoke xpath_node methods
    // We need a valid xpath_node to call attribute(), parent(), node()
    if (!set.empty()) {
        // Get the first node from the set. set.begin() returns an iterator (pointer).
        // We must dereference it to get the xpath_node object.
        pugi::xpath_node node = *set.begin();
        
        // 4. Invoke pugi::xpath_node::attribute
        pugi::xml_attribute attr = node.attribute();
        (void)attr;

        // 5. Invoke pugi::xpath_node::parent
        pugi::xml_node parent = node.parent();
        (void)parent;

        // 6. Invoke pugi::xpath_node::node
        pugi::xml_node xml_node_val = node.node();
        (void)xml_node_val;
    }

    // 7. Invoke pugi::xml_node::root
    // Get the root node of the document
    pugi::xml_node root = doc.root();
    (void)root;

    // Cleanup is handled by RAII (destructors of pugi objects)
    return 0;
}