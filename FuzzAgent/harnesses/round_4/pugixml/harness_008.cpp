#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

namespace harness_008 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for comprehensive XPath evaluation testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t parse_method = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t evaluation_mode = fdp.ConsumeIntegral<uint8_t>() % 12; // 12 different evaluation scenarios
    uint8_t var_count = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    bool test_all_evaluation_types = fdp.ConsumeBool();
    bool use_string_buffer_version = fdp.ConsumeBool();
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    
    // Consume XPath expressions specifically targeting the 11 XPath functions
    std::vector<std::string> xpath_expressions;
    for (int i = 0; i < 8 && fdp.remaining_bytes() > 20; i++) {
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(512));
    }
    
    // Consume variable names and values for parameterized queries
    std::vector<std::string> var_names;
    std::vector<uint8_t> var_types; // 0=boolean, 1=number, 2=string, 3=node_set
    std::vector<std::string> var_string_values;
    std::vector<double> var_number_values;
    std::vector<bool> var_bool_values;
    
    // Separate indices for each type vector
    size_t bool_idx = 0;
    size_t num_idx = 0;
    size_t str_idx = 0;
    
    for (int i = 0; i < var_count && fdp.remaining_bytes() > 30; i++) {
        // Create valid variable names
        std::string name = "var" + std::to_string(i) + "_" + fdp.ConsumeRandomLengthString(8);
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
    
    // If parsing failed, create a structured document specifically for XPath evaluation testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        
        // Create a rich document structure for comprehensive XPath testing
        for (int i = 0; i < 15; i++) {
            pugi::xml_node item = root.append_child(("item" + std::to_string(i)).c_str());
            item.append_attribute("id") = std::to_string(i).c_str();
            item.append_attribute("value") = std::to_string(i * 10).c_str();
            item.append_attribute("active") = (i % 2 == 0) ? "true" : "false";
            item.append_attribute("category") = (i % 3 == 0) ? "A" : (i % 3 == 1) ? "B" : "C";
            item.text().set(("Item content " + std::to_string(i)).c_str());
            
            // Add nested elements for complex XPath queries
            if (i % 4 == 0) {
                pugi::xml_node nested = item.append_child("nested");
                nested.append_attribute("level") = "2";
                nested.text().set("Nested text");
                
                // Add grandchildren
                for (int j = 0; j < 2; j++) {
                    pugi::xml_node grandchild = nested.append_child(("grandchild" + std::to_string(j)).c_str());
                    grandchild.text().set(("Grandchild " + std::to_string(j)).c_str());
                }
            }
            
            // Add attributes with different data types
            if (i % 5 == 0) {
                item.append_attribute("special") = "yes";
                item.append_attribute("priority") = std::to_string(i % 10).c_str();
            }
        }
        
        // Add special elements for targeted XPath queries
        pugi::xml_node special = root.append_child("special");
        special.append_attribute("type") = "unique";
        special.append_attribute("count") = "100";
        special.text().set("Special content for XPath testing");
        
        pugi::xml_node data = root.append_child("data");
        for (int i = 0; i < 5; i++) {
            pugi::xml_node entry = data.append_child(("entry" + std::to_string(i)).c_str());
            entry.append_attribute("key") = ("key" + std::to_string(i)).c_str();
            entry.append_attribute("value") = std::to_string(i * 100).c_str();
        }
    }
    
    // Get document root for XPath evaluation
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }
    
    // Create xpath_node for evaluation context
    pugi::xpath_node context(root);
    
    // Setup xpath_variable_set for parameterized queries
    pugi::xpath_variable_set vars;
    
    // Add variables to the set
    // Use indices for each type vector (already declared above)
    
    for (size_t i = 0; i < var_names.size(); i++) {
        switch (var_types[i]) {
            case 0: // boolean
                if (bool_idx < var_bool_values.size() && 
                    vars.add(var_names[i].c_str(), pugi::xpath_type_boolean)) {
                    vars.set(var_names[i].c_str(), var_bool_values[bool_idx++]);
                }
                break;
            case 1: // number
                if (num_idx < var_number_values.size() && 
                    vars.add(var_names[i].c_str(), pugi::xpath_type_number)) {
                    vars.set(var_names[i].c_str(), var_number_values[num_idx++]);
                }
                break;
            case 2: // string
                if (str_idx < var_string_values.size() && 
                    vars.add(var_names[i].c_str(), pugi::xpath_type_string)) {
                    vars.set(var_names[i].c_str(), var_string_values[str_idx++].c_str());
                }
                break;
            case 3: // node_set
                // For node_set variables, we need to create a node_set
                if (str_idx < var_string_values.size() && 
                    vars.add(var_names[i].c_str(), pugi::xpath_type_node_set)) {
                    // Create a node_set using select_nodes
                    pugi::xpath_node_set node_set = root.select_nodes("*");
                    if (node_set.empty()) {
                        // If empty, create a minimal node_set with root
                        pugi::xpath_node_set empty_set;
                        vars.set(var_names[i].c_str(), empty_set);
                    } else {
                        vars.set(var_names[i].c_str(), node_set);
                    }
                    str_idx++; // Consume a string value for node_set type
                }
                break;
        }
    }
    // Test retrieval of variables
    for (size_t i = 0; i < var_names.size(); i++) {
        pugi::xpath_variable* var = vars.get(var_names[i].c_str());
        (void)var; // Avoid unused warning
    }
    
    // Test all XPath evaluation functions for each expression
    for (const auto& xpath_expr : xpath_expressions) {
        if (xpath_expr.empty()) continue;
        
        try {
            // Create xpath_query object
            pugi::xpath_query query(xpath_expr.c_str(), var_count > 0 ? &vars : nullptr);
            
            // Check if query compilation was successful
            if (!query) {
                continue;
            }
            
            // Determine which evaluation functions to test based on mode
            switch (evaluation_mode) {
                case 0:
                    // Test pugi::xpath_query::evaluate_boolean
                    {
                        bool result = query.evaluate_boolean(context);
                        (void)result;
                    }
                    break;
                    
                case 1:
                    // Test pugi::xpath_query::evaluate_number
                    {
                        double result = query.evaluate_number(context);
                        (void)result;
                    }
                    break;
                    
                case 2:
                    // Test pugi::xpath_query::evaluate_string (STL version)
                    #ifndef PUGIXML_NO_STL
                    {
                        std::string result = query.evaluate_string(context);
                        (void)result;
                    }
                    #endif
                    break;
                    
                case 3:
                    // Test pugi::xpath_query::evaluate_string (buffer version)
                    {
                        char buffer[256];
                        size_t result_size = query.evaluate_string(buffer, sizeof(buffer), context);
                        (void)result_size;
                    }
                    break;
                    
                case 4:
                    // Test pugi::xpath_query::evaluate_node
                    {
                        pugi::xpath_node result = query.evaluate_node(context);
                        (void)result;
                    }
                    break;
                    
                case 5:
                    // Test pugi::xpath_query::evaluate_node_set
                    {
                        pugi::xpath_node_set result = query.evaluate_node_set(context);
                        (void)result;
                        
                        // Iterate through node set
                        for (pugi::xpath_node_set::const_iterator it = result.begin(); it != result.end(); ++it) {
                            (void)*it;
                        }
                    }
                    break;
                    
                case 6:
                    // Test pugi::xml_node::select_node with string query
                    {
                        pugi::xpath_node result = root.select_node(xpath_expr.c_str(), var_count > 0 ? &vars : nullptr);
                        (void)result;
                    }
                    break;
                    
                case 7:
                    // Test pugi::xml_node::select_nodes with string query
                    {
                        pugi::xpath_node_set result = root.select_nodes(xpath_expr.c_str(), var_count > 0 ? &vars : nullptr);
                        (void)result;
                    }
                    break;
                    
                case 8:
                    // Test pugi::xml_node::select_node with xpath_query object
                    {
                        pugi::xpath_node result = root.select_node(query);
                        (void)result;
                    }
                    break;
                    
                case 9:
                    // Test pugi::xml_node::select_nodes with xpath_query object
                    {
                        pugi::xpath_node_set result = root.select_nodes(query);
                        (void)result;
                    }
                    break;
                    
                case 10:
                    // Test pugi::xml_node::select_single_node (deprecated but still available)
                    {
                        pugi::xpath_node result = root.select_single_node(xpath_expr.c_str(), var_count > 0 ? &vars : nullptr);
                        (void)result;
                    }
                    break;
                    
                case 11:
                    // Test all evaluation types in sequence
                    if (test_all_evaluation_types) {
                        // 1. evaluate_boolean
                        bool bool_result = query.evaluate_boolean(context);
                        (void)bool_result;
                        
                        // 2. evaluate_number
                        double num_result = query.evaluate_number(context);
                        (void)num_result;
                        
                        // 3. evaluate_string (both versions)
                        #ifndef PUGIXML_NO_STL
                        std::string str_result = query.evaluate_string(context);
                        (void)str_result;
                        #endif
                        
                        if (use_string_buffer_version) {
                            char buffer[512];
                            size_t buf_size = query.evaluate_string(buffer, sizeof(buffer), context);
                            (void)buf_size;
                        }
                        
                        // 4. evaluate_node
                        pugi::xpath_node node_result = query.evaluate_node(context);
                        (void)node_result;
                        
                        // 5. evaluate_node_set
                        pugi::xpath_node_set set_result = query.evaluate_node_set(context);
                        (void)set_result;
                        
                        // 6. select_node with string
                        pugi::xpath_node select_node_result = root.select_node(xpath_expr.c_str(), var_count > 0 ? &vars : nullptr);
                        (void)select_node_result;
                        
                        // 7. select_nodes with string
                        pugi::xpath_node_set select_nodes_result = root.select_nodes(xpath_expr.c_str(), var_count > 0 ? &vars : nullptr);
                        (void)select_nodes_result;
                    }
                    break;
            }
            
        } catch (...) {
            // Ignore exceptions - they are part of normal XPath evaluation
        }
    }
    
    // Additional testing: Create queries with complex XPath expressions
    if (fdp.remaining_bytes() > 100) {
        // Create more specialized XPath queries
        std::vector<std::string> complex_queries;
        complex_queries.push_back("//item[@active='true']");
        complex_queries.push_back("//item[@category='A' and @value>50]");
        complex_queries.push_back("//item[position() mod 2 = 0]");
        complex_queries.push_back("//item/text()");
        complex_queries.push_back("//item/@*");
        complex_queries.push_back("count(//item)");
        complex_queries.push_back("string(//special)");
        complex_queries.push_back("boolean(//item[@special='yes'])");
        complex_queries.push_back("//item[contains(text(), 'content')]");
        complex_queries.push_back("//nested/../@id");
        
        for (const auto& query_str : complex_queries) {
            try {
                pugi::xpath_query query(query_str.c_str());
                if (query) {
                    // Test multiple evaluation types
                    query.evaluate_boolean(context);
                    query.evaluate_number(context);
                    #ifndef PUGIXML_NO_STL
                    query.evaluate_string(context);
                    #endif
                    query.evaluate_node(context);
                    query.evaluate_node_set(context);
                    
                    // Test node selection methods
                    root.select_node(query);
                    root.select_nodes(query);
                    root.select_node(query_str.c_str());
                    root.select_nodes(query_str.c_str());
                }
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Test variable set operations more thoroughly
    if (var_count > 0) {
        // Create a new variable set and test add/set/get operations
        pugi::xpath_variable_set vars2;
        
        // Add variables of different types
        if (fdp.remaining_bytes() > 50) {
            // Boolean variable
            if (vars2.add("test_bool", pugi::xpath_type_boolean)) {
                vars2.set("test_bool", fdp.ConsumeBool());
            }
            
            // Number variable
            if (vars2.add("test_num", pugi::xpath_type_number)) {
                vars2.set("test_num", fdp.ConsumeFloatingPoint<double>());
            }
            
            // String variable
            if (vars2.add("test_str", pugi::xpath_type_string)) {
                vars2.set("test_str", fdp.ConsumeRandomLengthString(100).c_str());
            }
            
            // Test retrieval
            pugi::xpath_variable* bool_var = vars2.get("test_bool");
            pugi::xpath_variable* num_var = vars2.get("test_num");
            pugi::xpath_variable* str_var = vars2.get("test_str");
            (void)bool_var; (void)num_var; (void)str_var;
            
            // Test query with new variable set
            if (fdp.remaining_bytes() > 20) {
                std::string test_query = fdp.ConsumeRandomLengthString(100);
                if (!test_query.empty()) {
                    try {
                        pugi::xpath_query query_with_vars(test_query.c_str(), &vars2);
                        if (query_with_vars) {
                            query_with_vars.evaluate_boolean(context);
                        }
                    } catch (...) {
                        // Ignore exceptions
                    }
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_008
