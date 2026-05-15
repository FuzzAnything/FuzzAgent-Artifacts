#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace harness_003 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML content and XPath operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: XML PARSING ====================
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    
    // Consume part of input for XML content
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(16, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    // Try parsing with different options to exercise different code paths
    pugi::xml_parse_result result_default = doc.load_buffer(xml_buffer.data(), xml_buffer.size());
    pugi::xml_parse_result result_minimal = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_minimal);
    pugi::xml_parse_result result_full = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If all parsing failed, create a simple XML document from remaining input
    if (!result_default && !result_minimal && !result_full && fdp.remaining_bytes() > 32) {
        std::string simple_xml = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
        doc.load_string(simple_xml.c_str());
    }
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // No valid XML, return early
        return 0;
    }

#ifndef PUGIXML_NO_XPATH
    try {
        // ==================== PHASE 2: XPath VARIABLE SETUP ====================
        // TARGET: xpath_variable_set::add, set, get (10-14 undiscovered branches each)
        pugi::xpath_variable_set vars;
        
        // Add and set boolean variables
        bool bool_var1 = fdp.ConsumeBool();
        bool bool_var2 = fdp.ConsumeBool();
        vars.add("bool1", pugi::xpath_type_boolean);
        vars.add("bool2", pugi::xpath_type_boolean);
        vars.set("bool1", bool_var1);
        vars.set("bool2", bool_var2);
        
        // Add and set number variables
        double num_var1 = fdp.ConsumeFloatingPoint<double>();
        double num_var2 = fdp.ConsumeFloatingPoint<double>();
        double num_var3 = fdp.ConsumeFloatingPointInRange<double>(-1000.0, 1000.0);
        vars.add("num1", pugi::xpath_type_number);
        vars.add("num2", pugi::xpath_type_number);
        vars.add("num3", pugi::xpath_type_number);
        vars.set("num1", num_var1);
        vars.set("num2", num_var2);
        vars.set("num3", num_var3);
        
        // Add and set string variables
        std::string str_var1 = fdp.ConsumeRandomLengthString(100);
        std::string str_var2 = fdp.ConsumeRandomLengthString(100);
        std::string str_var3 = fdp.ConsumeRandomLengthString(50);
        vars.add("str1", pugi::xpath_type_string);
        vars.add("str2", pugi::xpath_type_string);
        vars.add("str3", pugi::xpath_type_string);
        vars.set("str1", str_var1.c_str());
        vars.set("str2", str_var2.c_str());
        vars.set("str3", str_var3.c_str());
        
        // Test getting variables
        pugi::xpath_variable* var_bool = vars.get("bool1");
        pugi::xpath_variable* var_num = vars.get("num1");
        pugi::xpath_variable* var_str = vars.get("str1");
        
        // ==================== PHASE 3: XPath QUERY CONSTRUCTION ====================
        // Create a variety of XPath expressions targeting different XPath features
        std::vector<std::string> xpath_expressions;
        
        // Basic path expressions
        xpath_expressions.push_back("//*");
        xpath_expressions.push_back("/node()");
        xpath_expressions.push_back("child::*");
        xpath_expressions.push_back("descendant::*");
        xpath_expressions.push_back("attribute::*");
        
        // Expressions with predicates
        xpath_expressions.push_back("//*[position() < 3]");
        xpath_expressions.push_back("//*[@id]");
        xpath_expressions.push_back("//*[text()]");
        xpath_expressions.push_back("//*[name() = 'item']");
        
        // Expressions with XPath functions
        xpath_expressions.push_back("string-length(//*)");
        xpath_expressions.push_back("count(//*)");
        xpath_expressions.push_back("sum(//@value)");
        xpath_expressions.push_back("concat('prefix-', //@name, '-suffix')");
        
        // Expressions with variables (using $var syntax)
        xpath_expressions.push_back("//*[@enabled = $bool1]");
        xpath_expressions.push_back("//*[position() < $num1]");
        xpath_expressions.push_back("//*[contains(@name, $str1)]");
        
        // Complex expressions combining multiple features
        xpath_expressions.push_back("//*[@id and @class][position() mod 2 = 0]");
        xpath_expressions.push_back("/root/item[position() > 1 and position() < last()]");
        
        // Add fuzzer-generated expressions
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
            std::string custom_expr = fdp.ConsumeRandomLengthString(200);
            if (!custom_expr.empty()) {
                xpath_expressions.push_back(custom_expr);
            }
        }
        
        // ==================== PHASE 4: QUERY EVALUATION ====================
        // TARGET: All xpath_query::evaluate_* methods (200+ undiscovered branches)
        for (const auto& expr : xpath_expressions) {
            if (expr.empty()) continue;
            
            // Create query with variable set
            pugi::xpath_query query(expr.c_str(), &vars);
            
            // Check if query compilation succeeded
            if (query) {
                // Get query return type
                pugi::xpath_value_type return_type = query.return_type();
                
                // Evaluate on different contexts: document, root, specific nodes
                std::vector<pugi::xpath_node> contexts;
                contexts.push_back(pugi::xpath_node(doc));
                contexts.push_back(pugi::xpath_node(root));
                
                // Add some child nodes as additional contexts
                for (pugi::xml_node child = root.first_child(); child && contexts.size() < 10; child = child.next_sibling()) {
                    contexts.push_back(pugi::xpath_node(child));
                }
                
                // Evaluate query in each context
                for (const auto& context : contexts) {
                    // TARGET: xpath_query::evaluate_boolean (42 undiscovered branches)
                    try {
                        bool bool_result = query.evaluate_boolean(context);
                        (void)bool_result; // Use result to avoid compiler warnings
                    } catch (...) {
                        // Ignore exceptions for fuzzing
                    }
                    
                    // TARGET: xpath_query::evaluate_number (42 undiscovered branches)
                    try {
                        double num_result = query.evaluate_number(context);
                        (void)num_result;
                    } catch (...) {
                        // Ignore exceptions for fuzzing
                    }
                    
#ifndef PUGIXML_NO_STL
                    // TARGET: xpath_query::evaluate_string (50 undiscovered branches)
                    try {
                        std::string string_result = query.evaluate_string(context);
                        (void)string_result;
                    } catch (...) {
                        // Ignore exceptions for fuzzing
                    }
#endif
                    
                    // TARGET: xpath_query::evaluate_string with buffer (variant)
                    try {
                        char buffer[256];
                        size_t len = query.evaluate_string(buffer, sizeof(buffer), context);
                        (void)len;
                    } catch (...) {
                        // Ignore exceptions for fuzzing
                    }
                    
                    // TARGET: xpath_query::evaluate_node (42 undiscovered branches)
                    try {
                        pugi::xpath_node node_result = query.evaluate_node(context);
                        (void)node_result;
                    } catch (...) {
                        // Ignore exceptions for fuzzing
                    }
                    
                    // TARGET: xpath_query::evaluate_node_set (42 undiscovered branches)
                    try {
                        pugi::xpath_node_set node_set_result = query.evaluate_node_set(context);
                        (void)node_set_result;
                        
                        // Process node set results
                        if (!node_set_result.empty()) {
                            // Test node set iteration and access
                            for (size_t i = 0; i < node_set_result.size() && i < 5; i++) {
                                pugi::xpath_node xnode = node_set_result[i];
                                pugi::xml_node node = xnode.node();
                                pugi::xml_attribute attr = xnode.attribute();
                                
                                // Access node/attribute properties
                                if (node) {
                                    const char* name = node.name();
                                    const char* value = node.value();
                                    (void)name;
                                    (void)value;
                                }
                                if (attr) {
                                    const char* attr_name = attr.name();
                                    const char* attr_value = attr.value();
                                    (void)attr_name;
                                    (void)attr_value;
                                }
                            }
                        }
                    } catch (...) {
                        // Ignore exceptions for fuzzing
                    }
                }
            }
        }
        
        // ==================== PHASE 5: XML NODE SELECTION ====================
        // TARGET: xml_node::select_nodes and select_node (42 branches each)
        
        // Create some XPath queries specifically for selection
        std::vector<std::string> selection_expressions;
        selection_expressions.push_back("//*");
        selection_expressions.push_back("child::*");
        selection_expressions.push_back("@*");
        selection_expressions.push_back("//*[@*]");
        selection_expressions.push_back("//*[text()]");
        
        // Add fuzzer-generated selection expressions
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
            std::string sel_expr = fdp.ConsumeRandomLengthString(100);
            if (!sel_expr.empty()) {
                selection_expressions.push_back(sel_expr);
            }
        }
        
        // Test selection on root node
        for (const auto& expr : selection_expressions) {
            if (expr.empty()) continue;
            
            // TARGET: xml_node::select_nodes with variable set
            try {
                pugi::xpath_node_set selected_nodes = root.select_nodes(expr.c_str(), &vars);
                (void)selected_nodes;
                
                // Process selected nodes
                for (pugi::xpath_node_set::const_iterator it = selected_nodes.begin(); it != selected_nodes.end(); ++it) {
                    pugi::xml_node node = it->node();
                    if (node) {
                        // Test node operations on selected nodes
                        pugi::xml_node parent = node.parent();
                        pugi::xml_node first_child = node.first_child();
                        pugi::xml_node next_sibling = node.next_sibling();
                        (void)parent;
                        (void)first_child;
                        (void)next_sibling;
                    }
                }
            } catch (...) {
                // Ignore exceptions for fuzzing
            }
            
            // TARGET: xml_node::select_node (single node selection)
            try {
                pugi::xpath_node selected_node = root.select_node(expr.c_str(), &vars);
                (void)selected_node;
                
                // Also test deprecated select_single_node for compatibility
                pugi::xpath_node single_node = root.select_single_node(expr.c_str(), &vars);
                (void)single_node;
            } catch (...) {
                // Ignore exceptions for fuzzing
            }
            
            // Test selection with pre-compiled queries
            pugi::xpath_query selection_query(expr.c_str(), &vars);
            if (selection_query) {
                // TARGET: xml_node::select_nodes with xpath_query object
                try {
                    pugi::xpath_node_set nodes_by_query = root.select_nodes(selection_query);
                    (void)nodes_by_query;
                } catch (...) {
                    // Ignore exceptions for fuzzing
                }
                
                // TARGET: xml_node::select_node with xpath_query object
                try {
                    pugi::xpath_node node_by_query = root.select_node(selection_query);
                    (void)node_by_query;
                } catch (...) {
                    // Ignore exceptions for fuzzing
                }
            }
        }
        
        // ==================== PHASE 6: ADDITIONAL XPath TESTING ====================
        // Test XPath queries on specific node types
        
        // Test on attribute nodes
        for (pugi::xml_attribute attr = root.first_attribute(); attr; attr = attr.next_attribute()) {
            pugi::xpath_node attr_xnode = pugi::xpath_node(attr, root);
            
            // Create simple queries for attributes
            pugi::xpath_query attr_query("string-length(.)");
            if (attr_query) {
                try {
                    double attr_len = attr_query.evaluate_number(attr_xnode);
                    (void)attr_len;
                } catch (...) {
                    // Ignore exceptions
                }
            }
        }
        
        // Test on text nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            if (child.type() == pugi::node_pcdata || child.type() == pugi::node_cdata) {
                pugi::xpath_node text_xnode = pugi::xpath_node(child);
                
                pugi::xpath_query text_query("string(.)");
                if (text_query) {
                    try {
                        std::string text_content = text_query.evaluate_string(text_xnode);
                        (void)text_content;
                    } catch (...) {
                        // Ignore exceptions
                    }
                }
            }
        }
        
    } catch (...) {
        // Catch any exceptions from XPath operations
        // In fuzzing, we want to continue even if exceptions occur
    }
#endif // PUGIXML_NO_XPATH
    
    // ==================== PHASE 7: CLEANUP ====================
    // TARGET: xml_document::reset (for completeness)
    doc.reset();
    
    return 0;
}

} // namespace harness_003
