#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>

namespace harness_001 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Now test XPath functionality
    #ifndef PUGIXML_NO_XPATH
    try {
        // Consume XPath expression
        std::string xpath_expr = fdp.ConsumeRandomLengthString(1024);
        
        // Create variable set
        pugi::xpath_variable_set vars;
        
        // Add some variables based on fuzzer input
        size_t var_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
        std::vector<std::string> var_name_storage;
        
        for (size_t i = 0; i < var_count && fdp.remaining_bytes() > 20; ++i) {
            var_name_storage.push_back(fdp.ConsumeRandomLengthString(128));
            
            const int xpath_value_type_count = pugi::xpath_type_boolean + 1;
            pugi::xpath_value_type value_type = static_cast<pugi::xpath_value_type>(
                fdp.ConsumeIntegralInRange(0, xpath_value_type_count - 1));
            
            vars.add(var_name_storage.back().c_str(), value_type);
        }
        
        // Create XPath query
        pugi::xpath_query query(xpath_expr.c_str(), &vars);
        
        // Create XML document for evaluation
        std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(
            fdp.ConsumeIntegralInRange<size_t>(0, 2048));
        
        pugi::xml_document doc;
        if (!xml_buffer.empty()) {
            doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
        } else {
            // Create a simple default document if no XML data provided
            pugi::xml_node root = doc.append_child("root");
            root.append_child("child1").append_attribute("attr") = "value1";
            root.append_child("child2").append_attribute("attr") = "value2";
            root.append_child("child3").append_child(pugi::node_pcdata).set_value("text content");
        }
        
        // Test all XPath evaluation methods if query is valid
        if (query) {
            // Test boolean evaluation
            query.evaluate_boolean(doc);
            
            // Test number evaluation
            query.evaluate_number(doc);
            
            // Test string evaluation
            query.evaluate_string(doc);
            
            // Test node evaluation
            query.evaluate_node(doc);
            
            // Test node set evaluation
            query.evaluate_node_set(doc);
            
            // Test return type
            pugi::xpath_value_type return_type = query.return_type();
            
            // Test result/parse information
            const pugi::xpath_parse_result& parse_result = query.result();
            
            // Test document select methods with XPath query object
            pugi::xpath_node selected_node = doc.select_node(query);
            pugi::xpath_node_set selected_nodes = doc.select_nodes(query);
            
            // Test document select methods with string expression
            if (fdp.remaining_bytes() > 10) {
                std::string alt_xpath = fdp.ConsumeRandomLengthString(256);
                if (!alt_xpath.empty()) {
                    pugi::xpath_node alt_selected = doc.select_node(alt_xpath.c_str());
                    pugi::xpath_node_set alt_selected_set = doc.select_nodes(alt_xpath.c_str());
                }
            }
            
            // Test xpath_node operations
            pugi::xpath_node xnode = selected_node;
            if (xnode) {
                pugi::xml_node node = xnode.node();
                pugi::xml_attribute attr = xnode.attribute();
                pugi::xml_node parent = xnode.parent();
                
                if (node) {
                    node.name();
                    node.value();
                }
                if (attr) {
                    attr.name();
                    attr.value();
                }
            }
            
            // Test xpath_node_set operations
            pugi::xpath_node_set node_set = selected_nodes;
            if (!node_set.empty()) {
                // Test size and empty
                size_t set_size = node_set.size();
                bool is_empty = node_set.empty();
                
                // Test type
                pugi::xpath_node_set::type_t set_type = node_set.type();
                
                // Test first node
                pugi::xpath_node first_node = node_set.first();
                
                // Test indexing
                if (set_size > 0) {
                    pugi::xpath_node first_by_index = node_set[0];
                    if (set_size > 1) {
                        pugi::xpath_node second_by_index = node_set[1];
                    }
                }
                
                // Test iteration
                for (pugi::xpath_node_set::const_iterator it = node_set.begin();
                     it != node_set.end(); ++it) {
                    if (it->node()) {
                        it->node().name();
                    }
                }
                
                // Test sorting
                node_set.sort(false);  // ascending
                node_set.sort(true);   // descending
            }
            
            // Test xpath_variable_set operations
            if (!var_name_storage.empty()) {
                // Look up some variables
                size_t lookup_idx = fdp.ConsumeIntegralInRange<size_t>(0, var_name_storage.size() - 1);
                pugi::xpath_variable* var = vars.get(var_name_storage[lookup_idx].c_str());
                
                if (var) {
                    pugi::xpath_value_type var_type = var->type();
                    
                    // Try to get variable values based on type
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
                        default:
                            break;
                    }
                }
            }
            
            // Test string evaluation with buffer
            if (fdp.remaining_bytes() > 100) {
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                std::vector<char> buffer(buffer_size);
                
                size_t written = query.evaluate_string(buffer.data(), buffer.size(), doc);
                
                // Consume remaining bytes for another test
                if (fdp.remaining_bytes() > 50) {
                    std::string remaining = fdp.ConsumeRemainingBytesAsString();
                    if (!remaining.empty()) {
                        // Try to create another query with remaining data
                        try {
                            pugi::xpath_query query2(remaining.c_str());
                            if (query2) {
                                query2.evaluate_boolean(doc);
                            }
                        } catch (...) {
                            // Expected to fail sometimes
                        }
                    }
                }
            }
        }
        
        // Test error cases with empty or malformed expressions
        if (fdp.remaining_bytes() > 10) {
            std::string short_expr = fdp.ConsumeBytesAsString(
                fdp.ConsumeIntegralInRange<size_t>(0, 10));
            
            if (!short_expr.empty()) {
                try {
                    pugi::xpath_query error_query(short_expr.c_str());
                    // Even if construction succeeds, evaluation might fail
                    if (error_query) {
                        try {
                            error_query.evaluate_boolean(doc);
                        } catch (...) {
                            // Expected for type mismatches
                        }
                    }
                } catch (pugi::xpath_exception&) {
                    // Expected for parsing errors
                }
            }
        }
        
        // Test with empty document
        pugi::xml_document empty_doc;
        if (query) {
            try {
                query.evaluate_boolean(empty_doc);
            } catch (...) {
                // Expected
            }
        }
        
    } catch (pugi::xpath_exception&) {
        // XPath parsing or evaluation errors are expected with random input
    } catch (...) {
        // Catch any other exceptions
    }
    #endif // PUGIXML_NO_XPATH
    
    return 0;
}

} // namespace harness_001
