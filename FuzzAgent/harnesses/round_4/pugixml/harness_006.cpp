#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

namespace harness_006 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful XPath evaluation with variables testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t parse_method = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t evaluation_mode = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t var_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 8); // At least 1 variable for testing
    bool use_precompiled_query = fdp.ConsumeBool();
    bool test_all_evaluation_types = fdp.ConsumeBool();
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    
    // Consume XPath expressions specifically designed for variable testing
    std::vector<std::string> xpath_expressions;
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 20; i++) {
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(512));
    }
    
    // Consume variable names - make them valid for XPath variable syntax
    std::vector<std::string> var_names;
    std::vector<uint8_t> var_types; // 0=boolean, 1=number, 2=string, 3=node_set
    std::vector<std::string> var_string_values;
    std::vector<double> var_number_values;
    std::vector<bool> var_bool_values;
    
    for (int i = 0; i < var_count && fdp.remaining_bytes() > 30; i++) {
        // Create variable name starting with letter
        std::string name = "var" + std::to_string(i) + "_" + fdp.ConsumeRandomLengthString(10);
        var_names.push_back(name);
        
        // Determine variable type
        uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 4;
        var_types.push_back(type);
        
        // Consume appropriate value based on type
        switch (type) {
            case 0: // boolean
                var_bool_values.push_back(fdp.ConsumeBool());
                break;
            case 1: // number
                var_number_values.push_back(fdp.ConsumeFloatingPoint<double>());
                break;
            case 2: // string
                var_string_values.push_back(fdp.ConsumeRandomLengthString(256));
                break;
            case 3: // node_set - we'll create dummy values
                var_string_values.push_back(fdp.ConsumeRandomLengthString(256));
                break;
        }
    }
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    // Parse XML with different methods
    switch (parse_method) {
        case 0:
            parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
            break;
        case 1:
            parse_result = doc.load_string(xml_content.c_str(), parse_options);
            break;
        case 2: {
            // Use remaining bytes as buffer
            std::vector<uint8_t> buffer = fdp.ConsumeRemainingBytes<uint8_t>();
            if (!buffer.empty()) {
                parse_result = doc.load_buffer(buffer.data(), buffer.size(), parse_options);
            }
            break;
        }
    }
    
    // If parsing failed, create a structured document specifically for XPath variable testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        
        // Create document structure with elements that can be referenced by XPath variables
        for (int i = 0; i < 10; i++) {
            pugi::xml_node item = root.append_child(("item" + std::to_string(i)).c_str());
            item.append_attribute("id") = std::to_string(i).c_str();
            item.append_attribute("value") = std::to_string(i * 10).c_str();
            item.append_attribute("active") = (i % 2 == 0) ? "true" : "false";
            item.text().set(("Text content for item " + std::to_string(i)).c_str());
            
            // Add child elements for node-set testing
            if (i % 3 == 0) {
                pugi::xml_node child = item.append_child("child");
                child.append_attribute("type") = "nested";
                child.text().set("Nested child text");
            }
        }
        
        // Add special elements with specific values for variable testing
        pugi::xml_node vars = root.append_child("variables");
        for (int i = 0; i < 5; i++) {
            pugi::xml_node var_elem = vars.append_child(("variable" + std::to_string(i)).c_str());
            var_elem.append_attribute("name") = ("test_var_" + std::to_string(i)).c_str();
            var_elem.append_attribute("value") = std::to_string(i * 100).c_str();
        }
    }
    
    // Create xpath_variable_set and add variables
    pugi::xpath_variable_set vars;
    bool vars_created = false;
    
    for (size_t i = 0; i < var_names.size(); i++) {
        pugi::xpath_variable* var = nullptr;
        
        switch (var_types[i]) {
            case 0: // boolean
                var = vars.add(var_names[i].c_str(), pugi::xpath_type_boolean);
                if (var && i < var_bool_values.size()) {
                    vars.set(var_names[i].c_str(), var_bool_values[i]);
                    vars_created = true;
                }
                break;
            case 1: // number
                var = vars.add(var_names[i].c_str(), pugi::xpath_type_number);
                if (var && i < var_number_values.size()) {
                    vars.set(var_names[i].c_str(), var_number_values[i]);
                    vars_created = true;
                }
                break;
            case 2: // string
                var = vars.add(var_names[i].c_str(), pugi::xpath_type_string);
                if (var && i < var_string_values.size()) {
                    vars.set(var_names[i].c_str(), var_string_values[i].c_str());
                    vars_created = true;
                }
                break;
            case 3: // node_set
                var = vars.add(var_names[i].c_str(), pugi::xpath_type_node_set);
                if (var && i < var_string_values.size()) {
                    // Create a simple node set for testing
                    pugi::xpath_node_set node_set;
                    // We can't easily create arbitrary node sets, so we'll use empty for now
                    vars_created = true;
                }
                break;
        }
    }
    
    // Get document root for XPath context
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.first_child();
    }
    
    // Test the targeted APIs with variable support
    for (const auto& xpath_expr : xpath_expressions) {
        if (xpath_expr.empty()) continue;
        
        try {
            // 1. Test xml_node::select_single_node with variable_set
            if (vars_created && evaluation_mode % 3 == 0) {
                pugi::xpath_node single_node = root.select_single_node(xpath_expr.c_str(), &vars);
                (void)single_node; // Use result to avoid unused variable warning
            }
            
            // 2. Test xml_node::select_nodes with variable_set
            if (vars_created && evaluation_mode % 3 == 1) {
                pugi::xpath_node_set nodes = root.select_nodes(xpath_expr.c_str(), &vars);
                (void)nodes; // Use result
                
                // If we got nodes, we can use them for evaluation
                if (!nodes.empty()) {
                    pugi::xpath_node context_node = nodes[0];
                    
                    // 3. Test xpath_query evaluation functions
                    if (use_precompiled_query) {
                        pugi::xpath_query query(xpath_expr.c_str(), &vars);
                        
                        if (query) {
                            // Test evaluate_string
                            if (test_all_evaluation_types || evaluation_mode % 4 == 0) {
                                try {
                                    std::string str_result = query.evaluate_string(context_node);
                                    (void)str_result;
                                } catch (...) {
                                    // Ignore exceptions from evaluation
                                }
                            }
                            
                            // Test evaluate_number
                            if (test_all_evaluation_types || evaluation_mode % 4 == 1) {
                                try {
                                    double num_result = query.evaluate_number(context_node);
                                    (void)num_result;
                                } catch (...) {
                                    // Ignore exceptions
                                }
                            }
                            
                            // Test evaluate_boolean
                            if (test_all_evaluation_types || evaluation_mode % 4 == 2) {
                                try {
                                    bool bool_result = query.evaluate_boolean(context_node);
                                    (void)bool_result;
                                } catch (...) {
                                    // Ignore exceptions
                                }
                            }
                        }
                    }
                }
            }
            
            // 3. Direct xpath_query creation and evaluation (without select_nodes)
            if (evaluation_mode % 3 == 2) {
                pugi::xpath_query query(xpath_expr.c_str(), vars_created ? &vars : nullptr);
                
                if (query) {
                    // Create a context node for evaluation
                    pugi::xpath_node context_node(root);
                    
                    // Test all evaluation types
                    try {
                        std::string str_result = query.evaluate_string(context_node);
                        (void)str_result;
                    } catch (...) {}
                    
                    try {
                        double num_result = query.evaluate_number(context_node);
                        (void)num_result;
                    } catch (...) {}
                    
                    try {
                        bool bool_result = query.evaluate_boolean(context_node);
                        (void)bool_result;
                    } catch (...) {}
                }
            }
            
        } catch (...) {
            // Catch all exceptions from pugixml (xpath_exception, std::bad_alloc, etc.)
            // Continue testing other expressions
        }
    }
    
    // Additional testing: create multiple variable sets and test them
    if (fdp.remaining_bytes() > 100) {
        pugi::xpath_variable_set vars2;
        
        // Add a few variables with different names
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
            std::string name = "alt_var" + std::to_string(i);
            uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 3;
            
            switch (type) {
                case 0:
                    if (vars2.add(name.c_str(), pugi::xpath_type_boolean)) {
                        vars2.set(name.c_str(), fdp.ConsumeBool());
                    }
                    break;
                case 1:
                    if (vars2.add(name.c_str(), pugi::xpath_type_number)) {
                        vars2.set(name.c_str(), fdp.ConsumeFloatingPoint<double>());
                    }
                    break;
                case 2:
                    if (vars2.add(name.c_str(), pugi::xpath_type_string)) {
                        std::string value = fdp.ConsumeRandomLengthString(100);
                        vars2.set(name.c_str(), value.c_str());
                    }
                    break;
            }
        }
        
        // Test with the new variable set
        if (!xpath_expressions.empty()) {
            try {
                pugi::xpath_query query2(xpath_expressions[0].c_str(), &vars2);
                if (query2) {
                    pugi::xpath_node context_node2(root);
                    
                    try { (void)query2.evaluate_string(context_node2); } catch (...) {}
                    try { (void)query2.evaluate_number(context_node2); } catch (...) {}
                    try { (void)query2.evaluate_boolean(context_node2); } catch (...) {}
                }
            } catch (...) {}
        }
    }
    
    // Cleanup
    doc.reset();
    
    return 0;
}

} // namespace harness_006
