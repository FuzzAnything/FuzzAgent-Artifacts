#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_010 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML content and XPath operations
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: XML DOCUMENT LOADING ====================
    // Create XML document with different structures for XPath testing
    pugi::xml_document doc;
    
    // Consume XML content from fuzzer input
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(32, std::min(size / 2, size_t(2048)));
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    // Try parsing with different options
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size());
    
    // If parsing failed, create a structured XML document for XPath testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        
        // Add children with different attributes for XPath testing
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; ++i) {
            std::string child_name = "child" + std::to_string(i);
            pugi::xml_node child = root.append_child(child_name.c_str());
            
            // Add attributes
            child.append_attribute("id") = std::to_string(i).c_str();
            child.append_attribute("type") = (i % 2 == 0) ? "even" : "odd";
            
            // Add text content
            std::string text = fdp.ConsumeRandomLengthString(50);
            child.text() = text.c_str();
            
            // Add grandchildren for deeper XPath testing
            if (i % 2 == 0 && fdp.remaining_bytes() > 5) {
                pugi::xml_node grandchild = child.append_child("grandchild");
                grandchild.append_attribute("level") = "2";
            }
        }
    }
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

#ifndef PUGIXML_NO_XPATH
    try {
        // ==================== PHASE 2: XPath VARIABLE SETUP ====================
        // Create variable set with different types of variables
        pugi::xpath_variable_set vars;
        
        // Add boolean variables
        bool bool_var = fdp.ConsumeBool();
        vars.add("bool_var", pugi::xpath_type_boolean);
        vars.set("bool_var", bool_var);
        
        // Add number variables  
        double num_var = fdp.ConsumeFloatingPoint<double>();
        vars.add("num_var", pugi::xpath_type_number);
        vars.set("num_var", num_var);
        
        // Add string variables
        std::string str_var = fdp.ConsumeRandomLengthString(100);
        vars.add("str_var", pugi::xpath_type_string);
        vars.set("str_var", str_var.c_str());
        
        // ==================== PHASE 3: COMPREHENSIVE XPath EXPRESSION TESTING ====================
        // Generate a wide variety of XPath expressions targeting different features
        
        // 1. Path expressions with different axes
        std::vector<std::string> path_expressions = {
            "//*",                                    // All elements
            "/root",                                  // Absolute path
            "child::*",                               // Child axis
            "descendant::*",                          // Descendant axis  
            "attribute::*",                           // Attribute axis
            "self::node()",                           // Self axis
            "following-sibling::*",                   // Following sibling axis
            "preceding-sibling::*",                   // Preceding sibling axis
            "ancestor::*",                            // Ancestor axis
            "namespace::*",                           // Namespace axis
        };
        
        // 2. Expressions with different predicates
        std::vector<std::string> predicate_expressions = {
            "//*[@id]",                               // Has attribute 'id'
            "//*[text()]",                            // Has text content
            "//*[position() < 3]",                    // Position predicate
            "//*[last()]",                            // Last position
            "//*[@type='even']",                      // Attribute value comparison
            "//*[contains(name(), 'child')]",        // Function in predicate
            "//*[@id > 2]",                           // Numeric comparison
            "//*[not(@type)]",                        // Negation
            "//*[@id and @type]",                     // Boolean AND
            "//*[@id or @type]",                      // Boolean OR
        };
        
        // 3. Expressions with XPath functions
        std::vector<std::string> function_expressions = {
            "count(//*)",                             // Count function
            "string-length(//*)",                     // String length function
            "name()",                                 // Name function
            "local-name()",                           // Local name function
            "normalize-space(' test ')",
            "concat('a', 'b', 'c')",                  // Concat function
            "substring('abcdef', 2, 3)",              // Substring function
            "translate('abc', 'abc', 'ABC')",         // Translate function
            "boolean(//*)",                           // Boolean function
            "number('123')",                          // Number function
            "sum(//*/@id)",                           // Sum function
            "floor(3.14)",                            // Floor function
            "ceiling(3.14)",                          // Ceiling function
            "round(3.14)",                            // Round function
        };
        
        // 4. Complex expressions combining multiple features
        std::vector<std::string> complex_expressions = {
            "//child[position() mod 2 = 0]",          // Mod operator
            "//*[starts-with(name(), 'child')]",      // Starts-with function
            "//*[ends-with(name(), 'd')]",            // Ends-with function
            "//*[contains(@type, 'e')]",              // Contains function
            "//*[string-length(name()) > 4]",         // Function in predicate
            "/root/child[@id='1']/grandchild",        // Complex path
            "//*[not(@type='odd')]",                  // Negation with comparison
            "//child | //grandchild",                 // Union operator
            "//*[@id >= 1 and @id <= 3]",             // Range check
            "//*[substring(name(), 1, 3) = 'chi']",   // Substring in predicate
        };
        
        // 5. Expressions from fuzzer input for randomness
        std::vector<std::string> random_expressions;
        int random_expr_count = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < random_expr_count && fdp.remaining_bytes() > 10; ++i) {
            size_t expr_len = fdp.ConsumeIntegralInRange<size_t>(1, 200);
            std::string expr = fdp.ConsumeRandomLengthString(expr_len);
            if (!expr.empty()) {
                random_expressions.push_back(expr);
            }
        }
        
        // Combine all expressions
        std::vector<std::string> all_expressions;
        all_expressions.insert(all_expressions.end(), path_expressions.begin(), path_expressions.end());
        all_expressions.insert(all_expressions.end(), predicate_expressions.begin(), predicate_expressions.end());
        all_expressions.insert(all_expressions.end(), function_expressions.begin(), function_expressions.end());
        all_expressions.insert(all_expressions.end(), complex_expressions.begin(), complex_expressions.end());
        all_expressions.insert(all_expressions.end(), random_expressions.begin(), random_expressions.end());
        
        // ==================== PHASE 4: SYSTEMATIC XPath EVALUATION ====================
        // Test all five evaluation functions with various context nodes
        
        // Get different context nodes for evaluation
        std::vector<pugi::xpath_node> context_nodes;
        
        // Root node context
        context_nodes.push_back(pugi::xpath_node(root));
        
        // Child node context
        pugi::xml_node first_child = root.first_child();
        if (first_child) {
            context_nodes.push_back(pugi::xpath_node(first_child));
            
            // Attribute node context
            pugi::xml_attribute first_attr = first_child.first_attribute();
            if (first_attr) {
                context_nodes.push_back(pugi::xpath_node(first_attr, first_child));
            }
        }
        
        // Empty node context (tests error handling)
        context_nodes.push_back(pugi::xpath_node());
        
        // Test each expression with different contexts and evaluation methods
        for (const auto& expr : all_expressions) {
            if (expr.empty()) continue;
            
            // Create query with and without variables
            bool use_vars = fdp.ConsumeBool();
            pugi::xpath_query query(expr.c_str(), use_vars ? &vars : nullptr);
            
            if (!query) {
                // Query compilation failed - this tests error paths
                const pugi::xpath_parse_result& result = query.result();
                continue;
            }
            
            // Get return type to guide evaluation
            pugi::xpath_value_type return_type = query.return_type();
            
            // Test with different context nodes
            for (const auto& context : context_nodes) {
                // Always try evaluate_string with buffer version (specific target)
                char buffer[256];
                size_t str_len = query.evaluate_string(buffer, sizeof(buffer), context);
                
                // Test based on return type
                switch (return_type) {
                    case pugi::xpath_type_boolean:
                        query.evaluate_boolean(context);
                        break;
                    case pugi::xpath_type_number:
                        query.evaluate_number(context);
                        break;
                    case pugi::xpath_type_string:
#ifndef PUGIXML_NO_STL
                        query.evaluate_string(context);
#endif
                        break;
                    case pugi::xpath_type_node_set:
                        query.evaluate_node_set(context);
                        query.evaluate_node(context);
                        break;
                    default:
                        // Try all evaluation methods for unknown types
                        query.evaluate_boolean(context);
                        query.evaluate_number(context);
#ifndef PUGIXML_NO_STL
                        query.evaluate_string(context);
#endif
                        query.evaluate_node_set(context);
                        query.evaluate_node(context);
                        break;
                }
            }
        }
        
        // ==================== PHASE 5: NODE SELECTION API TESTING ====================
        // Test the convenience selection methods on xml_node
        
        // Test select_node with xpath_query
        for (const auto& expr : all_expressions) {
            if (expr.empty()) continue;
            
            pugi::xpath_query query(expr.c_str());
            if (query) {
                pugi::xpath_node selected = root.select_node(query);
                
                // Also test deprecated select_single_node
                pugi::xpath_node selected_single = root.select_single_node(query);
            }
        }
        
        // Test select_node with string expression
        for (const auto& expr : all_expressions) {
            if (expr.empty()) continue;
            
            bool use_vars = fdp.ConsumeBool();
            pugi::xpath_node selected = root.select_node(expr.c_str(), use_vars ? &vars : nullptr);
        }
        
        // Test select_nodes with string expression
        for (const auto& expr : all_expressions) {
            if (expr.empty()) continue;
            
            bool use_vars = fdp.ConsumeBool();
            pugi::xpath_node_set nodes = root.select_nodes(expr.c_str(), use_vars ? &vars : nullptr);
            
            // Process the node set
            for (pugi::xpath_node_set::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
                pugi::xml_node node = it->node();
                if (node) {
                    // Exercise node access methods
                    const char* name = node.name();
                    const char* value = node.value();
                }
            }
        }
        
    } catch (...) {
        // Catch any exceptions thrown by XPath evaluation
        // This is expected behavior for invalid XPath expressions
    }
#endif // PUGIXML_NO_XPATH
    
    return 0;
}

} // namespace harness_010
