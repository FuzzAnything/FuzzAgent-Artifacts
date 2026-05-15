#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>

namespace harness_014 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for comprehensive xpath_variable_set testing
    if (size < 96) return 0;  // Ensure size/3 >= 32 for valid range in ConsumeIntegralInRange
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Parse XML document or create one for testing
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(32, size / 3);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, create a structured document
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        
        // Add elements with attributes for node-set testing
        for (int i = 0; i < 5; i++) {
            pugi::xml_node elem = root.append_child("element");
            elem.append_attribute("id") = std::to_string(i).c_str();
            elem.append_attribute("value") = (i * 10.5);
            elem.append_child(pugi::node_pcdata).set_value(("text" + std::to_string(i)).c_str());
        }
    }
    
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        root = doc.first_child();
        if (root.empty()) return 0;
    }
    
    // Step 2: Create primary xpath_variable_set with comprehensive testing
    pugi::xpath_variable_set vars1;
    
    // Add variables of all types with fuzzed names and values
    std::vector<std::string> var_names;
    size_t var_count = fdp.ConsumeIntegralInRange<size_t>(1, 15);
    
    for (size_t i = 0; i < var_count && fdp.remaining_bytes() > 20; ++i) {
        std::string var_name = fdp.ConsumeRandomLengthString(30);
        var_names.push_back(var_name);
        
        int var_type = fdp.ConsumeIntegralInRange(0, 3);
        
        switch (var_type) {
            case 0: {  // boolean
                bool val = fdp.ConsumeBool();
                vars1.add(var_name.c_str(), pugi::xpath_type_boolean);
                vars1.set(var_name.c_str(), val);
                break;
            }
            case 1: {  // number
                double val = fdp.ConsumeFloatingPointInRange<double>(-1000.0, 1000.0);
                vars1.add(var_name.c_str(), pugi::xpath_type_number);
                vars1.set(var_name.c_str(), val);
                break;
            }
            case 2: {  // string
                std::string val = fdp.ConsumeRandomLengthString(100);
                vars1.add(var_name.c_str(), pugi::xpath_type_string);
                vars1.set(var_name.c_str(), val.c_str());
                break;
            }
            case 3: {  // node-set
                pugi::xpath_node_set node_set = root.select_nodes("//*");
                vars1.add(var_name.c_str(), pugi::xpath_type_node_set);
                vars1.set(var_name.c_str(), node_set);
                break;
            }
        }
    }
    
    // TARGET 1: Test copy constructor - critical coverage gap
    // This exercises the complex assignment operator with 15+ undiscovered branches
    pugi::xpath_variable_set vars_copy_ctor(vars1);  // Copy constructor
    
    // Verify copied variables
    for (const auto& name : var_names) {
        const pugi::xpath_variable* orig_var = vars1.get(name.c_str());
        const pugi::xpath_variable* copied_var = vars_copy_ctor.get(name.c_str());
        
        if (orig_var && copied_var) {
            // Verify types match
            if (orig_var->type() != copied_var->type()) {
                // Type mismatch - this shouldn't happen with proper copy
            }
            
            // Access values to exercise getters
            switch (orig_var->type()) {
                case pugi::xpath_type_boolean:
                    (void)orig_var->get_boolean();
                    (void)copied_var->get_boolean();
                    break;
                case pugi::xpath_type_number:
                    (void)orig_var->get_number();
                    (void)copied_var->get_number();
                    break;
                case pugi::xpath_type_string:
                    (void)orig_var->get_string();
                    (void)copied_var->get_string();
                    break;
                case pugi::xpath_type_node_set:
                    (void)orig_var->get_node_set();
                    (void)copied_var->get_node_set();
                    break;
                default:
                    break;
            }
        }
    }
    
    // TARGET 2: Test assignment operator - the main coverage gap
    pugi::xpath_variable_set vars_assigned;
    
    // First add some different variables to vars_assigned
    if (fdp.remaining_bytes() > 30) {
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; ++i) {
            std::string temp_name = "temp_" + std::to_string(i);
            double temp_val = fdp.ConsumeFloatingPoint<double>();
            vars_assigned.add(temp_name.c_str(), pugi::xpath_type_number);
            vars_assigned.set(temp_name.c_str(), temp_val);
        }
    }
    
    // Now perform assignment - this should trigger the complex assignment logic
    vars_assigned = vars1;  // Assignment operator
    
    // Verify assignment worked
    for (const auto& name : var_names) {
        const pugi::xpath_variable* assigned_var = vars_assigned.get(name.c_str());
        if (assigned_var) {
            // Exercise type-specific getters
            switch (assigned_var->type()) {
                case pugi::xpath_type_boolean:
                    (void)assigned_var->get_boolean();
                    break;
                case pugi::xpath_type_number:
                    (void)assigned_var->get_number();
                    break;
                case pugi::xpath_type_string:
                    (void)assigned_var->get_string();
                    break;
                case pugi::xpath_type_node_set:
                    (void)assigned_var->get_node_set();
                    break;
                default:
                    break;
            }
        }
    }
    
    // TARGET 3: Test self-assignment (edge case)
    vars_assigned = vars_assigned;  // Self-assignment
    
    // TARGET 4: Test move semantics if available
    #ifdef PUGIXML_HAS_MOVE
    if (fdp.remaining_bytes() > 20) {
        // Create a temporary variable set
        pugi::xpath_variable_set temp_vars;
        std::string move_var_name = fdp.ConsumeRandomLengthString(20);
        bool move_var_val = fdp.ConsumeBool();
        temp_vars.add(move_var_name.c_str(), pugi::xpath_type_boolean);
        temp_vars.set(move_var_name.c_str(), move_var_val);
        
        // Test move constructor
        pugi::xpath_variable_set moved_vars(std::move(temp_vars));
        
        // Test move assignment
        pugi::xpath_variable_set move_assigned;
        move_assigned = std::move(moved_vars);
    }
    #endif
    
    // TARGET 5: Test variable retrieval operations with edge cases
    for (const auto& name : var_names) {
        // Test get() with const and non-const versions
        pugi::xpath_variable* var = vars1.get(name.c_str());
        const pugi::xpath_variable* const_var = vars1.get(name.c_str());
        
        if (var && const_var) {
            // Test type() method
            pugi::xpath_value_type var_type = var->type();
            
            // Test name() method
            const char* var_name_ptr = var->name();
            
            // Test set() with different types (type mismatch testing)
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                switch (var_type) {
                    case pugi::xpath_type_boolean:
                        // Try to set with wrong type (should fail)
                        (void)var->set(fdp.ConsumeFloatingPoint<double>());
                        (void)var->set("wrong type string");
                        break;
                    case pugi::xpath_type_number:
                        (void)var->set(fdp.ConsumeBool());
                        (void)var->set("wrong type string");
                        break;
                    case pugi::xpath_type_string:
                        (void)var->set(fdp.ConsumeBool());
                        (void)var->set(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case pugi::xpath_type_node_set:
                        (void)var->set(fdp.ConsumeBool());
                        (void)var->set(fdp.ConsumeFloatingPoint<double>());
                        (void)var->set("wrong type string");
                        break;
                    default:
                        break;
                }
            }
        }
        
        // Test get() with non-existent name
        std::string non_existent = "non_existent_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
        pugi::xpath_variable* missing_var = vars1.get(non_existent.c_str());
        // missing_var should be null
    }
    
    // TARGET 6: Test add() with duplicate names and type mismatches
    if (!var_names.empty() && fdp.remaining_bytes() > 10) {
        std::string existing_name = var_names[0];
        
        // Try to add variable with same name but different type
        int new_type = fdp.ConsumeIntegralInRange(0, 3);
        pugi::xpath_variable* dup_var = vars1.add(existing_name.c_str(), 
            static_cast<pugi::xpath_value_type>(new_type));
        // dup_var might be null or return existing variable
        
        // Try to add variable with same name and same type
        pugi::xpath_variable* existing_var = vars1.get(existing_name.c_str());
        if (existing_var) {
            pugi::xpath_variable* same_type_var = vars1.add(existing_name.c_str(), existing_var->type());
        }
    }
    // TARGET 7: Test integration with xpath_query
    if (fdp.remaining_bytes() > 50 && !var_names.empty()) {
        // Create XPath queries that use variables
        std::vector<std::string> xpath_queries;
        size_t query_count = fdp.ConsumeIntegralInRange<size_t>(1, 5);
        
        for (size_t i = 0; i < query_count && fdp.remaining_bytes() > 20; ++i) {
            std::string query = fdp.ConsumeRandomLengthString(100);
            xpath_queries.push_back(query);
        }
        
        // Add some common XPath patterns
        xpath_queries.push_back("//*[@id]");
        xpath_queries.push_back("//element");
        xpath_queries.push_back("/root/*");
        
        for (const auto& query : xpath_queries) {
            try {
                // Create xpath_query with variable set
                pugi::xpath_query xq(query.c_str(), &vars1);
                
                if (xq) {
                    // Test all evaluation methods
                    bool bool_result = xq.evaluate_boolean(root);
                    (void)bool_result;
                    
                    double num_result = xq.evaluate_number(root);
                    (void)num_result;
                    
                    #ifndef PUGIXML_NO_STL
                    std::string str_result = xq.evaluate_string(root).c_str();
                    (void)str_result;
                    #endif
                    
                    pugi::xpath_node_set node_set_result = xq.evaluate_node_set(root);
                    (void)node_set_result;
                    
                    // Test return_type()
                    pugi::xpath_value_type return_type = xq.return_type();
                    (void)return_type;
                }
            } catch (...) {
                // Ignore exceptions from invalid queries
            }
        }
        
        // Test select_nodes and select_single_node with variable set
        for (const auto& query : xpath_queries) {
            try {
                pugi::xpath_node_set nodes = root.select_nodes(query.c_str(), &vars1);
                (void)nodes;
                
                pugi::xpath_node single_node = root.select_single_node(query.c_str(), &vars1);
                (void)single_node;
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // TARGET 8: Test variable value updates and type changes
    if (!var_names.empty() && fdp.remaining_bytes() > 20) {
        for (const auto& name : var_names) {
            pugi::xpath_variable* var = vars1.get(name.c_str());
            if (var) {
                // Update value with same type
                switch (var->type()) {
                    case pugi::xpath_type_boolean:
                        (void)vars1.set(name.c_str(), fdp.ConsumeBool());
                        break;
                    case pugi::xpath_type_number:
                        (void)vars1.set(name.c_str(), fdp.ConsumeFloatingPoint<double>());
                        break;
                    case pugi::xpath_type_string:
                        if (fdp.remaining_bytes() > 10) {
                            std::string new_str = fdp.ConsumeRandomLengthString(50);
                            (void)vars1.set(name.c_str(), new_str.c_str());
                        }
                        break;
                    case pugi::xpath_type_node_set:
                        if (fdp.remaining_bytes() > 5) {
                            pugi::xpath_node_set new_set = root.select_nodes("/root");
                            (void)vars1.set(name.c_str(), new_set);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    // TARGET 9: Test variable set operations with empty/null cases
    pugi::xpath_variable_set empty_vars;
    
    // Copy empty set
    pugi::xpath_variable_set empty_copy(empty_vars);
    
    // Assign empty set to non-empty set
    if (fdp.ConsumeBool()) {
        vars_assigned = empty_vars;
    }
    
    // Test get() on empty set
    pugi::xpath_variable* no_var = empty_vars.get("nonexistent");
    
    // Test set() on empty set
    (void)empty_vars.set("nonexistent", true);
    (void)empty_vars.set("nonexistent", 3.14);
    (void)empty_vars.set("nonexistent", "test");
    
    // TARGET 10: Test variable lifetime and destruction
    {
        // Create local scope variable set
        pugi::xpath_variable_set local_vars;
        
        // Add variables
        if (fdp.remaining_bytes() > 30) {
            for (int i = 0; i < 3; ++i) {
                std::string local_name = "local_" + std::to_string(i);
                double local_val = fdp.ConsumeFloatingPoint<double>();
                local_vars.add(local_name.c_str(), pugi::xpath_type_number);
                local_vars.set(local_name.c_str(), local_val);
            }
        }
        
        // Copy to outer scope (tests destruction of local copy)
        pugi::xpath_variable_set local_copy(local_vars);
        
        // local_vars will be destroyed when scope ends
    }
    
    // The copied variables should still be accessible
    for (const auto& name : var_names) {
        const pugi::xpath_variable* var = vars_copy_ctor.get(name.c_str());
        if (var) {
            // Final access to ensure everything works
            (void)var->type();
        }
    }
    
    return 0;
}

} // namespace harness_014
