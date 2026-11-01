// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <vector>

static pugi::xml_document create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.append_child("root");
        return doc;
    }

    // Ensure the data is valid UTF-8 or UTF-16 by adding a header if needed
    // For simplicity, we treat input as a string and let pugixml handle parsing
    // We wrap the data in a root element if it's not well-formed
    std::string input(reinterpret_cast<const char*>(data), size);
    
    // Try to parse directly first
    if (doc.load_string(input.c_str())) {
        return doc;
    }

    // If parsing fails, try to wrap in a root element
    std::string wrapped = "<root>" + input + "</root>";
    doc.load_string(wrapped.c_str());
    
    // If still fails, create a minimal document
    if (!doc.root()) {
        doc.append_child("root");
    }
    
    return doc;
}

static std::string extract_query_from_fuzz_data(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "";
    
    size_t end = offset;
    while (end < size && data[end] != '\0' && data[end] != '\n' && data[end] != '\r') {
        end++;
    }
    
    if (end == offset) return "/*"; // Default fallback
    
    std::string query(reinterpret_cast<const char*>(data + offset), end - offset);
    return query.empty() ? "/*" : query;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    try {
        pugi::xml_document doc = create_document_from_fuzz_data(Data, Size);
        pugi::xml_node root = doc.root();
        if (!root) return 0;

        // Prepare variable set for XPath queries
        pugi::xpath_variable_set vars;

        // Extract multiple queries from the input data
        std::vector<std::string> queries;
        size_t offset = 0;
        while (offset < Size) {
            std::string q = extract_query_from_fuzz_data(Data, Size, offset);
            if (!q.empty()) {
                queries.push_back(q);
            }
            offset += q.size() + 1;
            if (offset >= Size) break;
        }
        
        // Ensure we have at least some queries
        if (queries.empty()) {
            queries.push_back("/*");
            queries.push_back(".//text()");
        }

        // 1. First select_node call (string overload)
        pugi::xpath_node node1 = root.select_node(queries[0].c_str(), &vars);
        
        // 2. Second select_node call (string overload)
        pugi::xpath_node node2 = root.select_node(queries.size() > 1 ? queries[1].c_str() : queries[0].c_str(), &vars);
        
        // 3. First node() call
        pugi::xml_node xml_node1 = node1.node();
        
        // 4. Second node() call
        pugi::xml_node xml_node2 = node2.node();
        
        // 5. Third select_node call (string overload)
        pugi::xpath_node node3 = root.select_node(queries.size() > 2 ? queries[2].c_str() : queries[0].c_str(), &vars);
        
        // 6. Fourth select_node call (string overload)
        pugi::xpath_node node4 = root.select_node(queries.size() > 3 ? queries[3].c_str() : queries[0].c_str(), &vars);
        
        // 7. Fifth select_node call (string overload)
        pugi::xpath_node node5 = root.select_node(queries.size() > 4 ? queries[4].c_str() : queries[0].c_str(), &vars);
        
        // 8. Third node() call
        pugi::xml_node xml_node3 = node3.node();
        
        // 9. Fourth node() call
        pugi::xml_node xml_node4 = node4.node();
        
        // Additional exploration: try compiled query overload
        try {
            pugi::xpath_query compiled_query(queries[0].c_str());
            pugi::xpath_node node6 = root.select_node(compiled_query);
            pugi::xml_node xml_node5 = node6.node();
            
            // Try with different nodes if available
            if (xml_node1 && xml_node1 != root) {
                pugi::xpath_node node7 = xml_node1.select_node(queries.size() > 1 ? queries[1].c_str() : queries[0].c_str(), &vars);
                pugi::xml_node xml_node6 = node7.node();
            }
        } catch (...) {
            // Ignore XPath compilation errors
        }
        
    } catch (...) {
        // Ignore all exceptions during fuzzing
    }
    
    return 0;
}