#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

namespace harness_018 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for various numeric operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: DOCUMENT SETUP ====================
    // Create XML document with structure for numeric testing
    pugi::xml_document doc;
    
    // Create root element
    pugi::xml_node root = doc.append_child("numeric_test_root");
    
    // Create specialized nodes for different numeric tests
    pugi::xml_node int_node = root.append_child("int_tests");
    pugi::xml_node uint_node = root.append_child("uint_tests");
    pugi::xml_node float_node = root.append_child("float_tests");
    pugi::xml_node double_node = root.append_child("double_tests");
    pugi::xml_node bool_node = root.append_child("bool_tests");
    pugi::xml_node edge_case_node = root.append_child("edge_cases");
    pugi::xml_node conversion_node = root.append_child("conversions");
    
    // Initialize with some numeric strings for parsing
    int_node.text() = "42";
    uint_node.text() = "4294967295";
    float_node.text() = "3.14159";
    double_node.text() = "2.718281828459045";
    bool_node.text() = "true";
    
    // Add attributes with numeric values
    int_node.append_attribute("int_attr") = "-123";
    uint_node.append_attribute("uint_attr") = "1234567890";
    float_node.append_attribute("float_attr") = "-123.456";
    double_node.append_attribute("double_attr") = "987.654321";
    bool_node.append_attribute("bool_attr") = "false";
    edge_case_node.append_attribute("scientific") = "1.23e-4";
    edge_case_node.append_attribute("hex") = "0x1A";
    edge_case_node.append_attribute("inf") = "inf";
    edge_case_node.append_attribute("nan") = "nan";
    
    // ==================== PHASE 2: NUMERIC CONVERSION TESTING ====================
    // TARGET: Comprehensive numeric value parsing and conversion functions
    
    // Get text objects for manipulation
    pugi::xml_text int_text = int_node.text();
    pugi::xml_text uint_text = uint_node.text();
    pugi::xml_text float_text = float_node.text();
    pugi::xml_text double_text = double_node.text();
    pugi::xml_text bool_text = bool_node.text();
    pugi::xml_text edge_text = edge_case_node.text();
    pugi::xml_text conv_text = conversion_node.text();
    
    // Get attribute objects
    pugi::xml_attribute int_attr = int_node.attribute("int_attr");
    pugi::xml_attribute uint_attr = uint_node.attribute("uint_attr");
    pugi::xml_attribute float_attr = float_node.attribute("float_attr");
    pugi::xml_attribute double_attr = double_node.attribute("double_attr");
    pugi::xml_attribute bool_attr = bool_node.attribute("bool_attr");
    pugi::xml_attribute scientific_attr = edge_case_node.attribute("scientific");
    pugi::xml_attribute hex_attr = edge_case_node.attribute("hex");
    pugi::xml_attribute inf_attr = edge_case_node.attribute("inf");
    pugi::xml_attribute nan_attr = edge_case_node.attribute("nan");
    
    // Consume data for numeric operations
    while (fdp.remaining_bytes() > 32) {
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 40;
        
        switch (operation) {
            // ========== TEXT NUMERIC CONVERSIONS ==========
            case 0: { // Test int conversions with various defaults
                int default_val = fdp.ConsumeIntegral<int>();
                int result1 = int_text.as_int(default_val);
                int result2 = edge_text.as_int(default_val);
                (void)result1; (void)result2;
                break;
            }
            case 1: { // Test unsigned int conversions
                unsigned int default_val = fdp.ConsumeIntegral<unsigned int>();
                unsigned int result1 = uint_text.as_uint(default_val);
                unsigned int result2 = edge_text.as_uint(default_val);
                (void)result1; (void)result2;
                break;
            }
            case 2: { // Test double conversions
                double default_val = fdp.ConsumeFloatingPoint<double>();
                double result1 = double_text.as_double(default_val);
                double result2 = scientific_attr.as_double(default_val);
                double result3 = inf_attr.as_double(default_val);
                double result4 = nan_attr.as_double(default_val);
                (void)result1; (void)result2; (void)result3; (void)result4;
                break;
            }
            case 3: { // Test float conversions
                float default_val = fdp.ConsumeFloatingPoint<float>();
                float result1 = float_text.as_float(default_val);
                float result2 = edge_text.as_float(default_val);
                (void)result1; (void)result2;
                break;
            }
            case 4: { // Test bool conversions
                bool default_val = fdp.ConsumeBool();
                bool result1 = bool_text.as_bool(default_val);
                bool result2 = int_text.as_bool(default_val);
                bool result3 = edge_text.as_bool(default_val);
                (void)result1; (void)result2; (void)result3;
                break;
            }
            #ifdef PUGIXML_HAS_LONG_LONG
            case 5: { // Test long long conversions
                long long default_val = fdp.ConsumeIntegral<long long>();
                long long result1 = int_text.as_llong(default_val);
                long long result2 = edge_text.as_llong(default_val);
                (void)result1; (void)result2;
                break;
            }
            case 6: { // Test unsigned long long conversions
                unsigned long long default_val = fdp.ConsumeIntegral<unsigned long long>();
                unsigned long long result1 = uint_text.as_ullong(default_val);
                unsigned long long result2 = edge_text.as_ullong(default_val);
                (void)result1; (void)result2;
                break;
            }
            #endif
            
            // ========== ATTRIBUTE NUMERIC CONVERSIONS ==========
            case 7: { // Test attribute int conversions
                int default_val = fdp.ConsumeIntegral<int>();
                int result1 = int_attr.as_int(default_val);
                int result2 = hex_attr.as_int(default_val);
                (void)result1; (void)result2;
                break;
            }
            case 8: { // Test attribute unsigned int conversions
                unsigned int default_val = fdp.ConsumeIntegral<unsigned int>();
                unsigned int result1 = uint_attr.as_uint(default_val);
                unsigned int result2 = hex_attr.as_uint(default_val);
                (void)result1; (void)result2;
                break;
            }
            case 9: { // Test attribute double conversions
                double default_val = fdp.ConsumeFloatingPoint<double>();
                double result1 = double_attr.as_double(default_val);
                double result2 = scientific_attr.as_double(default_val);
                (void)result1; (void)result2;
                break;
            }
            case 10: { // Test attribute float conversions
                float default_val = fdp.ConsumeFloatingPoint<float>();
                float result1 = float_attr.as_float(default_val);
                float result2 = scientific_attr.as_float(default_val);
                (void)result1; (void)result2;
                break;
            }
            case 11: { // Test attribute bool conversions
                bool default_val = fdp.ConsumeBool();
                bool result1 = bool_attr.as_bool(default_val);
                bool result2 = int_attr.as_bool(default_val);
                (void)result1; (void)result2;
                break;
            }
            
            // ========== NUMERIC STRING PARSING EDGE CASES ==========
            case 12: { // Test parsing of various numeric string formats
                // Generate different numeric string formats
                int int_val = fdp.ConsumeIntegral<int>();
                double double_val = fdp.ConsumeFloatingPoint<double>();
                bool bool_val = fdp.ConsumeBool();
                
                // Create strings in different formats
                std::string int_str = std::to_string(int_val);
                std::string double_str = std::to_string(double_val);
                std::string bool_str = bool_val ? "true" : "false";
                std::string sci_str = std::to_string(double_val * 1e-6) + "e6";
                
                // Test parsing these strings
                conv_text.set(int_str.c_str());
                int parsed_int = conv_text.as_int(0);
                
                conv_text.set(double_str.c_str());
                double parsed_double = conv_text.as_double(0.0);
                
                conv_text.set(bool_str.c_str());
                bool parsed_bool = conv_text.as_bool(false);
                
                conv_text.set(sci_str.c_str());
                double parsed_sci = conv_text.as_double(0.0);
                
                (void)parsed_int; (void)parsed_double; (void)parsed_bool; (void)parsed_sci;
                break;
            }
            
            // ========== ARITHMETIC WITH CONVERTED VALUES ==========
            case 13: { // Perform arithmetic with converted values
                int int1 = int_text.as_int(0);
                int int2 = int_attr.as_int(0);
                int sum = int1 + int2;
                int diff = int1 - int2;
                int prod = int1 * int2;
                // Avoid division by zero
                int quot = (int2 != 0) ? int1 / int2 : 0;
                
                // Use results to set new values
                conv_text.set(sum);
                conv_text.set(diff);
                conv_text.set(prod);
                conv_text.set(quot);
                
                (void)sum; (void)diff; (void)prod; (void)quot;
                break;
            }
            case 14: { // Floating point arithmetic
                double d1 = double_text.as_double(0.0);
                double d2 = double_attr.as_double(1.0);
                double sum = d1 + d2;
                double diff = d1 - d2;
                double prod = d1 * d2;
                // Avoid division by zero
                double quot = (std::abs(d2) > 1e-10) ? d1 / d2 : 0.0;
                
                conv_text.set(sum);
                conv_text.set(diff);
                conv_text.set(prod);
                conv_text.set(quot, 6); // Set with precision
                
                (void)sum; (void)diff; (void)prod; (void)quot;
                break;
            }
            
            // ========== SETTING NUMERIC VALUES ==========
            case 15: { // Set text with various numeric types
                int int_val = fdp.ConsumeIntegral<int>();
                unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                long long_val = fdp.ConsumeIntegral<long>();
                unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
                double double_val = fdp.ConsumeFloatingPoint<double>();
                float float_val = fdp.ConsumeFloatingPoint<float>();
                bool bool_val = fdp.ConsumeBool();
                
                conv_text.set(int_val);
                conv_text.set(uint_val);
                conv_text.set(long_val);
                conv_text.set(ulong_val);
                conv_text.set(double_val);
                conv_text.set(float_val);
                conv_text.set(bool_val);
                break;
            }
            case 16: { // Set attribute with various numeric types
                int int_val = fdp.ConsumeIntegral<int>();
                unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                double double_val = fdp.ConsumeFloatingPoint<double>();
                float float_val = fdp.ConsumeFloatingPoint<float>();
                bool bool_val = fdp.ConsumeBool();
                
                pugi::xml_attribute new_attr = conversion_node.append_attribute("dynamic_attr");
                new_attr.set_value(int_val);
                new_attr.set_value(uint_val);
                new_attr.set_value(double_val);
                new_attr.set_value(float_val);
                new_attr.set_value(bool_val);
                break;
            }
            
            // ========== OPERATOR= OVERLOADS FOR NUMERIC TYPES ==========
            case 17: { // Test operator= for numeric types
                int int_val = fdp.ConsumeIntegral<int>();
                unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                long long_val = fdp.ConsumeIntegral<long>();
                unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
                double double_val = fdp.ConsumeFloatingPoint<double>();
                float float_val = fdp.ConsumeFloatingPoint<float>();
                bool bool_val = fdp.ConsumeBool();
                
                conv_text = int_val;
                conv_text = uint_val;
                conv_text = long_val;
                conv_text = ulong_val;
                conv_text = double_val;
                conv_text = float_val;
                conv_text = bool_val;
                break;
            }
            case 18: { // Test attribute operator= for numeric types
                int int_val = fdp.ConsumeIntegral<int>();
                unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                long long_val = fdp.ConsumeIntegral<long>();
                unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
                double double_val = fdp.ConsumeFloatingPoint<double>();
                float float_val = fdp.ConsumeFloatingPoint<float>();
                bool bool_val = fdp.ConsumeBool();
                
                pugi::xml_attribute op_attr = conversion_node.append_attribute("op_attr");
                op_attr = int_val;
                op_attr = uint_val;
                op_attr = long_val;
                op_attr = ulong_val;
                op_attr = double_val;
                op_attr = float_val;
                op_attr = bool_val;
                break;
            }
            
            // ========== NUMERIC PRECISION TESTING ==========
            case 19: { // Test numeric precision in string conversions
                double double_val = fdp.ConsumeFloatingPoint<double>();
                float float_val = fdp.ConsumeFloatingPoint<float>();
                int precision = fdp.ConsumeIntegralInRange<int>(0, 15);
                
                // Test setting with precision
                conv_text.set(double_val, precision);
                conv_text.set(float_val, precision);
                
                // Test attribute setting with precision
                pugi::xml_attribute prec_attr = conversion_node.append_attribute("precision_test");
                prec_attr.set_value(double_val, precision);
                prec_attr.set_value(float_val, precision);
                break;
            }
            
            // ========== BOUNDARY VALUE TESTING ==========
            case 20: { // Test boundary values
                // Test extreme values
                conv_text.set(std::numeric_limits<int>::min());
                conv_text.set(std::numeric_limits<int>::max());
                conv_text.set(std::numeric_limits<unsigned int>::max());
                conv_text.set(std::numeric_limits<double>::min());
                conv_text.set(std::numeric_limits<double>::max());
                conv_text.set(std::numeric_limits<double>::infinity());
                conv_text.set(-std::numeric_limits<double>::infinity());
                conv_text.set(std::numeric_limits<double>::quiet_NaN());
                
                // Parse them back
                int min_int = conv_text.as_int(0);
                int max_int = conv_text.as_int(0);
                unsigned int max_uint = conv_text.as_uint(0);
                double min_double = conv_text.as_double(0.0);
                double max_double = conv_text.as_double(0.0);
                double inf = conv_text.as_double(0.0);
                double neg_inf = conv_text.as_double(0.0);
                double nan = conv_text.as_double(0.0);
                
                (void)min_int; (void)max_int; (void)max_uint;
                (void)min_double; (void)max_double; (void)inf; (void)neg_inf; (void)nan;
                break;
            }
            
            // ========== MIXED TYPE CONVERSIONS ==========
            case 21: { // Mixed type conversions and comparisons
                int int_val = int_text.as_int(0);
                double double_val = double_text.as_double(0.0);
                
                // Convert between types
                double int_to_double = static_cast<double>(int_val);
                int double_to_int = static_cast<int>(double_val);
                
                // Compare values
                bool int_lt_double = int_val < double_val;
                bool double_gt_int = double_val > int_val;
                bool approx_equal = std::abs(int_val - double_val) < 1.0;
                
                conv_text.set(int_to_double);
                conv_text.set(double_to_int);
                conv_text.set(int_lt_double);
                conv_text.set(double_gt_int);
                conv_text.set(approx_equal);
                
                (void)int_to_double; (void)double_to_int;
                (void)int_lt_double; (void)double_gt_int; (void)approx_equal;
                break;
            }
            
            // ========== NUMERIC STRING FORMAT TESTING ==========
            case 22: { // Test different numeric string formats
                // Consume data to create various numeric string formats
                int base_val = fdp.ConsumeIntegral<int>();
                double sci_val = fdp.ConsumeFloatingPoint<double>();
                
                // Create different string representations
                std::string decimal_str = std::to_string(base_val);
                std::string hex_str = "0x" + std::to_string(base_val & 0xFF);
                std::string sci_str = std::to_string(sci_val) + "e" + std::to_string(fdp.ConsumeIntegralInRange<int>(-3, 3));
                std::string with_spaces = "  " + std::to_string(base_val) + "  ";
                std::string with_plus = "+" + std::to_string(base_val);
                std::string leading_zeros = "00" + std::to_string(base_val);
                
                // Test parsing each format
                conv_text.set(decimal_str.c_str());
                int decimal_parsed = conv_text.as_int(0);
                
                conv_text.set(hex_str.c_str());
                int hex_parsed = conv_text.as_int(0);
                
                conv_text.set(sci_str.c_str());
                double sci_parsed = conv_text.as_double(0.0);
                
                conv_text.set(with_spaces.c_str());
                int spaced_parsed = conv_text.as_int(0);
                
                conv_text.set(with_plus.c_str());
                int plus_parsed = conv_text.as_int(0);
                
                conv_text.set(leading_zeros.c_str());
                int zeros_parsed = conv_text.as_int(0);
                
                (void)decimal_parsed; (void)hex_parsed; (void)sci_parsed;
                (void)spaced_parsed; (void)plus_parsed; (void)zeros_parsed;
                break;
            }
            
            // ========== DYNAMIC NUMERIC VALUE GENERATION ==========
            case 23: { // Generate and test dynamic numeric values
                // Generate a numeric XML structure dynamically
                pugi::xml_node dynamic_node = root.append_child("dynamic_numbers");
                
                // Add numeric attributes
                for (int i = 0; i < 5 && fdp.remaining_bytes() > 16; i++) {
                    int attr_val = fdp.ConsumeIntegral<int>();
                    std::string attr_name = "attr_" + std::to_string(i);
                    dynamic_node.append_attribute(attr_name.c_str()) = attr_val;
                }
                
                // Add numeric text
                for (int i = 0; i < 5 && fdp.remaining_bytes() > 16; i++) {
                    double text_val = fdp.ConsumeFloatingPoint<double>();
                    pugi::xml_node num_node = dynamic_node.append_child("number");
                    num_node.text() = text_val;
                }
                
                // Process the dynamic structure
                for (pugi::xml_attribute attr = dynamic_node.first_attribute(); attr; attr = attr.next_attribute()) {
                    int attr_num = attr.as_int(0);
                    double attr_double = attr.as_double(0.0);
                    (void)attr_num; (void)attr_double;
                }
                
                for (pugi::xml_node child = dynamic_node.first_child(); child; child = child.next_sibling()) {
                    double child_num = child.text().as_double(0.0);
                    (void)child_num;
                }
                break;
            }
            
            // ========== ERROR HANDLING FOR INVALID NUMERICS ==========
            case 24: { // Test handling of invalid numeric strings
                // Create invalid numeric strings
                std::string invalid_strs[] = {
                    "not a number",
                    "123abc",
                    "12.34.56",
                    "",
                    "   ",
                    "infinity",
                    "NaN",
                    "1.2.3.4",
                    "0xZZZ",
                    "1e999"  // Might cause overflow
                };
                
                for (const auto& invalid_str : invalid_strs) {
                    conv_text.set(invalid_str.c_str());
                    int as_int = conv_text.as_int(-1);
                    double as_double = conv_text.as_double(-1.0);
                    bool as_bool = conv_text.as_bool(false);
                    (void)as_int; (void)as_double; (void)as_bool;
                }
                break;
            }
            
            // ========== REMAINING CASES FOR OTHER OPERATIONS ==========
            default: {
                // Test remaining numeric APIs
                uint8_t sub_op = fdp.ConsumeIntegral<uint8_t>() % 8;
                
                switch (sub_op) {
                    case 0: { // Test as_string() with numeric conversion
                        const char* str_val = conv_text.as_string("default");
                        (void)str_val;
                        break;
                    }
                    case 1: { // Test empty() check
                        bool is_empty = conv_text.empty();
                        (void)is_empty;
                        break;
                    }
                    case 2: { // Test get() method
                        const char* text = conv_text.get();
                        (void)text;
                        break;
                    }
                    case 3: { // Test data() method for text
                        pugi::xml_node data_node = conv_text.data();
                        (void)data_node;
                        break;
                    }
                    case 4: { // Test numeric comparisons
                        int val1 = fdp.ConsumeIntegral<int>();
                        int val2 = fdp.ConsumeIntegral<int>();
                        conv_text.set(val1);
                        pugi::xml_text other_text = conversion_node.append_child("other").text();
                        other_text.set(val2);
                        
                        // Note: xml_text doesn't have comparison operators,
                        // but we can compare the numeric values
                        int parsed1 = conv_text.as_int(0);
                        int parsed2 = other_text.as_int(0);
                        bool equal = (parsed1 == parsed2);
                        bool less = (parsed1 < parsed2);
                        (void)equal; (void)less;
                        break;
                    }
                    case 5: { // Test setting with size parameter
                        std::string num_str = std::to_string(fdp.ConsumeIntegral<int>());
                        bool success = conv_text.set(num_str.c_str(), num_str.size());
                        (void)success;
                        break;
                    }
                    case 6: { // Test string_view operations if available
                        #ifdef PUGIXML_HAS_STRING_VIEW
                        std::string num_str = std::to_string(fdp.ConsumeIntegral<int>());
                        pugi::string_view_t sv(num_str.data(), num_str.size());
                        bool success = conv_text.set(sv);
                        (void)success;
                        #endif
                        break;
                    }
                    case 7: { // Test operator! for empty check
                        bool is_not_empty = !conv_text;
                        (void)is_not_empty;
                        break;
                    }
                }
                break;
            }
        }
        
        // Check if we should continue
        if (fdp.remaining_bytes() < 32) {
            break;
        }
    }
    
    // ==================== PHASE 3: FINAL VALIDATION ====================
    // Perform final validation of all numeric conversions
    std::vector<pugi::xml_node> all_nodes;
    all_nodes.push_back(int_node);
    all_nodes.push_back(uint_node);
    all_nodes.push_back(float_node);
    all_nodes.push_back(double_node);
    all_nodes.push_back(bool_node);
    all_nodes.push_back(edge_case_node);
    all_nodes.push_back(conversion_node);
    
    for (const auto& node : all_nodes) {
        // Convert node text
        int node_int = node.text().as_int(0);
        double node_double = node.text().as_double(0.0);
        bool node_bool = node.text().as_bool(false);
        (void)node_int; (void)node_double; (void)node_bool;
        
        // Convert node attributes
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            int attr_int = attr.as_int(0);
            double attr_double = attr.as_double(0.0);
            bool attr_bool = attr.as_bool(false);
            (void)attr_int; (void)attr_double; (void)attr_bool;
        }
    }
    
    return 0;
}

} // namespace harness_018
