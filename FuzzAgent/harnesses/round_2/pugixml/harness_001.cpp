#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>

namespace harness_001 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) return 0;  // Minimum size check
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume XML data from input
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(0, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, we can still test some XPath APIs with empty document
    if (!parse_result) {
        // Create empty document for testing
        doc.reset();
        doc.append_child(pugi::node_element).set_name("root");
    }
    
    // Get root node for XPath evaluation context
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        root = doc.first_child();
    }
    
    // Create XPath variable set
    pugi::xpath_variable_set vars;
    
    // Add variables with different types based on fuzzed input
    size_t var_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    std::vector<std::string> var_names;
    
    for (size_t i = 0; i < var_count && fdp.remaining_bytes() > 10; ++i) {
        // Consume variable name
        std::string var_name = fdp.ConsumeRandomLengthString(50);
        var_names.push_back(var_name);
        
        // Consume variable type
        int var_type = fdp.ConsumeIntegralInRange(0, 3);  // 0: boolean, 1: number, 2: string, 3: node_set
        
        switch (var_type) {
            case 0: {  // boolean
                bool bool_val = fdp.ConsumeBool();
                vars.add(var_name.c_str(), pugi::xpath_type_boolean);
                vars.set(var_name.c_str(), bool_val);
                break;
            }
            case 1: {  // number
                double num_val = fdp.ConsumeFloatingPoint<double>();
                vars.add(var_name.c_str(), pugi::xpath_type_number);
                vars.set(var_name.c_str(), num_val);
                break;
            }
            case 2: {  // string
                std::string str_val = fdp.ConsumeRandomLengthString(100);
                vars.add(var_name.c_str(), pugi::xpath_type_string);
                vars.set(var_name.c_str(), str_val.c_str());
                break;
            }
            case 3: {  // node_set
                // Create an empty node set for testing
                pugi::xpath_node_set node_set;
                vars.add(var_name.c_str(), pugi::xpath_type_node_set);
                vars.set(var_name.c_str(), node_set);
                break;
            }
        }
    }
    
    // Test xpath_query construction and evaluation
    if (fdp.remaining_bytes() > 10) {
        // Consume XPath query string
        std::string xpath_query_str = fdp.ConsumeRandomLengthString(200);
        
        try {
            // Create xpath_query with optional variable set
            bool use_vars = fdp.ConsumeBool();
            pugi::xpath_query* query = nullptr;
            
            if (use_vars && var_count > 0) {
                query = new pugi::xpath_query(xpath_query_str.c_str(), &vars);
            } else {
                query = new pugi::xpath_query(xpath_query_str.c_str());
            }
            
            // Test all evaluate_* methods if query is valid
            if (query && *query) {
                // Evaluate as boolean
                bool bool_result = query->evaluate_boolean(root);
                (void)bool_result;  // Use result to avoid unused variable warning
                
                // Evaluate as number
                double num_result = query->evaluate_number(root);
                (void)num_result;
                
                // Evaluate as string (if STL is available)
                #ifndef PUGIXML_NO_STL
                std::string str_result = query->evaluate_string(root);
                (void)str_result;
                #endif
                
                // Evaluate as string with buffer
                char buffer[256];
                size_t str_len = query->evaluate_string(buffer, sizeof(buffer), root);
                (void)str_len;
                
                // Evaluate as node
                pugi::xpath_node node_result = query->evaluate_node(root);
                (void)node_result;
                
                // Evaluate as node set
                pugi::xpath_node_set node_set_result = query->evaluate_node_set(root);
                (void)node_set_result;
                
                // Test return_type() method
                pugi::xpath_value_type return_type = query->return_type();
                (void)return_type;
                
                // Test result() method for parsing errors
                const pugi::xpath_parse_result& parse_result = query->result();
                (void)parse_result;
            }
            
            // Test xml_node select methods with the query
            if (!root.empty() && query && *query) {
                // select_node - returns first matching node
                pugi::xpath_node selected_node = root.select_node(*query);
                (void)selected_node;
                
                // select_nodes - returns all matching nodes
                pugi::xpath_node_set selected_nodes = root.select_nodes(*query);
                (void)selected_nodes;
                
                // select_single_node (deprecated but still test)
                pugi::xpath_node single_node = root.select_single_node(*query);
                (void)single_node;
            }
            
            // Clean up query
            delete query;
            
        } catch (pugi::xpath_exception&) {
            // Expected for invalid XPath queries
        } catch (std::bad_alloc&) {
            // Handle out of memory
        }
    }
    
    // Test variable get methods
    for (const auto& var_name : var_names) {
        pugi::xpath_variable* var = vars.get(var_name.c_str());
        if (var) {
            // Test get methods based on variable type
            pugi::xpath_value_type var_type = var->type();
            switch (var_type) {
                case pugi::xpath_type_boolean: {
                    bool val = var->get_boolean();
                    (void)val;
                    break;
                }
                case pugi::xpath_type_number: {
                    double val = var->get_number();
                    (void)val;
                    break;
                }
                case pugi::xpath_type_string: {
                    const char* val = var->get_string();
                    (void)val;
                    break;
                }
                case pugi::xpath_type_node_set: {
                    const pugi::xpath_node_set& val = var->get_node_set();
                    (void)val;
                    break;
                }
                default:
                    break;
            }
        }
    }
    
    // Test multiple xpath_query objects with different expressions
    if (fdp.remaining_bytes() > 50) {
        size_t query_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        for (size_t i = 0; i < query_count && fdp.remaining_bytes() > 10; ++i) {
            std::string query_str = fdp.ConsumeRandomLengthString(100);
            try {
                pugi::xpath_query q(query_str.c_str());
                if (q) {
                    // Quick evaluation test
                    q.evaluate_boolean(root);
                }
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Test document-level select methods
    if (!root.empty() && fdp.remaining_bytes() > 10) {
        std::string doc_query_str = fdp.ConsumeRandomLengthString(100);
        try {
            pugi::xpath_query doc_query(doc_query_str.c_str());
            if (doc_query) {
                pugi::xpath_node_set doc_nodes = doc.select_nodes(doc_query);
                (void)doc_nodes;
                
                pugi::xpath_node doc_node = doc.select_node(doc_query);
                (void)doc_node;
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Clean up
    doc.reset();
    
    return 0;
}

} // namespace harness_001
