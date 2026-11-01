// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
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
#include <stdexcept>

static pugi::xml_document parse_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    
    if (!doc.load_buffer(data, size)) {
        return doc;
    }
    return doc;
}

static pugi::xpath_query compile_query_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xpath_query query;
    if (size == 0) {
        return query;
    }
    
    if (size > 4096) size = 4096;
    
    char* buffer = new char[size + 1];
    std::memcpy(buffer, data, size);
    buffer[size] = '\0';
    
    try {
        query = pugi::xpath_query(buffer);
    } catch (...) {
        delete[] buffer;
        return pugi::xpath_query();
    }
    delete[] buffer;
    return query;
}

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t mid = Size / 2;
    const uint8_t* xml_data = Data;
    size_t xml_size = mid;
    const uint8_t* xpath_data = Data + mid;
    size_t xpath_size = Size - mid;

    pugi::xml_document doc = parse_document_from_fuzz_data(xml_data, xml_size);
    pugi::xpath_query query = compile_query_from_fuzz_data(xpath_data, xpath_size);
    
    pugi::xpath_node context_node;
    if (doc.root().type() != pugi::node_null) {
        context_node = pugi::xpath_node(doc.root());
    }

    try {
        pugi::xpath_node node_result = query.evaluate_node(context_node);
        (void)node_result;

        bool bool_result = query.evaluate_boolean(context_node);
        (void)bool_result;

        double num_result = query.evaluate_number(context_node);
        (void)num_result;

        pugi::xpath_node_set node_set_result = query.evaluate_node_set(context_node);
        (void)node_set_result;

        pugi::string_t str_result = query.evaluate_string(context_node);
        (void)str_result;

        if (doc.root().type() != pugi::node_null) {
            pugi::xpath_node_set select_result = doc.root().select_nodes(query);
            (void)select_result;
        }
        
    } catch (const std::exception&) {
    } catch (...) {
    }

    return 0;
}