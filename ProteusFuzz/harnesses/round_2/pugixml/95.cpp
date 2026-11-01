// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
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

static void fuzz_xpath_variable_name(const uint8_t* data, size_t size) {
    pugi::xpath_variable_set variables;
    
    if (size == 0) return;
    
    // Create a safe name string from fuzzed data
    // Ensure the name is null-terminated and doesn't contain embedded nulls
    std::string name_str;
    size_t name_len = (size > 100) ? 100 : size;
    
    for (size_t i = 0; i < name_len; ++i) {
        if (data[i] == '\0') continue;
        name_str += static_cast<char>(data[i]);
    }
    
    if (name_str.empty()) {
        name_str = "var";
    }
    
    // Try adding as string type
    pugi::xpath_variable* var_str = variables.add(name_str.c_str(), pugi::xpath_type_string);
    if (var_str) {
        const pugi::char_t* name = var_str->name();
        (void)name; 
        const pugi::char_t* str_val = var_str->get_string();
        (void)str_val;
    }
    
    // Try adding as number type
    pugi::xpath_variable* var_num = variables.add(name_str.c_str(), pugi::xpath_type_number);
    if (var_num) {
        const pugi::char_t* name = var_num->name();
        (void)name;
    }
    
    // Try adding as boolean type
    pugi::xpath_variable* var_bool = variables.add(name_str.c_str(), pugi::xpath_type_boolean);
    if (var_bool) {
        const pugi::char_t* name = var_bool->name();
        (void)name;
    }
    
    // Try adding as node_set type
    pugi::xpath_variable* var_set = variables.add(name_str.c_str(), pugi::xpath_type_node_set);
    if (var_set) {
        const pugi::char_t* name = var_set->name();
        (void)name;
    }
    
    // Test get with const - use a pointer to avoid copy constructor issues
    const pugi::xpath_variable_set* variables_const_ptr = &variables;
    const pugi::xpath_variable* var_const = variables_const_ptr->get(name_str.c_str());
    if (var_const) {
        const pugi::char_t* name = var_const->name();
        (void)name;
    }
}

static void fuzz_xpath_variable_set_operations(const uint8_t* data, size_t size) {
    pugi::xpath_variable_set variables;
    
    if (size < 2) return;
    
    // Create a name from the first part of data
    std::string name;
    size_t name_len = (data[0] % (size - 1)) + 1;
    if (name_len > 100) name_len = 100;
    
    for (size_t i = 0; i < name_len && i < size; ++i) {
        if (data[i] == '\0') continue;
        name += static_cast<char>(data[i]);
    }
    if (name.empty()) name = "var";
    
    // Add a variable
    pugi::xpath_variable* var = variables.add(name.c_str(), pugi::xpath_type_string);
    
    // Try to get it
    pugi::xpath_variable* retrieved = variables.get(name.c_str());
    if (retrieved) {
        const pugi::char_t* name_val = retrieved->name();
        (void)name_val;
    }
    
    // Try set with node_set (will fail if type doesn't match)
    pugi::xpath_node_set empty_set;
    bool set_result = variables.set(name.c_str(), empty_set);
    (void)set_result;
}

static void fuzz_select_nodes(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Create a simple XML structure
    pugi::xml_node root = doc.append_child("root");
    root.append_child("child1");
    root.append_child("child2");
    root.append_child("child3");
    
    // Prepare variables
    pugi::xpath_variable_set variables;
    
    // Use data as query string
    std::string query;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == '\0') continue;
        // Filter out control characters that might break parsing
        if (data[i] >= 32 && data[i] < 127) {
            query += static_cast<char>(data[i]);
        }
    }
    
    // If query is empty, use a default one
    if (query.empty()) {
        query = "//child1";
    }
    
    try {
        pugi::xpath_node_set result = root.select_nodes(query.c_str(), &variables);
        (void)result;
    } catch (const std::exception&) {
        // Expected for invalid queries or memory issues
    }
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz xpath_variable::name and related operations
    fuzz_xpath_variable_name(Data, Size);
    
    // Fuzz xpath_variable_set::get and set operations
    fuzz_xpath_variable_set_operations(Data, Size);
    
    // Fuzz xml_node::select_nodes
    fuzz_select_nodes(Data, Size);
    
    return 0;
}