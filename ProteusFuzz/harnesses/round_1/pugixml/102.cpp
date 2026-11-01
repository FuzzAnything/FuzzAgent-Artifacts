// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
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

// Helper to convert fuzz data to null-terminated string safely
static std::string make_cstr(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    std::string result;
    result.reserve(size + 1);
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == 0) continue; // Skip nulls to keep string continuous for names
        result += static_cast<char>(data[i]);
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    pugi::xpath_variable_set variables;
    pugi::xpath_variable_set variables_const_copy = variables; // Test copy constructor

    std::string name_str = make_cstr(Data, Size);
    const pugi::char_t* name = name_str.c_str();
    
    // 1. Test pugi::xpath_variable_set::add
    // Try adding different types
    pugi::xpath_variable* var_node = variables.add(name, pugi::xpath_type_node_set);
    pugi::xpath_variable* var_num = variables.add(name, pugi::xpath_type_number);
    pugi::xpath_variable* var_str = variables.add(name, pugi::xpath_type_string);
    pugi::xpath_variable* var_bool = variables.add(name, pugi::xpath_type_boolean);

    // 2. Test pugi::xpath_variable_set::get (non-const and const)
    pugi::xpath_variable* retrieved = variables.get(name);
    const pugi::xpath_variable_set& const_set = variables_const_copy;
    const pugi::xpath_variable* retrieved_const = const_set.get(name);

    // 3. Test pugi::xpath_variable::type
    if (var_node) {
        pugi::xpath_value_type t = var_node->type();
        (void)t;
    }
    if (var_num) {
        pugi::xpath_value_type t = var_num->type();
        (void)t;
    }
    if (var_str) {
        pugi::xpath_value_type t = var_str->type();
        (void)t;
    }
    if (var_bool) {
        pugi::xpath_value_type t = var_bool->type();
        (void)t;
    }

    // 4. Test pugi::xpath_variable::name
    if (var_node) {
        const pugi::char_t* n = var_node->name();
        (void)n;
    }
    if (var_num) {
        const pugi::char_t* n = var_num->name();
        (void)n;
    }
    if (var_str) {
        const pugi::char_t* n = var_str->name();
        (void)n;
    }
    if (var_bool) {
        const pugi::char_t* n = var_bool->name();
        (void)n;
    }

    // 5. Test pugi::xpath_variable::get_string
    if (var_node) {
        const pugi::char_t* s = var_node->get_string();
        (void)s;
    }
    if (var_num) {
        const pugi::char_t* s = var_num->get_string();
        (void)s;
    }
    if (var_str) {
        const pugi::char_t* s = var_str->get_string();
        (void)s;
    }
    if (var_bool) {
        const pugi::char_t* s = var_bool->get_string();
        (void)s;
    }

    // 6. Test pugi::xpath_query::return_type
    // Wrap in try-catch to handle exceptions thrown by invalid XPath queries
    try {
        // The constructor takes (const char_t* query, xpath_variable_set* variables)
        // We pass the variable set pointer, not an xml_document
        pugi::xpath_query query(name_str.c_str(), &variables);
        pugi::xpath_value_type ret_type = query.return_type();
        (void)ret_type;
    } catch (...) {
        // Ignore exceptions from invalid queries
    }

    // Additional exploration: Try with empty string or specific invalid chars
    pugi::xpath_variable* var_empty = variables.add("", pugi::xpath_type_string);
    if (var_empty) {
        var_empty->type();
        var_empty->name();
        var_empty->get_string();
    }

    return 0;
}