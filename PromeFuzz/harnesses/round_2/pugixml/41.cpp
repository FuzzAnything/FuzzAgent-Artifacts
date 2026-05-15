// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
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

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for potential file-based operations
    writeDummyFile(Data, Size);
    
    // Parse XML from input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        // If parsing fails, create a minimal valid XML document to continue fuzzing
        const char* minimal_xml = "<root><child/></root>";
        doc.load_string(minimal_xml);
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare XPath query strings from input data
    // Use first half of data for query1, second half for query2 (with bounds checking)
    size_t half_size = Size / 2;
    std::string query1, query2;
    
    if (half_size > 0) {
        query1.assign(reinterpret_cast<const char*>(Data), std::min(half_size, Size));
        query2.assign(reinterpret_cast<const char*>(Data + half_size), Size - half_size);
    } else if (Size > 0) {
        query1.assign(reinterpret_cast<const char*>(Data), Size);
        query2 = "//*";
    } else {
        query1 = "//*";
        query2 = "//*";
    }
    
    // Create xpath_variable_set for second overload
    pugi::xpath_variable_set variables;
    
    // 1. First pugi::xml_node::select_node (both overloads)
    pugi::xpath_query xq1(query1.c_str());
    pugi::xpath_node xn1_1 = root.select_node(xq1);
    pugi::xpath_node xn1_2 = root.select_node(query1.c_str(), &variables);
    
    // 2. Second pugi::xml_node::select_node (both overloads)
    pugi::xpath_query xq2(query2.c_str());
    pugi::xpath_node xn2_1 = root.select_node(xq2);
    pugi::xpath_node xn2_2 = root.select_node(query2.c_str(), &variables);
    
    // 3. First pugi::xpath_node::node()
    pugi::xml_node node3 = xn1_1.node();
    
    // 4. Second pugi::xpath_node::node()
    pugi::xml_node node4 = xn1_2.node();
    
    // 5. Third pugi::xml_node::select_node (both overloads)
    pugi::xpath_node xn5_1 = node3.select_node(xq1);
    pugi::xpath_node xn5_2 = node3.select_node(query1.c_str(), &variables);
    
    // 6. Fourth pugi::xml_node::select_node (both overloads)
    pugi::xpath_node xn6_1 = node4.select_node(xq2);
    pugi::xpath_node xn6_2 = node4.select_node(query2.c_str(), &variables);
    
    // 7. Fifth pugi::xml_node::select_node (both overloads)
    pugi::xpath_node xn7_1 = root.select_node(xq1);
    pugi::xpath_node xn7_2 = root.select_node(query2.c_str(), &variables);
    
    // 8. Third pugi::xpath_node::node()
    pugi::xml_node node8 = xn5_1.node();
    
    // 9. Fourth pugi::xpath_node::node()
    pugi::xml_node node9 = xn5_2.node();
    
    // Additional exploration: try with different nodes from the document
    for (pugi::xml_node child : root.children()) {
        // Try select_node on child nodes
        pugi::xpath_node child_xn = child.select_node(xq1);
        pugi::xml_node child_node = child_xn.node();
        
        // Try with attribute nodes if they exist
        for (pugi::xml_attribute attr : child.attributes()) {
            // Use the child node (which is the parent of the attribute) for XPath queries
            pugi::xpath_node attr_xn = child.select_node(xq2);
            pugi::xml_node attr_node = attr_xn.node();
        }
    }
    
    // Cleanup: file will be overwritten in next iteration
    
    return 0;
}