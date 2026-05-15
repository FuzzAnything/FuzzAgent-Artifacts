#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "pugixml.hpp"

namespace harness_003 {


// Custom tree walker for testing xml_node::traverse API
class FuzzingTreeWalker : public pugi::xml_tree_walker {
private:
    int max_depth;
    int node_count;
    
public:
    FuzzingTreeWalker() : max_depth(0), node_count(0) {}
    
    virtual bool begin(pugi::xml_node& node) override {
        max_depth = 0;
        node_count = 0;
        return true; // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        node_count++;
        max_depth = std::max(max_depth, depth());
        
        // Access node properties to increase coverage
        const char* node_name = node.name();
        const char* node_value = node.value();
        pugi::xml_node_type node_type = node.type();
        
        // Access attributes
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
        }
        
        return true; // Continue traversal
    }
    
    virtual bool end(pugi::xml_node& node) override {
        return true;
    }
    
    int getNodeCount() const { return node_count; }
    int getMaxDepth() const { return max_depth; }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for XML content, variable operations, and XPath queries
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 different test scenarios
    bool use_variable_set = fdp.ConsumeBool();
    bool use_traverse = fdp.ConsumeBool();
    bool test_select_node = fdp.ConsumeBool();
    bool test_select_nodes = fdp.ConsumeBool();
    bool test_evaluate_string = fdp.ConsumeBool();
    uint8_t num_variables = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);  // Up to 10 variables
    
    // Consume XPath query strings
    std::string main_xpath_query = fdp.ConsumeRandomLengthString(256);
    std::string secondary_xpath_query = fdp.ConsumeRandomLengthString(256);
    
    // Consume variable names and types
    std::vector<std::string> var_names;
    std::vector<uint8_t> var_types;  // 0=bool, 1=number, 2=string, 3=node_set
    
    for (uint8_t i = 0; i < num_variables; ++i) {
        var_names.push_back(fdp.ConsumeRandomLengthString(32));
        var_types.push_back(fdp.ConsumeIntegral<uint8_t>() % 4);
    }
    
    // Consume variable values
    std::vector<bool> bool_values;
    std::vector<double> num_values;
    std::vector<std::string> string_values;
    
    for (uint8_t i = 0; i < num_variables; ++i) {
        if (var_types[i] == 0) {
            bool_values.push_back(fdp.ConsumeBool());
        } else if (var_types[i] == 1) {
            num_values.push_back(fdp.ConsumeFloatingPoint<double>());
        } else if (var_types[i] == 2) {
            string_values.push_back(fdp.ConsumeRandomLengthString(128));
        }
        // Node set values will be created from parsed XML
    }
    
    // Use remaining bytes as XML content
    std::vector<uint8_t> xml_content = fdp.ConsumeRemainingBytes<uint8_t>();
    if (xml_content.empty()) return 0;
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_default);
    
    // If parsing succeeded, test advanced XPath features with variable binding
    if (result) {
        try {
            // Create variable set if requested
            std::unique_ptr<pugi::xpath_variable_set> var_set;
            if (use_variable_set) {
                var_set = std::make_unique<pugi::xpath_variable_set>();
                
                // Add variables to the set
                int bool_idx = 0, num_idx = 0, str_idx = 0;
                for (uint8_t i = 0; i < num_variables; ++i) {
                    const char* var_name = var_names[i].c_str();
                    
                    switch (var_types[i]) {
                        case 0: // boolean
                            if (bool_idx < bool_values.size()) {
                                var_set->add(var_name, pugi::xpath_type_boolean);
                                var_set->set(var_name, bool_values[bool_idx++]);
                            }
                            break;
                        case 1: // number
                            if (num_idx < num_values.size()) {
                                var_set->add(var_name, pugi::xpath_type_number);
                                var_set->set(var_name, num_values[num_idx++]);
                            }
                            break;
                        case 2: // string
                            if (str_idx < string_values.size()) {
                                var_set->add(var_name, pugi::xpath_type_string);
                                var_set->set(var_name, string_values[str_idx++].c_str());
                            }
                            break;
                        case 3: // node_set
                            // Create node set from parsed document
                            var_set->add(var_name, pugi::xpath_type_node_set);
                            if (!doc.empty()) {
                                pugi::xml_node root = doc.document_element();
                                if (root) {
                                    pugi::xpath_node_set node_set = root.select_nodes("*");
                                    if (!node_set.empty()) {
                                        var_set->set(var_name, node_set);
                                    }
                                }
                            }
                            break;
                    }
                }
                
                // Test variable retrieval
                for (uint8_t i = 0; i < num_variables; ++i) {
                    const pugi::xpath_variable* var = var_set->get(var_names[i].c_str());
                    if (var) {
                        // Access variable properties
                        const char* retrieved_name = var->name();
                        pugi::xpath_value_type retrieved_type = var->type();
                        
                        // Test getters based on type
                        switch (retrieved_type) {
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
                        }
                    }
                }
            }
            
            // Get context node for XPath evaluation
            pugi::xml_node context_node;
            if (!doc.empty()) {
                context_node = doc.document_element();
                if (!context_node) {
                    context_node = doc.first_child();
                }
            }
            pugi::xpath_node xpath_context(context_node);
            
            // Test different scenarios based on test_scenario
            switch (test_scenario) {
                case 0: // Test xpath_query with variable binding
                {
                    if (!main_xpath_query.empty()) {
                        pugi::xpath_query query(main_xpath_query.c_str(), var_set.get());
                        
                        // Test all evaluation methods
                        if (query) {
                            // Evaluate as boolean
                            (void)query.evaluate_boolean(xpath_context);
                            
                            // Evaluate as number
                            (void)query.evaluate_number(xpath_context);
                            
                            #ifndef PUGIXML_NO_STL
                            // Evaluate as string (STL version)
                            if (test_evaluate_string) {
                                std::string string_result = query.evaluate_string(xpath_context);
                                (void)string_result;
                            }
                            #endif
                            
                            // Evaluate as string (buffer version)
                            char buffer[256];
                            size_t buffer_size = query.evaluate_string(buffer, sizeof(buffer), xpath_context);
                            (void)buffer_size;
                            
                            // Evaluate as node set
                            pugi::xpath_node_set node_set = query.evaluate_node_set(xpath_context);
                            (void)node_set;
                            
                            // Evaluate single node
                            pugi::xpath_node single_node = query.evaluate_node(xpath_context);
                            (void)single_node;
                            
                            // Test query properties
                            pugi::xpath_value_type return_type = query.return_type();
                            pugi::xpath_parse_result parse_result = query.result();
                            (void)return_type;
                            (void)parse_result;
                        }
                    }
                    break;
                }
                
                case 1: // Test xml_node::select_node with variable binding
                {
                    if (test_select_node && context_node && !main_xpath_query.empty()) {
                        pugi::xpath_node selected = context_node.select_node(main_xpath_query.c_str(), var_set.get());
                        if (selected.node()) {
                            const char* node_name = selected.node().name();
                            const char* node_value = selected.node().value();
                        }
                        if (selected.attribute()) {
                            const char* attr_name = selected.attribute().name();
                            const char* attr_value = selected.attribute().value();
                        }
                    }
                    break;
                }
                
                case 2: // Test xml_node::select_nodes with variable binding
                {
                    if (test_select_nodes && context_node && !main_xpath_query.empty()) {
                        pugi::xpath_node_set selected_nodes = context_node.select_nodes(main_xpath_query.c_str(), var_set.get());
                        
                        // Process selected nodes
                        for (pugi::xpath_node_set::const_iterator it = selected_nodes.begin(); it != selected_nodes.end(); ++it) {
                            pugi::xml_node node = it->node();
                            pugi::xml_attribute attr = it->attribute();
                            
                            if (node) {
                                const char* node_name = node.name();
                                const char* node_value = node.value();
                            }
                            
                            if (attr) {
                                const char* attr_name = attr.name();
                                const char* attr_value = attr.value();
                            }
                        }
                        
                        // Test node set operations
                        selected_nodes.sort();
                        selected_nodes.sort(true); // Reverse sort
                        
                        if (!selected_nodes.empty()) {
                            size_t set_size = selected_nodes.size();
                            pugi::xpath_node first = selected_nodes[0];
                            pugi::xpath_node last = selected_nodes[set_size - 1];
                            pugi::xpath_node mid = selected_nodes[set_size / 2];
                        }
                    }
                    break;
                }
                
                case 3: // Test multiple queries with shared variable set
                {
                    if (!main_xpath_query.empty() && !secondary_xpath_query.empty() && var_set) {
                        pugi::xpath_query query1(main_xpath_query.c_str(), var_set.get());
                        pugi::xpath_query query2(secondary_xpath_query.c_str(), var_set.get());
                        
                        if (query1 && query2) {
                            // Evaluate both queries
                            (void)query1.evaluate_boolean(xpath_context);
                            (void)query2.evaluate_number(xpath_context);
                            
                            #ifndef PUGIXML_NO_STL
                            std::string str1 = query1.evaluate_string(xpath_context);
                            std::string str2 = query2.evaluate_string(xpath_context);
                            #endif
                            
                            pugi::xpath_node_set nodes1 = query1.evaluate_node_set(xpath_context);
                            pugi::xpath_node_set nodes2 = query2.evaluate_node_set(xpath_context);
                        }
                    }
                    break;
                }
                
                case 4: // Test variable set modification during evaluation
                {
                    if (var_set && !main_xpath_query.empty()) {
                        // Create query with initial variable set
                        pugi::xpath_query query(main_xpath_query.c_str(), var_set.get());
                        
                        // Modify variable values
                        for (uint8_t i = 0; i < num_variables && i < var_names.size(); ++i) {
                            switch (var_types[i]) {
                                case 0: // boolean
                                    if (i < bool_values.size()) {
                                        var_set->set(var_names[i].c_str(), !bool_values[i]); // Toggle value
                                    }
                                    break;
                                case 1: // number
                                    if (i < num_values.size()) {
                                        var_set->set(var_names[i].c_str(), num_values[i] * 2.0);
                                    }
                                    break;
                                case 2: // string
                                    if (i < string_values.size()) {
                                        std::string modified = string_values[i] + "_modified";
                                        var_set->set(var_names[i].c_str(), modified.c_str());
                                    }
                                    break;
                            }
                        }
                        
                        // Re-evaluate with modified variables
                        if (query) {
                            (void)query.evaluate_boolean(xpath_context);
                        }
                    }
                    break;
                }
                
                case 5: // Test xml_node::traverse API
                {
                    if (use_traverse && context_node) {
                        FuzzingTreeWalker walker;
                        context_node.traverse(walker);
                        
                        // Access walker results
                        int node_count = walker.getNodeCount();
                        int max_depth = walker.getMaxDepth();
                        (void)node_count;
                        (void)max_depth;
                    }
                    break;
                }
                
                case 6: // Test combination of traverse and XPath
                {
                    if (use_traverse && context_node) {
                        // First traverse the tree
                        FuzzingTreeWalker walker;
                        context_node.traverse(walker);
                        
                        // Then run XPath queries
                        if (!main_xpath_query.empty()) {
                            pugi::xpath_query query(main_xpath_query.c_str(), var_set.get());
                            if (query) {
                                (void)query.evaluate_boolean(xpath_context);
                            }
                        }
                    }
                    break;
                }
                
                case 7: // Test complex variable scenarios with node sets
                {
                    if (var_set && context_node) {
                        // Create complex XPath queries that reference variables
                        std::string complex_query = "//*[name()=$var1 or @id=$var2 or text()=$var3]";
                        
                        // Add variables if they don't exist
                        var_set->add("var1", pugi::xpath_type_string);
                        var_set->set("var1", "test");
                        var_set->add("var2", pugi::xpath_type_string);
                        var_set->set("var2", "id_value");
                        var_set->add("var3", pugi::xpath_type_string);
                        var_set->set("var3", "sample");
                        
                        pugi::xpath_query query(complex_query.c_str(), var_set.get());
                        if (query) {
                            pugi::xpath_node_set results = query.evaluate_node_set(xpath_context);
                            (void)results;
                        }
                    }
                    break;
                }
            }
            
            // Additional tests for edge cases
            if (var_set) {
                // Test getting non-existent variable
                const pugi::xpath_variable* non_existent = var_set->get("non_existent_variable");
                (void)non_existent;
                
                // Test setting wrong type (should fail)
                if (num_variables > 0 && var_names.size() > 0) {
                    const char* first_var = var_names[0].c_str();
                    const pugi::xpath_variable* var = var_set->get(first_var);
                    if (var && var->type() == pugi::xpath_type_boolean) {
                        // Try to set wrong type
                        var_set->set(first_var, 42.0); // Should fail for boolean variable
                    }
                }
            }
            
        } catch (...) {
            // Ignore exceptions - they're expected for malformed XPath or invalid operations
        }
    }
    
    // Test with a second document using different parsing mode
    if (xml_content.size() > 50) {
        pugi::xml_document doc2;
        pugi::xml_parse_result result2 = doc2.load_buffer(xml_content.data(), 
                                                         std::min(xml_content.size(), (size_t)50),
                                                         pugi::parse_fragment);
        
        if (result2) {
            try {
                // Test basic variable operations on fragment
                pugi::xpath_variable_set var_set2;
                var_set2.add("fragment_var", pugi::xpath_type_string);
                var_set2.set("fragment_var", "fragment_value");
                
                if (!doc2.empty()) {
                    pugi::xml_node fragment_node = doc2.first_child();
                    if (fragment_node && !main_xpath_query.empty()) {
                        pugi::xpath_query frag_query(main_xpath_query.c_str(), &var_set2);
                        if (frag_query) {
                            pugi::xpath_node frag_context(fragment_node);
                            (void)frag_query.evaluate_boolean(frag_context);
                        }
                    }
                }
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    return 0;
}

} // namespace harness_003
