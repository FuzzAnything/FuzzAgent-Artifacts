#include "pugixml.hpp"
#include "fuzzer/FuzzedDataProvider.h"
#include <cstdint>
#include <string>
#include <vector>

namespace harness_006 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for basic operation
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool use_variable_set = fdp.ConsumeBool();
    size_t max_xpath_length = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    size_t max_xml_length = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    
    // Consume XPath expression(s)
    std::string xpath_expr1 = fdp.ConsumeRandomLengthString(max_xpath_length);
    std::string xpath_expr2 = fdp.ConsumeRandomLengthString(max_xpath_length);
    std::string xpath_expr3 = fdp.ConsumeRandomLengthString(max_xpath_length);
    
    // Consume XML data
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(
        fdp.ConsumeIntegralInRange<size_t>(0, max_xml_length));
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(
        xml_buffer.data(), xml_buffer.size(), pugi::parse_full | pugi::parse_ws_pcdata);
    
    // If parsing failed, we can still test some APIs with empty document
    if (!parse_result) {
        // Still create empty document for testing
        doc.reset();
    }
    
    // Create variable set if requested
    pugi::xpath_variable_set vars;
    if (use_variable_set) {
        size_t var_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
        for (size_t i = 0; i < var_count; ++i) {
            std::string var_name = fdp.ConsumeRandomLengthString(32);
            pugi::xpath_value_type value_type = static_cast<pugi::xpath_value_type>(
                fdp.ConsumeIntegralInRange<int>(0, pugi::xpath_type_node_set + 1));
            
            // Add variable with dummy value based on type
            switch (value_type) {
                case pugi::xpath_type_boolean:
                    vars.add(var_name.c_str(), value_type);
                    vars.set(var_name.c_str(), fdp.ConsumeBool());
                    break;
                case pugi::xpath_type_number:
                    vars.add(var_name.c_str(), value_type);
                    vars.set(var_name.c_str(), fdp.ConsumeFloatingPoint<double>());
                    break;
                case pugi::xpath_type_string:
                    vars.add(var_name.c_str(), value_type);
                    vars.set(var_name.c_str(), fdp.ConsumeRandomLengthString(128).c_str());
                    break;
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
        // Create xpath_query objects with different expressions
        pugi::xpath_query query1(xpath_expr1.c_str(), use_variable_set ? &vars : nullptr);
        pugi::xpath_query query2(xpath_expr2.c_str(), use_variable_set ? &vars : nullptr);
        pugi::xpath_query query3(xpath_expr3.c_str(), use_variable_set ? &vars : nullptr);
        
        // Get root node for queries
        pugi::xml_node root = doc.root();
        pugi::xpath_node xroot(root);
        
        // Execute different operations based on operation_type
        switch (operation_type) {
            case 0: {
                // Test xml_node::select_single_node(xpath_query const&)
                pugi::xpath_node result1 = root.select_single_node(query1);
                pugi::xpath_node result2 = root.select_single_node(query2);
                // Also test deprecated version
                pugi::xpath_node result3 = root.select_single_node(query3);
                break;
            }
            case 1: {
                // Test xml_node::select_node(char const*, xpath_variable_set*)
                pugi::xpath_node result1 = root.select_node(xpath_expr1.c_str(), 
                                                           use_variable_set ? &vars : nullptr);
                pugi::xpath_node result2 = root.select_node(xpath_expr2.c_str(), 
                                                           use_variable_set ? &vars : nullptr);
                break;
            }
            case 2: {
                // Test xml_node::select_node(xpath_query const&)
                pugi::xpath_node result1 = root.select_node(query1);
                pugi::xpath_node result2 = root.select_node(query2);
                break;
            }
            case 3: {
                // Test xml_node::select_single_node(char const*, xpath_variable_set*)
                pugi::xpath_node result1 = root.select_single_node(xpath_expr1.c_str(), 
                                                                  use_variable_set ? &vars : nullptr);
                pugi::xpath_node result2 = root.select_single_node(xpath_expr2.c_str(), 
                                                                  use_variable_set ? &vars : nullptr);
                break;
            }
            case 4: {
                // Test direct xpath_query evaluation methods
                if (query1) {
                    // xpath_query::evaluate_node(xpath_node const&)
                    pugi::xpath_node eval_node = query1.evaluate_node(xroot);
                    
                    // xpath_query::evaluate_boolean(xpath_node const&)
                    bool eval_bool = query1.evaluate_boolean(xroot);
                    
                    // xpath_query::evaluate_string(xpath_node const&)
                    #ifndef PUGIXML_NO_STL
                    std::string eval_str = query1.evaluate_string(xroot);
                    #endif
                    
                    // xpath_query::evaluate_number(xpath_node const&)
                    double eval_num = query1.evaluate_number(xroot);
                    
                    // xpath_query::evaluate_node_set(xpath_node const&)
                    pugi::xpath_node_set eval_set = query1.evaluate_node_set(xroot);
                }
                break;
            }
            case 5: {
                // Test all evaluate methods on multiple queries
                if (query1 && query2) {
                    bool bool1 = query1.evaluate_boolean(xroot);
                    bool bool2 = query2.evaluate_boolean(xroot);
                    
                    double num1 = query1.evaluate_number(xroot);
                    double num2 = query2.evaluate_number(xroot);
                    
                    #ifndef PUGIXML_NO_STL
                    std::string str1 = query1.evaluate_string(xroot);
                    std::string str2 = query2.evaluate_string(xroot);
                    #endif
                    
                    pugi::xpath_node node1 = query1.evaluate_node(xroot);
                    pugi::xpath_node node2 = query2.evaluate_node(xroot);
                    
                    pugi::xpath_node_set set1 = query1.evaluate_node_set(xroot);
                    pugi::xpath_node_set set2 = query2.evaluate_node_set(xroot);
                }
                break;
            }
            case 6: {
                // Test select_nodes variants
                pugi::xpath_node_set nodes1 = root.select_nodes(xpath_expr1.c_str(), 
                                                               use_variable_set ? &vars : nullptr);
                pugi::xpath_node_set nodes2 = root.select_nodes(query1);
                
                // Also test single node selection with variables
                if (use_variable_set) {
                    pugi::xpath_node single1 = root.select_single_node(xpath_expr2.c_str(), &vars);
                    pugi::xpath_node single2 = root.select_node(xpath_expr3.c_str(), &vars);
                }
                break;
            }
            case 7: {
                // Comprehensive mixed testing
                // Test with variable set
                if (use_variable_set && query1) {
                    pugi::xpath_node result_var = root.select_single_node(xpath_expr1.c_str(), &vars);
                    bool eval_bool_var = query1.evaluate_boolean(xroot);
                }
                
                // Test without variable set
                if (query2) {
                    pugi::xpath_node result_no_var = root.select_node(query2);
                    double eval_num_no_var = query2.evaluate_number(xroot);
                }
                
                // Test string evaluation with buffer
                if (query3) {
                    char buffer[256];
                    size_t str_len = query3.evaluate_string(buffer, sizeof(buffer), xroot);
                }
                break;
            }
        }
        
        // Additional tests on document child nodes
        for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling()) {
            // Quick test on child nodes
            pugi::xpath_node xchild(child);
            if (query1) {
                bool child_bool = query1.evaluate_boolean(xchild);
                double child_num = query1.evaluate_number(xchild);
            }
        }
    }
#ifndef PUGIXML_NO_EXCEPTIONS
    catch (pugi::xpath_exception&) {
        // XPath compilation or evaluation errors are expected during fuzzing
    }
    catch (std::bad_alloc&) {
        // Memory allocation errors are also expected
    }
#endif
    
    return 0;
}

} // namespace harness_006
