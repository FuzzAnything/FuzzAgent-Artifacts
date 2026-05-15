#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>

namespace harness_003 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) return 0;  // Minimum size for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume XML data from input
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(0, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    // Parse XML document - using load_buffer as specified in guidance
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, create a simple document for testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child(pugi::node_element);
        root.set_name("root");
        
        // Add some elements to make XPath queries more interesting
        for (int i = 0; i < 5; i++) {
            pugi::xml_node child = root.append_child(pugi::node_element);
            child.set_name(("element" + std::to_string(i)).c_str());
            child.append_child(pugi::node_pcdata).set_value(("value" + std::to_string(i)).c_str());
            
            // Add attributes
            child.append_attribute(("attr" + std::to_string(i)).c_str()) = (i * 10);
        }
    }
    
    // Get root node for XPath evaluation context
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        root = doc.first_child();
    }
    
    // Create XPath variable set as specified in guidance
    pugi::xpath_variable_set vars;
    
    // Add variables with different types based on fuzzed input
    // Follow guidance: set variables with various data types (string, number, boolean, node-set)
    size_t var_count = fdp.ConsumeIntegralInRange<size_t>(0, 8);
    std::vector<std::string> var_names;
    
    for (size_t i = 0; i < var_count && fdp.remaining_bytes() > 10; ++i) {
        // Consume variable name
        std::string var_name = fdp.ConsumeRandomLengthString(50);
        var_names.push_back(var_name);
        
        // Consume variable type (0: boolean, 1: number, 2: string, 3: node_set)
        int var_type = fdp.ConsumeIntegralInRange(0, 3);
        
        switch (var_type) {
            case 0: {  // boolean
                bool bool_val = fdp.ConsumeBool();
                // Use add() then set() pattern
                pugi::xpath_variable* var = vars.add(var_name.c_str(), pugi::xpath_type_boolean);
                if (var) {
                    var->set(bool_val);
                }
                break;
            }
            case 1: {  // number
                double num_val = fdp.ConsumeFloatingPoint<double>();
                pugi::xpath_variable* var = vars.add(var_name.c_str(), pugi::xpath_type_number);
                if (var) {
                    var->set(num_val);
                }
                break;
            }
            case 2: {  // string
                std::string str_val = fdp.ConsumeRandomLengthString(100);
                pugi::xpath_variable* var = vars.add(var_name.c_str(), pugi::xpath_type_string);
                if (var) {
                    var->set(str_val.c_str());
                }
                break;
            }
            case 3: {  // node-set
                // Create a node set from the document using select_nodes
                pugi::xpath_node_set node_set = root.select_nodes("//*");
                pugi::xpath_variable* var = vars.add(var_name.c_str(), pugi::xpath_type_node_set);
                if (var) {
                    var->set(node_set);
                }
                break;
            }
        }
    }
    
    // Consume XPath query strings for testing
    std::vector<std::string> xpath_queries;
    size_t query_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    
    for (size_t i = 0; i < query_count && fdp.remaining_bytes() > 10; ++i) {
        std::string query = fdp.ConsumeRandomLengthString(200);
        xpath_queries.push_back(query);
    }
    
    // TARGET 1: Test xml_node::select_node with variable set
    // This directly targets _ZNK4pugi8xml_node11select_nodeEPKcPNS_18xpath_variable_setE
    for (const auto& query : xpath_queries) {
        if (fdp.remaining_bytes() > 0) {
            bool use_vars = fdp.ConsumeBool();
            pugi::xpath_variable_set* vars_ptr = use_vars && var_count > 0 ? &vars : nullptr;
            
            try {
                pugi::xpath_node result = root.select_node(query.c_str(), vars_ptr);
                (void)result;  // Use result to avoid unused variable warning
            } catch (...) {
                // Ignore exceptions from invalid XPath queries
            }
        }
    }
    
    // TARGET 2: Test xml_node::select_nodes with variable set  
    // This directly targets _ZNK4pugi8xml_node12select_nodesEPKcPNS_18xpath_variable_setE
    for (const auto& query : xpath_queries) {
        if (fdp.remaining_bytes() > 0) {
            bool use_vars = fdp.ConsumeBool();
            pugi::xpath_variable_set* vars_ptr = use_vars && var_count > 0 ? &vars : nullptr;
            
            try {
                pugi::xpath_node_set results = root.select_nodes(query.c_str(), vars_ptr);
                (void)results;  // Use results to avoid unused variable warning
                
                // Optionally iterate through results
                for (pugi::xpath_node_set::const_iterator it = results.begin(); it != results.end(); ++it) {
                    (void)it;  // Use iterator
                }
            } catch (...) {
                // Ignore exceptions from invalid XPath queries
            }
        }
    }
    
    // TARGET 3: Test xml_node::select_single_node with variable set (deprecated but still available)
    // This directly targets _ZNK4pugi8xml_node18select_single_nodeEPKcPNS_18xpath_variable_setE
    for (const auto& query : xpath_queries) {
        if (fdp.remaining_bytes() > 0) {
            bool use_vars = fdp.ConsumeBool();
            pugi::xpath_variable_set* vars_ptr = use_vars && var_count > 0 ? &vars : nullptr;
            
            try {
                pugi::xpath_node result = root.select_single_node(query.c_str(), vars_ptr);
                (void)result;  // Use result to avoid unused variable warning
            } catch (...) {
                // Ignore exceptions from invalid XPath queries
            }
        }
    }
    
    // Additional testing: Create XPath queries with variable sets
    if (fdp.remaining_bytes() > 10 && var_count > 0) {
        for (const auto& query : xpath_queries) {
            try {
                // Create xpath_query with variable set
                pugi::xpath_query xpath_query_obj(query.c_str(), &vars);
                
                if (xpath_query_obj) {
                    // Evaluate using the query object
                    bool bool_result = xpath_query_obj.evaluate_boolean(root);
                    (void)bool_result;
                    
                    double num_result = xpath_query_obj.evaluate_number(root);
                    (void)num_result;
                    
                    std::string str_result = xpath_query_obj.evaluate_string(root).c_str();
                    (void)str_result;
                    
                    pugi::xpath_node_set node_set_result = xpath_query_obj.evaluate_node_set(root);
                    (void)node_set_result;
                }
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Verify variable values as suggested in guidance
    for (const auto& var_name : var_names) {
        const pugi::xpath_variable* var = vars.get(var_name.c_str());
        if (var) {
            // Get values based on type
            switch (var->type()) {
                case pugi::xpath_type_boolean:
                    (void)var->get_boolean();
                    break;
                case pugi::xpath_type_number:
                    (void)var->get_number();
                    break;
                case pugi::xpath_type_string:
                    (void)var->get_string();
                    break;
                case pugi::xpath_type_node_set:
                    (void)var->get_node_set();
                    break;
                default:
                    break;
            }
        }
    }
    
    // Test variable set methods directly
    if (var_count > 0 && fdp.remaining_bytes() > 10) {
        for (const auto& var_name : var_names) {
            // Try setting with different values
            bool set_as_bool = fdp.ConsumeBool();
            if (set_as_bool) {
                bool bool_val = fdp.ConsumeBool();
                (void)vars.set(var_name.c_str(), bool_val);
            }
            
            if (fdp.remaining_bytes() > 0) {
                double num_val = fdp.ConsumeFloatingPoint<double>();
                (void)vars.set(var_name.c_str(), num_val);
            }
            
            if (fdp.remaining_bytes() > 0) {
                std::string str_val = fdp.ConsumeRandomLengthString(50);
                (void)vars.set(var_name.c_str(), str_val.c_str());
            }
        }
    }
    
    return 0;
}

} // namespace harness_003
