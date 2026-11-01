// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
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

static const char* s_dummy_xml = "<root><item>test</item></root>";

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Parse XML document
    pugi::xml_document doc;
    if (!doc.load_string(s_dummy_xml)) {
        return 0;
    }

    pugi::xml_node root = doc.document_element();
    if (!root) return 0;

    // Prepare variable set
    pugi::xpath_variable_set vars;

    // Create variables of different types to test type mismatches and correct usage
    pugi::xpath_variable* var_num_ptr = vars.add("num", pugi::xpath_type_number);
    pugi::xpath_variable* var_str_ptr = vars.add("str", pugi::xpath_type_string);
    pugi::xpath_variable* var_bool_ptr = vars.add("bool", pugi::xpath_type_boolean);
    pugi::xpath_variable* var_set_ptr = vars.add("set", pugi::xpath_type_node_set);

    if (!var_num_ptr || !var_str_ptr || !var_bool_ptr || !var_set_ptr) {
        return 0;
    }

    // 1. Test pugi::xpath_variable::get_number
    double num_val = var_num_ptr->get_number();
    (void)num_val;

    // 2. Test pugi::xpath_variable::get_string
    const pugi::char_t* str_val = var_str_ptr->get_string();
    (void)str_val;

    // 3. Test pugi::xpath_variable::get_node_set
    const pugi::xpath_node_set& set_val = var_set_ptr->get_node_set();
    (void)set_val;

    // 4. Test pugi::xpath_variable_set::set (with double value)
    bool set_res_num = vars.set("num", 42.0);
    bool set_res_str = vars.set("str", 1.0);
    (void)set_res_num;
    (void)set_res_str;

    // 5. Test pugi::xpath_variable::set (with node_set)
    pugi::xpath_node_set dummy_set;
    bool set_set_res = var_set_ptr->set(dummy_set);
    bool set_num_res = var_num_ptr->set(dummy_set);
    (void)set_set_res;
    (void)set_num_res;

    // 6. Test pugi::xml_node::select_nodes
    // Construct query from fuzzed input safely
    const size_t max_query_len = 4096;
    if (Size > max_query_len) return 0;

    pugi::char_t query_buf[max_query_len + 1];
    std::memcpy(query_buf, Data, Size);
    query_buf[Size] = 0;

    // Wrap in try-catch to handle exceptions thrown by xpath_query constructor
    // when PUGIXML_NO_EXCEPTIONS is not defined.
    try {
        pugi::xpath_query query(query_buf, &vars);
        pugi::xpath_parse_result result = query.result();
        
        // If query is valid, execute select_nodes
        if (!result.error) {
            pugi::xpath_node_set nodes = root.select_nodes(query);
            (void)nodes;
        }
    } catch (...) {
        // Ignore exceptions from invalid XPath queries
    }

    // Also test with a known valid query to ensure code path coverage
    try {
        pugi::xpath_query valid_query(".//item", &vars);
        if (!valid_query.result().error) {
            pugi::xpath_node_set valid_nodes = root.select_nodes(valid_query);
            (void)valid_nodes;
        }
    } catch (...) {
        // Ignore exceptions
    }

    return 0;
}