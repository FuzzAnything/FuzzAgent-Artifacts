// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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

// Check if xpath_exception is available (it might be disabled in some builds)
#ifdef PUGIXML_NO_EXCEPTIONS
    #define USE_EXCEPTIONS 0
#else
    #define USE_EXCEPTIONS 1
#endif

static void fuzz_select_node(const uint8_t* data, size_t size) {
    if (size < 10) return;

    // Split data: first part for XML, second part for XPath query
    size_t xml_size = size / 2;
    size_t query_size = size - xml_size;

    // Ensure non-zero sizes
    if (xml_size == 0 || query_size == 0) return;

    // Create XML document from fuzz data
    std::string xml_str(reinterpret_cast<const char*>(data), xml_size);
    pugi::xml_document doc;
    if (!doc.load_string(xml_str.c_str())) {
        return;
    }

    // Extract query string
    std::string query_str(reinterpret_cast<const char*>(data + xml_size), query_size);
    const pugi::char_t* query = query_str.c_str();

    pugi::xpath_variable_set vars;

#if USE_EXCEPTIONS
    // Test select_node with exception handling
    try {
        pugi::xpath_node result = doc.select_node(query, &vars);
        // If successful, we can traverse the result if it's not empty
        if (result.node()) {
            (void)result.node().name();
        }
    } catch (const pugi::xpath_exception& e) {
        // Test xpath_exception::result
        const pugi::xpath_parse_result& res = e.result();
        (void)res.description();
        (void)e.what();
    } catch (...) {
        // Ignore other exceptions
    }

    // Test xpath_query construction and evaluate_node
    try {
        pugi::xpath_query xquery(query);
        // Check result even if no exception was thrown (for PUGIXML_NO_EXCEPTIONS mode)
        const pugi::xpath_parse_result& parse_res = xquery.result();
        (void)parse_res.description();

        // Evaluate on document root
        pugi::xpath_node eval_result = xquery.evaluate_node(pugi::xpath_node(doc));
        if (eval_result.node()) {
            (void)eval_result.node().name();
        }
    } catch (const pugi::xpath_exception& e) {
        const pugi::xpath_parse_result& res = e.result();
        (void)res.description();
        (void)e.what();
    } catch (...) {
        // Ignore other exceptions
    }
#else
    // Fallback for PUGIXML_NO_EXCEPTIONS mode
    // Test select_node (no exceptions thrown)
    pugi::xpath_node result = doc.select_node(query, &vars);
    if (result.node()) {
        (void)result.node().name();
    }

    // Test xpath_query construction
    pugi::xpath_query xquery(query);
    const pugi::xpath_parse_result& parse_res = xquery.result();
    (void)parse_res.description();

    // Evaluate on document root
    pugi::xpath_node eval_result = xquery.evaluate_node(pugi::xpath_node(doc));
    if (eval_result.node()) {
        (void)eval_result.node().name();
    }
#endif
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_select_node(Data, Size);

    return 0;
}