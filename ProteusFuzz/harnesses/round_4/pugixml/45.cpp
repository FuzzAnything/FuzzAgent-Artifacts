// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment: create a document and load data
    pugi::xml_document doc;
    
    // Ensure the input looks like XML by wrapping it if necessary to avoid immediate parse failures
    std::string raw(reinterpret_cast<const char*>(Data), Size);
    
    pugi::xml_parse_result result = doc.load_string(raw.c_str());
    if (!result) {
        // If parsing fails, try to wrap the data in a root element to create a valid tree
        std::string wrapped = "<root>" + raw + "</root>";
        result = doc.load_string(wrapped.c_str());
    }

    if (!result) {
        // If still invalid, create a minimal valid document
        doc.append_child("root");
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // Prepare variables for XPath
    pugi::xpath_variable_set variables;

    // Convert input to wide string if needed for XPath queries
    // We use the raw data as the query string, which may be invalid, triggering exceptions we catch
    std::string query_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure query is not empty for safety
    if (query_str.empty()) query_str = "//";

    try {
        // 1. pugi::xml_node::select_node (const char_t* query)
        pugi::xpath_node node1 = root.select_node(query_str.c_str(), &variables);
        
        // 2. pugi::xml_node::select_node (const xpath_query& query)
        try {
            pugi::xpath_query compiled_query(query_str.c_str(), &variables);
            pugi::xpath_node node2 = root.select_node(compiled_query);
            (void)node2;
        } catch (...) {
            // Ignore compilation errors for the second overload
        }

        // 3. pugi::xpath_node::node
        pugi::xml_node attr1 = node1.node();
        (void)attr1;

        // 4. pugi::xpath_node::node (on a potentially different node if we had one, but reusing node1)
        pugi::xml_node attr2 = node1.node();
        (void)attr2;

        // 5. pugi::xml_node::select_node (const char_t* query)
        pugi::xpath_node node3 = root.select_node(query_str.c_str(), &variables);

        // 6. pugi::xml_node::select_node (const xpath_query& query)
        try {
            pugi::xpath_query compiled_query2(query_str.c_str(), &variables);
            pugi::xpath_node node4 = root.select_node(compiled_query2);
            (void)node4;
        } catch (...) {
            // Ignore
        }

        // 7. pugi::xml_node::select_node (const char_t* query)
        pugi::xpath_node node5 = root.select_node(query_str.c_str(), &variables);

        // 8. pugi::xpath_node::node
        pugi::xml_node attr3 = node5.node();
        (void)attr3;

        // 9. pugi::xpath_node::node
        pugi::xml_node attr4 = node5.node();
        (void)attr4;

    } catch (...) {
        // Catch any exceptions, including pugi::xpath_exception if defined or other errors
    }

    return 0;
}