// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <fstream>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void testXPathQuery(pugi::xpath_query& query, const pugi::xpath_node& context) {
    // Test evaluate_node
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
    #endif
        (void)query.evaluate_node(context);
    #ifndef PUGIXML_NO_EXCEPTIONS
    } catch (const pugi::xpath_exception&) {
    } catch (const std::bad_alloc&) {
    }
    #endif

    // Test evaluate_boolean
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
    #endif
        (void)query.evaluate_boolean(context);
    #ifndef PUGIXML_NO_EXCEPTIONS
    } catch (const std::bad_alloc&) {
    }
    #endif

    // Test evaluate_string
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
    #endif
        (void)query.evaluate_string(context);
    #ifndef PUGIXML_NO_EXCEPTIONS
    } catch (const std::bad_alloc&) {
    }
    #endif

    // Test evaluate_number
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
    #endif
        (void)query.evaluate_number(context);
    #ifndef PUGIXML_NO_EXCEPTIONS
    } catch (const std::bad_alloc&) {
    }
    #endif

    // Test evaluate_node_set
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
    #endif
        (void)query.evaluate_node_set(context);
    #ifndef PUGIXML_NO_EXCEPTIONS
    } catch (const pugi::xpath_exception&) {
    } catch (const std::bad_alloc&) {
    }
    #endif
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write data to dummy file for potential file-based XML loading
    writeDummyFile(Data, Size);

    // Split input into two parts for XML content and XPath expression
    size_t xmlSize = Size / 2;
    size_t xpathSize = Size - xmlSize;
    
    const char* xmlData = reinterpret_cast<const char*>(Data);
    const char* xpathData = reinterpret_cast<const char*>(Data + xmlSize);

    // Create XML document from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result parseResult = doc.load_buffer(xmlData, xmlSize);
    
    // If parsing fails, try loading from file
    if (!parseResult) {
        doc.load_file("./dummy_file");
    }

    // Create XPath query from null-terminated string
    pugi::xpath_query query;
    std::string xpathStr;
    
    if (xpathSize > 0) {
        xpathStr.assign(xpathData, std::min(xpathSize, (size_t)1024)); // Limit size for safety
    } else {
        xpathStr = "";
    }
    
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
    #endif
        query = pugi::xpath_query(xpathStr.c_str());
    #ifndef PUGIXML_NO_EXCEPTIONS
    } catch (const pugi::xpath_exception&) {
        // If query construction fails, create empty query
        query = pugi::xpath_query();
    } catch (const std::bad_alloc&) {
        query = pugi::xpath_query();
    }
    #endif

    // Get root node for context
    pugi::xml_node root = doc.root();
    pugi::xpath_node context(root);

    // Test evaluate functions with root context
    testXPathQuery(query, context);

    // Test select_nodes on root node
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
    #endif
        (void)root.select_nodes(query);
    #ifndef PUGIXML_NO_EXCEPTIONS
    } catch (const pugi::xpath_exception&) {
    } catch (const std::bad_alloc&) {
    }
    #endif

    // Test with different contexts to explore more states
    if (root) {
        // Test with first child as context
        pugi::xml_node firstChild = root.first_child();
        if (firstChild) {
            pugi::xpath_node childContext(firstChild);
            testXPathQuery(query, childContext);
            
            // Test select_nodes on child node
            #ifndef PUGIXML_NO_EXCEPTIONS
            try {
            #endif
                (void)firstChild.select_nodes(query);
            #ifndef PUGIXML_NO_EXCEPTIONS
            } catch (const pugi::xpath_exception&) {
            } catch (const std::bad_alloc&) {
            }
            #endif
        }

        // Test with first attribute as context (requires both attribute and parent)
        pugi::xml_attribute firstAttr = root.first_attribute();
        if (firstAttr) {
            pugi::xpath_node attrContext(firstAttr, root);
            testXPathQuery(query, attrContext);
        }

        // Test with empty node as context
        pugi::xml_node emptyNode;
        pugi::xpath_node emptyContext(emptyNode);
        testXPathQuery(query, emptyContext);
    }

    // Test with empty document
    pugi::xml_document emptyDoc;
    pugi::xml_node emptyRoot = emptyDoc.root();
    pugi::xpath_node emptyRootContext(emptyRoot);
    testXPathQuery(query, emptyRootContext);

    // Test select_nodes on empty node
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
    #endif
        (void)emptyRoot.select_nodes(query);
    #ifndef PUGIXML_NO_EXCEPTIONS
    } catch (const pugi::xpath_exception&) {
    } catch (const std::bad_alloc&) {
    }
    #endif

    return 0;
}