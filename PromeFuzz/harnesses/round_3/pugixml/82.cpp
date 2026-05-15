// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
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
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: xml_parse_result::description
    {
        pugi::xml_parse_result parse_result;
        parse_result.status = static_cast<pugi::xml_parse_status>(Data[0] % 10);
        const char* desc = parse_result.description();
        (void)desc;
    }

    // Test 2: xpath_parse_result::description
    {
        pugi::xpath_parse_result xpath_result;
        xpath_result.error = "test error";
        xpath_result.offset = Size % 100;
        const char* desc = xpath_result.description();
        (void)desc;
    }

    // Test 3: Parse XML document and test xml_node::select_single_node
    {
        writeDummyFile(Data, Size);
        
        pugi::xml_document doc;
        pugi::xml_parse_result parse_result = doc.load_file("./dummy_file");
        
        const char* parse_desc = parse_result.description();
        (void)parse_desc;
        
        if (parse_result) {
            pugi::xml_node root = doc.root();
            
            // Create XPath query from input data (limit length)
            size_t query_len = Size > 100 ? 100 : Size;
            std::string xpath_query(reinterpret_cast<const char*>(Data), query_len);
            
            // Test select_single_node
            pugi::xpath_node selected = root.select_single_node(xpath_query.c_str());
            (void)selected;
            
            // Test 4: xpath_query::result
            #ifndef PUGIXML_NO_EXCEPTIONS
            try {
            #endif
                pugi::xpath_query query(xpath_query.c_str());
                const pugi::xpath_parse_result& query_result = query.result();
                const char* query_desc = query_result.description();
                (void)query_desc;
            #ifndef PUGIXML_NO_EXCEPTIONS
            } catch (const pugi::xpath_exception& e) {
                // Test xpath_exception::what and xpath_exception::result
                const char* what_msg = e.what();
                (void)what_msg;
                
                const pugi::xpath_parse_result& exc_result = e.result();
                const char* exc_desc = exc_result.description();
                (void)exc_desc;
            }
            #endif
        }
    }

    // Test 5: xpath_exception::what and xpath_exception::result
    // Note: xpath_exception is only available when PUGIXML_NO_EXCEPTIONS is not defined
    #ifndef PUGIXML_NO_EXCEPTIONS
    {
        pugi::xpath_parse_result xpath_result;
        xpath_result.error = "fuzzer error";
        xpath_result.offset = Size % 50;
        
        try {
            throw pugi::xpath_exception(xpath_result);
        } catch (const pugi::xpath_exception& e) {
            // Test what()
            const char* what_msg = e.what();
            (void)what_msg;
            
            // Test result()
            const pugi::xpath_parse_result& exc_result = e.result();
            const char* exc_desc = exc_result.description();
            (void)exc_desc;
        }
    }
    #endif

    // Additional test: Create xpath_query with random data and check result
    {
        size_t query_size = Size > 200 ? 200 : Size;
        std::string random_query(reinterpret_cast<const char*>(Data), query_size);
        
        #ifndef PUGIXML_NO_EXCEPTIONS
        try {
        #endif
            pugi::xpath_query q(random_query.c_str());
            const pugi::xpath_parse_result& res = q.result();
            const char* res_desc = res.description();
            (void)res_desc;
        #ifndef PUGIXML_NO_EXCEPTIONS
        } catch (const pugi::xpath_exception& e) {
            // Handle the exception safely
            const char* what_msg = e.what();
            (void)what_msg;
            
            const pugi::xpath_parse_result& exc_result = e.result();
            const char* exc_desc = exc_result.description();
            (void)exc_desc;
        }
        #endif
    }

    return 0;
}