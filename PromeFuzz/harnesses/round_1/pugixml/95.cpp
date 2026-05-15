// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result xml_result = doc.load_buffer(Data, Size, pugi::parse_default);
    
    // Test xml_parse_result::description
    const char* xml_desc = xml_result.description();
    (void)xml_desc;
    
    // Create XPath query from part of the input
    std::string xpath_str;
    if (Size > 0) {
        size_t xpath_len = Size / 2;
        if (xpath_len == 0) xpath_len = Size;
        xpath_str.assign(reinterpret_cast<const char*>(Data), xpath_len);
    }
    
    // Test xpath_query functions
    pugi::xpath_query query;
    
    // Wrap in try-catch since xpath_query constructor can throw xpath_exception
    try {
        if (!xpath_str.empty()) {
            query = pugi::xpath_query(xpath_str.c_str());
        }
        
        // Test xpath_query::result and xpath_parse_result::description
        const pugi::xpath_parse_result& xpath_result = query.result();
        const char* xpath_parse_desc = xpath_result.description();
        (void)xpath_parse_desc;
        
        // Test evaluate_node_set if we have a valid document
        if (xml_result) {
            pugi::xpath_node context(doc.root());
            pugi::xpath_node_set node_set = query.evaluate_node_set(context);
            
            // Test xpath_node_set methods
            if (!node_set.empty()) {
                pugi::xpath_node_set::const_iterator it = node_set.begin();
                pugi::xpath_node_set::const_iterator end = node_set.end();
                (void)it; (void)end;
                
                pugi::xpath_node_set::type_t type = node_set.type();
                (void)type;
            }
        }
        
    } catch (const std::exception& e) {
        // Catch any exception that might be thrown
        const char* exception_what = e.what();
        (void)exception_what;
    }
    
    // Test default constructors
    pugi::xpath_node_set empty_set;
    pugi::xpath_node empty_node;
    pugi::xpath_parse_result default_xpath_result;
    pugi::xml_parse_result default_xml_result;
    
    // Test description on default results
    const char* default_xpath_desc = default_xpath_result.description();
    const char* default_xml_desc = default_xml_result.description();
    (void)default_xpath_desc;
    (void)default_xml_desc;
    
    return 0;
}