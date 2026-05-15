#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

namespace harness_005 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for complex XPath variable operations
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Test XPath variable set functionality and advanced node selection
    #ifndef PUGIXML_NO_XPATH
    try {
        // First, create an XML document with complex structure for testing
        pugi::xml_document doc;
        pugi::xml_node root = doc.append_child("root");
        
        // Create a complex document structure with various node types
        size_t node_count = fdp.ConsumeIntegralInRange<size_t>(1, 20);
        std::vector<pugi::xml_node> created_nodes;
        
        for (size_t i = 0; i < node_count && fdp.remaining_bytes() > 20; ++i) {
            std::string node_name = "node_" + std::to_string(i);
            pugi::xml_node node = root.append_child(node_name.c_str());
            
            // Add attributes with fuzzed names/values
            size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
            for (size_t j = 0; j < attr_count && fdp.remaining_bytes() > 10; ++j) {
                std::string attr_name = "attr_" + std::to_string(j);
                std::string attr_value = fdp.ConsumeRandomLengthString(32);
                node.append_attribute(attr_name.c_str()) = attr_value.c_str();
            }
            
            // Add text content
            std::string text_content = fdp.ConsumeRandomLengthString(128);
            node.append_child(pugi::node_pcdata).set_value(text_content.c_str());
            
            created_nodes.push_back(node);
        }
        
        // Add some special nodes for advanced selection
        if (fdp.ConsumeBool()) {
            pugi::xml_node comment_node = root.append_child(pugi::node_comment);
            comment_node.set_value(fdp.ConsumeRandomLengthString(100).c_str());
        }
        
        if (fdp.ConsumeBool()) {
            pugi::xml_node cdata_node = root.append_child(pugi::node_cdata);
            cdata_node.set_value(fdp.ConsumeRandomLengthString(150).c_str());
        }
        
        // Create multiple xpath_variable_sets for testing
        size_t var_set_count = fdp.ConsumeIntegralInRange<size_t>(1, 3);
        std::vector<pugi::xpath_variable_set> var_sets(var_set_count);
        std::vector<std::vector<std::string>> var_names(var_set_count);
        
        // Populate variable sets with different types of variables
        for (size_t set_idx = 0; set_idx < var_set_count && fdp.remaining_bytes() > 30; ++set_idx) {
            size_t var_count = fdp.ConsumeIntegralInRange<size_t>(1, 8);
            
            for (size_t var_idx = 0; var_idx < var_count && fdp.remaining_bytes() > 20; ++var_idx) {
                std::string var_name = "var" + std::to_string(set_idx) + "_" + std::to_string(var_idx);
                var_names[set_idx].push_back(var_name);
                
                // Choose variable type from fuzzer input
                int var_type_int = fdp.ConsumeIntegralInRange(0, static_cast<int>(pugi::xpath_type_node_set));
                pugi::xpath_value_type var_type = static_cast<pugi::xpath_value_type>(var_type_int);
                
                // Add variable to set
                pugi::xpath_variable* var = var_sets[set_idx].add(var_name.c_str(), var_type);
                
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
                            if (!created_nodes.empty()) {
                                size_t nodes_to_add = fdp.ConsumeIntegralInRange<size_t>(1, 
                                    std::min<size_t>(created_nodes.size(), 5));
                                
                                // Create array of xpath_node objects
                                std::vector<pugi::xpath_node> node_array;
                                for (size_t n = 0; n < nodes_to_add; ++n) {
                                    size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, created_nodes.size() - 1);
                                    node_array.push_back(pugi::xpath_node(created_nodes[node_idx]));
                                }
                                
                                // Create xpath_node_set from iterator range
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
        }
        
        // Consume XPath expressions for testing
        size_t xpath_count = fdp.ConsumeIntegralInRange<size_t>(1, 5);
        std::vector<std::string> xpath_expressions;
        
        for (size_t i = 0; i < xpath_count && fdp.remaining_bytes() > 10; ++i) {
            std::string xpath_expr = fdp.ConsumeRandomLengthString(512);
            if (!xpath_expr.empty()) {
                xpath_expressions.push_back(xpath_expr);
            }
        }
        
        // Test advanced XPath queries with variable sets
        for (size_t expr_idx = 0; expr_idx < xpath_expressions.size(); ++expr_idx) {
            // Try each expression with different variable sets
            for (size_t set_idx = 0; set_idx < var_set_count; ++set_idx) {
                try {
                    // Create query with variable set
                    pugi::xpath_query query(xpath_expressions[expr_idx].c_str(), &var_sets[set_idx]);
                    
                    if (query) {
                        // Test all evaluation methods
                        query.evaluate_boolean(doc);
                        query.evaluate_number(doc);
                        query.evaluate_string(doc);
                        query.evaluate_node(doc);
                        
                        // Test node set evaluation
                        pugi::xpath_node_set result_set = query.evaluate_node_set(doc);
                        
                        // Test advanced node set operations
                        if (!result_set.empty()) {
                            // Test sorting
                            result_set.sort(false);  // ascending
                            result_set.sort(true);   // descending
                            
                            // Test indexing and iteration
                            for (size_t i = 0; i < result_set.size() && i < 10; ++i) {
                                pugi::xpath_node xnode = result_set[i];
                                if (xnode) {
                                    pugi::xml_node node = xnode.node();
                                    pugi::xml_attribute attr = xnode.attribute();
                                    
                                    if (node) {
                                        node.name();
                                        node.value();
                                        
                                        // Test advanced node selection methods
                                        pugi::xml_node first_child = node.first_child();
                                        pugi::xml_node last_child = node.last_child();
                                        pugi::xml_node next_sibling = node.next_sibling();
                                        pugi::xml_node previous_sibling = node.previous_sibling();
                                        
                                        // Test child/attribute finding with complex criteria
                                        if (fdp.remaining_bytes() > 10) {
                                            std::string child_name = fdp.ConsumeRandomLengthString(32);
                                            node.child(child_name.c_str());
                                            
                                            std::string attr_name = fdp.ConsumeRandomLengthString(32);
                                            node.attribute(attr_name.c_str());
                                        }
                                    }
                                    
                                    if (attr) {
                                        attr.name();
                                        attr.value();
                                    }
                                }
                            }
                        }
                        
                        // Test document select methods with variable sets
                        pugi::xpath_node selected_node = doc.select_node(query);
                        pugi::xpath_node_set selected_nodes = doc.select_nodes(query);
                        
                        // Test with string expression and variable set
                        if (fdp.remaining_bytes() > 20) {
                            std::string alt_xpath = fdp.ConsumeRandomLengthString(256);
                            if (!alt_xpath.empty()) {
                                doc.select_node(alt_xpath.c_str(), &var_sets[set_idx]);
                                doc.select_nodes(alt_xpath.c_str(), &var_sets[set_idx]);
                            }
                        }
                    }
                } catch (pugi::xpath_exception&) {
                    // Expected for malformed expressions
                } catch (...) {
                    // Catch any other exceptions
                }
            }
        }
        
        // Test variable set operations in depth
        for (size_t set_idx = 0; set_idx < var_set_count; ++set_idx) {
            // Test variable retrieval and manipulation
            for (const std::string& var_name : var_names[set_idx]) {
                pugi::xpath_variable* var = var_sets[set_idx].get(var_name.c_str());
                const pugi::xpath_variable* const_var = var_sets[set_idx].get(var_name.c_str());
                
                if (var && const_var) {
                    // Test type checking
                    pugi::xpath_value_type var_type = var->type();
                    
                    // Test value retrieval based on type
                    switch (var_type) {
                        case pugi::xpath_type_boolean:
                            var->get_boolean();
                            break;
                        case pugi::xpath_type_number:
                            var->get_number();
                            break;
                        case pugi::xpath_type_string:
                            var->get_string();
                            break;
                        case pugi::xpath_type_node_set:
                            var->get_node_set();
                            break;
                    }
                    
                    // Test variable setting with different values
                    if (fdp.remaining_bytes() > 20) {
                        switch (var_type) {
                            case pugi::xpath_type_boolean:
                                var->set(fdp.ConsumeBool());
                                break;
                            case pugi::xpath_type_number:
                                var->set(fdp.ConsumeFloatingPoint<double>());
                                break;
                            case pugi::xpath_type_string: {
                                std::string new_str = fdp.ConsumeRandomLengthString(128);
                                var->set(new_str.c_str());
                                break;
                            }
                            case pugi::xpath_type_node_set: {
                                if (!created_nodes.empty() && fdp.remaining_bytes() > 10) {
                                    size_t nodes_to_add = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                                    std::vector<pugi::xpath_node> node_array;
                                    for (size_t n = 0; n < nodes_to_add; ++n) {
                                        size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, created_nodes.size() - 1);
                                        node_array.push_back(pugi::xpath_node(created_nodes[node_idx]));
                                    }
                                    if (!node_array.empty()) {
                                        pugi::xpath_node_set new_set(&node_array[0], &node_array[0] + node_array.size());
                                        var->set(new_set);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
            
            // Test variable set copying and assignment
            if (set_idx + 1 < var_set_count) {
                // Test copy construction
                pugi::xpath_variable_set copied_set(var_sets[set_idx]);
                
                // Test assignment
                pugi::xpath_variable_set assigned_set;
                assigned_set = var_sets[set_idx];
                
                // Test move semantics if available
                #ifdef PUGIXML_HAS_MOVE
                pugi::xpath_variable_set moved_set(std::move(var_sets[set_idx]));
                var_sets[set_idx] = std::move(moved_set);
                #endif
            }
        }
        
        // Test complex XPath expressions with multiple variables
        if (fdp.remaining_bytes() > 100) {
            std::string complex_xpath = fdp.ConsumeRemainingBytesAsString();
            if (!complex_xpath.empty()) {
                // Create a new variable set specifically for complex testing
                pugi::xpath_variable_set complex_vars;
                
                // Add variables with all types
                complex_vars.add("bool_var", pugi::xpath_type_boolean);
                complex_vars.add("num_var", pugi::xpath_type_number);
                complex_vars.add("str_var", pugi::xpath_type_string);
                complex_vars.add("nodeset_var", pugi::xpath_type_node_set);
                
                // Set values
                complex_vars.set("bool_var", fdp.ConsumeBool());
                complex_vars.set("num_var", fdp.ConsumeFloatingPoint<double>());
                complex_vars.set("str_var", fdp.ConsumeRandomLengthString(100).c_str());
                
                // Create a node set
                pugi::xpath_node_set complex_node_set;
                if (!created_nodes.empty()) {
                    std::vector<pugi::xpath_node> node_array;
                    for (size_t i = 0; i < std::min<size_t>(created_nodes.size(), 3); ++i) {
                        node_array.push_back(pugi::xpath_node(created_nodes[i]));
                    }
                    if (!node_array.empty()) {
                        complex_node_set = pugi::xpath_node_set(&node_array[0], &node_array[0] + node_array.size());
                    }
                }
                if (!complex_node_set.empty()) {
                    complex_vars.set("nodeset_var", complex_node_set);
                }
                // Try to create and evaluate query
                try {
                    pugi::xpath_query complex_query(complex_xpath.c_str(), &complex_vars);
                    if (complex_query) {
                        complex_query.evaluate_boolean(doc);
                        complex_query.evaluate_node_set(doc);
                    }
                } catch (...) {
                    // Expected to fail with random input
                }
            }
        }
        
        // Test error conditions and edge cases
        // Test with null variable pointers
        try {
            std::string test_xpath = "//node";
            pugi::xpath_query null_var_query(test_xpath.c_str(), nullptr);
            if (null_var_query) {
                null_var_query.evaluate_boolean(doc);
            }
        } catch (...) {
            // Handle exceptions
        }
        
        // Test variable lookup with non-existent names
        if (!var_sets.empty()) {
            std::string non_existent_var = "non_existent_var_12345";
            pugi::xpath_variable* missing_var = var_sets[0].get(non_existent_var.c_str());
            // missing_var should be nullptr
        }
        
        // Test adding duplicate variables with different types (should fail)
        if (!var_names.empty() && !var_names[0].empty()) {
            std::string existing_var_name = var_names[0][0];
            pugi::xpath_variable* dup_var = var_sets[0].add(existing_var_name.c_str(), 
                pugi::xpath_type_boolean);
            // dup_var should be nullptr since variable already exists
        }
        
    } catch (pugi::xpath_exception&) {
        // XPath parsing or evaluation errors are expected
    } catch (...) {
        // Catch any other exceptions
    }
    #endif // PUGIXML_NO_XPATH
    
    return 0;
}

} // namespace harness_005
