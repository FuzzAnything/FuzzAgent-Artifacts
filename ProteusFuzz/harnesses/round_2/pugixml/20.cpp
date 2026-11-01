// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
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
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Ensure the input is valid XML by wrapping it if necessary or just trying to parse
    // We create a null-terminated copy to safely pass to load_string
    // This prevents heap-buffer-overflow caused by strlen reading past the buffer
    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    
    bool parse_success = false;
    
    // Only attempt to parse if it looks like XML
    if (Size > 0 && xml_str[0] == '<') {
        parse_success = doc.load_string(xml_str.c_str());
    }
    
    if (!parse_success) {
        // Fallback: create a simple valid XML structure
        doc.append_child("root");
        doc.first_child().append_child("child");
        doc.first_child().first_child().text().set("test");
    }

    pugi::xml_node target_node = doc.root();
    if (!target_node) {
        target_node = doc;
    }

    // Prepare XPath query. 
    // We limit the query length to avoid excessive allocation.
    const size_t max_query_len = 1024;
    size_t query_len = (Size < max_query_len) ? Size : max_query_len;
    
    // Create a null-terminated buffer for the query
    char* query_buf = new char[query_len + 1];
    std::memcpy(query_buf, Data, query_len);
    query_buf[query_len] = '\0';

    pugi::xpath_variable_set* variables = nullptr;

    // Step 1: Test on an empty set (default constructed)
    // This satisfies the requirement to call size, type, empty, first, end before select_nodes
    // if we interpret the order as "exercise these methods on an empty set, then call select_nodes".
    pugi::xpath_node_set empty_set;
    
    // 1. size
    size_t s1 = empty_set.size();
    (void)s1;
    
    // 2. type
    pugi::xpath_node_set::type_t t1 = empty_set.type();
    (void)t1;
    
    // 3. empty
    bool e1 = empty_set.empty();
    (void)e1;
    
    // 4. first
    pugi::xpath_node n1 = empty_set.first();
    (void)n1;
    
    // 5. end
    auto it1 = empty_set.end();
    (void)it1;

    // Step 2: Invoke select_nodes to get a potentially non-empty set.
    // pugixml does not throw exceptions by default, so we don't need a try-catch block for xpath_exception.
    // However, if PUGIXML_NO_EXCEPTIONS is not defined, it might throw std::runtime_error or similar.
    // To be safe and compliant with standard C++ fuzzing practices, we can use a generic catch-all.
    pugi::xpath_node_set result_set;
    try {
        result_set = target_node.select_nodes(query_buf, variables);
        
        // Now invoke the methods on the result_set to cover non-empty states
        // 1. size
        size_t s2 = result_set.size();
        (void)s2;
        
        // 2. type
        pugi::xpath_node_set::type_t t2 = result_set.type();
        (void)t2;
        
        // 3. empty
        bool e2 = result_set.empty();
        (void)e2;
        
        // 4. first
        pugi::xpath_node n2 = result_set.first();
        (void)n2;
        
        // 5. end
        auto it2 = result_set.end();
        (void)it2;
        
    } catch (...) {
        // Ignore any exceptions (e.g., from invalid XPath queries if exceptions are enabled).
        // The empty_set tests above already covered the API calls.
        // This prevents the fuzzer from crashing on malformed input.
    }

    // Cleanup
    delete[] query_buf;
    
    return 0;
}