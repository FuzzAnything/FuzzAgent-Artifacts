#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <utility>

namespace harness_011 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for xpath_variable_set assignment operator testing
    // Need enough data for XML content, variable names/values, and XPath expressions
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t var_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);  // At least 1 variable
    uint8_t test_sequence = fdp.ConsumeIntegral<uint8_t>() % 4;  // Different test sequences
    bool test_move_semantics = fdp.ConsumeBool();
    bool test_copy_after_modification = fdp.ConsumeBool();
    bool test_nested_queries = fdp.ConsumeBool();
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(2048);
    
    // Consume XPath expressions for variable testing
    std::vector<std::string> xpath_expressions;
    for (int i = 0; i < 4 && fdp.remaining_bytes() > 20; i++) {
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(512));
    }
    
    // Consume variable names and values for parameterized queries
    std::vector<std::string> var_names;
    std::vector<uint8_t> var_types; // 0=boolean, 1=number, 2=string
    std::vector<std::string> var_string_values;
    std::vector<double> var_number_values;
    std::vector<bool> var_bool_values;
    
    // Separate indices for each type vector
    size_t bool_idx = 0;
    size_t num_idx = 0;
    size_t str_idx = 0;
    
    for (int i = 0; i < var_count && fdp.remaining_bytes() > 30; i++) {
        // Create variable names from fuzzer input
        std::string name = fdp.ConsumeRandomLengthString(32);
        if (name.empty()) name = "var" + std::to_string(i);
        var_names.push_back(name);
        
        // Determine variable type (focus on boolean, number, string for simplicity)
        uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 3;
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
        }
    }
    
    // Step 1: Create XML document and parse XML content
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_string(xml_content.c_str(), parse_options);
    
    // If parsing failed, create a simple document for XPath variable testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        
        // Create a structured document with predictable content for XPath testing
        for (int i = 0; i < 5; i++) {
            std::string node_name = "node" + std::to_string(i);
            pugi::xml_node child = root.append_child(node_name.c_str());
            child.append_attribute("id") = std::to_string(i).c_str();
            child.append_attribute("value") = std::to_string(i * 10).c_str();
            child.text().set(("Text content " + std::to_string(i)).c_str());
        }
        
        // Add boolean-like elements
        pugi::xml_node flags = root.append_child("flags");
        flags.append_child("true").text().set("1");
        flags.append_child("false").text().set("0");
        
        // Add numeric elements
        pugi::xml_node numbers = root.append_child("numbers");
        for (int i = 0; i < 3; i++) {
            pugi::xml_node num = numbers.append_child(("num" + std::to_string(i)).c_str());
            num.text().set(std::to_string(i * 100).c_str());
        }
        
        // Add string elements
        pugi::xml_node strings = root.append_child("strings");
        strings.append_child("hello").text().set("world");
        strings.append_child("foo").text().set("bar");
    }
    
    // Get document root for XPath evaluation
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }
    
    // Create xpath_node for evaluation context
    pugi::xpath_node context(root);
    
    // Step 2: Create first xpath_variable_set and add variables
    pugi::xpath_variable_set vars1;
    
    // Add variables to the first set
    for (size_t i = 0; i < var_names.size(); i++) {
        switch (var_types[i]) {
            case 0: // boolean
                if (bool_idx < var_bool_values.size() && 
                    vars1.add(var_names[i].c_str(), pugi::xpath_type_boolean)) {
                    vars1.set(var_names[i].c_str(), var_bool_values[bool_idx++]);
                }
                break;
            case 1: // number
                if (num_idx < var_number_values.size() && 
                    vars1.add(var_names[i].c_str(), pugi::xpath_type_number)) {
                    vars1.set(var_names[i].c_str(), var_number_values[num_idx++]);
                }
                break;
            case 2: // string
                if (str_idx < var_string_values.size() && 
                    vars1.add(var_names[i].c_str(), pugi::xpath_type_string)) {
                    vars1.set(var_names[i].c_str(), var_string_values[str_idx++].c_str());
                }
                break;
        }
    }
    
    // Step 3: Test pugi::xpath_variable_set::get function on vars1
    // This tests the get() function which has 7 undiscovered branches
    for (size_t i = 0; i < var_names.size(); i++) {
        // Test both const and non-const get versions
        pugi::xpath_variable* var = vars1.get(var_names[i].c_str());
        const pugi::xpath_variable* const_var = const_cast<const pugi::xpath_variable_set&>(vars1).get(var_names[i].c_str());
        (void)var;
        (void)const_var;
    }
    
    // Step 4: Test copy assignment operator (16 undiscovered branches)
    pugi::xpath_variable_set vars2;
    
    // First add a different variable to vars2 to test overwrite
    if (fdp.remaining_bytes() > 10) {
        std::string temp_name = fdp.ConsumeRandomLengthString(16);
        if (!temp_name.empty()) {
            vars2.add(temp_name.c_str(), pugi::xpath_type_string);
            vars2.set(temp_name.c_str(), "temp_value");
        }
    }
    
    // Now test copy assignment: vars2 = vars1
    vars2 = vars1;  // Copy assignment operator
    
    // Verify that vars2 contains all variables from vars1
    for (size_t i = 0; i < var_names.size(); i++) {
        pugi::xpath_variable* var1 = vars1.get(var_names[i].c_str());
        pugi::xpath_variable* var2 = vars2.get(var_names[i].c_str());
        
        // Both should be non-null if the variable was successfully added
        (void)var1;
        (void)var2;
    }
    // Step 5: Test move assignment operator (2 undiscovered branches) if supported
    pugi::xpath_variable_set vars3;
    bool vars3_initialized = false;
    
    #ifdef PUGIXML_HAS_MOVE
    if (test_move_semantics) {
        // Add some variables to vars3
        if (fdp.remaining_bytes() > 10) {
            std::string move_name = fdp.ConsumeRandomLengthString(16);
            if (!move_name.empty()) {
                vars3.add(move_name.c_str(), pugi::xpath_type_number);
                vars3.set(move_name.c_str(), 42.0);
            }
        }
        
        // Test move assignment: vars3 = std::move(vars1)
        vars3 = std::move(vars1);  // Move assignment operator
        vars3_initialized = true;
        
        // vars1 should now be in a valid but unspecified state
        // vars3 should have the variables that were in vars1
        
        // Test that vars3 has the variables
        for (size_t i = 0; i < var_names.size(); i++) {
            pugi::xpath_variable* var3 = vars3.get(var_names[i].c_str());
            (void)var3;
        }
        
        // Test copy after modification scenario
        if (test_copy_after_modification) {
            // Modify vars3
            if (!var_names.empty()) {
                std::string first_var = var_names[0];
                pugi::xpath_variable* var = vars3.get(first_var.c_str());
                if (var && var->type() == pugi::xpath_type_string) {
                    vars3.set(first_var.c_str(), "modified_value");
                }
            }
            
            // Copy modified vars3 to another set
            pugi::xpath_variable_set vars4 = vars3;  // Copy constructor
            (void)vars4;
        }
    }
    #endif
    
    // Step 6: Create xpath_query with variable sets to test variable usage in XPath evaluation
    for (const auto& xpath_expr : xpath_expressions) {
        if (xpath_expr.empty()) continue;
        
        try {
            // Test query with vars2 (copied set)
            pugi::xpath_query query1(xpath_expr.c_str(), &vars2);
            
            if (query1) {
                // Test pugi::xpath_query::evaluate_string with variables
                std::string result1 = query1.evaluate_string(context);
                (void)result1;
                
                // Also test evaluate_boolean and evaluate_number for coverage
                bool bool_result = query1.evaluate_boolean(context);
                double num_result = query1.evaluate_number(context);
                pugi::xpath_node_set node_result = query1.evaluate_node_set(context);
                (void)bool_result;
                (void)num_result;
                (void)node_result;
            }
            
            // Test nested queries with different variable sets
            if (test_nested_queries && fdp.remaining_bytes() > 20) {
                std::string nested_expr = fdp.ConsumeRandomLengthString(256);
                if (!nested_expr.empty()) {
                    #ifdef PUGIXML_HAS_MOVE
                    if (test_move_semantics && vars3_initialized) {
                        // Use vars3 (move-assigned set) for nested query
                        pugi::xpath_query nested_query(nested_expr.c_str(), &vars3);
                        if (nested_query) {
                            std::string nested_result = nested_query.evaluate_string(context);
                            (void)nested_result;
                        }
                    }
                    #endif
                }
            }
            
        } catch (...) {
            // Ignore exceptions from invalid XPath expressions
        }
    }
    
    // Step 7: Test assignment chain operations
    switch (test_sequence) {
        case 0:
            // Test vars2 = vars1; vars3 = vars2; (chain copy)
            {
                pugi::xpath_variable_set vars_chain;
                vars_chain = vars2;  // Copy from already-copied set
                (void)vars_chain;
            }
            break;
            
        case 1:
            // Test self-assignment: vars2 = vars2;
            vars2 = vars2;
            break;
            
        case 2:
            // Test assignment with empty set
            {
                pugi::xpath_variable_set empty_set;
                pugi::xpath_variable_set target_set;
                
                // Add something to target_set first
                target_set.add("test_var", pugi::xpath_type_boolean);
                target_set.set("test_var", true);
                
                // Assign empty set
                target_set = empty_set;
                
                // Verify target_set is now empty
                pugi::xpath_variable* var = target_set.get("test_var");
                (void)var;
            }
            break;
            
        case 3:
            // Test get() on non-existent variables
            if (fdp.remaining_bytes() > 10) {
                std::string non_existent = fdp.ConsumeRandomLengthString(32);
                pugi::xpath_variable* var = vars2.get(non_existent.c_str());
                (void)var;
            }
            break;
    }
    
    // Step 8: Final cleanup
    doc.reset();
    
    return 0;
}

} // namespace harness_011
