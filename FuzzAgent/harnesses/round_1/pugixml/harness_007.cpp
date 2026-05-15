#include "pugixml.hpp"
#include "fuzzer/FuzzedDataProvider.h"
#include <cstdint>
#include <string>
#include <vector>
#include <cstring>

namespace harness_007 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for XPath testing with variables
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_variables = fdp.ConsumeBool();
    size_t max_xpath_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    size_t max_xml_len = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
    
    // Consume XPath expressions - generate multiple for variety
    std::vector<std::string> xpath_expressions;
    size_t num_expressions = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    for (size_t i = 0; i < num_expressions; ++i) {
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(max_xpath_len));
    }
    
    // Consume XML data
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(
        fdp.ConsumeIntegralInRange<size_t>(0, max_xml_len));
    
    // Load XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(
        xml_buffer.data(), xml_buffer.size(), 
        pugi::parse_full | pugi::parse_ws_pcdata);
    
    // Create xpath_variable_set if requested
    pugi::xpath_variable_set vars;
    if (use_variables) {
        size_t var_count = fdp.ConsumeIntegralInRange<size_t>(0, 8);
        for (size_t i = 0; i < var_count; ++i) {
            std::string var_name = fdp.ConsumeRandomLengthString(32);
            
            // Determine variable type
            pugi::xpath_value_type value_type = static_cast<pugi::xpath_value_type>(
                fdp.ConsumeIntegralInRange<int>(pugi::xpath_type_node_set, 
                                              pugi::xpath_type_boolean));
            
            // Add variable with appropriate value based on type
            switch (value_type) {
                case pugi::xpath_type_boolean:
                    vars.add(var_name.c_str(), value_type);
                    vars.set(var_name.c_str(), fdp.ConsumeBool());
                    break;
                    
                case pugi::xpath_type_number:
                    vars.add(var_name.c_str(), value_type);
                    vars.set(var_name.c_str(), fdp.ConsumeFloatingPoint<double>());
                    break;
                    
                case pugi::xpath_type_string: {
                    vars.add(var_name.c_str(), value_type);
                    std::string str_val = fdp.ConsumeRandomLengthString(128);
                    vars.set(var_name.c_str(), str_val.c_str());
                    break;
                }
                    
                case pugi::xpath_type_node_set:
                    // Node set variables are complex - skip for simplicity
                    break;
                    
                default:
                    break;
            }
        }
    }
    
#ifndef PUGIXML_NO_EXCEPTIONS
    try
#endif
    {
        // Get root node for XPath evaluation
        pugi::xml_node root = doc.root();
        pugi::xpath_node xroot(root);
        
        // Create xpath_query objects
        std::vector<pugi::xpath_query> queries;
        for (const auto& expr : xpath_expressions) {
            queries.emplace_back(expr.c_str(), use_variables ? &vars : nullptr);
        }
        
        // Execute different test modes to target various XPath evaluation paths
        switch (test_mode) {
            case 0: {
                // Comprehensive evaluation of all five target functions for each query
                for (const auto& query : queries) {
                    if (!query) continue;
                    
                    // 1. xpath_query::evaluate_node - target: 30 undiscovered branches
                    pugi::xpath_node eval_node_result = query.evaluate_node(xroot);
                    
                    // 2. xpath_query::evaluate_boolean - target: 30 undiscovered branches
                    bool eval_bool_result = query.evaluate_boolean(xroot);
                    
                    // 3. xpath_query::evaluate_string - target: 30 undiscovered branches
#ifndef PUGIXML_NO_STL
                    std::string eval_string_result = query.evaluate_string(xroot);
#endif
                    
                    // 4. xpath_query::evaluate_number - target: 30 undiscovered branches
                    double eval_number_result = query.evaluate_number(xroot);
                    
                    // 5. xpath_query::evaluate_node_set - target: 30 undiscovered branches
                    pugi::xpath_node_set eval_nodeset_result = query.evaluate_node_set(xroot);
                    
                    // Also test select_nodes/select_node APIs as specified in guidance
                    pugi::xpath_node_set selected_nodes = root.select_nodes(query);
                    pugi::xpath_node selected_node = root.select_node(query);
                }
                break;
            }
            
            case 1: {
                // Focus on evaluate_node and evaluate_node_set with different contexts
                for (const auto& query : queries) {
                    if (!query) continue;
                    
                    // Test evaluate_node with different xpath_node contexts
                    pugi::xpath_node eval_node_root = query.evaluate_node(xroot);
                    
                    // Test with child nodes if available
                    for (pugi::xml_node child : root.children()) {
                        pugi::xpath_node xchild(child);
                        pugi::xpath_node eval_node_child = query.evaluate_node(xchild);
                        pugi::xpath_node_set eval_nodeset_child = query.evaluate_node_set(xchild);
                    }
                    
                    // Test evaluate_node_set on root
                    pugi::xpath_node_set eval_nodeset_root = query.evaluate_node_set(xroot);
                    
                    // Iterate through node set results
                    for (pugi::xpath_node_set::const_iterator it = eval_nodeset_root.begin();
                         it != eval_nodeset_root.end(); ++it) {
                        // Use the node to potentially trigger more code paths
                    }
                }
                break;
            }
            
            case 2: {
                // Focus on evaluate_boolean, evaluate_number, evaluate_string combinations
                for (const auto& query : queries) {
                    if (!query) continue;
                    
                    // Test boolean evaluation with different contexts
                    bool bool_root = query.evaluate_boolean(xroot);
                    
                    // Test number evaluation
                    double number_root = query.evaluate_number(xroot);
                    
                    // Test string evaluation
#ifndef PUGIXML_NO_STL
                    std::string string_root = query.evaluate_string(xroot);
#endif
                    
                    // Test evaluate_string with buffer version
                    char buffer[256];
                    size_t str_len = query.evaluate_string(buffer, sizeof(buffer), xroot);
                    
                    // Test with child nodes
                    for (pugi::xml_node child : root.children()) {
                        pugi::xpath_node xchild(child);
                        bool bool_child = query.evaluate_boolean(xchild);
                        double number_child = query.evaluate_number(xchild);
                    }
                }
                break;
            }
            
            case 3: {
                // Mixed comprehensive testing with variable set variations
                for (size_t i = 0; i < queries.size(); ++i) {
                    if (!queries[i]) continue;
                    
                    // Alternate between different evaluation methods
                    switch (i % 5) {
                        case 0:
                            queries[i].evaluate_node(xroot);
                            break;
                        case 1:
                            queries[i].evaluate_boolean(xroot);
                            break;
                        case 2:
                            queries[i].evaluate_number(xroot);
                            break;
                        case 3:
#ifndef PUGIXML_NO_STL
                            queries[i].evaluate_string(xroot);
#endif
                            break;
                        case 4:
                            queries[i].evaluate_node_set(xroot);
                            break;
                    }
                    
                    // Also test select_nodes and select_node as per guidance
                    if (i % 2 == 0) {
                        root.select_nodes(queries[i]);
                    } else {
                        root.select_node(queries[i]);
                    }
                }
                break;
            }
        }
        
        // Cleanup as specified in guidance
        doc.reset();
    }
#ifndef PUGIXML_NO_EXCEPTIONS
    catch (const pugi::xpath_exception&) {
        // XPath exceptions are expected with malformed queries
    }
    catch (const std::bad_alloc&) {
        // Memory allocation failures are also possible
    }
#endif
    
    return 0;
}

} // namespace harness_007
