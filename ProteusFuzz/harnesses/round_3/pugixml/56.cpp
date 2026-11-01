// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    pugi::xml_document doc;
    write_dummy_file(Data, Size);
    
    if (!doc.load_file("./dummy_file")) {
        // If file load fails, try to parse directly from buffer as a fallback
        // Ensure null termination for C-string parsing
        if (Size < 2) return 0;
        if (Data[Size-1] == 0) {
            doc.load_string(reinterpret_cast<const char*>(Data));
        } else {
            // Create a safe copy with null terminator
            char* buf = new char[Size + 1];
            std::memcpy(buf, Data, Size);
            buf[Size] = '\0';
            doc.load_string(buf);
            delete[] buf;
        }
    }

    pugi::xml_node root = doc.root();
    if (root.empty()) {
        // Create a dummy root if parsing failed or empty
        root = doc.append_child("root");
    }

    // Prepare XPath query from input data
    size_t max_query_len = 4096; // Limit query size to prevent excessive allocation
    size_t query_len = (Size < max_query_len) ? Size : max_query_len;
    
    char* safe_query = new char[query_len + 1];
    std::memcpy(safe_query, Data, query_len);
    safe_query[query_len] = '\0';

    try {
        // Overload 1: select_single_node(const xpath_query& query)
        pugi::xpath_query query_obj(safe_query);
        pugi::xpath_node result1 = root.select_single_node(query_obj);

        // Overload 2: select_single_node(const char_t* query, xpath_variable_set* variables)
        pugi::xpath_variable_set variables;
        pugi::xpath_node result2 = root.select_single_node(safe_query, &variables);

        // Invoke xpath_node::node() on result1
        pugi::xml_node node1 = result1.node();
        (void)node1; // Suppress unused warning

        // Invoke xpath_node::node() on result2
        pugi::xml_node node2 = result2.node();
        (void)node2; // Suppress unused warning
    } catch (...) {
        // Catch any exceptions thrown by pugixml (e.g., xpath_exception, std::bad_alloc)
        // to prevent the fuzzer from crashing on malformed XPath queries.
    }

    // Cleanup
    delete[] safe_query;
    // Cleanup is handled by RAII (destructors of doc, query_obj, variables)
    return 0;
}