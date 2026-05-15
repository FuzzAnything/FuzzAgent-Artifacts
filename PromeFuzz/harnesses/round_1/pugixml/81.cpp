// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    // Test pugi::as_utf8 with wchar_t* overload
    if (Size > 0) {
        // Create wide string from fuzzer input
        size_t wlen = Size / sizeof(wchar_t);
        if (wlen > 0) {
            std::vector<wchar_t> wchars(wlen);
            memcpy(wchars.data(), Data, wlen * sizeof(wchar_t));
            wchars.push_back(0); // Null terminate
            
            // Test wchar_t* overload
            try {
                pugi::as_utf8(wchars.data());
            } catch (...) {
                // Ignore exceptions
            }
            
            // Test std::wstring overload
            try {
                std::basic_string<wchar_t> wstr(wchars.data(), wlen);
                pugi::as_utf8(wstr);
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Test pugi::as_wide with char* overload
    if (Size > 0) {
        // Create null-terminated C string from fuzzer input
        std::vector<char> chars(Size + 1);
        memcpy(chars.data(), Data, Size);
        chars[Size] = 0;
        
        // Test char* overload
        try {
            pugi::as_wide(chars.data());
        } catch (...) {
            // Ignore exceptions
        }
        
        // Test std::string overload
        try {
            std::basic_string<char> str(chars.data(), Size);
            pugi::as_wide(str);
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test xml_node functions
    {
        // Create empty node
        pugi::xml_node node;
        
        // Test path() with default delimiter
        try {
            node.path();
        } catch (...) {
            // Ignore exceptions
        }
        
        // Test path() with custom delimiter
        try {
            char delimiter = '.';
            if (Size > 0) {
                delimiter = static_cast<char>(Data[0] % 128);
                if (delimiter == 0) delimiter = '.';
            }
            node.path(delimiter);
        } catch (...) {
            // Ignore exceptions
        }
        
        // Test name()
        try {
            node.name();
        } catch (...) {
            // Ignore exceptions
        }
        
        // Try to parse XML from fuzzer input and test on real nodes
        if (Size > 0) {
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_buffer(Data, Size);
            
            if (result) {
                // Test on document node
                try {
                    doc.path();
                    doc.name();
                } catch (...) {
                    // Ignore exceptions
                }
                
                // Test on root node
                pugi::xml_node root = doc.root();
                try {
                    root.path();
                    root.name();
                } catch (...) {
                    // Ignore exceptions
                }
                
                // Test on child nodes
                for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling()) {
                    try {
                        child.path();
                        child.name();
                    } catch (...) {
                        // Ignore exceptions
                    }
                }
            }
        }
    }
    
    return 0;
}