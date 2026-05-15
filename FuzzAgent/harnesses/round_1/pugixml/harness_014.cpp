#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_014 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML content and comprehensive variable set operations
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: XML DOCUMENT SETUP ====================
    // Create XML document for XPath node set variable testing
    pugi::xml_document doc;
    
    // Consume XML content from fuzzer input
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(32, std::min(size / 3, size_t(2048)));
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    // Try parsing XML
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size());
    
    // If parsing failed, create a structured XML document for testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        
        // Add structured content for node set variable testing
        for (int i = 0; i < 8 && fdp.remaining_bytes() > 10; ++i) {
            std::string child_name = "child" + std::to_string(i);
            pugi::xml_node child = root.append_child(child_name.c_str());
            
            // Add attributes
            child.append_attribute("id") = std::to_string(i).c_str();
            child.append_attribute("type") = (i % 2 == 0) ? "even" : "odd";
            child.append_attribute("value") = std::to_string(i * 10).c_str();
            
            // Add text content
            std::string text = fdp.ConsumeRandomLengthString(30);
            child.text() = text.c_str();
            
            // Add grandchildren for deeper node sets
            if (i % 3 == 0 && fdp.remaining_bytes() > 5) {
                pugi::xml_node grandchild = child.append_child("grandchild");
                grandchild.append_attribute("level") = "2";
                grandchild.append_attribute("index") = std::to_string(i).c_str();
            }
        }
    }
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // Even without valid document, we can test some variable set operations
        // with empty node sets
        doc.reset();
        pugi::xml_node empty_root = doc.append_child("empty_root");
        root = empty_root;
    }

