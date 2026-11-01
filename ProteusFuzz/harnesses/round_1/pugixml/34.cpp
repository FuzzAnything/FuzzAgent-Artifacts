// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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

static void cleanup_document(pugi::xml_document& doc) {
    // pugixml xml_document does not have reset_memory, it cleans up automatically on destruction.
    // No explicit cleanup needed here other than letting it go out of scope.
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // Prepare environment: Load XML from fuzz input
    // We treat the input as a UTF-8 string. If it's invalid, parsing will fail gracefully.
    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    result = doc.load_string(xml_str.c_str());

    // If parsing failed, we can still test with an empty document or skip
    // However, to test select_nodes, we need a root node.
    // If load failed, we try to load a minimal valid XML to ensure the API path is exercised.
    if (!result) {
        // Fallback to a simple valid XML to ensure we have a root node for select_nodes
        doc.load_string("<root/>");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Prepare a query. We use a fixed safe query to ensure valid XPath syntax.
    const char* query = "//*"; // Select all nodes

    // Invoke: pugi::xml_node::select_nodes
    pugi::xpath_node_set nodes = root.select_nodes(query);

    // Invoke: pugi::xpath_node_set::size
    size_t sz = nodes.size();

    // Invoke: pugi::xpath_node_set::type
    pugi::xpath_node_set::type_t t = nodes.type();

    // Invoke: pugi::xpath_node_set::empty
    bool is_empty = nodes.empty();

    // Invoke: pugi::xpath_node_set::first
    pugi::xpath_node first_node = nodes.first();

    // Invoke: pugi::xpath_node_set::end
    pugi::xpath_node_set::const_iterator end_iter = nodes.end();

    // Additional exploration: Iterate if not empty to exercise more states
    if (!is_empty) {
        for (auto it = nodes.begin(); it != end_iter; ++it) {
            // Just access to ensure no crash
            (void)it->node();
        }
    }

    // Cleanup
    cleanup_document(doc);

    return 0;
}