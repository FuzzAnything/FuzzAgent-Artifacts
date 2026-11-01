// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

static std::string generate_xml_from_input(const uint8_t *data, size_t size) {
    std::string result;
    result.reserve(size + 100);
    result += "<root>";
    
    for (size_t i = 0; i < size; ++i) {
        unsigned char c = data[i];
        if (c >= 32 && c < 127 && c != '<' && c != '>' && c != '&' && c != '"' && c != '\'') {
            result += static_cast<char>(c);
        } else {
            if (c == 0) continue;
            result += "\\x";
            result += "0123456789abcdef"[c >> 4];
            result += "0123456789abcdef"[c & 0xf];
        }
    }
    result += "</root>";
    return result;
}

static std::string generate_xpath_from_input(const uint8_t *data, size_t size) {
    std::string result;
    result.reserve(size + 10);
    result += "//";
    
    for (size_t i = 0; i < size; ++i) {
        unsigned char c = data[i];
        if (c >= 32 && c < 127 && c != '/' && c != '[' && c != ']' && c != '*' && c != '.' && c != '@') {
            result += static_cast<char>(c);
        } else {
            if (c == 0) continue;
            result += "a";
        }
    }
    if (result.length() < 3) {
        result = "//node";
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    size_t xml_size = (Size / 3);
    size_t xpath_size = (Size / 3);
    
    std::string xml_content = generate_xml_from_input(Data, xml_size);
    std::string xpath_str = generate_xpath_from_input(Data + xml_size, xpath_size);
    
    pugi::xml_document doc;
    if (!doc.load_string(xml_content.c_str())) {
        return 0;
    }
    
    pugi::xml_node root = doc.root();
    
    pugi::xpath_variable_set vars;
    
    // Wrap query construction in try-catch to handle invalid XPath syntax gracefully
    // pugixml throws pugi::xpath_exception, which inherits from std::exception
    pugi::xpath_query query;
    bool query_valid = false;
    try {
        query = pugi::xpath_query(xpath_str.c_str(), &vars);
        query_valid = true;
    } catch (const std::exception&) {
        // Query compilation failed, proceed with string-based calls only or skip query-based calls
        query_valid = false;
    }
    
    // 1. select_node (string overload)
    pugi::xpath_node node1;
    try {
        node1 = root.select_node(xpath_str.c_str(), &vars);
    } catch (...) {
        // Ignore exceptions
    }
    
    // 2. select_node (query overload)
    pugi::xpath_node node2;
    if (query_valid) {
        try {
            node2 = root.select_node(query);
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // 3. xpath_node::node
    pugi::xml_node xml_node1 = node1.node();
    
    // 4. xpath_node::node
    pugi::xml_node xml_node2 = node2.node();
    
    // 5. select_node (string overload) on result node if valid
    pugi::xpath_node node3;
    try {
        if (xml_node1) {
            node3 = xml_node1.select_node(xpath_str.c_str(), &vars);
        } else {
            node3 = root.select_node(xpath_str.c_str(), &vars);
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // 6. select_node (query overload) on result node if valid
    pugi::xpath_node node4;
    if (query_valid) {
        try {
            if (xml_node2) {
                node4 = xml_node2.select_node(query);
            } else {
                node4 = root.select_node(query);
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // 7. select_node (string overload) again
    pugi::xpath_node node5;
    try {
        if (node3.node()) {
            node5 = node3.node().select_node(xpath_str.c_str(), &vars);
        } else {
            node5 = root.select_node(xpath_str.c_str(), &vars);
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // 8. xpath_node::node
    pugi::xml_node xml_node3 = node3.node();
    
    // 9. xpath_node::node
    pugi::xml_node xml_node4 = node4.node();
    
    // Cleanup is handled by RAII
    (void)xml_node1;
    (void)xml_node2;
    (void)xml_node3;
    (void)xml_node4;
    (void)node1;
    (void)node2;
    (void)node3;
    (void)node4;
    (void)node5;
    (void)query;
    (void)vars;

    return 0;
}