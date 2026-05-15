// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
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

static void test_xpath_variable_set_get(const pugi::xpath_variable_set& set, const char* name) {
    const pugi::xpath_variable* var = set.get(name);
    (void)var;
}

static void test_xpath_variable_get_node_set(const pugi::xpath_variable* var) {
    if (var && var->type() == pugi::xpath_type_node_set) {
        const pugi::xpath_node_set& node_set = var->get_node_set();
        (void)node_set;
    }
}

static void test_xpath_variable_get_number(const pugi::xpath_variable* var) {
    if (var && var->type() == pugi::xpath_type_number) {
        double num = var->get_number();
        (void)num;
    }
}

static void test_xpath_variable_type(const pugi::xpath_variable* var) {
    if (var) {
        pugi::xpath_value_type type = var->type();
        (void)type;
    }
}

static void test_xpath_query_return_type(const pugi::xpath_query& query) {
    pugi::xpath_value_type return_type = query.return_type();
    (void)return_type;
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create xpath_variable_set and add variables with different types
    pugi::xpath_variable_set var_set;
    
    // Use input data to create variable names
    std::string var_name1(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)50));
    std::string var_name2;
    if (Size > 50) {
        var_name2 = std::string(reinterpret_cast<const char*>(Data + 50), std::min(Size - 50, (size_t)50));
    } else {
        var_name2 = "var2";
    }
    
    // Add variables with different types
    pugi::xpath_variable* var1 = var_set.add(var_name1.c_str(), pugi::xpath_type_number);
    pugi::xpath_variable* var2 = var_set.add(var_name2.c_str(), pugi::xpath_type_node_set);
    pugi::xpath_variable* var3 = var_set.add("bool_var", pugi::xpath_type_boolean);
    pugi::xpath_variable* var4 = var_set.add("string_var", pugi::xpath_type_string);
    
    // Test get() function
    test_xpath_variable_set_get(var_set, var_name1.c_str());
    test_xpath_variable_set_get(var_set, var_name2.c_str());
    test_xpath_variable_set_get(var_set, "nonexistent");
    
    // Test type() function
    test_xpath_variable_type(var1);
    test_xpath_variable_type(var2);
    test_xpath_variable_type(var3);
    test_xpath_variable_type(var4);
    
    // Test get_number() function
    test_xpath_variable_get_number(var1);
    test_xpath_variable_get_number(var2);  // Should return NaN
    test_xpath_variable_get_number(var3);  // Should return NaN
    
    // Test get_node_set() function
    test_xpath_variable_get_node_set(var2);
    test_xpath_variable_get_node_set(var1);  // Should return empty set
    
    // Create a dummy XML document for node set testing
    pugi::xml_document doc;
    const char* xml_content = "<root><element1>text1</element1><element2>text2</element2></root>";
    if (doc.load_string(xml_content)) {
        pugi::xpath_node node(doc.first_child());
        pugi::xpath_node_set node_set = doc.select_nodes("//*");
        
        // Set node set value for var2
        if (var2) {
            var2->set(node_set);
            test_xpath_variable_get_node_set(var2);  // Now should return non-empty set
        }
    }
    
    // Test xpath_query return_type
    std::string xpath_expr;
    if (Size > 100) {
        xpath_expr = std::string(reinterpret_cast<const char*>(Data + 100), std::min(Size - 100, (size_t)100));
    } else {
        xpath_expr = "//element";
    }
    
    try {
        pugi::xpath_query query(xpath_expr.c_str());
        test_xpath_query_return_type(query);
    } catch (...) {
        // Ignore parsing errors
    }
    
    // Test with invalid query
    pugi::xpath_query invalid_query("invalid[");
    test_xpath_query_return_type(invalid_query);
    
    // Test const version of get()
    const pugi::xpath_variable_set& const_var_set = var_set;
    const pugi::xpath_variable* const_var = const_var_set.get(var_name1.c_str());
    (void)const_var;
    
    return 0;
}