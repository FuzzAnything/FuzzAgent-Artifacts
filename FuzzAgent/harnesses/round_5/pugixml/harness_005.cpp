#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include "pugixml.hpp"

namespace harness_005 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for XML content, XPath queries, and buffer operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;  // 6 different test scenarios
    bool test_buffer_evaluation = fdp.ConsumeBool();
    bool test_select_node = fdp.ConsumeBool();
    bool test_select_nodes = fdp.ConsumeBool();
    bool test_select_single_node = fdp.ConsumeBool();
    uint8_t buffer_capacity_type = fdp.ConsumeIntegral<uint8_t>() % 4;  // Different buffer capacity strategies
    
    // Consume XPath query strings (max 256 chars each)
    std::string main_xpath_query = fdp.ConsumeRandomLengthString(256);
    std::string secondary_xpath_query = fdp.ConsumeRandomLengthString(256);
    
    // Consume buffer capacities for evaluate_string
    size_t buffer_capacity = 0;
    switch (buffer_capacity_type) {
        case 0: buffer_capacity = fdp.ConsumeIntegralInRange<size_t>(1, 64); break;  // Small buffer
        case 1: buffer_capacity = fdp.ConsumeIntegralInRange<size_t>(65, 256); break;  // Medium buffer
        case 2: buffer_capacity = fdp.ConsumeIntegralInRange<size_t>(257, 1024); break;  // Large buffer
        case 3: buffer_capacity = fdp.ConsumeIntegralInRange<size_t>(1025, 4096); break;  // Very large buffer
    }
    
    // Use remaining bytes as XML content
    std::vector<uint8_t> xml_content = fdp.ConsumeRemainingBytes<uint8_t>();
    if (xml_content.empty()) return 0;
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_default);
    
    // If parsing succeeded, test XPath buffer-based evaluation and node selection
    if (result) {
        try {
            // Get document element or first child for context
            pugi::xml_node context_node = doc.document_element();
            if (!context_node) {
                context_node = doc.first_child();
            }
            
            pugi::xpath_node xpath_context(context_node);
            
            // Create XPath query objects
            pugi::xpath_query query1(main_xpath_query.c_str());
            pugi::xpath_query query2(secondary_xpath_query.c_str());
            
            // Test buffer-based string evaluation if enabled
            if (test_buffer_evaluation && buffer_capacity > 0) {
                // Allocate buffer with specified capacity
                std::vector<char> buffer(buffer_capacity);
                
                // Test evaluate_string with buffer for query1
                size_t result_size1 = query1.evaluate_string(buffer.data(), buffer_capacity, xpath_context);
                (void)result_size1;  // Avoid unused variable warning
                
                // Test evaluate_string with buffer for query2
                size_t result_size2 = query2.evaluate_string(buffer.data(), buffer_capacity, xpath_context);
                (void)result_size2;  // Avoid unused variable warning
                
                // Test with different buffer fill levels
                if (buffer_capacity >= 2) {
                    // Test with half-filled buffer
                    std::vector<char> half_buffer(buffer_capacity / 2);
                    size_t half_result = query1.evaluate_string(half_buffer.data(), half_buffer.size(), xpath_context);
                    (void)half_result;
                    
                    // Test with very small buffer (1 char)
                    if (buffer_capacity >= 1) {
                        char tiny_buffer[1];
                        size_t tiny_result = query1.evaluate_string(tiny_buffer, 1, xpath_context);
                        (void)tiny_result;
                    }
                }
                
                // Test with zero capacity buffer (edge case)
                if (buffer_capacity >= 1) {
                    size_t zero_result = query1.evaluate_string(nullptr, 0, xpath_context);
                    (void)zero_result;
                }
            }
            
            // Test select_node if enabled
            if (test_select_node && context_node) {
                // Test select_node with char* query
                pugi::xpath_node selected_node1 = context_node.select_node(main_xpath_query.c_str());
                (void)selected_node1;
                
                // Test select_node with xpath_query object
                pugi::xpath_node selected_node2 = context_node.select_node(query1);
                (void)selected_node2;
                
                // Test with secondary query
                pugi::xpath_node selected_node3 = context_node.select_node(secondary_xpath_query.c_str());
                (void)selected_node3;
            }
            
            // Test select_nodes if enabled
            if (test_select_nodes && context_node) {
                // Test select_nodes with char* query
                pugi::xpath_node_set nodes1 = context_node.select_nodes(main_xpath_query.c_str());
                
                // Process the node set to maximize coverage
                if (!nodes1.empty()) {
                    for (pugi::xpath_node_set::const_iterator it = nodes1.begin(); it != nodes1.end(); ++it) {
                        pugi::xml_node node = it->node();
                        pugi::xml_attribute attr = it->attribute();
                        
                        if (node) {
                            const char* node_name = node.name();
                            const char* node_value = node.value();
                            (void)node_name;
                            (void)node_value;
                        }
                    }
                }
                
                // Test select_nodes with xpath_query object
                pugi::xpath_node_set nodes2 = context_node.select_nodes(query2);
                
                if (!nodes2.empty()) {
                    // Test node set size and empty properties
                    size_t node_count = nodes2.size();
                    bool is_empty = nodes2.empty();
                    (void)node_count;
                    (void)is_empty;
                    
                    // Test array access
                    if (node_count > 0) {
                        pugi::xpath_node first_node = nodes2[0];
                        pugi::xpath_node last_node = nodes2[node_count - 1];
                        (void)first_node;
                        (void)last_node;
                    }
                }
            }
            
            // Test select_single_node (deprecated but still present) if enabled
            if (test_select_single_node && context_node) {
                // Test select_single_node with char* query
                pugi::xpath_node single_node1 = context_node.select_single_node(main_xpath_query.c_str());
                (void)single_node1;
                
                // Test select_single_node with xpath_query object
                pugi::xpath_node single_node2 = context_node.select_single_node(query1);
                (void)single_node2;
            }
            
            // Test different scenarios based on test_scenario
            switch (test_scenario) {
                case 0: {
                    // Scenario 0: Test all buffer capacities with same query
                    if (buffer_capacity > 0) {
                        std::vector<char> buffer(buffer_capacity);
                        for (size_t i = 1; i <= 4 && i <= buffer_capacity; i *= 2) {
                            size_t cap = std::min(i, buffer_capacity);
                            size_t res = query1.evaluate_string(buffer.data(), cap, xpath_context);
                            (void)res;
                        }
                    }
                    break;
                }
                
                case 1: {
                    // Scenario 1: Test node selection with empty/null queries
                    if (context_node) {
                        pugi::xpath_node_set empty_nodes = context_node.select_nodes("");
                        (void)empty_nodes;
                        
                        // Test with null query pointer
                        try {
                            pugi::xpath_node_set null_nodes = context_node.select_nodes(nullptr);
                            (void)null_nodes;
                        } catch (...) {
                            // Ignore exceptions from null pointer
                        }
                    }
                    break;
                }
                
                case 2: {
                    // Scenario 2: Test buffer evaluation with different context nodes
                    if (buffer_capacity > 0 && !doc.empty()) {
                        std::vector<char> buffer(buffer_capacity);
                        
                        // Test with document as context
                        pugi::xpath_node doc_context(doc);
                        size_t doc_result = query1.evaluate_string(buffer.data(), buffer_capacity, doc_context);
                        (void)doc_result;
                        
                        // Test with child nodes as context
                        for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling()) {
                            pugi::xpath_node child_context(child);
                            size_t child_result = query1.evaluate_string(buffer.data(), buffer_capacity, child_context);
                            (void)child_result;
                            
                            if (child_result > 0) break; // Test a few children only
                        }
                    }
                    break;
                }
                
                case 3: {
                    // Scenario 3: Test combined operations
                    if (context_node && buffer_capacity > 0) {
                        std::vector<char> buffer(buffer_capacity);
                        
                        // First evaluate to buffer
                        size_t eval_result = query1.evaluate_string(buffer.data(), buffer_capacity, xpath_context);
                        (void)eval_result;
                        
                        // Then select nodes
                        pugi::xpath_node_set nodes = context_node.select_nodes(query1);
                        
                        // Process selected nodes
                        for (pugi::xpath_node_set::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
                            if (it->node()) {
                                // Evaluate buffer for each selected node
                                pugi::xpath_node node_context(it->node());
                                size_t node_eval = query2.evaluate_string(buffer.data(), buffer_capacity, node_context);
                                (void)node_eval;
                            }
                        }
                    }
                    break;
                }
                
                case 4: {
                    // Scenario 4: Test edge cases with minimal buffer
                    if (buffer_capacity >= 1) {
                        char min_buffer[1] = {0};
                        size_t min_result = query1.evaluate_string(min_buffer, 1, xpath_context);
                        (void)min_result;
                        
                        // Test with exactly sized buffer
                        if (min_result > 0 && min_result < 1024) {
                            std::vector<char> exact_buffer(min_result);
                            size_t exact_result = query1.evaluate_string(exact_buffer.data(), min_result, xpath_context);
                            (void)exact_result;
                        }
                    }
                    break;
                }
                
                case 5: {
                    // Scenario 5: Test all selection methods together
                    if (context_node) {
                        // Test all three selection methods
                        pugi::xpath_node single1 = context_node.select_node(main_xpath_query.c_str());
                        pugi::xpath_node_set multi1 = context_node.select_nodes(main_xpath_query.c_str());
                        pugi::xpath_node single2 = context_node.select_single_node(main_xpath_query.c_str());
                        
                        (void)single1;
                        (void)single2;
                        
                        // Compare results
                        if (!multi1.empty() && single1.node()) {
                            // Check if single node is in the multi set
                            bool found = false;
                            for (pugi::xpath_node_set::const_iterator it = multi1.begin(); it != multi1.end(); ++it) {
                                if (it->node() == single1.node()) {
                                    found = true;
                                    break;
                                }
                            }
                            (void)found;
                        }
                    }
                    break;
                }
            }
            
        } catch (const pugi::xpath_exception& e) {
            // Handle XPath exceptions gracefully
            const char* error_msg = e.what();
            (void)error_msg;
        } catch (...) {
            // Catch any other exceptions
        }
    }
    
    // Clean up document resources
    doc.reset();
    
    return 0;
}

} // namespace harness_005
