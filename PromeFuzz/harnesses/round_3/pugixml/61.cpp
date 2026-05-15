// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
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
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>

#include "pugixml.hpp"

static pugi::xml_document create_dummy_doc() {
    pugi::xml_document doc;
    const char* xml_content = "<root><node1>value1</node1><node2>value2</node2></root>";
    doc.load_string(xml_content);
    return doc;
}

static pugi::xpath_node_set create_dummy_node_set(const pugi::xml_document& doc) {
    return doc.select_nodes("/root/node1");
}

static std::string extract_string(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) return "";
    size_t length = std::min<size_t>(data[offset] % 32, size - offset - 1);
    offset++;
    std::string result(reinterpret_cast<const char*>(data + offset), length);
    offset += length;
    return result;
}

static pugi::xpath_value_type extract_type(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) return pugi::xpath_type_none;
    pugi::xpath_value_type types[] = {
        pugi::xpath_type_none,
        pugi::xpath_type_node_set,
        pugi::xpath_type_number,
        pugi::xpath_type_string,
        pugi::xpath_type_boolean
    };
    uint8_t index = data[offset] % 5;
    offset++;
    return types[index];
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* data, size_t size) {
    static pugi::xml_document dummy_doc = create_dummy_doc();
    static pugi::xpath_node_set dummy_node_set = create_dummy_node_set(dummy_doc);
    
    size_t offset = 0;
    
    pugi::xpath_variable_set var_set;
    
    std::string var_name1 = extract_string(data, size, offset);
    pugi::xpath_value_type type1 = extract_type(data, size, offset);
    
    pugi::xpath_variable* var1 = var_set.add(var_name1.c_str(), type1);
    if (var1) {
        pugi::xpath_value_type retrieved_type = var1->type();
        double num_val = var1->get_number();
        const pugi::xpath_node_set& node_set = var1->get_node_set();
    }
    
    std::string var_name2 = extract_string(data, size, offset);
    pugi::xpath_variable* var2 = var_set.get(var_name2.c_str());
    if (var2) {
        pugi::xpath_value_type type2 = var2->type();
        double num_val2 = var2->get_number();
        const pugi::xpath_node_set& node_set2 = var2->get_node_set();
    }
    
    std::string xpath_expr = extract_string(data, size, offset);
    if (!xpath_expr.empty()) {
        pugi::xpath_query query(xpath_expr.c_str());
        pugi::xpath_value_type return_type = query.return_type();
    }
    
    const pugi::xpath_variable_set& const_var_set = var_set;
    const pugi::xpath_variable* const_var = const_var_set.get(var_name1.c_str());
    if (const_var) {
        pugi::xpath_value_type const_type = const_var->type();
        double const_num = const_var->get_number();
        const pugi::xpath_node_set& const_node_set = const_var->get_node_set();
    }
    
    return 0;
}