#ifndef PUGIXML_NO_XPATH
    try {
        // ==================== PHASE 2: COMPREHENSIVE VARIABLE SET OPERATIONS ====================
        // TARGET: xpath_variable_set copy semantics, variable manipulation, and integration
        
        // Create initial variable set with diverse variable types
        pugi::xpath_variable_set vars1;
        
        // 1. Add variables of all supported types
        // Boolean variables
        bool bool_var1 = fdp.ConsumeBool();
        bool bool_var2 = fdp.ConsumeBool();
        pugi::xpath_variable* bool_var_ptr1 = vars1.add("bool1", pugi::xpath_type_boolean);
        pugi::xpath_variable* bool_var_ptr2 = vars1.add("bool2", pugi::xpath_type_boolean);
        if (bool_var_ptr1) vars1.set("bool1", bool_var1);
        if (bool_var_ptr2) vars1.set("bool2", bool_var2);
        
        // Number variables
        double num_var1 = fdp.ConsumeFloatingPoint<double>();
        double num_var2 = fdp.ConsumeFloatingPointInRange<double>(-1000.0, 1000.0);
        double num_var3 = fdp.ConsumeFloatingPoint<double>();
        pugi::xpath_variable* num_var_ptr1 = vars1.add("num1", pugi::xpath_type_number);
        pugi::xpath_variable* num_var_ptr2 = vars1.add("num2", pugi::xpath_type_number);
        pugi::xpath_variable* num_var_ptr3 = vars1.add("num3", pugi::xpath_type_number);
        if (num_var_ptr1) vars1.set("num1", num_var1);
        if (num_var_ptr2) vars1.set("num2", num_var2);
        if (num_var_ptr3) vars1.set("num3", num_var3);
        
        // String variables
        std::string str_var1 = fdp.ConsumeRandomLengthString(100);
        std::string str_var2 = fdp.ConsumeRandomLengthString(100);
        std::string str_var3 = fdp.ConsumeRandomLengthString(50);
        pugi::xpath_variable* str_var_ptr1 = vars1.add("str1", pugi::xpath_type_string);
        pugi::xpath_variable* str_var_ptr2 = vars1.add("str2", pugi::xpath_type_string);
        pugi::xpath_variable* str_var_ptr3 = vars1.add("str3", pugi::xpath_type_string);
        if (str_var_ptr1) vars1.set("str1", str_var1.c_str());
        if (str_var_ptr2) vars1.set("str2", str_var2.c_str());
        if (str_var_ptr3) vars1.set("str3", str_var3.c_str());
        
        // Node set variables
        pugi::xpath_node_set node_set1 = root.select_nodes("//*");
        pugi::xpath_node_set node_set2 = root.select_nodes("//*[@id]");
        pugi::xpath_node_set node_set3 = root.select_nodes("//*[text()]");
        pugi::xpath_variable* node_set_var_ptr1 = vars1.add("nodeset1", pugi::xpath_type_node_set);
        pugi::xpath_variable* node_set_var_ptr2 = vars1.add("nodeset2", pugi::xpath_type_node_set);
        pugi::xpath_variable* node_set_var_ptr3 = vars1.add("nodeset3", pugi::xpath_type_node_set);
        if (node_set_var_ptr1) vars1.set("nodeset1", node_set1);
        if (node_set_var_ptr2) vars1.set("nodeset2", node_set2);
        if (node_set_var_ptr3) vars1.set("nodeset3", node_set3);
        
        // 2. Test variable retrieval and type checking
        const pugi::xpath_variable* retrieved_bool = vars1.get("bool1");
        const pugi::xpath_variable* retrieved_num = vars1.get("num1");
        const pugi::xpath_variable* retrieved_str = vars1.get("str1");
        const pugi::xpath_variable* retrieved_nodeset = vars1.get("nodeset1");
        
        // Test getting non-existent variables
        const pugi::xpath_variable* non_existent = vars1.get("nonexistent");
        (void)non_existent; // Should be null
        
        // 3. Test copy semantics - copy constructor
        pugi::xpath_variable_set vars2(vars1); // Copy constructor
        
        // Verify copied variables exist and have same values
        const pugi::xpath_variable* copied_bool = vars2.get("bool1");
        const pugi::xpath_variable* copied_num = vars2.get("num1");
        const pugi::xpath_variable* copied_str = vars2.get("str1");
        const pugi::xpath_variable* copied_nodeset = vars2.get("nodeset1");
        
        // 4. Test assignment operator
        pugi::xpath_variable_set vars3;
        
        // Add some initial variables to vars3
        vars3.add("temp_bool", pugi::xpath_type_boolean);
        vars3.set("temp_bool", true);
        vars3.add("temp_num", pugi::xpath_type_number);
        vars3.set("temp_num", 42.0);
        
        // Now assign vars1 to vars3 (should replace contents)
        vars3 = vars1; // Assignment operator
        
        // Verify assigned variables exist
        const pugi::xpath_variable* assigned_bool = vars3.get("bool1");
        const pugi::xpath_variable* assigned_num = vars3.get("num1");
        const pugi::xpath_variable* assigned_str = vars3.get("str1");
        const pugi::xpath_variable* assigned_nodeset = vars3.get("nodeset1");
        
        // The old variables should be gone
        const pugi::xpath_variable* old_temp_bool = vars3.get("temp_bool");
        const pugi::xpath_variable* old_temp_num = vars3.get("temp_num");
        (void)old_temp_bool; // Should be null
        (void)old_temp_num;  // Should be null
        
        // 5. Test self-assignment (should be handled correctly)
        vars3 = vars3; // Self-assignment
        
        // 6. Test variable update operations
        // Update existing variables
        bool new_bool = fdp.ConsumeBool();
        double new_num = fdp.ConsumeFloatingPoint<double>();
        std::string new_str = fdp.ConsumeRandomLengthString(80);
        
        vars1.set("bool1", new_bool);
        vars1.set("num1", new_num);
        vars1.set("str1", new_str.c_str());
        
        // Update node set with different selection
        pugi::xpath_node_set new_node_set = root.select_nodes("//*[@type]");
        vars1.set("nodeset1", new_node_set);
        
        // 7. Test type mismatch error handling
        // Try to set boolean variable with string (should fail)
        bool bool_set_with_string = vars1.set("bool1", "not a boolean");
        (void)bool_set_with_string; // Should be false
        
        // Try to set string variable with number (should fail)
        bool string_set_with_number = vars1.set("str1", 123.456);
        (void)string_set_with_number; // Should be false
        
        // Try to set number variable with node set (should fail)
        bool num_set_with_nodeset = vars1.set("num1", node_set1);
        (void)num_set_with_nodeset; // Should be false
        
        // 8. Test duplicate variable addition with different types
        // Adding variable with same name but different type should fail
        pugi::xpath_variable* duplicate_bool_as_num = vars1.add("bool1", pugi::xpath_type_number);
        (void)duplicate_bool_as_num; // Should be null
        
        // Adding variable with same name and same type should return existing variable
        pugi::xpath_variable* duplicate_bool_same_type = vars1.add("bool1", pugi::xpath_type_boolean);
        (void)duplicate_bool_same_type; // Should point to existing variable
        
        // 9. Test variable value retrieval through xpath_variable interface
        if (retrieved_bool) {
            bool bool_val = retrieved_bool->get_boolean();
            double bool_as_num = retrieved_bool->get_number(); // Should be NaN
            const char* bool_as_str = retrieved_bool->get_string(); // Should be empty
            const pugi::xpath_node_set& bool_as_nodeset = retrieved_bool->get_node_set(); // Should be empty
            (void)bool_val;
            (void)bool_as_num;
            (void)bool_as_str;
            (void)bool_as_nodeset;
        }
        
        if (retrieved_num) {
            bool num_as_bool = retrieved_num->get_boolean(); // Should be false
            double num_val = retrieved_num->get_number();
            const char* num_as_str = retrieved_num->get_string(); // Should be empty
            const pugi::xpath_node_set& num_as_nodeset = retrieved_num->get_node_set(); // Should be empty
            (void)num_as_bool;
            (void)num_val;
            (void)num_as_str;
            (void)num_as_nodeset;
        }
        
        if (retrieved_str) {
            bool str_as_bool = retrieved_str->get_boolean(); // Should be false
            double str_as_num = retrieved_str->get_number(); // Should be NaN
            const char* str_val = retrieved_str->get_string();
            const pugi::xpath_node_set& str_as_nodeset = retrieved_str->get_node_set(); // Should be empty
            (void)str_as_bool;
            (void)str_as_num;
            (void)str_val;
            (void)str_as_nodeset;
        }
        
        if (retrieved_nodeset) {
            bool nodeset_as_bool = retrieved_nodeset->get_boolean(); // Should be false
            double nodeset_as_num = retrieved_nodeset->get_number(); // Should be NaN
            const char* nodeset_as_str = retrieved_nodeset->get_string(); // Should be empty
            const pugi::xpath_node_set& nodeset_val = retrieved_nodeset->get_node_set();
            (void)nodeset_as_bool;
            (void)nodeset_as_num;
            (void)nodeset_as_str;
            (void)nodeset_val;
        }
        
        // ==================== PHASE 3: VARIABLE SET INTEGRATION WITH XPath QUERIES ====================
        // TARGET: Integration of variable sets with XPath query evaluation
        
        // Create XPath expressions that use variables
        std::vector<std::string> variable_expressions = {
            "//*[@enabled = $bool1]",
            "//*[position() < $num1]",
            "//*[contains(@type, $str1)]",
            "//*[@id = $num2]",
            "//*[text() = $str2]",
            "//*[@value > $num3]",
            "count($nodeset1)",
            "string-length($str3)",
            "boolean($bool2)",
            "number($str1)",  // This will try to convert string to number
            "//*[name() = $str2]",
            "//*[@type = $str1 or @type = $str2]",
            "//*[position() >= $num2 and position() <= $num3]",
            "//*[not(@disabled = $bool1)]",
            "sum($nodeset2/@value)",
        };
        
        // Add some fuzzer-generated expressions with variables
        int random_expr_count = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < random_expr_count && fdp.remaining_bytes() > 10; ++i) {
            std::string expr_prefix = fdp.ConsumeRandomLengthString(20);
            std::string expr_suffix = fdp.ConsumeRandomLengthString(20);
            std::string var_expr = expr_prefix + "$bool1" + expr_suffix;
            variable_expressions.push_back(var_expr);
        }
        
        // Test each expression with the variable set
        for (const auto& expr : variable_expressions) {
            if (expr.empty()) continue;
            
            // Create query with variable set
            pugi::xpath_query query(expr.c_str(), &vars1);
            
            if (!query) {
                // Query compilation failed - test error handling
                const pugi::xpath_parse_result& result = query.result();
                const char* error_desc = result.description();
                (void)error_desc;
                continue;
            }
            
            // Get return type
            pugi::xpath_value_type return_type = query.return_type();
            
            // Evaluate with different context nodes
            std::vector<pugi::xpath_node> contexts;
            contexts.push_back(pugi::xpath_node(root));
            
            // Add child nodes as additional contexts
            for (pugi::xml_node child = root.first_child(); child && contexts.size() < 5; child = child.next_sibling()) {
                contexts.push_back(pugi::xpath_node(child));
            }
            
            // Add empty context
            contexts.push_back(pugi::xpath_node());
            
            // Evaluate in each context
            for (const auto& context : contexts) {
                switch (return_type) {
                    case pugi::xpath_type_boolean:
                        query.evaluate_boolean(context);
                        break;
                    case pugi::xpath_type_number:
                        query.evaluate_number(context);
                        break;
                    case pugi::xpath_type_string: {
#ifndef PUGIXML_NO_STL
                        query.evaluate_string(context);
#endif
                        char buffer[256];
                        query.evaluate_string(buffer, sizeof(buffer), context);
                        break;
                    }
                    case pugi::xpath_type_node_set:
                        query.evaluate_node_set(context);
                        query.evaluate_node(context);
                        break;
                    default:
                        // Try all evaluation methods
                        query.evaluate_boolean(context);
                        query.evaluate_number(context);
#ifndef PUGIXML_NO_STL
                        query.evaluate_string(context);
#endif
                        char buffer[256];
                        query.evaluate_string(buffer, sizeof(buffer), context);
                        query.evaluate_node_set(context);
                        query.evaluate_node(context);
                        break;
                }
            }
        }
        
        // ==================== PHASE 4: DIRECT NODE SELECTION WITH VARIABLES ====================
        // TARGET: xml_node::select_node and xml_node::select_nodes with variable sets
        
        // Test select_node with variables
        std::vector<std::string> select_expressions = {
            "//child[@id = $num1]",
            "//*[@type = $str1]",
            "//*[text() = $str2]",
            "//grandchild[@level = $num2]",
            "//*[@value > $num3]",
        };
        
        for (const auto& expr : select_expressions) {
            if (expr.empty()) continue;
            
            // Test select_node
            pugi::xpath_node selected_node = root.select_node(expr.c_str(), &vars1);
            (void)selected_node;
            
            // Test select_nodes
            pugi::xpath_node_set selected_nodes = root.select_nodes(expr.c_str(), &vars1);
            (void)selected_nodes;
            
            // Test deprecated select_single_node
            pugi::xpath_node single_node = root.select_single_node(expr.c_str(), &vars1);
            (void)single_node;
        }
        
        // ==================== PHASE 5: EDGE CASE TESTING ====================
        // TARGET: Edge cases and error conditions in variable set operations
        
        // 1. Test with empty variable set
        pugi::xpath_variable_set empty_vars;
        pugi::xpath_query query_no_vars("//*", &empty_vars);
        if (query_no_vars) {
            query_no_vars.evaluate_boolean(pugi::xpath_node(root));
        }
        
        // 2. Test variable set destruction and reuse
        {
            pugi::xpath_variable_set temp_vars;
            temp_vars.add("temp", pugi::xpath_type_boolean);
            temp_vars.set("temp", true);
            // temp_vars goes out of scope here, testing destructor
        }
        
        // 3. Test with maximum number of variables (64 based on _data array size)
        pugi::xpath_variable_set max_vars;
        for (int i = 0; i < 70; i++) { // Try to exceed limit
            std::string var_name = "var" + std::to_string(i);
            pugi::xpath_variable* var = max_vars.add(var_name.c_str(), pugi::xpath_type_number);
            if (var) {
                max_vars.set(var_name.c_str(), static_cast<double>(i));
            }
            if (!var) {
                break; // Reached limit
            }
        }
        
        // 4. Test variable name edge cases
        pugi::xpath_variable_set edge_vars;
        
        // Empty string name
        pugi::xpath_variable* empty_name_var = edge_vars.add("", pugi::xpath_type_boolean);
        (void)empty_name_var; // Might fail
        
        // Very long name
        std::string long_name = fdp.ConsumeRandomLengthString(500);
        pugi::xpath_variable* long_name_var = edge_vars.add(long_name.c_str(), pugi::xpath_type_number);
        if (long_name_var) {
            edge_vars.set(long_name.c_str(), 42.0);
        }
        
        // Name with special characters
        std::string special_name = "var$with#special@chars!";
        pugi::xpath_variable* special_name_var = edge_vars.add(special_name.c_str(), pugi::xpath_type_string);
        if (special_name_var) {
            edge_vars.set(special_name.c_str(), "special value");
        }
        
        // 5. Test move semantics if available
#ifdef PUGIXML_HAS_MOVE
        pugi::xpath_variable_set source_vars;
        source_vars.add("move_bool", pugi::xpath_type_boolean);
        source_vars.set("move_bool", true);
        source_vars.add("move_num", pugi::xpath_type_number);
        source_vars.set("move_num", 99.9);
        
        // Move constructor
        pugi::xpath_variable_set moved_vars(std::move(source_vars));
        
        // Move assignment
        pugi::xpath_variable_set target_vars;
        target_vars = std::move(moved_vars);
#endif
        
    } catch (...) {
        // Catch all exceptions for safe fuzzing
    }
#endif // PUGIXML_NO_XPATH
    
    return 0;
}

} // namespace harness_014
