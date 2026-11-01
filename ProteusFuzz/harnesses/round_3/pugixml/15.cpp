// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
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

static pugi::char_t* create_char_string(const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;
    // Ensure null termination
    size_t len = size;
    if (len > 0 && data[len - 1] == 0) {
        len--;
    }
    pugi::char_t* result = new pugi::char_t[len + 1];
    if (len > 0) {
        std::memcpy(result, data, len * sizeof(pugi::char_t));
    }
    result[len] = 0;
    return result;
}

static void cleanup_char_string(pugi::char_t* str) {
    delete[] str;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    pugi::xpath_variable_set variables;
    pugi::xml_node node;

    // Helper lambda to get a safe substring as char_t
    // Returns nullptr if the range is out of bounds or if the string is empty
    auto get_str = [&](size_t start, size_t max_len) -> pugi::char_t* {
        if (start >= Size) return nullptr;
        size_t len = (Size - start < max_len) ? (Size - start) : max_len;
        // Find first null or limit length
        size_t actual_len = 0;
        while (actual_len < len && Data[start + actual_len] != 0) {
            actual_len++;
        }
        if (actual_len == 0) return nullptr; // Avoid empty strings which might cause issues
        
        pugi::char_t* res = new pugi::char_t[actual_len + 1];
        for (size_t i = 0; i < actual_len; ++i) {
            res[i] = static_cast<pugi::char_t>(Data[start + i]);
        }
        res[actual_len] = 0;
        return res;
    };

    // 1. First set: double
    pugi::char_t* name1 = get_str(0, 10);
    if (name1) {
        double val1 = 0.0;
        if (Size > 0) val1 = static_cast<double>(Data[0]) / 256.0;
        variables.set(name1, val1);
        cleanup_char_string(name1);
    }

    // 2. Second set: string
    pugi::char_t* name2 = get_str(10, 10);
    pugi::char_t* str_val = get_str(20, 20);
    if (name2 && str_val) {
        variables.set(name2, str_val);
    }
    cleanup_char_string(name2);
    cleanup_char_string(str_val);

    // 3. Third set: bool
    pugi::char_t* name3 = get_str(30, 10);
    if (name3) {
        bool val3 = (Size > 40) ? (Data[40] % 2 == 0) : true;
        variables.set(name3, val3);
        cleanup_char_string(name3);
    }

    // 4. Fourth set: xpath_node_set
    pugi::char_t* name4 = get_str(40, 10);
    if (name4) {
        pugi::xpath_node_set empty_set;
        variables.set(name4, empty_set);
        cleanup_char_string(name4);
    }

    // 5. select_nodes
    // Wrap in try-catch to handle xpath_exception thrown by invalid queries
    pugi::char_t* query = get_str(50, 50);
    try {
        if (query) {
            pugi::xpath_node_set result = node.select_nodes(query, &variables);
            cleanup_char_string(query);
            (void)result; // Suppress unused variable warning
        } else {
            // Fallback if query is null/empty to ensure some execution path
            pugi::char_t* default_query = new pugi::char_t[2];
            default_query[0] = '.';
            default_query[1] = 0;
            pugi::xpath_node_set result = node.select_nodes(default_query, &variables);
            cleanup_char_string(default_query);
            (void)result;
        }
    } catch (...) {
        // Ignore exceptions from invalid XPath queries
        if (query) cleanup_char_string(query);
    }

    // 6. First get
    pugi::char_t* get_name1 = get_str(60, 10);
    if (get_name1) {
        pugi::xpath_variable* var1 = variables.get(get_name1);
        cleanup_char_string(get_name1);
        (void)var1;
    }

    // 7. Second get
    pugi::char_t* get_name2 = get_str(70, 10);
    if (get_name2) {
        pugi::xpath_variable* var2 = variables.get(get_name2);
        cleanup_char_string(get_name2);
        (void)var2;
    }

    // Const get hint
    const pugi::xpath_variable_set variables_const = variables;
    pugi::char_t* const_get_name = get_str(80, 10);
    if (const_get_name) {
        const pugi::xpath_variable* var_const = variables_const.get(const_get_name);
        cleanup_char_string(const_get_name);
        (void)var_const;
    }

    return 0;
}