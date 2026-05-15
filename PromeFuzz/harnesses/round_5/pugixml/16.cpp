// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
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
#include <cstddef>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create dummy file with fuzzer data
    writeDummyFile(Data, Size);
    
    // Initialize xpath_variable_set
    pugi::xpath_variable_set vars;
    
    // Create dummy xpath_node_set for first set call
    pugi::xpath_node_set node_set;
    
    // Create string from fuzzer data for variable names/values
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Call set() overloads in order
    // 1. xpath_node_set overload
    vars.set("var1", node_set);
    
    // 2. bool overload
    bool bool_val = Size > 0 ? (Data[0] % 2) == 0 : false;
    vars.set("var2", bool_val);
    
    // 3. double overload
    double double_val = Size > 8 ? *reinterpret_cast<const double*>(Data) : 0.0;
    vars.set("var3", double_val);
    
    // 4. const char_t* overload
    const char* str_val = input_str.c_str();
    vars.set("var4", str_val);
    
    // Create XML document and node for select_nodes
    pugi::xml_document doc;
    const char* xml_content = "<root><element>test</element></root>";
    if (doc.load_string(xml_content)) {
        pugi::xml_node root = doc.root();
        
        // Create XPath query from fuzzer data
        std::string xpath_query = input_str.substr(0, std::min(Size, (size_t)100));
        
        // Call select_nodes without exception handling
        pugi::xpath_node_set result = root.select_nodes(xpath_query.c_str(), &vars);
        
        // Call get() twice
        pugi::xpath_variable* var1 = vars.get("var1");
        pugi::xpath_variable* var2 = vars.get("var2");
        
        // Also test const version as hinted
        const pugi::xpath_variable_set& vars_const = vars;
        const pugi::xpath_variable* var_const = vars_const.get("var3");
    }
    
    return 0;
}