// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
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

// Helper to safely convert input data to a C-string
// Ensures the string is null-terminated by creating a temporary buffer if needed
static const pugi::char_t* safe_cstr(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    
    // Check if the input already ends with a null terminator
    if (size > 0 && data[size - 1] == 0) {
        return reinterpret_cast<const pugi::char_t*>(data);
    }
    
    // Use a static buffer for the string conversion to avoid allocation overhead in the hot path
    // Note: This is not thread-safe, but fuzzing is single-threaded.
    static pugi::char_t static_buf[4096];
    size_t len = (size > sizeof(static_buf) - 1) ? sizeof(static_buf) - 1 : size;
    
    // Copy data, ensuring null termination
    memcpy(static_buf, data, len);
    static_buf[len] = 0;
    
    return static_buf;
}

// Helper to create a temporary buffer for evaluate_string buffer overload
static pugi::char_t* create_temp_buffer(size_t size) {
    if (size == 0) size = 1;
    // Cap size to prevent huge allocations
    if (size > 1024 * 1024) size = 1024 * 1024;
    
    pugi::char_t* buf = new (std::nothrow) pugi::char_t[size];
    if (!buf) return nullptr;
    memset(buf, 0, size * sizeof(pugi::char_t));
    return buf;
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    try {
        // 1. Prepare the environment
        pugi::xml_document doc;
        
        // Use safe_cstr to ensure null-termination for load_string
        const pugi::char_t* xml_str = safe_cstr(Data, Size);
        
        // Parse the input as XML. If parsing fails, we still have an empty document to test against.
        pugi::xml_parse_result result = doc.load_string(xml_str);
        
        // Get the root node (or the document node itself if no root)
        pugi::xml_node root = doc.first_child();
        if (!root) {
            root = doc;
        }

        // Prepare XPath query
        pugi::xpath_query query;
        try {
            // Ensure the query string is also null-terminated
            const pugi::char_t* query_str = safe_cstr(Data, Size);
            query = pugi::xpath_query(query_str);
        } catch (...) {
            // If query construction fails, we use a default valid query to continue testing other APIs
            query = pugi::xpath_query("true()");
        }

        // Create a context node for XPath
        pugi::xpath_node context_node(root);

        // 2. Invoke the target functions in the specified order

        // Step 1: pugi::xml_node::child
        const pugi::char_t* child_name = safe_cstr(Data, Size);
        pugi::xml_node child1 = root.child(child_name);

        // Step 2: pugi::xml_node::attribute
        pugi::xml_node node_for_attr = child1 ? child1 : root;
        pugi::xml_attribute attr = node_for_attr.attribute(child_name);

        // Step 3: pugi::xml_node::child (again)
        pugi::xml_node child2 = node_for_attr ? node_for_attr.child(child_name) : root.child(child_name);

        // Step 4: pugi::xpath_query::evaluate_boolean
        bool bool_result = query.evaluate_boolean(context_node);
        (void)bool_result;

        // Step 5: pugi::xpath_query::evaluate_number
        double num_result = query.evaluate_number(context_node);
        (void)num_result;

        // Step 6: pugi::xpath_query::evaluate_string (overload 1: return string_t)
        pugi::string_t str_result1 = query.evaluate_string(context_node);
        (void)str_result1;

        // Step 7: pugi::xpath_query::evaluate_string (overload 2: write to buffer)
        size_t buf_size = Size > 0 ? Size : 1;
        if (buf_size > 1024 * 1024) buf_size = 1024 * 1024; // Cap at 1MB
        pugi::char_t* buf = create_temp_buffer(buf_size);
        if (buf) {
            size_t written = query.evaluate_string(buf, buf_size, context_node);
            (void)written;
            delete[] buf;
        }

        // Step 8: pugi::xpath_query::evaluate_string (again, as per sequence)
        pugi::string_t str_result2 = query.evaluate_string(context_node);
        (void)str_result2;

        // Step 9: pugi::xpath_query::evaluate_node_set
        pugi::xpath_node_set node_set = query.evaluate_node_set(context_node);
        (void)node_set;

        // Step 10: pugi::xpath_query::evaluate_node
        pugi::xpath_node single_node = query.evaluate_node(context_node);
        (void)single_node;

    } catch (...) {
        // Catch any exceptions (bad_alloc, xpath_exception, etc.) to keep fuzzer running
        return 0;
    }

    return 0;
}