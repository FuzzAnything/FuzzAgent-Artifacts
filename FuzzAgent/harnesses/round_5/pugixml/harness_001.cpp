#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "pugixml.hpp"

namespace harness_001 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for XML content and XPath query
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first
    uint8_t evaluation_type = fdp.ConsumeIntegral<uint8_t>() % 4;  // 4 different evaluation types
    uint8_t xpath_query_type = fdp.ConsumeIntegral<uint8_t>() % 8;  // Different XPath query patterns
    bool use_variables = fdp.ConsumeBool();
    bool use_node_context = fdp.ConsumeBool();
    
    // Consume XPath query string (max 256 chars)
    std::string xpath_query = fdp.ConsumeRandomLengthString(256);
    if (xpath_query.empty()) {
        // Use some default XPath patterns if empty
        switch (xpath_query_type) {
            case 0: xpath_query = "//*"; break;
            case 1: xpath_query = "/node()"; break;
            case 2: xpath_query = "text()"; break;
            case 3: xpath_query = "@*"; break;
            case 4: xpath_query = "count(//*)"; break;
            case 5: xpath_query = "string-length(name())"; break;
            case 6: xpath_query = "boolean(/*)"; break;
            case 7: xpath_query = "position()"; break;
        }
    }
    
    // Use remaining bytes as XML content
    std::vector<uint8_t> xml_content = fdp.ConsumeRemainingBytes<uint8_t>();
    if (xml_content.empty()) return 0;
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_default);
    
    // If parsing succeeded, test XPath evaluation
    if (result) {
        try {
            // Create XPath query object
            pugi::xpath_query query(xpath_query.c_str());
            
            // Get context node for evaluation
            pugi::xml_node context_node;
            if (use_node_context && !doc.empty()) {
                context_node = doc.document_element();
                if (!context_node) {
                    context_node = doc.first_child();
                }
            }
            pugi::xpath_node xpath_context(context_node);
            
            // Test different evaluation types based on control value
            switch (evaluation_type) {
                case 0: // Evaluate as boolean
                {
                    bool bool_result = query.evaluate_boolean(xpath_context);
                    (void)bool_result; // Avoid unused variable warning
                    break;
                }
                case 1: // Evaluate as number
                {
                    double num_result = query.evaluate_number(xpath_context);
                    (void)num_result; // Avoid unused variable warning
                    break;
                }
                case 2: // Evaluate as string (STL version)
                {
                    #ifndef PUGIXML_NO_STL
                    std::string string_result = query.evaluate_string(xpath_context);
                    (void)string_result; // Avoid unused variable warning
                    #endif
                    
                    // Also test buffer version
                    char buffer[256];
                    size_t buffer_size = query.evaluate_string(buffer, sizeof(buffer), xpath_context);
                    (void)buffer_size; // Avoid unused variable warning
                    break;
                }
                case 3: // Evaluate as node set
                {
                    pugi::xpath_node_set node_set = query.evaluate_node_set(xpath_context);
                    
                    // Process node set to maximize coverage
                    if (!node_set.empty()) {
                        // Test node set iteration
                        for (pugi::xpath_node_set::const_iterator it = node_set.begin(); it != node_set.end(); ++it) {
                            pugi::xml_node node = it->node();
                            pugi::xml_attribute attr = it->attribute();
                            
                            if (node) {
                                const char* node_name = node.name();
                                const char* node_value = node.value();
                                (void)node_name;
                                (void)node_value;
                            }
                            
                            if (attr) {
                                const char* attr_name = attr.name();
                                const char* attr_value = attr.value();
                                (void)attr_name;
                                (void)attr_value;
                            }
                        }
                        
                        // Test node set size and indexing
                        size_t set_size = node_set.size();
                        if (set_size > 0) {
                            pugi::xpath_node first_node = node_set[0];
                            pugi::xpath_node last_node = node_set[set_size - 1];
                            (void)first_node;
                            (void)last_node;
                        }
                        
                        // Test node set sorting
                        node_set.sort();
                        node_set.sort(true); // Reverse sort
                    }
                    break;
                }
            }
            
            // Test evaluate_node function as well
            if (evaluation_type != 3) { // Don't test if we already tested node set
                pugi::xpath_node single_node = query.evaluate_node(xpath_context);
                (void)single_node;
            }
            
            // Test query return type
            pugi::xpath_value_type return_type = query.return_type();
            (void)return_type;
            
            // Test query result validity
            pugi::xpath_parse_result parse_result = query.result();
            (void)parse_result;
            
        } catch (...) {
            // Ignore XPath exceptions - they're expected for malformed queries
        }
    }
    
    // Test additional XPath scenarios with different parsing modes
    if (xml_content.size() > 100) {
        pugi::xml_document doc2;
        pugi::xml_parse_result result2 = doc2.load_buffer(xml_content.data(), 
                                                         std::min(xml_content.size(), (size_t)100),
                                                         pugi::parse_fragment);
        
        if (result2) {
            try {
                // Test with different XPath queries
                const char* test_queries[] = {
                    ".",
                    "..",
                    "*",
                    "node()",
                    "text()",
                    "comment()",
                    "processing-instruction()"
                };
                
                uint8_t query_index = evaluation_type % (sizeof(test_queries) / sizeof(test_queries[0]));
                pugi::xpath_query simple_query(test_queries[query_index]);
                
                if (!doc2.empty()) {
                    pugi::xml_node context = doc2.first_child();
                    pugi::xpath_node xpath_ctx(context);
                    
                    // Quick test of all evaluation methods
                    simple_query.evaluate_boolean(xpath_ctx);
                    simple_query.evaluate_number(xpath_ctx);
                    #ifndef PUGIXML_NO_STL
                    simple_query.evaluate_string(xpath_ctx);
                    #endif
                    simple_query.evaluate_node_set(xpath_ctx);
                    simple_query.evaluate_node(xpath_ctx);
                }
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    return 0;
}

} // namespace harness_001
