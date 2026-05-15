#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

namespace harness_006 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for complex XML structure and XPath operations
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Test the three uncovered xml_node methods: select_single_node, select_nodes, first_element_by_path
    #ifndef PUGIXML_NO_XPATH
    try {
        // Create a complex XML document with nested structure for path-based selection
        pugi::xml_document doc;
        pugi::xml_node root = doc.append_child("root");
        
        // Create a deep hierarchical structure for path testing
        size_t depth = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        std::vector<pugi::xml_node> node_path;
        pugi::xml_node current = root;
        
        // Build hierarchical structure
        for (size_t i = 0; i < depth && fdp.remaining_bytes() > 20; ++i) {
            std::string node_name = "level" + std::to_string(i);
            current = current.append_child(node_name.c_str());
            
            // Add attributes with fuzzed values
            size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
            for (size_t j = 0; j < attr_count && fdp.remaining_bytes() > 10; ++j) {
                std::string attr_name = "attr" + std::to_string(j);
                std::string attr_value = fdp.ConsumeRandomLengthString(64);
                current.append_attribute(attr_name.c_str()) = attr_value.c_str();
            }
            
            // Add text content
            std::string text_content = fdp.ConsumeRandomLengthString(128);
            current.append_child(pugi::node_pcdata).set_value(text_content.c_str());
            
            node_path.push_back(current);
            
            // Add siblings at each level
            size_t sibling_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
            for (size_t s = 0; s < sibling_count && fdp.remaining_bytes() > 15; ++s) {
                std::string sibling_name = "sibling" + std::to_string(s);
                pugi::xml_node sibling = node_path.back().parent().append_child(sibling_name.c_str());
                
                std::string sibling_attr = fdp.ConsumeRandomLengthString(32);
                sibling.append_attribute("id") = sibling_attr.c_str();
            }
        }
        
        // Add some elements with special names for path matching
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 30) {
            std::string special_name = fdp.ConsumeRandomLengthString(50);
            if (!special_name.empty()) {
                pugi::xml_node special = root.append_child(special_name.c_str());
                special.append_attribute("type") = "special";
            }
        }
        
        // Add elements with numeric names for edge cases
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
            std::string num_name = std::to_string(fdp.ConsumeIntegral<uint32_t>());
            pugi::xml_node num_node = root.append_child(num_name.c_str());
            num_node.append_attribute("value") = "numeric";
        }
        
        // Create xpath_variable_set for testing
        pugi::xpath_variable_set vars;
        size_t var_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
        std::vector<std::string> var_names;
        
        for (size_t i = 0; i < var_count && fdp.remaining_bytes() > 25; ++i) {
            std::string var_name = "var" + std::to_string(i);
            var_names.push_back(var_name);
            
            // Choose variable type from fuzzer input
            int var_type_int = fdp.ConsumeIntegralInRange(0, static_cast<int>(pugi::xpath_type_node_set));
            pugi::xpath_value_type var_type = static_cast<pugi::xpath_value_type>(var_type_int);
            
            pugi::xpath_variable* var = vars.add(var_name.c_str(), var_type);
            
            if (var) {
                // Set variable value based on type
                switch (var_type) {
                    case pugi::xpath_type_boolean: {
                        bool bool_val = fdp.ConsumeBool();
                        var->set(bool_val);
                        break;
                    }
                    case pugi::xpath_type_number: {
                        double num_val = fdp.ConsumeFloatingPoint<double>();
                        var->set(num_val);
                        break;
                    }
                    case pugi::xpath_type_string: {
                        std::string str_val = fdp.ConsumeRandomLengthString(256);
                        var->set(str_val.c_str());
                        break;
                    }
                    case pugi::xpath_type_node_set: {
                        // Create a node set from our document
                        if (!node_path.empty()) {
                            size_t nodes_to_add = fdp.ConsumeIntegralInRange<size_t>(1, 
                                std::min<size_t>(node_path.size(), 5));
                            
                            std::vector<pugi::xpath_node> node_array;
                            for (size_t n = 0; n < nodes_to_add; ++n) {
                                size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_path.size() - 1);
                                node_array.push_back(pugi::xpath_node(node_path[node_idx]));
                            }
                            
                            if (!node_array.empty()) {
                                pugi::xpath_node_set node_set(&node_array[0], &node_array[0] + node_array.size());
                                var->set(node_set);
                            }
                        }
                        break;
                    }
                }
            }
        }
        
        // Consume XPath expressions for select_single_node and select_nodes testing
        size_t xpath_count = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        std::vector<std::string> xpath_expressions;
        
        for (size_t i = 0; i < xpath_count && fdp.remaining_bytes() > 10; ++i) {
            std::string xpath_expr = fdp.ConsumeRandomLengthString(512);
            if (!xpath_expr.empty()) {
                xpath_expressions.push_back(xpath_expr);
            }
        }
        
        // Consume path strings for first_element_by_path testing
        size_t path_count = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        std::vector<std::string> path_strings;
        
        for (size_t i = 0; i < path_count && fdp.remaining_bytes() > 10; ++i) {
            std::string path_str = fdp.ConsumeRandomLengthString(256);
            if (!path_str.empty()) {
                path_strings.push_back(path_str);
            }
        }
        
        // Test select_single_node (deprecated) - both overloads
        for (size_t i = 0; i < xpath_expressions.size() && i < 5; ++i) {
            try {
                // Test with string query and variable set
                pugi::xpath_node single_node1 = root.select_single_node(xpath_expressions[i].c_str(), &vars);
                
                // Test with xpath_query object
                pugi::xpath_query query(xpath_expressions[i].c_str(), &vars);
                if (query) {
                    pugi::xpath_node single_node2 = root.select_single_node(query);
                    
                    // If node found, test operations on it
                    if (single_node2) {
                        pugi::xml_node found_node = single_node2.node();
                        if (found_node) {
                            // Test the found node's properties
                            found_node.name();
                            found_node.value();
                            found_node.first_attribute();
                            found_node.first_child();
                        }
                    }
                }
            } catch (...) {
                // Expected to fail for invalid XPath expressions
            }
        }
        
        // Test select_nodes - both overloads
        for (size_t i = 0; i < xpath_expressions.size() && i < 5; ++i) {
            try {
                // Test with string query and variable set
                pugi::xpath_node_set nodes1 = root.select_nodes(xpath_expressions[i].c_str(), &vars);
                
                // Test operations on the result set
                if (!nodes1.empty()) {
                    size_t set_size = nodes1.size();
                    nodes1.sort(false);  // ascending
                    nodes1.sort(true);   // descending
                    
                    // Test iteration
                    for (size_t j = 0; j < nodes1.size() && j < 10; ++j) {
                        pugi::xpath_node xnode = nodes1[j];
                        if (xnode) {
                            pugi::xml_node node = xnode.node();
                            if (node) {
                                node.name();
                                node.value();
                            }
                        }
                    }
                }
                
                // Test with xpath_query object
                pugi::xpath_query query(xpath_expressions[i].c_str(), &vars);
                if (query) {
                    pugi::xpath_node_set nodes2 = root.select_nodes(query);
                    
                    // Test type and first node
                    if (!nodes2.empty()) {
                        pugi::xpath_node_set::type_t set_type = nodes2.type();
                        pugi::xpath_node first_node = nodes2.first();
                    }
                }
            } catch (...) {
                // Expected to fail for invalid XPath expressions
            }
        }
        
        // Test first_element_by_path with various delimiters
        for (size_t i = 0; i < path_strings.size() && i < 5; ++i) {
            // Test with default delimiter '/'
            pugi::xml_node path_node1 = root.first_element_by_path(path_strings[i].c_str());
            
            if (path_node1) {
                // Test operations on found node
                path_node1.name();
                path_node1.value();
                path_node1.first_attribute();
                
                // Try to navigate from found node
                pugi::xml_node parent = path_node1.parent();
                pugi::xml_node first_child = path_node1.first_child();
            }
            
            // Test with alternative delimiter '.'
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                pugi::xml_node path_node2 = root.first_element_by_path(path_strings[i].c_str(), '.');
                
                // Also test with '|' delimiter
                pugi::xml_node path_node3 = root.first_element_by_path(path_strings[i].c_str(), '|');
            }
        }
        
        // Test combination: use first_element_by_path then select_nodes on the result
        if (!path_strings.empty() && !xpath_expressions.empty() && fdp.remaining_bytes() > 20) {
            size_t path_idx = fdp.ConsumeIntegralInRange<size_t>(0, path_strings.size() - 1);
            size_t xpath_idx = fdp.ConsumeIntegralInRange<size_t>(0, xpath_expressions.size() - 1);
            
            pugi::xml_node path_node = root.first_element_by_path(path_strings[path_idx].c_str());
            if (path_node) {
                // Test select_nodes on the found node
                try {
                    pugi::xpath_node_set nested_nodes = path_node.select_nodes(xpath_expressions[xpath_idx].c_str(), &vars);
                    
                    // Test select_single_node on the found node
                    pugi::xpath_node nested_single = path_node.select_single_node(xpath_expressions[xpath_idx].c_str(), &vars);
                } catch (...) {
                    // Expected to fail sometimes
                }
            }
        }
        
        // Test edge cases with empty and malformed inputs
        if (fdp.remaining_bytes() > 50) {
            std::string empty_xpath = "";
            std::string empty_path = "";
            
            // Should handle empty strings gracefully
            root.select_single_node(empty_xpath.c_str(), &vars);
            root.select_nodes(empty_xpath.c_str(), &vars);
            root.first_element_by_path(empty_path.c_str());
            
            // Test with very long strings
            std::string long_xpath = fdp.ConsumeRandomLengthString(1024);
            std::string long_path = fdp.ConsumeRandomLengthString(1024);
            
            try {
                root.select_single_node(long_xpath.c_str(), &vars);
                root.select_nodes(long_xpath.c_str(), &vars);
                root.first_element_by_path(long_path.c_str());
            } catch (...) {
                // Expected to fail for very complex/invalid inputs
            }
        }
        
        // Test with different starting nodes (not just root)
        if (!node_path.empty() && fdp.remaining_bytes() > 30) {
            size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_path.size() - 1);
            pugi::xml_node start_node = node_path[node_idx];
            
            if (!xpath_expressions.empty()) {
                size_t expr_idx = fdp.ConsumeIntegralInRange<size_t>(0, xpath_expressions.size() - 1);
                
                try {
                    pugi::xpath_node_set nodes_from_mid = start_node.select_nodes(xpath_expressions[expr_idx].c_str(), &vars);
                    pugi::xpath_node single_from_mid = start_node.select_single_node(xpath_expressions[expr_idx].c_str(), &vars);
                } catch (...) {
                    // Expected to fail sometimes
                }
            }
            
            if (!path_strings.empty()) {
                size_t path_idx = fdp.ConsumeIntegralInRange<size_t>(0, path_strings.size() - 1);
                pugi::xml_node path_from_mid = start_node.first_element_by_path(path_strings[path_idx].c_str());
            }
        }
        
        // Test variable set operations on the results
        if (!var_names.empty() && fdp.remaining_bytes() > 20) {
            size_t var_idx = fdp.ConsumeIntegralInRange<size_t>(0, var_names.size() - 1);
            pugi::xpath_variable* test_var = vars.get(var_names[var_idx].c_str());
            
            if (test_var) {
                pugi::xpath_value_type var_type = test_var->type();
                
                // Test getting variable values
                switch (var_type) {
                    case pugi::xpath_type_boolean:
                        test_var->get_boolean();
                        break;
                    case pugi::xpath_type_number:
                        test_var->get_number();
                        break;
                    case pugi::xpath_type_string:
                        test_var->get_string();
                        break;
                    case pugi::xpath_type_node_set:
                        test_var->get_node_set();
                        break;
                }
            }
        }
        
    } catch (...) {
        // Catch any unexpected exceptions
    }
    #endif
    
    return 0;
}

} // namespace harness_006
