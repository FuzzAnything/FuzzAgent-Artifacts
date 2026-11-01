// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

static std::string create_xml_from_input(const uint8_t* data, size_t size) {
    if (size == 0) {
        return "<root><child>default</child></root>";
    }
    // Escape basic XML special characters to ensure well-formedness where possible
    std::string result;
    result.reserve(size * 2 + 20);
    result += "<root>";
    for (size_t i = 0; i < size; ++i) {
        uint8_t c = data[i];
        if (c == '<') {
            result += "&lt;";
        } else if (c == '>') {
            result += "&gt;";
        } else if (c == '&') {
            result += "&amp;";
        } else if (c == '"') {
            result += "&quot;";
        } else if (c == '\'') {
            result += "&apos;";
        } else if (c >= 32 && c < 127) {
            result += static_cast<char>(c);
        } else {
            // Replace non-printable chars with a space or skip
            result += " ";
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    std::string xml_content = create_xml_from_input(Data, Size);
    
    pugi::xml_document doc;
    if (!doc.load_string(xml_content.c_str())) {
        // If parsing fails, try to load a minimal valid XML to test structure
        doc.load_string("<root><child>test</child></root>");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // Prepare a valid XPath query for testing
    // We construct a query string from the input data, ensuring it's valid XPath syntax
    std::string xpath_expr = "//";
    // Take a safe subset of data for the tag name to avoid syntax errors
    for (size_t i = 0; i < std::min(Size, static_cast<size_t>(10)); ++i) {
        uint8_t c = Data[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            xpath_expr += static_cast<char>(c);
        }
    }
    if (xpath_expr == "//") {
        xpath_expr = "//child";
    }

    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(xpath_expr.c_str());
    } catch (...) {
        // If query compilation fails, try a default one
        try {
            query = pugi::xpath_query("//child");
        } catch (...) {
            return 0;
        }
    }

    pugi::xpath_node context_node;
    if (root) {
        context_node = pugi::xpath_node(root);
    } else {
        context_node = pugi::xpath_node(doc);
    }

    // 2. Invoke target functions in the specified order with error handling

    // Step 1: pugi::xml_node::child
    // We use a string from input or a default name
    const char* child_name = "child";
    if (Size > 0) {
        // Try to use input as name if printable
        if (Data[0] >= 'a' && Data[0] <= 'z') {
            child_name = reinterpret_cast<const char*>(Data);
        }
    }
    pugi::xml_node node1 = root.child(child_name);

    // Step 2: pugi::xml_node::attribute
    const char* attr_name = "id";
    if (Size > 10) {
        if (Data[10] >= 'a' && Data[10] <= 'z') {
            attr_name = reinterpret_cast<const char*>(Data + 10);
        }
    }
    pugi::xml_attribute attr = node1.attribute(attr_name);

    // Step 3: pugi::xml_node::child (again)
    pugi::xml_node node2 = node1.child(child_name);

    // Step 4: pugi::xpath_query::evaluate_boolean
    try {
        bool b = query.evaluate_boolean(context_node);
        (void)b;
    } catch (...) {
        // Ignore exceptions
    }

    // Step 5: pugi::xpath_query::evaluate_number
    try {
        double num = query.evaluate_number(context_node);
        (void)num;
    } catch (...) {
        // Ignore exceptions
    }

    // Step 6: pugi::xpath_query::evaluate_string (overload 1: buffer)
    char buffer[256];
    try {
        size_t len = query.evaluate_string(buffer, sizeof(buffer), context_node);
        (void)len;
    } catch (...) {
        // Ignore exceptions
    }

    // Step 6 (continued): pugi::xpath_query::evaluate_string (overload 2: string_t)
    try {
        pugi::string_t str = query.evaluate_string(context_node);
        (void)str;
    } catch (...) {
        // Ignore exceptions
    }

    // Step 7: pugi::xpath_query::evaluate_string (again as per sequence)
    try {
        pugi::string_t str2 = query.evaluate_string(context_node);
        (void)str2;
    } catch (...) {
        // Ignore exceptions
    }

    // Step 8: pugi::xpath_query::evaluate_node_set
    try {
        pugi::xpath_node_set nodes = query.evaluate_node_set(context_node);
        (void)nodes;
    } catch (...) {
        // Ignore exceptions
    }

    // Step 9: pugi::xpath_query::evaluate_node
    try {
        pugi::xpath_node node = query.evaluate_node(context_node);
        (void)node;
    } catch (...) {
        // Ignore exceptions
    }

    // 3. Cleanup
    // pugixml handles memory internally, no explicit cleanup needed for document
    return 0;
}