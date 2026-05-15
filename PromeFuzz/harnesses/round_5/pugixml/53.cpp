// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    // Test pugi::as_wide overloads
    if (Size > 0) {
        // Create null-terminated C-style string from fuzzer input
        char* cstr = new char[Size + 1];
        std::memcpy(cstr, Data, Size);
        cstr[Size] = '\0';
        
        // Test first overload: const char*
        std::basic_string<wchar_t> wide_result1 = pugi::as_wide(cstr);
        
        // Test second overload: std::basic_string<char>
        std::basic_string<char> str(cstr, Size);
        std::basic_string<wchar_t> wide_result2 = pugi::as_wide(str);
        
        delete[] cstr;
        
        // Test pugi::as_utf8 overloads using the wide strings we just created
        if (!wide_result1.empty()) {
            // Test first overload: const wchar_t*
            std::basic_string<char> utf8_result1 = pugi::as_utf8(wide_result1.c_str());
            
            // Test second overload: std::basic_string<wchar_t>
            std::basic_string<char> utf8_result2 = pugi::as_utf8(wide_result1);
        }
    }
    
    // Test pugi::xml_node::name and pugi::xml_node::path
    {
        // Create a default xml_node (null node)
        pugi::xml_node node;
        
        // Test xml_node::name on null node
        const pugi::char_t* node_name = node.name();
        
        // Test xml_node::path with default delimiter
        pugi::string_t path_default = node.path();
        
        // Test xml_node::path with custom delimiter
        if (Size > 0) {
            char delimiter = static_cast<char>(Data[0] % 128);
            if (delimiter == 0) delimiter = '/'; // Avoid null delimiter
            pugi::string_t path_custom = node.path(delimiter);
        }
    }
    
    return 0;
}