// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

static void write_to_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static std::string generate_xpath_string(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "//*";
    size_t len = std::min(size - offset, static_cast<size_t>(100));
    return std::string(reinterpret_cast<const char*>(data + offset), len);
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Write data to dummy file for potential file-based operations
    write_to_dummy_file(Data, Size);

    // Parse XML from the input data
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    if (!parse_result) return 0;

    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // Prepare XPath variables and queries
    pugi::xpath_variable_set vars;
    std::string xpath1 = generate_xpath_string(Data, Size, 0);
    std::string xpath2 = generate_xpath_string(Data, Size, Size/3);
    std::string xpath3 = generate_xpath_string(Data, Size, 2*Size/3);

    pugi::xpath_query query1(xpath1.c_str());
    pugi::xpath_query query2(xpath2.c_str());
    pugi::xpath_query query3(xpath3.c_str());

    // First select_node (char* overload)
    pugi::xpath_node result1 = root.select_node(xpath1.c_str());
    // Second select_node (xpath_query overload)
    pugi::xpath_node result2 = root.select_node(query1);
    // First node() call
    pugi::xml_node node1 = result1.node();
    // Second node() call
    pugi::xml_node node2 = result2.node();

    // Third select_node (char* overload)
    pugi::xpath_node result3 = root.select_node(xpath2.c_str(), &vars);
    // Fourth select_node (xpath_query overload)
    pugi::xpath_node result4 = root.select_node(query2);
    // Fifth select_node (char* overload)
    pugi::xpath_node result5 = root.select_node(xpath3.c_str());
    // Sixth select_node (xpath_query overload)
    pugi::xpath_node result6 = root.select_node(query3);

    // Third node() call
    pugi::xml_node node3 = result3.node();
    // Fourth node() call
    pugi::xml_node node4 = result4.node();

    // Additional operations to explore program states
    if (node1) {
        node1.select_node(xpath1.c_str());
    }
    if (node2) {
        node2.select_node(query1);
    }
    if (node3) {
        node3.select_node(xpath2.c_str(), &vars);
    }
    if (node4) {
        node4.select_node(query2);
    }

    // Cleanup
    // No explicit cleanup needed for pugixml objects (RAII)

    return 0;
}