// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
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
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* Data, size_t Size) {
    // Write input to dummy file for potential file-based parsing
    writeDummyFile(Data, Size);
    
    // Parse XML document from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        return 0;
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }
    
    // First child call
    std::string_view child_name1;
    if (Size > 0) {
        // Use first byte as length for child name (modulo 64 to keep reasonable)
        size_t name_len = (Data[0] % 64) + 1;
        if (name_len > Size - 1) name_len = Size - 1;
        if (name_len > 0) {
            char* name_buf = new char[name_len + 1];
            memcpy(name_buf, Data + 1, name_len);
            name_buf[name_len] = '\0';
            child_name1 = std::string_view(name_buf, name_len);
            
            pugi::xml_node child1 = root.child(child_name1.data());
            
            // Attribute call
            std::string_view attr_name;
            if (Size > name_len + 1) {
                size_t attr_len = (Data[name_len + 1] % 64) + 1;
                if (attr_len > Size - name_len - 2) attr_len = Size - name_len - 2;
                if (attr_len > 0) {
                    char* attr_buf = new char[attr_len + 1];
                    memcpy(attr_buf, Data + name_len + 2, attr_len);
                    attr_buf[attr_len] = '\0';
                    attr_name = std::string_view(attr_buf, attr_len);
                    
                    pugi::xml_attribute attr = child1.attribute(attr_name.data());
                    
                    // Second child call
                    std::string_view child_name2;
                    if (Size > name_len + attr_len + 2) {
                        size_t child2_len = (Data[name_len + attr_len + 2] % 64) + 1;
                        if (child2_len > Size - name_len - attr_len - 3) child2_len = Size - name_len - attr_len - 3;
                        if (child2_len > 0) {
                            char* child2_buf = new char[child2_len + 1];
                            memcpy(child2_buf, Data + name_len + attr_len + 3, child2_len);
                            child2_buf[child2_len] = '\0';
                            child_name2 = std::string_view(child2_buf, child2_len);
                            
                            pugi::xml_node child2 = child1.child(child_name2.data());
                            
                            // Create XPath query
                            std::string xpath_str;
                            size_t xpath_start = name_len + attr_len + child2_len + 3;
                            if (xpath_start < Size) {
                                size_t xpath_len = Size - xpath_start;
                                if (xpath_len > 0) {
                                    xpath_str.assign(reinterpret_cast<const char*>(Data + xpath_start), xpath_len);
                                    // Ensure null termination
                                    xpath_str.push_back('\0');
                                }
                            }
                            
                            if (!xpath_str.empty()) {
                                try {
                                    pugi::xpath_query query(xpath_str.c_str());
                                    pugi::xpath_node context(child1);
                                    
                                    // Evaluate functions in specified order
                                    bool bool_result = query.evaluate_boolean(context);
                                    (void)bool_result;
                                    
                                    double num_result = query.evaluate_number(context);
                                    (void)num_result;
                                    
                                    // First evaluate_string overload
                                    pugi::string_t str_result = query.evaluate_string(context);
                                    (void)str_result;
                                    
                                    // Second evaluate_string overload
                                    char buffer[256];
                                    size_t str_len = query.evaluate_string(buffer, sizeof(buffer), context);
                                    (void)str_len;
                                    
                                    pugi::xpath_node_set node_set = query.evaluate_node_set(context);
                                    (void)node_set;
                                    
                                    pugi::xpath_node node_result = query.evaluate_node(context);
                                    (void)node_result;
                                    
                                } catch (...) {
                                    // Catch any exceptions from XPath evaluation
                                }
                            }
                            
                            delete[] child2_buf;
                        }
                    }
                    
                    delete[] attr_buf;
                }
            }
            
            delete[] name_buf;
        }
    }
    
    return 0;
}