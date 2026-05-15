#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <functional>
#include <cmath>

namespace harness_009 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for complex attribute operations
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create XML document with complex attribute structure
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Determine test scenario from fuzzer input
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Create a complex attribute structure for testing
    size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    std::vector<pugi::xml_attribute> created_attrs;
    std::vector<std::string> attr_names;
    std::vector<std::string> attr_values;
    
    for (size_t i = 0; i < attr_count && fdp.remaining_bytes() > 20; ++i) {
        // Create attribute with fuzzed name and value
        std::string attr_name = fdp.ConsumeRandomLengthString(32);
        std::string attr_value = fdp.ConsumeRandomLengthString(128);
        
        pugi::xml_attribute attr = root.append_attribute(attr_name.c_str());
        attr.set_value(attr_value.c_str());
        
        created_attrs.push_back(attr);
        attr_names.push_back(attr_name);
        attr_values.push_back(attr_value);
    }
    
    // Test different attribute manipulation scenarios
    switch (scenario) {
        case 0: {
            // Scenario 0: Comprehensive type conversion testing
            if (!created_attrs.empty()) {
                pugi::xml_attribute& test_attr = created_attrs[0];
                
                // Test all type conversion methods with default values
                const char* as_string_default = test_attr.as_string("default_string");
                int as_int_default = test_attr.as_int(42);
                unsigned int as_uint_default = test_attr.as_uint(100);
                double as_double_default = test_attr.as_double(3.14159);
                float as_float_default = test_attr.as_float(2.71828f);
                bool as_bool_default = test_attr.as_bool(true);
                
                #ifdef PUGIXML_HAS_LONG_LONG
                long long as_llong_default = test_attr.as_llong(123456789012LL);
                unsigned long long as_ullong_default = test_attr.as_ullong(987654321098ULL);
                #endif
                
                // Test numeric conversion edge cases
                if (fdp.remaining_bytes() > 30) {
                    // Set numeric values and test conversions
                    test_attr.set_value(fdp.ConsumeIntegral<int>());
                    int fresh_int = test_attr.as_int();
                    
                    test_attr.set_value(fdp.ConsumeFloatingPoint<double>());
                    double fresh_double = test_attr.as_double();
                    
                    test_attr.set_value(fdp.ConsumeBool());
                    bool fresh_bool = test_attr.as_bool();
                    
                    // Test with precision for floating point
                    double precise_val = fdp.ConsumeFloatingPoint<double>();
                    int precision = fdp.ConsumeIntegralInRange<int>(0, 15);
                    test_attr.set_value(precise_val, precision);
                }
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Complex attribute lookup and hint optimization
            if (!created_attrs.empty() && attr_names.size() > 1) {
                // Test attribute lookup with hint for optimization
                std::string search_name = attr_names[attr_names.size() / 2];
                pugi::xml_attribute hint = created_attrs[0];
                
                // Test attribute() with hint parameter
                pugi::xml_attribute found_with_hint = root.attribute(search_name.c_str(), hint);
                
                // Test attribute() without hint
                pugi::xml_attribute found_no_hint = root.attribute(search_name.c_str());
                
                // Test string_view version if available
                #ifdef PUGIXML_HAS_STRING_VIEW
                std::string_view search_sv(search_name);
                pugi::xml_attribute found_sv = root.attribute(search_sv);
                pugi::xml_attribute found_sv_hint = root.attribute(search_sv, hint);
                #endif
                
                // Test last_attribute() and first_attribute()
                pugi::xml_attribute first_attr = root.first_attribute();
                pugi::xml_attribute last_attr = root.last_attribute();
                
                if (first_attr && last_attr) {
                    // Test navigation between attributes
                    pugi::xml_attribute next_from_first = first_attr.next_attribute();
                    pugi::xml_attribute prev_from_last = last_attr.previous_attribute();
                    
                    // Test attribute iteration
                    size_t iter_count = 0;
                    for (pugi::xml_attribute attr = first_attr; attr && iter_count < 100; 
                         attr = attr.next_attribute(), ++iter_count) {
                        // Just iterate to test the iteration logic
                        attr.name();
                        attr.value();
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Predicate-based attribute searching
            if (!created_attrs.empty()) {
                // Test find_attribute with various predicates
                
                // Predicate 1: Find by name prefix
                std::string search_prefix = fdp.ConsumeRandomLengthString(8);
                auto name_prefix_pred = [&search_prefix](pugi::xml_attribute attr) {
                    const char* name = attr.name();
                    return name && strncmp(name, search_prefix.c_str(), search_prefix.length()) == 0;
                };
                pugi::xml_attribute found_by_prefix = root.find_attribute(name_prefix_pred);
                
                // Predicate 2: Find by value containing substring
                std::string search_substr = fdp.ConsumeRandomLengthString(6);
                auto value_contains_pred = [&search_substr](pugi::xml_attribute attr) {
                    const char* value = attr.value();
                    return value && strstr(value, search_substr.c_str()) != nullptr;
                };
                pugi::xml_attribute found_by_value = root.find_attribute(value_contains_pred);
                
                // Predicate 3: Find by numeric value range
                int min_val = fdp.ConsumeIntegralInRange<int>(-1000, 1000);
                int max_val = min_val + fdp.ConsumeIntegralInRange<int>(1, 100);
                auto numeric_range_pred = [min_val, max_val](pugi::xml_attribute attr) {
                    int val = attr.as_int();
                    return val >= min_val && val <= max_val;
                };
                pugi::xml_attribute found_by_numeric = root.find_attribute(numeric_range_pred);
                
                // Predicate 4: Find empty attributes
                auto empty_value_pred = [](pugi::xml_attribute attr) {
                    const char* value = attr.value();
                    return value && value[0] == '\0';
                };
                pugi::xml_attribute found_empty = root.find_attribute(empty_value_pred);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Attribute hash values and comparisons
            if (created_attrs.size() >= 2) {
                // Test hash values
                for (size_t i = 0; i < std::min<size_t>(created_attrs.size(), 5); ++i) {
                    size_t hash_val = created_attrs[i].hash_value();
                    
                    // Test comparison operators
                    for (size_t j = 0; j < std::min<size_t>(created_attrs.size(), 5); ++j) {
                        bool eq = (created_attrs[i] == created_attrs[j]);
                        bool ne = (created_attrs[i] != created_attrs[j]);
                        bool lt = (created_attrs[i] < created_attrs[j]);
                        bool gt = (created_attrs[i] > created_attrs[j]);
                        bool le = (created_attrs[i] <= created_attrs[j]);
                        bool ge = (created_attrs[i] >= created_attrs[j]);
                        
                        // Test empty() method
                        bool is_empty = created_attrs[i].empty();
                    }
                }
                
                // Test comparison with empty attribute
                pugi::xml_attribute empty_attr;
                bool eq_empty = (created_attrs[0] == empty_attr);
                bool ne_empty = (created_attrs[0] != empty_attr);
                bool empty_is_empty = empty_attr.empty();
                
                // Test boolean conversion operators
                bool bool_conv_empty = static_cast<bool>(empty_attr);
                bool bool_conv_valid = static_cast<bool>(created_attrs[0]);
                bool not_op_empty = !empty_attr;
                bool not_op_valid = !created_attrs[0];
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Complex attribute value assignment and edge cases
            if (!created_attrs.empty()) {
                pugi::xml_attribute& test_attr = created_attrs[0];
                
                // Test all assignment operators
                test_attr = fdp.ConsumeRandomLengthString(256).c_str();
                test_attr = fdp.ConsumeIntegral<int>();
                test_attr = fdp.ConsumeIntegral<unsigned int>();
                test_attr = fdp.ConsumeIntegral<long>();
                test_attr = fdp.ConsumeIntegral<unsigned long>();
                test_attr = fdp.ConsumeFloatingPoint<double>();
                test_attr = fdp.ConsumeFloatingPoint<float>();
                test_attr = fdp.ConsumeBool();
                
                #ifdef PUGIXML_HAS_STRING_VIEW
                std::string sv_test = fdp.ConsumeRandomLengthString(128);
                std::string_view sv(sv_test);
                test_attr = sv;
                #endif
                
                #ifdef PUGIXML_HAS_LONG_LONG
                test_attr = fdp.ConsumeIntegral<long long>();
                test_attr = fdp.ConsumeIntegral<unsigned long long>();
                #endif
                
                // Test set_value variants
                std::string set_value_str = fdp.ConsumeRandomLengthString(200);
                test_attr.set_value(set_value_str.c_str());
                test_attr.set_value(set_value_str.c_str(), set_value_str.size());
                
                // Test set_name variants
                std::string set_name_str = fdp.ConsumeRandomLengthString(50);
                test_attr.set_name(set_name_str.c_str());
                test_attr.set_name(set_name_str.c_str(), set_name_str.size());
                
                #ifdef PUGIXML_HAS_STRING_VIEW
                std::string_view set_name_sv(set_name_str);
                test_attr.set_name(set_name_sv);
                
                std::string_view set_value_sv(set_value_str);
                test_attr.set_value(set_value_sv);
                #endif
                
                // Test edge cases: empty strings, null pointers
                test_attr.set_value("");
                test_attr.set_name("");
                
                // Test with size parameter for empty strings
                test_attr.set_value("", 0);
                test_attr.set_name("", 0);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Attribute copy and move operations
            if (created_attrs.size() >= 3) {
                // Test append_copy, prepend_copy, insert_copy_after, insert_copy_before
                pugi::xml_node copy_test_node = doc.append_child("copy_test");
                
                // Append copy of existing attribute
                pugi::xml_attribute appended_copy = copy_test_node.append_copy(created_attrs[0]);
                
                // Prepend copy of another attribute
                pugi::xml_attribute prepended_copy = copy_test_node.prepend_copy(created_attrs[1]);
                
                // Insert copies with position hints
                if (copy_test_node.first_attribute()) {
                    pugi::xml_attribute first_attr = copy_test_node.first_attribute();
                    pugi::xml_attribute insert_after_copy = copy_test_node.insert_copy_after(created_attrs[2], first_attr);
                    pugi::xml_attribute insert_before_copy = copy_test_node.insert_copy_before(created_attrs[0], first_attr);
                }
                
                // Test that copies have same values
                if (appended_copy) {
                    const char* orig_name = created_attrs[0].name();
                    const char* copy_name = appended_copy.name();
                    const char* orig_value = created_attrs[0].value();
                    const char* copy_value = appended_copy.value();
                }
                
                // Test internal_object() method
                for (size_t i = 0; i < std::min<size_t>(created_attrs.size(), 3); ++i) {
                    pugi::xml_attribute_struct* internal_ptr = created_attrs[i].internal_object();
                }
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Complex type conversion edge cases and error handling
            if (!created_attrs.empty()) {
                // Test conversion of invalid numeric strings
                pugi::xml_attribute& test_attr = created_attrs[0];
                
                // Set values that might cause conversion issues
                const char* test_cases[] = {
                    "not-a-number",
                    "123abc",
                    "1.2.3",
                    "infinity",
                    "NaN",
                    "",
                    "   ",
                    "123e999",  // Potential overflow
                    "-123e999",
                    "0xDEADBEEF",  // Hex not supported by default
                };
                
                size_t test_case_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(test_cases)/sizeof(test_cases[0]) - 1);
                test_attr.set_value(test_cases[test_case_idx]);
                
                // Test all conversions with error values
                int as_int_err = test_attr.as_int(999);
                unsigned int as_uint_err = test_attr.as_uint(888);
                double as_double_err = test_attr.as_double(777.777);
                float as_float_err = test_attr.as_float(666.666f);
                bool as_bool_err = test_attr.as_bool(false);
                const char* as_string_err = test_attr.as_string("ERROR");
                
                // Test boolean conversion edge cases
                const char* bool_test_cases[] = {
                    "true", "TRUE", "True", "1", "t", "T", "y", "Y", "yes", "YES",
                    "false", "FALSE", "False", "0", "f", "F", "n", "N", "no", "NO",
                    "maybe", "2", "-1", "on", "off"
                };
                
                size_t bool_case_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(bool_test_cases)/sizeof(bool_test_cases[0]) - 1);
                test_attr.set_value(bool_test_cases[bool_case_idx]);
                bool bool_conv = test_attr.as_bool(true);
            }
            break;
        }
        
        case 7: {
            // Scenario 7: Mixed operations and stress testing
            // Create a new node with many attributes for stress testing
            pugi::xml_node stress_node = doc.append_child("stress_test");
            
            size_t stress_attr_count = fdp.ConsumeIntegralInRange<size_t>(10, 50);
            std::vector<pugi::xml_attribute> stress_attrs;
            
            for (size_t i = 0; i < stress_attr_count && fdp.remaining_bytes() > 10; ++i) {
                std::string stress_name = "stress_" + std::to_string(i);
                std::string stress_value = fdp.ConsumeRandomLengthString(50);
                
                pugi::xml_attribute attr = stress_node.append_attribute(stress_name.c_str());
                attr = stress_value.c_str();
                stress_attrs.push_back(attr);
                
                // Randomly perform operations on attributes
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
                    // Change value type
                    uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                    switch (value_type) {
                        case 0: attr = fdp.ConsumeIntegral<int>(); break;
                        case 1: attr = fdp.ConsumeIntegral<unsigned int>(); break;
                        case 2: attr = fdp.ConsumeFloatingPoint<double>(); break;
                        case 3: attr = fdp.ConsumeBool(); break;
                        case 4: attr = fdp.ConsumeRandomLengthString(30).c_str(); break;
                        case 5: {
                            double val = fdp.ConsumeFloatingPoint<double>();
                            int prec = fdp.ConsumeIntegralInRange<int>(0, 10);
                            attr.set_value(val, prec);
                            break;
                        }
                    }
                }
                
                // Test conversions on random attributes
                if (fdp.ConsumeBool() && !stress_attrs.empty()) {
                    size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, stress_attrs.size() - 1);
                    pugi::xml_attribute& random_attr = stress_attrs[idx];
                    
                    // Perform random conversion
                    uint8_t conv_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                    switch (conv_type) {
                        case 0: random_attr.as_int(); break;
                        case 1: random_attr.as_uint(); break;
                        case 2: random_attr.as_double(); break;
                        case 3: random_attr.as_float(); break;
                        case 4: random_attr.as_bool(); break;
                    }
                }
            }
            
            // Test iteration and lookup on stress node
            size_t attr_found_count = 0;
            for (pugi::xml_attribute attr = stress_node.first_attribute(); attr; attr = attr.next_attribute()) {
                attr_found_count++;
                
                // Test name and value access
                attr.name();
                attr.value();
                
                // Test hash
                attr.hash_value();
            }
            
            // Test find operations on stress node
            if (fdp.remaining_bytes() > 20) {
                std::string search_term = fdp.ConsumeRandomLengthString(10);
                auto search_pred = [&search_term](pugi::xml_attribute attr) {
                    const char* name = attr.name();
                    return name && strstr(name, search_term.c_str()) != nullptr;
                };
                pugi::xml_attribute found = stress_node.find_attribute(search_pred);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_009
