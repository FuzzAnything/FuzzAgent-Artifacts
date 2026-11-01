// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xpath_variable_set vars;

    // Determine the type to create based on fuzz data
    pugi::xpath_value_type target_type = pugi::xpath_type_none;
    if (Size > 0) {
        uint8_t type_selector = Data[0] % 5;
        switch (type_selector) {
            case 0: target_type = pugi::xpath_type_boolean; break;
            case 1: target_type = pugi::xpath_type_number; break;
            case 2: target_type = pugi::xpath_type_string; break;
            case 3: target_type = pugi::xpath_type_node_set; break;
            case 4: target_type = pugi::xpath_type_none; break;
        }
    }

    // Add a variable to the set. 
    pugi::xpath_variable* var_ptr = vars.add("fuzz_var", target_type);
    if (!var_ptr) {
        return 0;
    }
    pugi::xpath_variable& var = *var_ptr;

    // 1. Test pugi::xpath_variable::type
    pugi::xpath_value_type vtype = var.type();
    (void)vtype;

    // 2. Test pugi::xpath_variable::get_number
    double num_val = var.get_number();
    (void)num_val;

    // 3. Test pugi::xpath_variable::get_boolean
    bool bool_val = var.get_boolean();
    (void)bool_val;

    // 4. Test pugi::xpath_variable::get_node_set
    const pugi::xpath_node_set& node_set = var.get_node_set();
    (void)node_set;

    // 5. Test pugi::xpath_variable::set (overload: double)
    double fuzz_double = 0.0;
    if (Size >= 8) {
        std::memcpy(&fuzz_double, Data, sizeof(double));
    } else {
        fuzz_double = static_cast<double>(Data[0]);
    }
    // This returns true only if the variable type is xpath_type_number
    bool set_double_result = var.set(fuzz_double);
    (void)set_double_result;

    // 6. Test pugi::xpath_variable::set (overload: xpath_node_set)
    pugi::xpath_node_set test_node_set;
    
    // Try to construct a node set from fuzz data if possible
    if (Size > 10) {
        char* xml_buffer = new char[Size + 1];
        std::memcpy(xml_buffer, Data, Size);
        xml_buffer[Size] = '\0';
        
        pugi::xml_document doc;
        if (doc.load_string(xml_buffer)) {
            if (doc.root()) {
                // Evaluate a simple XPath expression to get a node set
                pugi::xpath_query query(".");
                // Check return_type instead of result_type
                if (query.return_type() != pugi::xpath_type_none) {
                     // evaluate_node_set takes only the context node
                     pugi::xpath_node context_node = pugi::xpath_node(doc.root());
                     test_node_set = query.evaluate_node_set(context_node);
                }
            }
        }
        delete[] xml_buffer;
    }
    
    // Call set with the node set. Returns true only if type is xpath_type_node_set
    bool set_node_set_result = var.set(test_node_set);
    (void)set_node_set_result;

    // Additional exploration: Create a second variable with a different type to test type mismatch behavior
    pugi::xpath_value_type other_type = (target_type == pugi::xpath_type_number) ? pugi::xpath_type_boolean : pugi::xpath_type_number;
    pugi::xpath_variable* var2_ptr = vars.add("fuzz_var2", other_type);
    if (var2_ptr) {
        pugi::xpath_variable& var2 = *var2_ptr;
        
        // Test setting a double on a non-number variable (should return false)
        bool set_mismatch = var2.set(fuzz_double);
        (void)set_mismatch;
        
        // Test setting a node set on a non-node-set variable (should return false)
        bool set_mismatch_ns = var2.set(test_node_set);
        (void)set_mismatch_ns;

        // Test get_number on a non-number variable (should return NaN)
        double num_val2 = var2.get_number();
        (void)num_val2;

        // Test get_boolean on a non-boolean variable (should return false)
        bool bool_val2 = var2.get_boolean();
        (void)bool_val2;
    }

    return 0;
}