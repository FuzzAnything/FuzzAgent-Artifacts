// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    pugi::xpath_variable_set variables;
    
    // Write data to dummy file and load
    write_dummy_file(Data, Size);
    
    // Load XML from dummy file
    if (!doc.load_file("./dummy_file")) {
        // If loading fails, try to parse as string directly if it looks like XML
        // Ensure the input is null-terminated for load_string
        if (Size > 0) {
            // Create a temporary buffer with a guaranteed null terminator
            std::vector<char> safe_buffer(Size + 1);
            std::memcpy(safe_buffer.data(), Data, Size);
            safe_buffer[Size] = '\0';
            
            // Check if the buffer contains a null byte before the end
            size_t null_pos = std::strlen(safe_buffer.data());
            if (null_pos < Size) {
                // Truncate at the first null byte to ensure valid C-string
                safe_buffer[null_pos] = '\0';
                pugi::xml_parse_result result = doc.load_string(safe_buffer.data());
                if (!result) {
                    // If still fails, create a minimal valid XML to proceed with fuzzing
                    doc.append_child("root");
                }
            } else {
                // No null byte found, treat as binary or invalid XML, create minimal XML
                doc.append_child("root");
            }
        } else {
            doc.append_child("root");
        }
    }

    // Create variables of different types
    pugi::xpath_variable* var_node_set = variables.add("ns", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_boolean = variables.add("b", pugi::xpath_type_boolean);
    pugi::xpath_variable* var_number = variables.add("n", pugi::xpath_type_number);
    pugi::xpath_variable* var_string = variables.add("s", pugi::xpath_type_string);

    if (!var_node_set || !var_boolean || !var_number || !var_string) {
        return 0;
    }

    // 1. pugi::xpath_variable::get_node_set
    const pugi::xpath_node_set& ns = var_node_set->get_node_set();
    (void)ns; // Use to avoid unused variable warning

    // 2. pugi::xpath_variable::set (boolean overload)
    var_boolean->set(Size % 2 != 0);

    // 3. pugi::xpath_variable::set (double overload)
    double num_val = static_cast<double>(Size);
    var_number->set(num_val);

    // 4. pugi::xpath_variable::set (string overload)
    // Create a safe string from input data, ensuring null termination
    pugi::char_t safe_str[256];
    size_t copy_len = (Size < 255) ? Size : 255;
    std::memcpy(safe_str, Data, copy_len);
    safe_str[copy_len] = 0;
    var_string->set(safe_str);

    // 5. pugi::xml_node::select_nodes
    pugi::xml_node root = doc.first_child();
    if (!root) {
        root = doc; // Fallback to document root
    }
    
    // Use a simple query that might use variables
    pugi::xpath_node_set result = root.select_nodes("/*", &variables);
    (void)result;

    // 6. pugi::xpath_variable::get_boolean
    bool bool_val = var_boolean->get_boolean();
    (void)bool_val;

    // 7. pugi::xpath_variable::get_number
    double num_val_out = var_number->get_number();
    (void)num_val_out;

    // 8. pugi::xpath_variable::get_string
    const pugi::char_t* str_val = var_string->get_string();
    (void)str_val;

    // 9. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();
    (void)first;

    return 0;
}