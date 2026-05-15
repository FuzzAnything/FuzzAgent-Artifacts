// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <fstream>

#include "pugixml.hpp"

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* Data, size_t Size) {
    // Test pugi::as_wide overloads
    {
        // Create a null-terminated C-style string from fuzzer input
        std::string str;
        if (Size > 0) {
            str.assign(reinterpret_cast<const char*>(Data), Size);
        }
        
        // Test first overload with std::string
        std::basic_string<wchar_t> wide1 = pugi::as_wide(str);
        
        // Test second overload with const char*
        if (!str.empty()) {
            std::basic_string<wchar_t> wide2 = pugi::as_wide(str.c_str());
        }
        
        // Test with empty string
        std::basic_string<wchar_t> wide3 = pugi::as_wide("");
    }
    
    // Test pugi::xml_node::name and pugi::xml_node::path
    {
        // Create XML document from fuzzer input
        write_dummy_file(Data, Size);
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file("./dummy_file");
        
        if (result) {
            // Test on document node
            auto doc_name = doc.name();
            auto doc_path = doc.path();
            
            // Test with custom delimiter
            auto doc_path_custom = doc.path('|');
            
            // Test on child nodes
            for (pugi::xml_node child : doc.children()) {
                auto child_name = child.name();
                auto child_path = child.path();
                auto child_path_custom = child.path('.');
                
                // Test on grandchildren
                for (pugi::xml_node grandchild : child.children()) {
                    auto grandchild_name = grandchild.name();
                    auto grandchild_path = grandchild.path();
                }
            }
            
            // Test on first node found
            pugi::xml_node first_node = doc.first_child();
            auto first_name = first_node.name();
            auto first_path = first_node.path();
        }
        
        // Test with null node
        pugi::xml_node null_node;
        auto null_name = null_node.name();
        auto null_path = null_node.path();
        
        // Test with constructed node from null pointer
        pugi::xml_node_struct* null_struct = nullptr;
        pugi::xml_node constructed_node(null_struct);
        auto constructed_name = constructed_node.name();
        auto constructed_path = constructed_node.path();
        auto constructed_path_custom = constructed_node.path('#');
    }
    
    // Test pugi::as_utf8 overloads
    {
        // Create wide string from fuzzer input
        if (Size >= sizeof(wchar_t)) {
            size_t wchar_count = Size / sizeof(wchar_t);
            const wchar_t* wdata = reinterpret_cast<const wchar_t*>(Data);
            
            // Create a null-terminated wide string for the const wchar_t* overload
            std::vector<wchar_t> terminated_wdata;
            terminated_wdata.reserve(wchar_count + 1);
            terminated_wdata.assign(wdata, wdata + wchar_count);
            terminated_wdata.push_back(0);  // Null-terminate
            
            // Test second overload with const wchar_t* (now null-terminated)
            std::basic_string<char> utf8_1 = pugi::as_utf8(terminated_wdata.data());
            
            // Test first overload with std::wstring
            std::basic_string<wchar_t> wstr(wdata, wchar_count);
            std::basic_string<char> utf8_2 = pugi::as_utf8(wstr);
            
            // Test with empty wide string
            std::basic_string<wchar_t> empty_wstr;
            std::basic_string<char> utf8_3 = pugi::as_utf8(empty_wstr);
        } else {
            // Test with empty wide string when input is too small
            std::basic_string<wchar_t> empty_wstr;
            std::basic_string<char> utf8_3 = pugi::as_utf8(empty_wstr);
        }
    }
    
    return 0;
}