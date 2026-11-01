// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // Try parsing the raw data first
    if (Size >= 4) {
        result = doc.load_buffer(Data, Size);
    } else {
        const char* xml_prefix = "<root>";
        const char* xml_suffix = "</root>";
        std::string buffer;
        buffer.reserve(strlen(xml_prefix) + Size + strlen(xml_suffix));
        buffer.append(xml_prefix);
        buffer.append(reinterpret_cast<const char*>(Data), Size);
        buffer.append(xml_suffix);
        result = doc.load_string(buffer.c_str());
    }

    // If parsing failed, try to load from a dummy file
    if (!result) {
        write_dummy_file(Data, Size);
        result = doc.load_file("./dummy_file");
    }

    // If still no valid document, construct a minimal one
    if (!result || doc.empty()) {
        doc.append_child("root");
    }

    pugi::xml_node node = doc.first_child();
    if (node.empty()) {
        node = doc;
    }

    // Prepare a safe XPath query.
    // Fuzzing arbitrary strings as XPath queries often throws exceptions if they are invalid syntax.
    // We will use a fixed valid query or a very simple one derived from input if possible,
    // but to ensure stability against the "deadly signal" caused by uncaught exceptions,
    // we will wrap the select_nodes call in a try-catch block.
    
    // Create a query string from input, ensuring it is null-terminated
    std::string query_str(reinterpret_cast<const char*>(Data), Size);
    query_str.push_back('\0');
    const char* query = query_str.c_str();

    pugi::xpath_node_set set;
    
    try {
        // Invoke select_nodes
        set = node.select_nodes(query);
    } catch (...) {
        // If the query is invalid and throws, we catch it and use an empty set
        // to continue testing the other methods on a valid (empty) object.
        return 0;
    }

    // Invoke the target functions in the specified order:
    
    // 1. pugi::xpath_node_set::size
    size_t sz = set.size();
    
    // 2. pugi::xpath_node_set::type
    pugi::xpath_node_set::type_t t = set.type();
    
    // 3. pugi::xpath_node_set::empty
    bool is_empty = set.empty();
    
    // 4. pugi::xpath_node_set::first
    pugi::xpath_node first_node = set.first();
    
    // 5. pugi::xpath_node_set::end
    auto end_iter = set.end();

    // Use the values to prevent unused variable warnings
    (void)sz;
    (void)t;
    (void)is_empty;
    (void)first_node;
    (void)end_iter;

    return 0;
}