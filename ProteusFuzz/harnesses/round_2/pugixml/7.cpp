// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a document and parse input data
    pugi::xml_document doc;
    
    // Limit the maximum size to prevent excessive memory usage or stack overflow
    const size_t max_size = 1000000;
    if (Size > max_size) {
        Size = max_size;
    }

    // We need to construct a valid XML string from the input data.
    // Since the input is raw bytes, it might contain null terminators or invalid characters.
    // We will wrap the data in a CDATA section or escape it, but for fuzzing, 
    // wrapping in a simple tag and ensuring null termination is crucial.
    
    // Create a buffer with a safe wrapper. 
    // We allocate enough space for the wrapper + data + null terminator.
    // We use a dynamic buffer to handle arbitrary sizes safely.
    size_t buffer_size = Size + 20; // <root>...</root> + null
    std::vector<char> buffer(buffer_size + 1);

    // Construct a valid XML string: <root>...data...</root>
    // We copy the input data directly. If it contains nulls, load_string will stop there,
    // which is safe. If it contains invalid XML, load_string will return false.
    std::strncpy(buffer.data(), "<root>", 6);
    std::memcpy(buffer.data() + 6, Data, Size);
    std::strncpy(buffer.data() + 6 + Size, "</root>", 7);
    buffer[6 + Size + 7] = '\0';

    // Ensure the buffer is null-terminated explicitly
    buffer[buffer_size] = '\0';

    bool parsed = doc.load_string(buffer.data());

    if (!parsed) {
        // If parsing fails with the wrapper, try to parse the raw data if it looks like XML
        // We only try this if the first byte is '<' to avoid obvious garbage
        if (Size > 0 && Data[0] == '<') {
            // We must ensure the raw data is null-terminated for load_string
            // Since we can't modify the input buffer, we create a temporary null-terminated copy
            // if it doesn't end with null. However, load_string handles non-null-terminated 
            // strings if we use load_buffer, but load_string expects null-terminated.
            // To be safe, we check if the last byte is null.
            bool is_null_terminated = (Data[Size - 1] == 0);
            
            if (!is_null_terminated) {
                // Create a copy with null terminator
                std::vector<char> safe_data(Size + 1);
                std::memcpy(safe_data.data(), Data, Size);
                safe_data[Size] = '\0';
                parsed = doc.load_string(safe_data.data());
            } else {
                parsed = doc.load_string(reinterpret_cast<const char*>(Data));
            }
        }
        
        if (!parsed) {
            // If parsing still fails, we cannot proceed with valid XML operations.
            // Return early to avoid undefined behavior.
            return 0;
        }
    }

    // Initialize xpath_node_set
    // We create one by evaluating an XPath expression.
    pugi::xpath_variable_set vars;
    pugi::xpath_query query("descendant-or-self::*", &vars);
    
    if (!query) {
        return 0;
    }

    pugi::xpath_node_set set = query.evaluate_node_set(doc);

    // 1. pugi::xpath_node_set::type
    pugi::xpath_node_set::type_t type = set.type();
    (void)type; 

    // 2. pugi::xpath_node_set::sort
    // Sort ascending
    set.sort(false);
    // Sort descending
    set.sort(true);

    // 3. pugi::xpath_node_set::empty
    bool is_empty = set.empty();
    (void)is_empty;

    // Prepare an xpath_node for the remaining tests
    pugi::xpath_node node;
    
    if (!set.empty()) {
        // Dereference the iterator returned by begin()
        node = *set.begin();
    } else {
        // If set is empty, create a dummy node from an attribute if possible
        pugi::xml_node root = doc.root();
        if (root) {
            pugi::xml_attribute attr = root.attribute("id");
            if (attr) {
                node = pugi::xpath_node(attr, root);
            } else {
                // Create a node from the root itself
                node = pugi::xpath_node(root);
            }
        } else {
            // Fallback to empty node
            node = pugi::xpath_node();
        }
    }

    // 4. pugi::xpath_node::attribute
    pugi::xml_attribute attr = node.attribute();
    (void)attr;

    // 5. pugi::xpath_node::parent
    pugi::xml_node parent = node.parent();
    (void)parent;

    // 6. pugi::xpath_node::node
    pugi::xml_node xml_node = node.node();
    (void)xml_node;

    // 7. pugi::xml_node::root
    // Use the parent node or the node itself to get the root
    pugi::xml_node root = (parent ? parent : xml_node).root();
    (void)root;

    return 0;
}