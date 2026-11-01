// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
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

static pugi::xml_document create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.append_child("root");
        return doc;
    }
    
    // Ensure the data is valid UTF-8 or ASCII for parsing
    std::string str(reinterpret_cast<const char*>(data), size);
    
    // Try to parse as XML
    pugi::xml_parse_result result = doc.load_string(str.c_str());
    
    // If parsing fails, create a minimal valid document
    if (!result) {
        doc.append_child("root");
        pugi::xml_node root = doc.child("root");
        if (root) {
            root.append_child("data").append_child(pugi::node_pcdata).set_value("fuzz");
        }
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a document from fuzz data
    pugi::xml_document doc = create_document_from_fuzz_data(Data, Size);
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Initialize variable set
    pugi::xpath_variable_set variables;
    pugi::xpath_variable_set variables_const;

    // Generate variable name from fuzz data
    std::string var_name_str(reinterpret_cast<const char*>(Data), Size);
    // Sanitize variable name (only alphanumeric and underscore)
    std::string clean_name;
    for (char c : var_name_str) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
            clean_name += c;
        } else {
            clean_name += '_';
        }
    }
    if (clean_name.empty()) {
        clean_name = "var";
    }
    const pugi::char_t* var_name = clean_name.c_str();

    // Test 1: Add variables of different types
    pugi::xpath_variable* var_node_set = variables.add(var_name, pugi::xpath_type_node_set);
    pugi::xpath_variable* var_number = variables.add(var_name, pugi::xpath_type_number);
    pugi::xpath_variable* var_string = variables.add(var_name, pugi::xpath_type_string);
    pugi::xpath_variable* var_boolean = variables.add(var_name, pugi::xpath_type_boolean);

    // Test 2: Get variable by name (non-const)
    if (var_node_set) {
        pugi::xpath_variable* retrieved = variables.get(var_name);
        (void)retrieved; // Suppress unused warning
    }

    // Test 3: Get variable by name (const)
    const pugi::xpath_variable* var_const = variables_const.get(var_name);
    (void)var_const;

    // Test 4: Get variable name
    if (var_node_set) {
        const pugi::char_t* name = var_node_set->name();
        (void)name;
    }
    if (var_number) {
        const pugi::char_t* name = var_number->name();
        (void)name;
    }
    if (var_string) {
        const pugi::char_t* name = var_string->name();
        (void)name;
    }
    if (var_boolean) {
        const pugi::char_t* name = var_boolean->name();
        (void)name;
    }

    // Test 5: Get string value (only meaningful for string type)
    if (var_string) {
        const pugi::char_t* str_val = var_string->get_string();
        (void)str_val;
    }

    // Test 6: Set node set value
    // Create a node set by selecting nodes, which is the proper way to create one
    // We use a simple query to get some nodes
    pugi::xpath_node_set node_set;
    try {
        node_set = root.select_nodes("./*", nullptr);
    } catch (...) {
        // If selection fails, node_set remains empty
    }
    
    bool set_result = variables.set(var_name, node_set);
    (void)set_result;

    // Test 7: Select nodes with various queries
    // Generate a query string from fuzz data
    std::string query_str(reinterpret_cast<const char*>(Data), Size);
    // Sanitize query to avoid obviously invalid syntax
    std::string clean_query;
    for (char c : query_str) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || 
            c == '/' || c == '.' || c == '@' || c == '*' || c == '[' || c == ']' || 
            c == '(' || c == ')' || c == ' ' || c == '<' || c == '>' || c == '=' || c == '-' || c == '+') {
            clean_query += c;
        } else {
            clean_query += ' ';
        }
    }
    if (clean_query.empty()) {
        clean_query = "root";
    }
    const pugi::char_t* query = clean_query.c_str();

    try {
        pugi::xpath_node_set result1 = root.select_nodes(query, &variables);
        (void)result1;
    } catch (...) {
        // Expected for invalid queries or memory issues
    }

    try {
        pugi::xpath_node_set result2 = root.select_nodes(query, nullptr);
        (void)result2;
    } catch (...) {
        // Expected for invalid queries or memory issues
    }

    // Test 8: Try setting with wrong type (should fail)
    // Add a number variable and try to set with node_set
    pugi::xpath_variable* num_var = variables.add("num_test", pugi::xpath_type_number);
    if (num_var) {
        bool fail_result = variables.set("num_test", node_set);
        (void)fail_result; // Should be false
    }

    // Test 9: Get non-existent variable
    pugi::xpath_variable* non_existent = variables.get("non_existent_var");
    (void)non_existent; // Should be nullptr

    // Test 10: Move semantics for variable set
    pugi::xpath_variable_set moved_vars(std::move(variables));
    (void)moved_vars;

    return 0;
}