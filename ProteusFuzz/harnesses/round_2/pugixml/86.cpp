// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
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
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <new>

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    
    char* buffer = new char[size + 1];
    if (!buffer) {
        return doc;
    }
    
    std::memcpy(buffer, data, size);
    buffer[size] = '\0';
    
    pugi::xml_parse_result result = doc.load_string(buffer);
    delete[] buffer;
    
    return doc;
}

static pugi::xpath_query create_query_from_input(const uint8_t* data, size_t size, pugi::xpath_variable_set* vars) {
    if (size == 0) {
        return pugi::xpath_query();
    }
    
    char* buffer = new char[size + 1];
    if (!buffer) {
        return pugi::xpath_query();
    }
    
    std::memcpy(buffer, data, size);
    buffer[size] = '\0';
    
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(buffer, vars);
    } catch (...) {
        query = pugi::xpath_query();
    }
    
    delete[] buffer;
    return query;
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t part1 = (Size / 3);
    size_t part2 = (Size / 3);
    size_t part3 = Size - part1 - part2;
    
    const uint8_t* data1 = Data;
    const uint8_t* data2 = Data + part1;
    const uint8_t* data3 = Data + part1 + part2;
    
    pugi::xml_document doc = create_document_from_input(data1, part1);
    if (doc.empty()) {
        doc.append_child(pugi::node_element);
    }
    
    pugi::xml_node root = doc.root();
    if (root.empty()) {
        root = doc.append_child(pugi::node_element);
    }
    
    pugi::xpath_variable_set vars;
    
    pugi::xpath_variable* p_var_node_set = vars.add("nodeSetVar", pugi::xpath_type_node_set);
    pugi::xpath_variable* p_var_string = vars.add("stringVar", pugi::xpath_type_string);
    pugi::xpath_variable* p_var_number = vars.add("numberVar", pugi::xpath_type_number);
    pugi::xpath_variable* p_var_boolean = vars.add("booleanVar", pugi::xpath_type_boolean);
    
    // Test 1: xpath_variable::get_node_set
    {
        pugi::xpath_node_set emptySet;
        if (p_var_node_set) {
            p_var_node_set->set(emptySet);
            const pugi::xpath_node_set& result = p_var_node_set->get_node_set();
            (void)result;
        }
    }
    
    // Test 2: xpath_variable_set::set (with double value)
    {
        double value = 0.0;
        if (part2 > 0) {
            uint64_t temp = 0;
            size_t copyLen = (part2 < sizeof(uint64_t)) ? part2 : sizeof(uint64_t);
            std::memcpy(&temp, data2, copyLen);
            value = static_cast<double>(temp);
        }
        vars.set("numberVar", value);
    }
    
    // Test 3: xml_node::select_nodes
    {
        pugi::xpath_query query = create_query_from_input(data2, part2, &vars);
        try {
            pugi::xpath_node_set nodes = root.select_nodes(query);
            (void)nodes;
        } catch (...) {
            // Catch exceptions thrown during XPath evaluation
        }
    }
    
    // Test 4: xpath_variable::get_string
    {
        if (p_var_string) {
            const pugi::char_t* str = p_var_string->get_string();
            (void)str;
        }
    }
    
    // Test 5: xpath_variable::get_number
    {
        if (p_var_number) {
            double num = p_var_number->get_number();
            (void)num;
        }
    }
    
    // Test 6: xpath_variable::set (with node_set)
    {
        pugi::xpath_node_set nodeSet;
        if (p_var_node_set) {
            bool success = p_var_node_set->set(nodeSet);
            (void)success;
        }
    }
    
    // Additional fuzzing: try to set variables with mismatched types
    {
        if (p_var_boolean) {
            pugi::xpath_node_set dummySet;
            p_var_boolean->set(dummySet);
        }
        
        if (p_var_string) {
            p_var_string->set(42.0);
        }
        
        if (p_var_number) {
            p_var_number->set("test");
        }
    }
    
    return 0;
}