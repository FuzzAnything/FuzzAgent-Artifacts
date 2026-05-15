#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace harness_001 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create XML document
    pugi::xml_document doc;
    
    // Parse XML with different parsing options
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, size));
    pugi::xml_parse_result result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, try to create XML from remaining input
    if (!result && fdp.remaining_bytes() > 10) {
        std::string xml_content = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
        result = doc.load_string(xml_content.c_str());
    }
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }
    
#ifndef PUGIXML_NO_XPATH
    try {
        // Create xpath_variable_set with variables
        pugi::xpath_variable_set vars;
        
        // Add boolean variable
        bool bool_var = fdp.ConsumeBool();
        vars.add("bool_var", pugi::xpath_type_boolean);
        vars.set("bool_var", bool_var);
        
        // Add number variable
        double num_var = fdp.ConsumeFloatingPoint<double>();
        vars.add("num_var", pugi::xpath_type_number);
        vars.set("num_var", num_var);
        
        // Add string variable
        std::string str_var = fdp.ConsumeRandomLengthString(100);
        vars.add("str_var", pugi::xpath_type_string);
        vars.set("str_var", str_var.c_str());
        
        // Create multiple XPath queries
        std::vector<std::string> xpath_expressions;
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(200));
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(200));
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(200));
        
        for (const auto& expr : xpath_expressions) {
            if (expr.empty()) continue;
            
            pugi::xpath_query query(expr.c_str(), &vars);
            
            if (query) {
                // Evaluate query on document
                query.evaluate_boolean(doc);
                query.evaluate_number(doc);
                query.evaluate_string(doc);
                
                // Evaluate query on specific nodes
                pugi::xpath_node_set node_set = query.evaluate_node_set(doc);
                
                // Process resulting nodes and manipulate them
                for (pugi::xpath_node_set::const_iterator it = node_set.begin(); it != node_set.end(); ++it) {
                    pugi::xml_node node = it->node();
                    if (node) {
                        // Node manipulation operations
                        std::string new_node_name = fdp.ConsumeRandomLengthString(20);
                        pugi::xml_node new_child = node.append_child(new_node_name.c_str());
                        
                        if (new_child) {
                            // Add attributes
                            std::string attr_name = fdp.ConsumeRandomLengthString(15);
                            std::string attr_value = fdp.ConsumeRandomLengthString(30);
                            new_child.append_attribute(attr_name.c_str()) = attr_value.c_str();
                            
                            // Add text
                            std::string text_content = fdp.ConsumeRandomLengthString(50);
                            new_child.text() = text_content.c_str();
                            
                            // Set node value
                            std::string node_value = fdp.ConsumeRandomLengthString(40);
                            node.set_value(node_value.c_str());
                            
                            // Remove child if we have enough input
                            if (fdp.ConsumeBool() && new_child) {
                                node.remove_child(new_child);
                            }
                        }
                        
                        // Test parent/child navigation
                        pugi::xml_node parent = node.parent();
                        pugi::xml_node first_child = node.first_child();
                        pugi::xml_node last_child = node.last_child();
                        pugi::xml_node next_sibling = node.next_sibling();
                        pugi::xml_node prev_sibling = node.previous_sibling();
                        
                        // Test attribute manipulation
                        pugi::xml_attribute attr = node.first_attribute();
                        while (attr) {
                            std::string attr_name = attr.name();
                            std::string attr_value = attr.value();
                            
                            // Modify attribute value
                            std::string new_attr_value = fdp.ConsumeRandomLengthString(20);
                            attr.set_value(new_attr_value.c_str());
                            
                            attr = attr.next_attribute();
                        }
                        
                        // Add new attribute
                        std::string new_attr_name = fdp.ConsumeRandomLengthString(15);
                        std::string new_attr_val = fdp.ConsumeRandomLengthString(25);
                        node.append_attribute(new_attr_name.c_str()) = new_attr_val.c_str();
                    }
                }
                
                // Evaluate as single node
                pugi::xpath_node single_node = query.evaluate_node(doc);
                if (single_node.node()) {
                    // Test node type and name
                    pugi::xml_node_type type = single_node.node().type();
                    const char* node_name = single_node.node().name();
                    const char* node_value = single_node.node().value();
                    
                    // Test xml_text operations
                    pugi::xml_text node_text = single_node.node().text();
                    std::string text_str = node_text.as_string();
                    
                    // Set text
                    std::string new_text = fdp.ConsumeRandomLengthString(30);
                    node_text.set(new_text.c_str());
                }
            }
        }
        
        // Test xpath_node_set operations
        pugi::xpath_query simple_query("//node", nullptr);
        if (simple_query) {
            pugi::xpath_node_set all_nodes = simple_query.evaluate_node_set(doc);
            
            // Test node set operations
            if (!all_nodes.empty()) {
                // Get size
                size_t node_count = all_nodes.size();
                
                // Get first and last
                pugi::xpath_node first_node = all_nodes[0];
                pugi::xpath_node last_node = all_nodes[node_count - 1];
                
                // Test sorting
                all_nodes.sort(false);  // ascending
                all_nodes.sort(true);   // descending
                
                // Test iterator operations
                for (pugi::xpath_node_set::const_iterator it = all_nodes.begin(); it != all_nodes.end(); ++it) {
                    pugi::xml_node node = it->node();
                    if (node) {
                        // Basic node information
                        pugi::xml_node_type node_type = node.type();
                        const char* name = node.name();
                    }
                }
            }
        }
        
        // Test XPath queries with different context nodes
        pugi::xpath_query child_query("child::*", nullptr);
        if (child_query) {
            // Evaluate on root
            child_query.evaluate_node_set(root);
            
            // Evaluate on child nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                child_query.evaluate_node_set(child);
            }
        }
        
    } catch (pugi::xpath_exception&) {
        // XPath parsing failed, which is expected for invalid expressions
    }
#endif
    
    // Cleanup happens automatically as objects go out of scope
    
    return 0;
}

} // namespace harness_001
