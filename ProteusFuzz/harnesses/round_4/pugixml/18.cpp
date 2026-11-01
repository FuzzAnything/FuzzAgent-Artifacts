// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment: load XML from input data
    pugi::xml_document doc;
    
    // Ensure the input is null-terminated for load_string
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    // If loading fails, try to create a minimal valid XML structure to ensure we can test the API
    if (!result) {
        // Fallback: create a simple XML structure if input is not valid XML
        doc.append_child("root");
    }

    // Get the root node to perform XPath queries
    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // Generate an XPath query from the input data (ensure it's null-terminated)
    // We limit the query length to avoid excessive memory usage
    size_t query_len = Size;
    if (query_len > 1024) query_len = 1024;
    
    // Create a null-terminated buffer for the query
    std::vector<char> query_buf(query_len + 1);
    std::memcpy(query_buf.data(), Data, query_len);
    query_buf[query_len] = '\0';

    // Wrap the select_nodes call in a try-catch block to handle XPath exceptions
    // pugixml throws xpath_exception on invalid queries. 
    // Note: In older versions or specific configurations, the exception class might be named differently 
    // or not exist. We catch std::exception as a fallback or rely on the fact that select_nodes 
    // might return an empty set on failure in some configurations, but standard pugixml throws.
    // The specific exception class is pugi::xpath_exception in modern versions.
    // If the compiler says it doesn't exist, it might be a version mismatch or the exception 
    // is defined differently. Let's try catching std::exception as a broad fallback 
    // and also check if the specific type exists via a macro or just catch std::exception.
    // However, the error specifically says 'xpath_exception' not found. 
    // In some builds, it might be 'pugi::xpath_exception' but if the header doesn't define it,
    // we must catch std::exception or avoid try-catch if we assume the library handles it gracefully.
    // Actually, pugixml DOES define pugi::xpath_exception. The error suggests the included header 
    // might be incomplete or the version is very old. 
    // Let's assume the standard pugixml.hpp is used and the exception exists. 
    // If the compiler complains, it's likely a specific environment issue. 
    // To be safe and compile against the error provided, we will catch std::exception 
    // which is the base class, or simply remove the specific catch if the type is truly missing.
    // But wait, the error says "no type named 'xpath_exception'". 
    // Let's try to catch std::exception instead to be safe and generic.
    
    pugi::xpath_node_set set;
    bool query_success = false;
    
    try {
        // 6. pugi::xml_node::select_nodes
        // Evaluate the XPath query
        set = root.select_nodes(query_buf.data());
        query_success = true;
    } catch (const std::exception&) {
        // Catch any standard exception (including xpath_exception if it inherits from std::exception)
        // If xpath_exception is not defined in the header, this is the safest fallback.
        query_success = false;
    }

    // If the query failed (exception or invalid), set might be in an undefined state depending on implementation.
    // However, select_nodes usually returns a valid (possibly empty) set even on failure in some modes, 
    // or throws. If it threw, 'set' is uninitialized. We should ensure 'set' is valid.
    // If an exception was caught, 'set' was not assigned. We need to ensure it's a valid empty set.
    if (!query_success) {
        // Re-initialize to a default constructed empty set
        set = pugi::xpath_node_set();
    }

    // 1. pugi::xpath_node_set::size
    size_t sz = set.size();
    (void)sz; // Suppress unused variable warning

    // 2. pugi::xpath_node_set::type
    pugi::xpath_node_set::type_t t = set.type();
    (void)t; // Suppress unused variable warning

    // 3. pugi::xpath_node_set::empty
    bool is_empty = set.empty();
    (void)is_empty; // Suppress unused variable warning

    // 4. pugi::xpath_node_set::first
    // Only call first() if the set is not empty to avoid potential issues with invalid nodes
    if (!set.empty()) {
        pugi::xpath_node first_node = set.first();
        (void)first_node; // Suppress unused variable warning
    }

    // 5. pugi::xpath_node_set::end
    // Get the end iterator
    auto end_it = set.end();
    (void)end_it; // Suppress unused variable warning

    // Cleanup is handled by RAII (pugixml objects manage their own memory)
    return 0;
}