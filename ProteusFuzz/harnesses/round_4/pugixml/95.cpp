// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <cstddef>

static pugi::xml_document* g_doc = nullptr;

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize document if not already done
    if (g_doc == nullptr) {
        g_doc = new pugi::xml_document();
        if (!g_doc) return 0;
    }

    // Parse input as XML if possible, otherwise use a dummy structure
    pugi::xml_document doc;
    bool parsed = false;
    
    // Try to parse the input as XML
    if (Size > 0) {
        // Ensure null termination
        char* buffer = new char[Size + 1];
        if (buffer) {
            std::memcpy(buffer, Data, Size);
            buffer[Size] = '\0';
            pugi::xml_parse_result result = doc.load_string(buffer);
            if (result.status == pugi::status_ok) {
                parsed = true;
            }
            delete[] buffer;
        }
    }

    // If parsing failed, create a simple dummy document
    if (!parsed) {
        doc.append_child("root").append_child("child").append_child("child");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        delete g_doc;
        g_doc = nullptr;
        return 0;
    }

    // Create variable set
    pugi::xpath_variable_set variables;
    pugi::xpath_variable_set variables_const = variables; // For const test

    // Extract potential variable name and query from input
    const char* var_name = nullptr;
    const char* query = nullptr;
    const char* string_val = nullptr;
    double double_val = 0.0;

    // Simple heuristic to extract strings from input
    if (Size > 0) {
        // Find first null-terminated string as variable name
        const char* start = reinterpret_cast<const char*>(Data);
        const char* end = start + Size;
        
        // Skip non-printable chars for variable name
        while (start < end && (*start < 32 || *start > 126)) start++;
        
        if (start < end) {
            const char* name_end = start;
            while (name_end < end && *name_end != '\0' && *name_end >= 32 && *name_end <= 126) name_end++;
            if (name_end > start) {
                var_name = start;
                // Create a safe copy for use
                size_t name_len = name_end - start;
                char* safe_name = new char[name_len + 1];
                if (safe_name) {
                    std::memcpy(safe_name, start, name_len);
                    safe_name[name_len] = '\0';
                    var_name = safe_name;
                }
            }
        }

        // Find another string for query
        start = end;
        // Search backwards for a reasonable query string
        const char* query_start = end;
        while (query_start > start && (*query_start < 32 || *query_start > 126)) query_start--;
        
        if (query_start > start) {
            const char* qs = query_start;
            while (qs > start && *(qs-1) >= 32 && *(qs-1) <= 126) qs--;
            if (qs < query_start) {
                size_t q_len = query_start - qs;
                char* safe_query = new char[q_len + 1];
                if (safe_query) {
                    std::memcpy(safe_query, qs, q_len);
                    safe_query[q_len] = '\0';
                    query = safe_query;
                }
            }
        }

        // Extract string value
        start = end;
        const char* sv_start = end;
        while (sv_start > start && (*sv_start < 32 || *sv_start > 126)) sv_start--;
        
        if (sv_start > start) {
            const char* svs = sv_start;
            while (svs > start && *(svs-1) >= 32 && *(svs-1) <= 126) svs--;
            if (svs < sv_start) {
                size_t sv_len = sv_start - svs;
                char* safe_sv = new char[sv_len + 1];
                if (safe_sv) {
                    std::memcpy(safe_sv, svs, sv_len);
                    safe_sv[sv_len] = '\0';
                    string_val = safe_sv;
                }
            }
        }

        // Extract double value from first few bytes
        if (Size >= 8) {
            double_val = *reinterpret_cast<const double*>(Data);
        }
    }

    // Fuzz pugi::xpath_variable_set::add
    if (var_name) {
        pugi::xpath_variable* var = variables.add(var_name, pugi::xpath_type_string);
        if (var) {
            // Fuzz pugi::xpath_variable::get_string
            const pugi::char_t* str = var->get_string();
            (void)str; // Use the result to avoid unused variable warning
        }

        var = variables.add(var_name, pugi::xpath_type_number);
        if (var) {
            // Try to get string from number type (should return empty)
            const pugi::char_t* str = var->get_string();
            (void)str;
        }

        var = variables.add(var_name, pugi::xpath_type_boolean);
        if (var) {
            const pugi::char_t* str = var->get_string();
            (void)str;
        }
    }

    // Fuzz pugi::xpath_variable_set::set (double overload)
    if (var_name) {
        bool result1 = variables.set(var_name, double_val);
        (void)result1;
    }

    // Fuzz pugi::xpath_variable_set::set (string overload)
    if (var_name && string_val) {
        bool result2 = variables.set(var_name, string_val);
        (void)result2;
    }

    // Fuzz pugi::xpath_variable_set::get
    if (var_name) {
        pugi::xpath_variable* var = variables.get(var_name);
        (void)var;
    }

    // Fuzz const version of get
    const pugi::xpath_variable* var_const = variables_const.get(var_name ? var_name : "");
    (void)var_const;

    // Fuzz pugi::xml_node::select_nodes
    if (query) {
        pugi::xpath_node_set nodes = root.select_nodes(query, &variables);
        (void)nodes;
    } else {
        // Fallback query
        pugi::xpath_node_set nodes = root.select_nodes("//child", &variables);
        (void)nodes;
    }

    // Cleanup local allocations
    if (var_name && var_name != reinterpret_cast<const char*>(Data)) {
        delete[] const_cast<char*>(var_name);
    }
    if (query && query != reinterpret_cast<const char*>(Data)) {
        delete[] const_cast<char*>(query);
    }
    if (string_val && string_val != reinterpret_cast<const char*>(Data)) {
        delete[] const_cast<char*>(string_val);
    }

    return 0;
}