// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    if (!doc.load_file("./dummy_file")) {
        // If loading fails, try to create a minimal valid XML structure manually
        doc.append_child("root");
    }

    pugi::xpath_variable_set vars;
    
    // Create variables of different types to test set/get operations
    pugi::xpath_variable* var_node_set = vars.add("ns", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_number = vars.add("num", pugi::xpath_type_number);
    pugi::xpath_variable* var_string = vars.add("str", pugi::xpath_type_string);
    pugi::xpath_variable* var_boolean = vars.add("bool", pugi::xpath_type_boolean);

    if (!var_node_set || !var_number || !var_string || !var_boolean) {
        return 0;
    }

    // 1. pugi::xpath_variable::get_node_set
    const pugi::xpath_node_set& empty_set = var_node_set->get_node_set();
    (void)empty_set; // Suppress unused variable warning

    // 2. pugi::xpath_variable::set (double overload)
    double fuzz_double = 0.0;
    if (Size >= sizeof(double)) {
        std::memcpy(&fuzz_double, Data, sizeof(double));
    }
    var_number->set(fuzz_double);

    // 3. pugi::xpath_variable::set (bool overload)
    bool fuzz_bool = (Size > 0 && Data[0] % 2 == 0);
    var_boolean->set(fuzz_bool);

    // 4. pugi::xpath_variable::set (string overload)
    // Create a null-terminated string from fuzz data if possible
    // Use pugi::char_t to ensure correct type (char or wchar_t depending on build)
    pugi::char_t fuzz_str[256] = {0};
    size_t str_len = (Size < 255) ? Size : 255;
    for (size_t i = 0; i < str_len; ++i) {
        fuzz_str[i] = static_cast<pugi::char_t>(Data[i]);
        if (Data[i] == 0) break;
    }
    var_string->set(fuzz_str);

    // 5. pugi::xml_node::select_nodes
    pugi::xml_node root = doc.first_child();
    if (root) {
        // Use a simple query that might use variables, or a static one
        // We try a few variations based on input to explore states
        // Use PUGIXML_TEXT macro to handle char_t vs wchar_t automatically
        const pugi::char_t* query = nullptr;
        if (Size > 0 && Data[0] == 1) {
            query = PUGIXML_TEXT("/root/*");
        } else if (Size > 0 && Data[0] == 2) {
            query = PUGIXML_TEXT("/*");
        } else {
            query = PUGIXML_TEXT("//*");
        }
        
        pugi::xpath_node_set nodes = root.select_nodes(query, &vars);
        (void)nodes;
    }

    // 6. pugi::xpath_variable::get_boolean
    bool retrieved_bool = var_boolean->get_boolean();
    (void)retrieved_bool;

    // 7. pugi::xpath_variable::get_number
    double retrieved_num = var_number->get_number();
    (void)retrieved_num;

    // 8. pugi::xpath_variable::get_string
    const pugi::char_t* retrieved_str = var_string->get_string();
    (void)retrieved_str;

    // 9. pugi::xml_node::first_child
    pugi::xml_node child = root.first_child();
    (void)child;

    return 0;
}