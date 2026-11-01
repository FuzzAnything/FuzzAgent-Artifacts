// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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
#include <string>
#include <vector>
#include <stdexcept>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    
    // Try to parse as UTF-8 XML
    if (size > 0) {
        // Ensure null termination for safety
        std::vector<char> buffer(size + 1, 0);
        std::memcpy(buffer.data(), data, size);
        
        pugi::xml_parse_result result = doc->load_string(buffer.data(), pugi::parse_default);
        if (!result) {
            // If parsing fails, create a minimal valid document
            doc->reset();
            doc->append_child("root");
        }
    } else {
        doc->append_child("root");
    }
    
    return doc;
}

static void cleanup_document(pugi::xml_document* doc) {
    delete doc;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    try {
        pugi::xml_document* doc = create_document_from_input(Data, Size);
        if (!doc) {
            return 0;
        }
        
        pugi::xml_node root = doc->document_element();
        if (!root) {
            cleanup_document(doc);
            return 0;
        }
        
        // Prepare XPath expressions from input data
        std::vector<std::string> queries;
        size_t chunk_size = Size / 9;
        if (chunk_size == 0) chunk_size = 1;
        
        for (size_t i = 0; i < 9 && i * chunk_size < Size; ++i) {
            size_t start = i * chunk_size;
            size_t len = (i == 8) ? (Size - start) : chunk_size;
            std::string query(reinterpret_cast<const char*>(Data + start), len);
            queries.push_back(query);
        }
        
        // Ensure we have at least some valid queries if input was too small
        while (queries.size() < 9) {
            queries.push_back("./*");
        }
        
        // Create variable set for queries that need it
        pugi::xpath_variable_set variables;
        
        // First select_node call (string overload)
        pugi::xpath_node result1;
        try {
            result1 = root.select_node(queries[0].c_str(), &variables);
        } catch (...) {
            // Ignore exceptions from invalid XPath
        }
        
        // Second select_node call (string overload)
        pugi::xpath_node result2;
        try {
            result2 = root.select_node(queries[1].c_str(), &variables);
        } catch (...) {
            // Ignore exceptions from invalid XPath
        }
        
        // First xpath_node::node call
        pugi::xml_node node1 = result1.node();
        
        // Second xpath_node::node call
        pugi::xml_node node2 = result2.node();
        
        // Third select_node call (compiled query overload)
        pugi::xpath_node result3;
        try {
            pugi::xpath_query query3(queries[2].c_str(), &variables);
            result3 = root.select_node(query3);
        } catch (...) {
            // Ignore exceptions from invalid XPath
        }
        
        // Fourth select_node call (compiled query overload)
        pugi::xpath_node result4;
        try {
            pugi::xpath_query query4(queries[3].c_str(), &variables);
            result4 = root.select_node(query4);
        } catch (...) {
            // Ignore exceptions from invalid XPath
        }
        
        // Fifth select_node call (string overload)
        pugi::xpath_node result5;
        try {
            result5 = root.select_node(queries[4].c_str(), &variables);
        } catch (...) {
            // Ignore exceptions from invalid XPath
        }
        
        // Sixth select_node call (string overload)
        pugi::xpath_node result6;
        try {
            result6 = root.select_node(queries[5].c_str(), &variables);
        } catch (...) {
            // Ignore exceptions from invalid XPath
        }
        
        // Seventh select_node call (compiled query overload)
        pugi::xpath_node result7;
        try {
            pugi::xpath_query query7(queries[6].c_str(), &variables);
            result7 = root.select_node(query7);
        } catch (...) {
            // Ignore exceptions from invalid XPath
        }
        
        // Third xpath_node::node call
        pugi::xml_node node3 = result5.node();
        
        // Fourth xpath_node::node call
        pugi::xml_node node4 = result6.node();
        
        // Use the nodes to trigger more code paths
        (void)node1;
        (void)node2;
        (void)node3;
        (void)node4;
        
        cleanup_document(doc);
        return 0;
        
    } catch (...) {
        // Catch any unexpected exceptions
        return 0;
    }
}