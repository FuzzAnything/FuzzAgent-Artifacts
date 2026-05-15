// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
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
#include <cstring>

static pugi::xml_document create_dummy_document() {
    pugi::xml_document doc;
    const char* xml_content = R"(<root><node attr="value">text</node><node>another</node></root>)";
    doc.load_string(xml_content);
    return doc;
}

static pugi::xpath_node_set create_dummy_node_set(const pugi::xml_document& doc) {
    return doc.select_nodes("/root/node");
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Initialize required objects
    pugi::xpath_variable_set var_set;
    pugi::xml_document doc = create_dummy_document();
    pugi::xml_node root = doc.root();
    pugi::xpath_node_set dummy_nodes = create_dummy_node_set(doc);
    
    // Use fuzzer input for variable names and values
    // We'll use the first part of input for string data
    // For safety, ensure we don't read beyond Size
    const char* name1 = "var1";
    const char* name2 = "var2";
    const char* name3 = "var3";
    const char* name4 = "var4";
    
    // Extract string value from fuzzer input if possible
    const char* string_value = "fuzz_string";
    if (Size > 0) {
        // Use a small buffer to avoid overflows
        static char buffer[256];
        size_t copy_size = Size < 255 ? Size : 255;
        memcpy(buffer, Data, copy_size);
        buffer[copy_size] = '\0';
        string_value = buffer;
    }
    
    // Extract double value - use first 8 bytes if available
    double double_value = 3.14;
    if (Size >= sizeof(double)) {
        memcpy(&double_value, Data, sizeof(double));
    }
    
    // Extract bool value - use first byte
    bool bool_value = true;
    if (Size > 0) {
        bool_value = (Data[0] & 1) != 0;
    }
    
    // First, add variables to the set (required before setting values)
    // Note: pugixml requires variables to be added before setting values
    var_set.add(name1, pugi::xpath_type_string);
    var_set.add(name2, pugi::xpath_type_node_set);
    var_set.add(name3, pugi::xpath_type_number);
    var_set.add(name4, pugi::xpath_type_boolean);
    
    // Call set() functions in specified order
    // 1. set(const char_t* name, const char_t* value)
    var_set.set(name1, string_value);
    
    // 2. set(const char_t* name, const xpath_node_set& value)
    var_set.set(name2, dummy_nodes);
    
    // 3. set(const char_t* name, double value)
    var_set.set(name3, double_value);
    
    // 4. set(const char_t* name, bool value)
    var_set.set(name4, bool_value);
    
    // Call select_nodes() with a simple query
    const char* query = "//node";
    try {
        pugi::xpath_node_set result = root.select_nodes(query, &var_set);
        (void)result; // Use result to avoid unused variable warning
    } catch (...) {
        // Ignore exceptions - empty result is acceptable per documentation
    }
    
    // Call get() functions
    // First get (non-const version)
    pugi::xpath_variable* var = var_set.get(name1);
    (void)var; // Use var to avoid unused variable warning
    
    // Second get (const version)
    const pugi::xpath_variable_set& const_var_set = var_set;
    const pugi::xpath_variable* const_var = const_var_set.get(name2);
    (void)const_var; // Use const_var to avoid unused variable warning
    
    return 0;
}