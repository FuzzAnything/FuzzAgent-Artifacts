#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <cstring>

namespace harness_001 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for XPath testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t xpath_operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    uint8_t var_count = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(2048);
    
    // Consume XPath expressions
    std::vector<std::string> xpath_expressions;
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(256));
    }
    
    // Consume variable names and values
    std::vector<std::string> var_names;
    std::vector<std::string> var_values;
    for (int i = 0; i < var_count && fdp.remaining_bytes() > 10; i++) {
        var_names.push_back(fdp.ConsumeRandomLengthString(64));
        var_values.push_back(fdp.ConsumeRandomLengthString(128));
    }
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    // Parse XML based on test_mode
    switch (test_mode) {
        case 0:
            parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
            break;
        case 1:
            parse_result = doc.load_string(xml_content.c_str(), parse_options);
            break;
        case 2:
            // Use remaining bytes as buffer
            std::vector<uint8_t> buffer = fdp.ConsumeRemainingBytes<uint8_t>();
            if (!buffer.empty()) {
                parse_result = doc.load_buffer(buffer.data(), buffer.size(), parse_options);
            }
            break;
    }
    
    // If parsing failed, create a simple document for XPath testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        for (int i = 0; i < 5; i++) {
            pugi::xml_node child = root.append_child(("node" + std::to_string(i)).c_str());
            child.append_attribute(("attr" + std::to_string(i)).c_str()) = ("value" + std::to_string(i)).c_str();
            child.text().set(("text" + std::to_string(i)).c_str());
        }
    }
    
    // Test XPath functionality
    for (const auto& xpath_expr : xpath_expressions) {
        if (xpath_expr.empty()) continue;
        
        try {
            // Create XPath variable set if we have variables
            pugi::xpath_variable_set vars;
            bool has_vars = false;
            
            for (size_t i = 0; i < var_names.size() && i < var_values.size(); i++) {
                // Randomly choose variable type
                uint8_t var_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                switch (var_type) {
                    case 0:
                        if (vars.add(var_names[i].c_str(), pugi::xpath_type_boolean)) {
                            vars.set(var_names[i].c_str(), fdp.ConsumeBool());
                            has_vars = true;
                        }
                        break;
                    case 1:
                        if (vars.add(var_names[i].c_str(), pugi::xpath_type_number)) {
                            vars.set(var_names[i].c_str(), fdp.ConsumeFloatingPoint<double>());
                            has_vars = true;
                        }
                        break;
                    case 2:
                        if (vars.add(var_names[i].c_str(), pugi::xpath_type_string)) {
                            vars.set(var_names[i].c_str(), var_values[i].c_str());
                            has_vars = true;
                        }
                        break;
                    case 3:
                        if (vars.add(var_names[i].c_str(), pugi::xpath_type_node_set)) {
                            // Create a simple node set using array constructor
                            pugi::xpath_node node(doc.first_child());
                            if (node) {
                                pugi::xpath_node nodes[] = {node};
                                pugi::xpath_node_set set(nodes, nodes + 1);
                                vars.set(var_names[i].c_str(), set);
                                has_vars = true;
                            }
                        }
                        break;
                    case 4:
                        vars.add(var_names[i].c_str(), pugi::xpath_type_none);
                        break;
                }
            }
            
            // Create XPath query
            pugi::xpath_query query(xpath_expr.c_str(), has_vars ? &vars : nullptr);
            
            if (query) {
                // Test different XPath operations based on xpath_operation
                switch (xpath_operation) {
                    case 0:
                        // Evaluate as boolean
                        query.evaluate_boolean(doc);
                        break;
                    case 1:
                        // Evaluate as number
                        query.evaluate_number(doc);
                        break;
                    case 2:
                        // Evaluate as string
                        {
                            std::string result = query.evaluate_string(doc);
                            (void)result; // Avoid unused warning
                        }
                        break;
                    case 3:
                        // Evaluate as node
                        {
                            pugi::xpath_node node_result = query.evaluate_node(doc);
                            (void)node_result; // Avoid unused warning
                        }
                        break;
                    case 4:
                        // Evaluate as node set
                        {
                            pugi::xpath_node_set set_result = query.evaluate_node_set(doc);
                            (void)set_result; // Avoid unused warning
                        }
                        break;
                    case 5:
                        // Test all evaluation types
                        query.evaluate_boolean(doc);
                        query.evaluate_number(doc);
                        query.evaluate_string(doc);
                        query.evaluate_node(doc);
                        query.evaluate_node_set(doc);
                        break;
                }
                
                // Test query properties
                pugi::xpath_value_type return_type = query.return_type();
                const pugi::xpath_parse_result& parse_result = query.result();
                (void)return_type;
                (void)parse_result;
                
                // Test result conversion if query returned a string
                if (return_type == pugi::xpath_type_string) {
                    std::string str_result = query.evaluate_string(doc);
                    (void)str_result;
                }
            }
            
            // Also test query without variables
            pugi::xpath_query query_simple(xpath_expr.c_str());
            if (query_simple) {
                query_simple.evaluate_boolean(doc);
                query_simple.evaluate_number(doc);
            }
            
        } catch (pugi::xpath_exception& e) {
            // XPath parsing error, ignore and continue
            const char* what = e.what();
            (void)what;
        } catch (...) {
            // Catch any other exceptions
        }
    }
    
    // Additional XPath tests with document navigation
    try {
        // Test XPath node set operations
        pugi::xpath_node_set all_nodes = doc.select_nodes("//*");
        
        if (!all_nodes.empty()) {
            // Test iteration over node set
            for (pugi::xpath_node_set::const_iterator it = all_nodes.begin(); it != all_nodes.end(); ++it) {
                pugi::xpath_node node = *it;
                pugi::xml_node xmlnode = node.node();
                pugi::xml_attribute attr = node.attribute();
                
                if (xmlnode) {
                    // Test XPath queries on individual nodes
                    pugi::xpath_node_set node_attrs = xmlnode.select_nodes("@*");
                    
                    // Test parent/child navigation via XPath
                    pugi::xpath_node_set children = xmlnode.select_nodes("*");
                    pugi::xpath_node parent = xmlnode.select_node("..");
                    
                    (void)node_attrs;
                    (void)children;
                    (void)parent;
                }
            }
            
            // Test sorting
            all_nodes.sort();
            
            // Test array access
            if (all_nodes.size() > 0) {
                pugi::xpath_node first = all_nodes[0];
                pugi::xpath_node last = all_nodes[all_nodes.size() - 1];
                (void)first;
                (void)last;
            }
        }
        
        // Test attribute XPath queries
        pugi::xpath_node_set all_attrs = doc.select_nodes("//@*");
        
        // Test text node XPath queries
        pugi::xpath_node_set text_nodes = doc.select_nodes("//text()");
        
        (void)all_attrs;
        (void)text_nodes;
        
    } catch (pugi::xpath_exception&) {
        // Ignore XPath exceptions
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_001
