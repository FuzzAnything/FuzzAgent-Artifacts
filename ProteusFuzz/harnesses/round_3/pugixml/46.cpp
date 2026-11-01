// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.append_child("root");
        return doc;
    }

    std::size_t escaped_size = size * 2 + 200;
    std::vector<char> buffer(escaped_size);
    std::size_t offset = 0;
    
    buffer[offset++] = '<';
    buffer[offset++] = 'r';
    buffer[offset++] = 'o';
    buffer[offset++] = 'o';
    buffer[offset++] = 't';
    buffer[offset++] = '>';
    
    for (std::size_t i = 0; i < size; ++i) {
        char c = static_cast<char>(data[i]);
        if (c == '&') {
            buffer[offset++] = '&'; buffer[offset++] = 'a'; buffer[offset++] = 'm'; buffer[offset++] = 'p'; buffer[offset++] = ';';
        } else if (c == '<') {
            buffer[offset++] = '&'; buffer[offset++] = 'l'; buffer[offset++] = 't'; buffer[offset++] = ';';
        } else if (c == '>') {
            buffer[offset++] = '&'; buffer[offset++] = 'g'; buffer[offset++] = 't'; buffer[offset++] = ';';
        } else if (c >= 32 && c < 127) {
            buffer[offset++] = c;
        } else {
            buffer[offset++] = ' '; 
        }
    }
    
    buffer[offset++] = '<';
    buffer[offset++] = '/';
    buffer[offset++] = 'r';
    buffer[offset++] = 'o';
    buffer[offset++] = 'o';
    buffer[offset++] = 't';
    buffer[offset++] = '>';
    buffer[offset] = '\0';

    if (!doc.load_string(buffer.data())) {
        doc.append_child("root");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc = create_document_from_input(Data, Size);
    pugi::xml_node root = doc.document_element();
    
    if (!root) return 0;

    pugi::xpath_variable_set vars;
    pugi::xpath_variable_set vars_copy(vars);

    const char* queries[] = {
        ".", "/root", "//text()", "count(*)", "name()", "local-name()", 
        "namespace-uri()", "string()", "number()", "boolean()", "not()", 
        "position()", "last()", "id()", "generate-id()", "concat(1,2)", 
        "substring(1,1)", "translate(1,2,3)", "normalize-space(1)", "upper-case(1)", 
        "lower-case(1)", "starts-with(1,2)", "ends-with(1,2)", "contains(1,2)", 
        "substring-before(1,2)", "substring-after(1,2)", "matches(1,2)", "replace(1,2,3)", 
        "tokenize(1,2)", "resolve-uri(1,2)", "unparsed-entity-uri(1)", "unparsed-entity-available(1)", 
        "unparsed-entity-public-id(1)", "unparsed-entity-system-id(1)", "unparsed-entity-byte-order-mark(1)", 
        "unparsed-entity-encoding(1)", "unparsed-entity-schema-location(1)", "unparsed-entity-notations(1)", 
        "unparsed-entity-base-uri(1)", "unparsed-entity-cdata-section(1)", "unparsed-entity-comment(1)", 
        "unparsed-entity-processing-instruction(1)", "unparsed-entity-attribute(1)", "unparsed-entity-element(1)", 
        "unparsed-entity-namespace(1)", "unparsed-entity-text(1)", "unparsed-entity-document(1)", 
        "unparsed-entity-root(1)", "unparsed-entity-parent(1)", "unparsed-entity-child(1)", 
        "unparsed-entity-sibling(1)", "unparsed-entity-ancestor(1)", "unparsed-entity-descendant(1)", 
        "unparsed-entity-follower(1)", "unparsed-entity-preceding(1)", "unparsed-entity-following(1)", 
        "unparsed-entity-attribute-of(1)", "unparsed-entity-namespace-of(1)", "unparsed-entity-text-of(1)", 
        "unparsed-entity-comment-of(1)", "unparsed-entity-processing-instruction-of(1)", "unparsed-entity-document-of(1)", 
        "unparsed-entity-root-of(1)", "unparsed-entity-parent-of(1)", "unparsed-entity-child-of(1)", 
        "unparsed-entity-sibling-of(1)", "unparsed-entity-ancestor-of(1)", "unparsed-entity-descendant-of(1)", 
        "unparsed-entity-follower-of(1)", "unparsed-entity-preceding-of(1)", "unparsed-entity-following-of(1)"
    };

    int q_idx = 0;
    if (Size > 0) q_idx = Data[0] % (sizeof(queries)/sizeof(queries[0]));
    
    // Call 1: select_node (string overload)
    pugi::xpath_node res1;
    try {
        res1 = root.select_node(queries[q_idx]);
    } catch (...) {
        return 0;
    }
    
    // Call 2: select_node (compiled query overload)
    pugi::xpath_node res2;
    try {
        pugi::xpath_query query2(queries[q_idx]);
        res2 = root.select_node(query2);
    } catch (...) {
        return 0;
    }

    // Call 3: node() on res1
    pugi::xml_node node3 = res1.node();
    
    // Call 4: node() on res2
    pugi::xml_node node4 = res2.node();

    // Call 5: select_node (string overload) on node3 (if valid) or root
    pugi::xml_node target5 = node3.empty() ? root : node3;
    int q_idx5 = (Size > 1) ? Data[1] % (sizeof(queries)/sizeof(queries[0])) : 0;
    pugi::xpath_node res5;
    try {
        res5 = target5.select_node(queries[q_idx5]);
    } catch (...) {
        return 0;
    }

    // Call 6: select_node (compiled query overload) on node4 (if valid) or root
    pugi::xml_node target6 = node4.empty() ? root : node4;
    int q_idx6 = (Size > 2) ? Data[2] % (sizeof(queries)/sizeof(queries[0])) : 0;
    pugi::xpath_node res6;
    try {
        pugi::xpath_query query6(queries[q_idx6]);
        res6 = target6.select_node(query6);
    } catch (...) {
        return 0;
    }

    // Call 7: select_node (string overload) with variables
    pugi::xml_node target7 = res5.node().empty() ? root : res5.node();
    int q_idx7 = (Size > 3) ? Data[3] % (sizeof(queries)/sizeof(queries[0])) : 0;
    pugi::xpath_node res7;
    try {
        res7 = target7.select_node(queries[q_idx7], &vars_copy);
    } catch (...) {
        return 0;
    }

    // Call 8: node() on res5
    pugi::xml_node node8 = res5.node();

    // Call 9: node() on res6
    pugi::xml_node node9 = res6.node();

    // Additional exploration: try to access properties if nodes are valid
    if (!node3.empty()) node3.attribute("test");
    if (!node4.empty()) node4.attribute("test");
    if (!node8.empty()) node8.attribute("test");
    if (!node9.empty()) node9.attribute("test");
    
    // Check is_attribute on xpath_node results using the attribute() method
    // If attribute() returns a non-empty attribute, then the xpath_node represents an attribute.
    if (!res1.attribute().empty()) {
        // Do something with attribute
    }
    if (!res2.attribute().empty()) {
        // Do something with attribute
    }
    if (!res5.attribute().empty()) {
        // Do something with attribute
    }
    if (!res6.attribute().empty()) {
        // Do something with attribute
    }
    if (!res7.attribute().empty()) {
        // Do something with attribute
    }

    return 0;
}