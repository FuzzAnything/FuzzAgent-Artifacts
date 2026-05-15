// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>

static pugi::xml_document create_dummy_document() {
    pugi::xml_document doc;
    const char* xml_content = 
        "<?xml version=\"1.0\"?>"
        "<root>"
        "  <child1 attr=\"value1\">Text1</child1>"
        "  <child2 attr=\"value2\">Text2</child2>"
        "  <child3 attr=\"value3\">Text3</child3>"
        "</root>";
    doc.load_string(xml_content);
    return doc;
}

static pugi::xpath_variable_set* create_variable_set() {
    static pugi::xpath_variable_set vars;
    
    // Create a document for the node set variable
    static pugi::xml_document doc_for_nodeset;
    const char* xml_content = 
        "<?xml version=\"1.0\"?>"
        "<root>"
        "  <child1 attr=\"value1\">Text1</child1>"
        "  <child2 attr=\"value2\">Text2</child2>"
        "</root>";
    doc_for_nodeset.load_string(xml_content);
    
    // Create node set from the document
    pugi::xpath_node_set nodeset = doc_for_nodeset.select_nodes("/root/child1");
    
    vars.set("bool_var", true);
    vars.set("string_var", "test_string");
    vars.set("number_var", 42.0);
    vars.set("nodeset_var", nodeset);
    
    return &vars;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    static pugi::xml_document doc = create_dummy_document();
    static pugi::xpath_variable_set* vars = create_variable_set();
    
    if (Size < 1) {
        return 0;
    }
    
    // Extract values from fuzzer input
    bool bool_val = (Data[0] & 0x1) != 0;
    
    const char* str_val = "default";
    if (Size > 1) {
        // Use a stack buffer for the string
        size_t str_len = Size - 1;
        if (str_len > 100) str_len = 100; // Limit size
        char* temp_str = static_cast<char*>(malloc(str_len + 1));
        if (temp_str) {
            memcpy(temp_str, Data + 1, str_len);
            temp_str[str_len] = '\0';
            str_val = temp_str;
        }
    }
    
    double num_val = 0.0;
    if (Size >= sizeof(double) + 1) {
        memcpy(&num_val, Data + 1, sizeof(double));
    }
    
    // Get variables from the set
    pugi::xpath_variable* bool_var = vars->get("bool_var");
    pugi::xpath_variable* string_var = vars->get("string_var");
    pugi::xpath_variable* number_var = vars->get("number_var");
    pugi::xpath_variable* nodeset_var = vars->get("nodeset_var");
    
    if (!bool_var || !string_var || !number_var || !nodeset_var) {
        return 0;
    }
    
    // 1. pugi::xpath_variable::get_node_set
    const pugi::xpath_node_set& node_set = nodeset_var->get_node_set();
    
    // 2-4. pugi::xpath_variable::set (all three overloads)
    bool_var->set(bool_val);
    string_var->set(str_val);
    number_var->set(num_val);
    
    // 5. pugi::xml_node::select_nodes
    pugi::xml_node root = doc.child("root");
    const char* query = "/root/child1";
    pugi::xpath_node_set selected_nodes = root.select_nodes(query, vars);
    
    // 6. pugi::xpath_variable::get_boolean
    bool bool_result = bool_var->get_boolean();
    
    // 7. pugi::xpath_variable::get_number
    double num_result = number_var->get_number();
    
    // 8. pugi::xpath_variable::get_string
    const pugi::char_t* str_result = string_var->get_string();
    
    // 9. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();
    
    // Clean up temporary string
    if (Size > 1) {
        free(const_cast<char*>(str_val));
    }
    
    return 0;
}