#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_008 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML content, XPath expressions, and variable values
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: XML DOCUMENT LOADING ====================
    // TARGET: xml_document::load_buffer
    pugi::xml_document doc;
    
    // Consume a portion of input for XML content
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(32, std::min(size / 2, size_t(4096)));
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    // Try parsing with default options
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size());
    
    // If parsing failed, create a simple XML document from remaining input
    if (!parse_result && fdp.remaining_bytes() > 64) {
        std::string simple_xml = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
        doc.load_string(simple_xml.c_str());
    }
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // No valid XML structure, return early
        return 0;
    }

#ifndef PUGIXML_NO_XPATH
    try {
        // ==================== PHASE 2: XPath VARIABLE SETUP ====================
        // TARGET: xpath_variable_set::add, xpath_variable_set::set
        pugi::xpath_variable_set vars;
        
        // Create multiple variables of different types from fuzzer input
        int var_count = fdp.ConsumeIntegralInRange<int>(1, 8);
        
        for (int i = 0; i < var_count && fdp.remaining_bytes() > 20; ++i) {
            // Consume variable name
            std::string var_name = "var" + std::to_string(i) + "_" + fdp.ConsumeRandomLengthString(10);
            
            // Choose variable type based on fuzzer input
            int type_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
            
            switch (type_choice) {
                case 0: { // Boolean
                    bool bool_val = fdp.ConsumeBool();
                    if (vars.add(var_name.c_str(), pugi::xpath_type_boolean)) {
                        vars.set(var_name.c_str(), bool_val);
                    }
                    break;
                }
                case 1: { // Number
                    double num_val = fdp.ConsumeFloatingPoint<double>();
                    if (vars.add(var_name.c_str(), pugi::xpath_type_number)) {
                        vars.set(var_name.c_str(), num_val);
                    }
                    break;
                }
                case 2: { // String
                    std::string str_val = fdp.ConsumeRandomLengthString(50);
                    if (vars.add(var_name.c_str(), pugi::xpath_type_string)) {
                        vars.set(var_name.c_str(), str_val.c_str());
                    }
                    break;
                }
                case 3: { // Node-set (create a node-set from document)
                    pugi::xpath_node_set node_set;
                    // Try to create a simple node set using evaluate_node_set on root
                    try {
                        pugi::xpath_query simple_query("//*");
                        if (simple_query) {
                            node_set = simple_query.evaluate_node_set(pugi::xpath_node(root));
                        }
                    } catch (...) {
                        // If evaluation fails, use empty node set
                    }
                    if (vars.add(var_name.c_str(), pugi::xpath_type_node_set)) {
                        vars.set(var_name.c_str(), node_set);
                    }
                    break;
                }
            }
        }
        
        // ==================== PHASE 3: COMPLEX XPath EXPRESSION GENERATION ====================
        // Generate a variety of complex XPath expressions targeting different evaluation types
        std::vector<std::string> xpath_expressions;
        
        // Boolean expressions (for evaluate_boolean)
        xpath_expressions.push_back("count(//*) > 0");
        xpath_expressions.push_back("boolean(//*)");
        xpath_expressions.push_back("not(//*)");
        xpath_expressions.push_back("true() and false()");
        xpath_expressions.push_back("1 = 1");
        xpath_expressions.push_back("string-length(//*) > 10");
        xpath_expressions.push_back("contains(//@name, 'test')");
        xpath_expressions.push_back("starts-with(//@name, 'prefix')");
        
        // Numeric expressions (for evaluate_number)
        xpath_expressions.push_back("count(//*)");
        xpath_expressions.push_back("sum(//@value)");
        xpath_expressions.push_back("number(//@value)");
        xpath_expressions.push_back("string-length(//*)");
        xpath_expressions.push_back("position()");
        xpath_expressions.push_back("last()");
        xpath_expressions.push_back("floor(3.14)");
        xpath_expressions.push_back("ceiling(3.14)");
        xpath_expressions.push_back("round(3.5)");
        
        // String expressions (for evaluate_string)
        xpath_expressions.push_back("string(//*)");
        xpath_expressions.push_back("concat('prefix', //@name, 'suffix')");
        xpath_expressions.push_back("substring(//@name, 1, 5)");
        xpath_expressions.push_back("translate(//@name, 'abc', 'ABC')");
        xpath_expressions.push_back("normalize-space(//text())");
        xpath_expressions.push_back("local-name(//*)");
        xpath_expressions.push_back("namespace-uri(//*)");
        xpath_expressions.push_back("name(//*)");
        
        // Node expressions (for evaluate_node, evaluate_node_set)
        xpath_expressions.push_back("//*");
        xpath_expressions.push_back("/*");
        xpath_expressions.push_back("child::*");
        xpath_expressions.push_back("descendant::*");
        xpath_expressions.push_back("ancestor::*");
        xpath_expressions.push_back("following-sibling::*");
        xpath_expressions.push_back("preceding-sibling::*");
        xpath_expressions.push_back("attribute::*");
        xpath_expressions.push_back("self::*");
        
        // Complex expressions with predicates
        xpath_expressions.push_back("//*[@id]");
        xpath_expressions.push_back("//*[position() < 3]");
        xpath_expressions.push_back("//*[last()]");
        xpath_expressions.push_back("//*[text()]");
        xpath_expressions.push_back("//*[@class='test'][position() mod 2 = 0]");
        xpath_expressions.push_back("/root/item[position() > 1 and position() < last()]");
        
        // Expressions with variables
        xpath_expressions.push_back("$var0_boolean");
        xpath_expressions.push_back("//*[@enabled = $var1_boolean]");
        xpath_expressions.push_back("//*[position() < $var2_number]");
        xpath_expressions.push_back("//*[contains(@name, $var3_string)]");
        xpath_expressions.push_back("count($var4_node_set)");
        
        // Complex expressions combining multiple features
        xpath_expressions.push_back("concat('Count: ', count(//*[@test]), ' items')");
        xpath_expressions.push_back("sum(//item[@price > 10]/@quantity)");
        xpath_expressions.push_back("//section[contains(@class, 'main')]//div[@id='content']/p[position() < $var5_number]");
        
        // Add fuzzer-generated expressions
        int custom_expr_count = fdp.ConsumeIntegralInRange<int>(0, 10);
        for (int i = 0; i < custom_expr_count && fdp.remaining_bytes() > 10; ++i) {
            std::string custom_expr = fdp.ConsumeRandomLengthString(200);
            if (!custom_expr.empty()) {
                xpath_expressions.push_back(custom_expr);
            }
        }
        
        // ==================== PHASE 4: XPath QUERY CONSTRUCTION AND EVALUATION ====================
        // TARGET: xpath_query constructor, evaluate_boolean, evaluate_number, evaluate_string, evaluate_node
        
        // Create evaluation contexts
        std::vector<pugi::xpath_node> evaluation_contexts;
        evaluation_contexts.push_back(pugi::xpath_node(doc));  // Document context
        evaluation_contexts.push_back(pugi::xpath_node(root)); // Root element context
        
        // Add some child nodes as additional contexts
        for (pugi::xml_node child = root.first_child(); child && evaluation_contexts.size() < 10; child = child.next_sibling()) {
            evaluation_contexts.push_back(pugi::xpath_node(child));
        }
        
        // Process each XPath expression
        for (const auto& expr : xpath_expressions) {
            if (expr.empty()) continue;
            
            // TARGET: xpath_query constructor
            pugi::xpath_query query(expr.c_str(), &vars);
            
            // Check if query compilation succeeded
            if (!query) {
                continue; // Skip invalid queries
            }
            
            // Get query return type
            pugi::xpath_value_type return_type = query.return_type();
            
            // Evaluate query in each context
            for (const auto& context : evaluation_contexts) {
                // TARGET: xpath_query::evaluate_boolean (0% covered)
                try {
                    bool bool_result = query.evaluate_boolean(context);
                    (void)bool_result; // Use to avoid compiler warnings
                } catch (...) {
                    // Ignore exceptions - they're expected for invalid expressions
                }
                
                // TARGET: xpath_query::evaluate_number (0% covered)
                try {
                    double num_result = query.evaluate_number(context);
                    (void)num_result;
                } catch (...) {
                    // Ignore exceptions
                }
                
#ifndef PUGIXML_NO_STL
                // TARGET: xpath_query::evaluate_string (0% covered)
                try {
                    std::string string_result = query.evaluate_string(context);
                    (void)string_result;
                } catch (...) {
                    // Ignore exceptions
                }
#endif
                
                // Alternative string evaluation with buffer
                try {
                    char buffer[512];
                    size_t len = query.evaluate_string(buffer, sizeof(buffer), context);
                    (void)len;
                } catch (...) {
                    // Ignore exceptions
                }
                
                // TARGET: xpath_query::evaluate_node (0% covered)
                try {
                    pugi::xpath_node node_result = query.evaluate_node(context);
                    (void)node_result;
                    
                    // If we got a node, test some operations on it
                    if (node_result) {
                        pugi::xml_node node = node_result.node();
                        if (node) {
                            // Test node properties
                            const char* node_name = node.name();
                            const char* node_value = node.value();
                            (void)node_name;
                            (void)node_value;
                        }
                    }
                } catch (...) {
                    // Ignore exceptions
                }
                
                // TARGET: xpath_query::evaluate_node_set
                try {
                    pugi::xpath_node_set node_set_result = query.evaluate_node_set(context);
                    (void)node_set_result;
                    
                    // Process node set if not empty
                    if (!node_set_result.empty()) {
                        // Test node set operations
                        size_t set_size = node_set_result.size();
                        pugi::xpath_node_set::const_iterator it = node_set_result.begin();
                        pugi::xpath_node_set::const_iterator end = node_set_result.end();
                        
                        // Access a few nodes from the set
                        for (size_t i = 0; i < std::min(set_size, size_t(5)); ++i) {
                            pugi::xpath_node xnode = node_set_result[i];
                            (void)xnode;
                        }
                    }
                } catch (...) {
                    // Ignore exceptions
                }
            }
        }
        
        // ==================== PHASE 5: ENTRY POINT API TESTING ====================
        // TARGET: xml_node::select_node, xml_node::select_nodes, xml_node::select_single_node
        
        // Create a subset of expressions for selection testing
        std::vector<std::string> selection_expressions;
        selection_expressions.push_back("//*");
        selection_expressions.push_back("child::*");
        selection_expressions.push_back("//*[@*]");
        selection_expressions.push_back("//*[text()]");
        selection_expressions.push_back("@*");
        
        // Add some fuzzer-generated selection expressions
        int sel_expr_count = fdp.ConsumeIntegralInRange<int>(0, 5);
        for (int i = 0; i < sel_expr_count && fdp.remaining_bytes() > 10; ++i) {
            std::string sel_expr = fdp.ConsumeRandomLengthString(100);
            if (!sel_expr.empty()) {
                selection_expressions.push_back(sel_expr);
            }
        }
        
        // Test selection APIs
        for (const auto& expr : selection_expressions) {
            if (expr.empty()) continue;
            
            // TARGET: xml_node::select_nodes
            try {
                pugi::xpath_node_set selected_nodes = root.select_nodes(expr.c_str(), &vars);
                (void)selected_nodes;
            } catch (...) {
                // Ignore exceptions
            }
            
            // TARGET: xml_node::select_node
            try {
                pugi::xpath_node selected_node = root.select_node(expr.c_str(), &vars);
                (void)selected_node;
            } catch (...) {
                // Ignore exceptions
            }
            
            // TARGET: xml_node::select_single_node (deprecated but still available)
            try {
                pugi::xpath_node single_node = root.select_single_node(expr.c_str(), &vars);
                (void)single_node;
            } catch (...) {
                // Ignore exceptions
            }
            
            // Also test with pre-compiled queries
            pugi::xpath_query sel_query(expr.c_str(), &vars);
            if (sel_query) {
                // TARGET: xml_node::select_nodes with xpath_query object
                try {
                    pugi::xpath_node_set nodes_by_query = root.select_nodes(sel_query);
                    (void)nodes_by_query;
                } catch (...) {
                    // Ignore exceptions
                }
                
                // TARGET: xml_node::select_node with xpath_query object
                try {
                    pugi::xpath_node node_by_query = root.select_node(sel_query);
                    (void)node_by_query;
                } catch (...) {
                    // Ignore exceptions
                }
            }
        }
        
    } catch (...) {
        // Catch any unhandled exceptions
        // In fuzzing, we want to continue even if exceptions occur
    }
#endif // PUGIXML_NO_XPATH
    
    // ==================== PHASE 6: CLEANUP ====================
    // TARGET: xml_document::reset
    doc.reset();
    
    return 0;
}

} // namespace harness_008
