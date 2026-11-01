// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;

    // Attempt to parse the input as XML
    // Ensure the input is null-terminated for load_string to avoid buffer overflows
    std::vector<char> safe_buffer(Size + 1);
    std::memcpy(safe_buffer.data(), Data, Size);
    safe_buffer[Size] = '\0';

    parse_result = doc.load_string(safe_buffer.data());

    // Test xml_parse_result::description
    const char* parse_desc = parse_result.description();
    (void)parse_desc;

    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) {
        // If parsing failed or no root, try to create a simple node structure manually for XPath testing
        root = doc.append_child("root");
    }

    // Convert input data to a null-terminated string for XPath query
    char query_buffer[2048];
    size_t copy_size = (Size < sizeof(query_buffer) - 1) ? Size : sizeof(query_buffer) - 1;
    std::memcpy(query_buffer, Data, copy_size);
    query_buffer[copy_size] = '\0';

    // Test 1: select_node with string query
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
        pugi::xpath_node result = root.select_node(query_buffer);
        (void)result;
    } catch (const pugi::xpath_exception& e) {
        // Test xpath_exception::what
        const char* what_msg = e.what();
        (void)what_msg;

        // Test xpath_exception::result
        const pugi::xpath_parse_result& exc_result = e.result();
        (void)exc_result;

        // Test xpath_parse_result::description from exception
        const char* exc_desc = exc_result.description();
        (void)exc_desc;
    } catch (...) {
        // Catch any other unexpected exceptions
    }
    #else
    // In NO_EXCEPTIONS mode, select_node returns an empty node on error.
    pugi::xpath_node result = root.select_node(query_buffer);
    (void)result;
    #endif

    // Test 2: xpath_query construction and result()
    pugi::xpath_query query_obj;
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
        query_obj = pugi::xpath_query(query_buffer);
    } catch (...) {
        // Ignore construction exceptions here
    }
    #else
    query_obj = pugi::xpath_query(query_buffer);
    #endif

    // Test xpath_query::result
    const pugi::xpath_parse_result& query_result = query_obj.result();
    (void)query_result;

    // Test xpath_parse_result::description on query result
    const char* query_desc = query_result.description();
    (void)query_desc;

    // Test 3: Direct xpath_parse_result construction and description
    pugi::xpath_parse_result direct_result;
    const char* direct_desc = direct_result.description();
    (void)direct_desc;

    // Test 4: Construct xpath_exception directly from a result
    #ifndef PUGIXML_NO_EXCEPTIONS
    {
        pugi::xpath_parse_result test_result;
        pugi::xpath_exception test_exc(test_result);
        (void)test_exc;
    }
    #endif

    return 0;
}