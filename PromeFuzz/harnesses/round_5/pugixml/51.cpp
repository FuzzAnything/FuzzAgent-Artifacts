// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <fstream>

static void writeToDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* Data, size_t Size) {
    // Test pugi::as_wide overloads
    if (Size > 0) {
        // First overload: const std::basic_string<char>&
        std::string str1(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)1000));
        auto wide_result1 = pugi::as_wide(str1);
        
        // Second overload: const char* - need to ensure null termination
        if (Size < 1000000) {  // Reasonable size limit
            std::vector<char> buffer(Size + 1);
            memcpy(buffer.data(), Data, Size);
            buffer[Size] = '\0';
            auto wide_result2 = pugi::as_wide(buffer.data());
            
            // Test pugi::as_utf8 with the wide strings we just created
            if (!wide_result1.empty()) {
                auto utf8_result1 = pugi::as_utf8(wide_result1);
                auto utf8_result2 = pugi::as_utf8(wide_result1.c_str());
            }
        }
    }
    
    // Test pugi::as_utf8 overloads with random wide data
    if (Size >= sizeof(wchar_t) && Size < 1000000) {
        size_t wchar_count = Size / sizeof(wchar_t);
        std::vector<wchar_t> wchar_buffer(wchar_count + 1);
        memcpy(wchar_buffer.data(), Data, wchar_count * sizeof(wchar_t));
        wchar_buffer[wchar_count] = L'\0';
        
        std::wstring wstr1(wchar_buffer.data(), wchar_count);
        
        // First overload: const std::basic_string<wchar_t>&
        auto utf8_result3 = pugi::as_utf8(wstr1);
        
        // Second overload: const wchar_t*
        auto utf8_result4 = pugi::as_utf8(wchar_buffer.data());
    }
    
    // Test xml_node::path and xml_node::name
    writeToDummyFile(Data, Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (result) {
        // Traverse all nodes and test path() and name()
        pugi::xml_node node = doc.first_child();
        
        while (node) {
            // Test path() with different delimiters
            std::string path1 = node.path().c_str();
            std::string path2 = node.path('.').c_str();
            std::string path3 = node.path('|').c_str();
            
            // Test name()
            const char* node_name = node.name();
            
            // Iteratively traverse children without recursion
            std::vector<pugi::xml_node> nodes_to_process;
            
            // Add all children of current node to process
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                nodes_to_process.push_back(child);
            }
            
            // Process all nodes in the vector
            while (!nodes_to_process.empty()) {
                pugi::xml_node current = nodes_to_process.back();
                nodes_to_process.pop_back();
                
                std::string child_path = current.path().c_str();
                const char* child_name = current.name();
                
                // Add children of current node to process
                for (pugi::xml_node grandchild = current.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                    nodes_to_process.push_back(grandchild);
                }
            }
            
            node = node.next_sibling();
        }
    }
    
    // Clean up
    std::remove("./dummy_file");
    
    return 0;
}