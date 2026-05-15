// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
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
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Write data to dummy file for potential file operations
    writeDummyFile(Data, Size);

    // 1. Test pugi::xpath_parse_result::description()
    {
        pugi::xpath_parse_result result1;
        const char* desc1 = result1.description();
        (void)desc1;

        // Create another xpath_parse_result with error state
        pugi::xpath_parse_result result2;
        if (Size > 0) {
            result2.offset = static_cast<ptrdiff_t>(Data[0] % 256);
            static const char* error_str = "Test error";
            // Note: This is not safe in general but acceptable for fuzzing
            // We're simulating an error state
            const_cast<const char*&>(result2.error) = error_str;
            const char* desc2 = result2.description();
            (void)desc2;
        }
    }

    // 2, 3. Test pugi::xpath_exception::what() and result()
    // Note: xpath_exception might not be available if PUGIXML_NO_EXCEPTIONS is defined
    // We'll use conditional compilation
    #ifndef PUGIXML_NO_EXCEPTIONS
    {
        pugi::xpath_parse_result parse_result;
        if (Size > 0) {
            parse_result.offset = static_cast<ptrdiff_t>(Data[0] % 256);
            static const char* error_str = "XPath error";
            const_cast<const char*&>(parse_result.error) = error_str;
        }

        try {
            pugi::xpath_exception exc(parse_result);
            const char* what_msg = exc.what();
            const pugi::xpath_parse_result& result_ref = exc.result();
            (void)what_msg;
            (void)result_ref;
        } catch (...) {
            // Ignore exceptions during fuzzing
        }
    }
    #endif

    // 4. Test pugi::xml_node::select_single_node() overloads
    {
        pugi::xml_document doc;
        
        // Try to parse XML from input data
        pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
        
        if (parse_result) {
            pugi::xml_node root = doc.root();
            
            // Create XPath query string from input data
            size_t query_size = Size > 256 ? 256 : Size;
            char* query_str = new char[query_size + 1];
            if (query_size > 0) {
                memcpy(query_str, Data, query_size);
            }
            query_str[query_size] = '\0';
            
            // First overload: select_single_node(const char_t* query, xpath_variable_set* variables)
            pugi::xpath_node node1 = root.select_single_node(query_str);
            (void)node1;
            
            // Second overload: select_single_node(const xpath_query& query)
            #ifndef PUGIXML_NO_EXCEPTIONS
            try {
                pugi::xpath_query query(query_str);
                pugi::xpath_node node2 = root.select_single_node(query);
                (void)node2;
            } catch (pugi::xpath_exception& e) {
                // Ignore XPath compilation errors
                (void)e;
            }
            #else
            // When exceptions are disabled, xpath_query constructor doesn't throw
            pugi::xpath_query query(query_str);
            pugi::xpath_node node2 = root.select_single_node(query);
            (void)node2;
            #endif
            
            delete[] query_str;
        }
        
        // Test with empty node
        pugi::xml_node empty_node;
        pugi::xpath_node empty_result = empty_node.select_single_node("test");
        (void)empty_result;
    }

    // 5. Test pugi::xml_parse_result::description()
    {
        pugi::xml_parse_result result1;
        const char* desc1 = result1.description();
        (void)desc1;

        // Create parse result from actual parsing
        pugi::xml_document doc;
        pugi::xml_parse_result result2 = doc.load_buffer(Data, Size);
        const char* desc2 = result2.description();
        (void)desc2;

        // Test with various status codes
        pugi::xml_parse_result results[5];
        for (int i = 0; i < 5 && i < static_cast<int>(Size); i++) {
            results[i].status = static_cast<pugi::xml_parse_status>((Data[i] % 10) + 1);
            const char* desc = results[i].description();
            (void)desc;
        }
    }

    return 0;
}