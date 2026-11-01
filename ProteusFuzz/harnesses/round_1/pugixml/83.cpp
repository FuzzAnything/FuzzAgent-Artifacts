// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
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

// Helper to convert fuzz input to pugi::char_t string safely
static const pugi::char_t* to_pugi_char_t(const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;
    
    static pugi::char_t buffer[4096];
    size_t len = (size < 4095) ? size : 4095;
    
    // Copy bytes, assuming char_t is compatible with uint8_t for the copy size
    // Note: In wide mode, this copies bytes which might be invalid UTF-16/32, 
    // but that's acceptable for fuzzing to trigger edge cases.
    std::memcpy(buffer, data, len * sizeof(pugi::char_t));
    buffer[len] = '\0';
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    root.append_child("child").append_child("grandchild");
    
    pugi::xpath_variable_set variables;
    pugi::xpath_variable_set variables_const;
    
    // Convert input to pugi::char_t string safely
    const pugi::char_t* input_str = to_pugi_char_t(Data, Size);

    // 2. Test pugi::xpath_variable_set::add
    // Try adding variables of different types
    pugi::xpath_variable* var_node_set = variables.add(input_str, pugi::xpath_type_node_set);
    pugi::xpath_variable* var_number = variables.add(input_str, pugi::xpath_type_number);
    pugi::xpath_variable* var_string = variables.add(input_str, pugi::xpath_type_string);
    pugi::xpath_variable* var_boolean = variables.add(input_str, pugi::xpath_type_boolean);

    // 3. Test pugi::xpath_variable::name
    if (var_node_set) {
        const pugi::char_t* name = var_node_set->name();
        (void)name; // Use to avoid warning
    }
    if (var_number) {
        const pugi::char_t* name = var_number->name();
        (void)name;
    }
    if (var_string) {
        const pugi::char_t* name = var_string->name();
        (void)name;
    }
    if (var_boolean) {
        const pugi::char_t* name = var_boolean->name();
        (void)name;
    }

    // 4. Test pugi::xpath_variable::get_string
    // Only safe for string type, but fuzzing should handle non-string gracefully (returns empty)
    if (var_string) {
        const pugi::char_t* str_val = var_string->get_string();
        (void)str_val;
    }
    // Intentionally call on non-string to test robustness
    if (var_number) {
        const pugi::char_t* str_val = var_number->get_string();
        (void)str_val;
    }

    // 5. Test pugi::xpath_variable_set::get
    pugi::xpath_variable* var_get = variables.get(input_str);
    (void)var_get;
    
    // Test const overload
    const pugi::xpath_variable* var_const = variables_const.get(input_str);
    (void)var_const;

    // 6. Test pugi::xpath_variable_set::set
    // Using the signature: bool set(const char_t* name, const char_t* value);
    // We reuse input_str for both name and value to test various states
    variables.set(input_str, input_str); 
    // Also try with a dummy value to vary state
    variables.set(input_str, "true");
    variables.set(input_str, "false");

    // 7. Test pugi::xml_node::select_nodes
    // We need an XPath query. We can use the input as a query, but it might be malformed.
    // That's fine for fuzzing.
    // Wrap in try-catch to handle exceptions thrown by invalid XPath queries
    try {
        pugi::xpath_node_set result = root.select_nodes(input_str, &variables);
        (void)result;
        
        // Try with no variables
        pugi::xpath_node_set result_no_vars = root.select_nodes(input_str, nullptr);
        (void)result_no_vars;
    } catch (...) {
        // Catch and ignore exceptions from invalid XPath queries
    }

    // 8. Cleanup
    // pugixml uses RAII, so variables and doc will be cleaned up automatically.
    
    return 0;
}