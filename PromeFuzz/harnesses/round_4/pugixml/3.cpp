// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>

static void initialize_xml_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size > 0) {
        doc.load_buffer(data, size);
    }
}

static pugi::xpath_variable* add_test_variable(pugi::xpath_variable_set& var_set) {
    return var_set.add("test_var", pugi::xpath_value_type::xpath_type_number);
}

static pugi::xpath_query create_test_query(const uint8_t* data, size_t size, pugi::xpath_variable_set* var_set = nullptr) {
    if (size == 0) {
        return pugi::xpath_query("//node");
    }
    
    std::string expr(reinterpret_cast<const char*>(data), size);
    if (expr.empty()) {
        expr = "//node";
    }
    
    try {
        return pugi::xpath_query(expr.c_str(), var_set);
    } catch (...) {
        return pugi::xpath_query("//node");
    }
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    pugi::xpath_variable_set var_set;
    pugi::xpath_variable* var = add_test_variable(var_set);
    if (var) {
        var->set(42.0);
    }

    pugi::xml_document doc;
    initialize_xml_document(doc, Data, Size);

    size_t split_point = Size / 2;
    pugi::xpath_query query = create_test_query(Data + split_point, Size - split_point, &var_set);

    pugi::xml_node root = doc.root();
    pugi::xpath_node context(root);

    try {
        bool bool_result = query.evaluate_boolean(context);
        (void)bool_result;

        double num_result = query.evaluate_number(context);
        (void)num_result;

        std::string str_result = query.evaluate_string(context).c_str();
        (void)str_result;

        pugi::xpath_node node_result = query.evaluate_node(context);
        (void)node_result;

        pugi::xpath_node_set node_set_result = query.evaluate_node_set(context);
        (void)node_set_result;
    } catch (...) {
    }

    return 0;
}