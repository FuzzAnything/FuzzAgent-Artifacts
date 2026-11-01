// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <cstdlib>

static void fuzz_xpath_variable_set_and_variable(const uint8_t* data, size_t size) {
    pugi::xpath_variable_set variables;
    
    // Generate variable name and type from input
    const char* name_base = "var";
    char name_buf[64];
    size_t name_len = (size > 0) ? (data[0] % 60) + 1 : 5;
    if (name_len > 60) name_len = 60;
    
    strncpy(name_buf, name_base, name_len);
    for (size_t i = 4; i < name_len && i < 60; ++i) {
        name_buf[i] = static_cast<char>('a' + (data[i % size] % 26));
    }
    name_buf[name_len] = '\0';
    
    // Try different types
    pugi::xpath_value_type types[] = {
        pugi::xpath_type_none,
        pugi::xpath_type_node_set,
        pugi::xpath_type_number,
        pugi::xpath_type_string,
        pugi::xpath_type_boolean
    };
    
    for (int t = 0; t < 5; ++t) {
        pugi::xpath_value_type type = types[t];
        
        // Test add function
        pugi::xpath_variable* var = variables.add(name_buf, type);
        
        if (var) {
            // Test type()
            pugi::xpath_value_type var_type = var->type();
            
            // Test name()
            const pugi::char_t* var_name = var->name();
            
            // Test get_string() - only meaningful for string type
            const pugi::char_t* str_val = var->get_string();
            
            // Test get() to retrieve the same variable
            pugi::xpath_variable* retrieved = variables.get(name_buf);
            
            // Test const get()
            const pugi::xpath_variable_set variables_const = variables;
            const pugi::xpath_variable* var_const = variables_const.get(name_buf);
        }
        
        // Try adding with different types to same name to test behavior
        if (t < 4) {
            pugi::xpath_value_type next_type = types[t + 1];
            pugi::xpath_variable* var2 = variables.add(name_buf, next_type);
            if (var2) {
                pugi::xpath_value_type var2_type = var2->type();
                const pugi::char_t* var2_name = var2->name();
                const pugi::char_t* var2_str = var2->get_string();
            }
        }
    }
}

static void fuzz_xpath_query(const uint8_t* data, size_t size) {
    // Create empty query
    pugi::xpath_query query;
    
    // Test return_type on uninitialized query
    pugi::xpath_value_type ret_type = query.return_type();
    
    // Create a valid query to test return_type
    pugi::xml_document doc;
    if (size > 0) {
        // Create simple XML from input data (sanitized)
        char xml_buf[256];
        size_t xml_len = (size > 250) ? 250 : size;
        for (size_t i = 0; i < xml_len; ++i) {
            xml_buf[i] = (data[i] >= 32 && data[i] < 127) ? static_cast<char>(data[i]) : 'x';
        }
        xml_buf[xml_len] = '\0';
        
        // Ensure it's valid XML by wrapping
        char full_xml[512];
        snprintf(full_xml, sizeof(full_xml), "<root>%s</root>", xml_buf);
        
        doc.load_string(full_xml);
        
        // Try to compile some XPath queries
        const char* queries[] = {
            "count(//*)",
            "string(//*)",
            "boolean(//*)",
            "number(//*)",
            "//node",
            "/root",
            "concat('a', 'b')"
        };
        
        for (int i = 0; i < 6; ++i) {
            pugi::xpath_query q(queries[i]);
            pugi::xpath_value_type q_type = q.return_type();
            
            // Test with variables
            pugi::xpath_variable_set vars;
            vars.add("test_var", pugi::xpath_type_string);
            vars.add("num_var", pugi::xpath_type_number);
            
            pugi::xpath_variable* v1 = vars.get("test_var");
            if (v1) {
                pugi::xpath_value_type v1_type = v1->type();
                const pugi::char_t* v1_name = v1->name();
                const pugi::char_t* v1_str = v1->get_string();
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Fuzz xpath_variable_set and xpath_variable functions
    fuzz_xpath_variable_set_and_variable(Data, Size);
    
    // Fuzz xpath_query functions
    fuzz_xpath_query(Data, Size);
    
    return 0;
}