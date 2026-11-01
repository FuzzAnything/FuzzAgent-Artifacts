// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
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
#include <stdexcept>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure the input data ends with a null terminator for string operations
    // We will use a copy to ensure safety. 
    // Allocate Size + 1 to safely add a null terminator.
    size_t safe_size = Size + 1;
    
    // Use pugi::char_t for compatibility with pugixml's internal types
    pugi::char_t* buffer = new pugi::char_t[safe_size];
    if (!buffer) return 0;
    
    // Copy input data to buffer
    memcpy(buffer, Data, Size);
    // Safely add null terminator at the end of the allocated buffer
    buffer[Size] = 0;

    // Parse the input as XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(buffer);
    
    // If parsing fails, we still want to test the API with an empty document
    // to trigger error handling paths, but we need a root node to call methods on.
    // If the document is empty, create a dummy root.
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }

    // Prepare XPath variable set
    pugi::xpath_variable_set variables;

    // 1. Test pugi::xml_node::select_node
    try {
        // Use the buffer as the query string. It might be invalid, which is fine for fuzzing.
        pugi::xpath_node node = root.select_node(buffer, &variables);
        (void)node; // Suppress unused variable warning
    } catch (...) {
        // Expected for invalid XPath queries
    }

    // 2. Test pugi::xpath_query::evaluate_node_set
    try {
        // Compile the query first
        pugi::xpath_query query(buffer, &variables);
        pugi::xpath_parse_result parse_res = query.result();
        
        // If compilation failed, we can't evaluate, but we can test with a valid node
        if (parse_res.error == nullptr) {
            pugi::xpath_node context(root);
            pugi::xpath_node_set nodes = query.evaluate_node_set(context);
            (void)nodes;
        }
    } catch (...) {
        // Expected for invalid queries or memory issues
    }

    // 3. Test pugi::xpath_variable_set::set
    // We need a variable name and a node set.
    // We'll try to set a variable named "test_var" with an empty node set.
    // The buffer might contain the name, but we need to ensure it's null-terminated properly.
    // Let's try to set a variable with a name derived from the input (first word).
    const pugi::char_t* var_name = buffer;
    // Find the first space or null to isolate a name
    const pugi::char_t* end_name = var_name;
    while (*end_name && *end_name != ' ' && *end_name != '\t') {
        end_name++;
    }
    
    // Create a temporary buffer for the name to ensure it's null-terminated
    // Max length is safe_size (which is Size + 1)
    pugi::char_t* name_buf = new pugi::char_t[safe_size];
    if (!name_buf) { delete[] buffer; return 0; }
    
    size_t name_len = end_name - var_name;
    if (name_len >= safe_size) name_len = safe_size - 1;
    memcpy(name_buf, var_name, name_len);
    name_buf[name_len] = 0;

    // Create an empty node set to assign
    pugi::xpath_node_set empty_set;
    
    // Attempt to set the variable. This will return false if the variable doesn't exist or type mismatch.
    // We are testing the logic of the set method.
    bool set_result = variables.set(name_buf, empty_set);
    (void)set_result;

    // 4. Test pugi::xml_node::select_nodes (both overloads)
    try {
        // Overload 1: select_nodes(const xpath_query& query)
        pugi::xpath_query query1(buffer, &variables);
        pugi::xpath_parse_result res1 = query1.result();
        if (res1.error == nullptr) {
            pugi::xpath_node_set set1 = root.select_nodes(query1);
            (void)set1;
        }
    } catch (...) {
        // Expected
    }

    try {
        // Overload 2: select_nodes(const char_t* query, xpath_variable_set* variables)
        pugi::xpath_node_set set2 = root.select_nodes(buffer, &variables);
        (void)set2;
    } catch (...) {
        // Expected
    }

    // 5. Test pugi::xpath_variable::get_node_set
    try {
        // We need a variable that exists and is of type node_set to get a non-empty set.
        // Since we didn't successfully set a node_set variable in step 3 (because it likely didn't exist),
        // we try to get it anyway to test the "static empty dummy set" path.
        
        // Re-create name buffer for get (reuse logic)
        pugi::char_t* name_buf2 = new pugi::char_t[safe_size];
        if (!name_buf2) { delete[] buffer; delete[] name_buf; return 0; }
        memcpy(name_buf2, var_name, name_len);
        name_buf2[name_len] = 0;

        pugi::xpath_variable* v = variables.get(name_buf2);
        if (v) {
            const pugi::xpath_node_set& ns = v->get_node_set();
            (void)ns;
        }
        delete[] name_buf2;
    } catch (...) {
        // Expected
    }

    delete[] buffer;
    delete[] name_buf;
    return 0;
}