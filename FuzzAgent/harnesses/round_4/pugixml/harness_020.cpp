#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <string>
#include <vector>
#include <limits>
#include <string_view>

namespace harness_020 {

// Conditionally include string_view based on pugixml configuration
#ifdef PUGIXML_HAS_STRING_VIEW
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for comprehensive xml_attribute type conversion testing
    // Need enough data for XML content and all numeric test values
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first for numeric test values
    // These will be used for setting attribute values via operator= and set_value()
    int int_val = fdp.ConsumeIntegral<int>();
    unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
    long long_val = fdp.ConsumeIntegral<long>();
    unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
    double double_val = fdp.ConsumeFloatingPoint<double>();
    float float_val = fdp.ConsumeFloatingPoint<float>();
    bool bool_val = fdp.ConsumeBool();
    
    // Test edge cases: maximum/minimum values
    int int_max = std::numeric_limits<int>::max();
    int int_min = std::numeric_limits<int>::min();
    unsigned int uint_max = std::numeric_limits<unsigned int>::max();
    long long_max = std::numeric_limits<long>::max();
    long long_min = std::numeric_limits<long>::min();
    unsigned long ulong_max = std::numeric_limits<unsigned long>::max();
    double double_max = std::numeric_limits<double>::max();
    double double_min = std::numeric_limits<double>::lowest();
    float float_max = std::numeric_limits<float>::max();
    float float_min = std::numeric_limits<float>::lowest();
    
    // Consume operation parameters
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: operator=, 1: set_value, 2: both
    uint8_t num_attributes = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);  // Number of attributes to test
    
    // Test long long types if supported
    #ifdef PUGIXML_HAS_LONG_LONG
    long long llong_val = fdp.ConsumeIntegral<long long>();
    unsigned long long ullong_val = fdp.ConsumeIntegral<unsigned long long>();
    long long llong_max = std::numeric_limits<long long>::max();
    long long llong_min = std::numeric_limits<long long>::min();
    unsigned long long ullong_max = std::numeric_limits<unsigned long long>::max();
    #endif
    
    // Consume variable-size data for XML content and attribute names
    std::string xml_content = fdp.ConsumeRandomLengthString(2048);
    std::string root_name = fdp.ConsumeRandomLengthString(64);
    std::string attr_prefix = fdp.ConsumeRandomLengthString(32);
    
    // Step 1: Create xml_document object
    pugi::xml_document doc;
    
    // Step 2: Load XML buffer containing attributes with various numeric values
    pugi::xml_parse_result result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
    
    // Check if parsing succeeded - if not, return early to avoid crashes
    if (!result) {
        return 0;  // Gracefully handle parse failures
    }
    
    // Get document element for testing
    pugi::xml_node root = doc.document_element();
    
    // If there's no root element, create one
    if (!root) {
        if (root_name.empty()) {
            root_name = "root";  // Default fallback
        }
        root = doc.append_child(root_name.c_str());
        if (!root) {
            return 0;  // Can't create root node
        }
    }
    
    // Create test attributes for type conversion testing
    std::vector<pugi::xml_attribute> attributes;
    
    // Either get existing attributes or create new ones
    if (root.first_attribute()) {
        // Use existing attributes
        for (pugi::xml_attribute attr = root.first_attribute(); attr && attributes.size() < num_attributes; attr = attr.next_attribute()) {
            attributes.push_back(attr);
        }
    }
    
    // Create new attributes if needed
    while (attributes.size() < num_attributes) {
        std::string attr_name;
        if (attr_prefix.empty()) {
            attr_name = "attr_" + std::to_string(attributes.size());
        } else {
            attr_name = attr_prefix + "_" + std::to_string(attributes.size());
        }
        pugi::xml_attribute new_attr = root.append_attribute(attr_name.c_str());
        if (new_attr) {
            attributes.push_back(new_attr);
        } else {
            // If we can't create attributes, break to avoid infinite loop
            break;
        }
    }
    
    // If we have no valid attributes, return early
    if (attributes.empty()) {
        return 0;
    }
    
    // Now test xml_attribute type conversions based on test_mode
    // We'll test different attribute setting/conversion strategies
    
    if (!attributes.empty()) {
        // Get first attribute for comprehensive testing
        pugi::xml_attribute& attr = attributes[0];
        
        switch (test_mode) {
            case 0: {
                // Test operator= assignment with different numeric types
                
                // Set values using operator=
                attr = int_val;
                attr = uint_val;
                attr = long_val;
                attr = ulong_val;
                attr = double_val;
                attr = float_val;
                attr = bool_val;
                
                // Test edge cases
                attr = int_max;
                attr = int_min;
                attr = uint_max;
                attr = long_max;
                attr = long_min;
                attr = ulong_max;
                attr = double_max;
                attr = double_min;
                attr = float_max;
                attr = float_min;
                
                #ifdef PUGIXML_HAS_LONG_LONG
                // Test long long types if supported
                attr = llong_val;
                attr = ullong_val;
                attr = llong_max;
                attr = llong_min;
                attr = ullong_max;
                #endif
                
                // Now retrieve values using as_* conversion functions
                int retrieved_int = attr.as_int();
                unsigned int retrieved_uint = attr.as_uint();
                double retrieved_double = attr.as_double();
                float retrieved_float = attr.as_float();
                bool retrieved_bool = attr.as_bool();
                
                #ifdef PUGIXML_HAS_LONG_LONG
                long long retrieved_llong = attr.as_llong();
                unsigned long long retrieved_ullong = attr.as_ullong();
                #endif
                
                // Use retrieved values to avoid unused variable warnings
                (void)retrieved_int;
                (void)retrieved_uint;
                (void)retrieved_double;
                (void)retrieved_float;
                (void)retrieved_bool;
                #ifdef PUGIXML_HAS_LONG_LONG
                (void)retrieved_llong;
                (void)retrieved_ullong;
                #endif
                
                break;
            }
            
            case 1: {
                // Test set_value() with different numeric types and precision
                
                // Set values using set_value
                attr.set_value(int_val);
                attr.set_value(uint_val);
                attr.set_value(long_val);
                attr.set_value(ulong_val);
                attr.set_value(double_val);
                attr.set_value(float_val);
                attr.set_value(bool_val);
                
                // Test with precision for floating point types
                attr.set_value(double_val, 4);   // 4 decimal places
                attr.set_value(float_val, 2);    // 2 decimal places
                
                // Test edge cases
                attr.set_value(int_max);
                attr.set_value(int_min);
                attr.set_value(uint_max);
                attr.set_value(long_max);
                attr.set_value(long_min);
                attr.set_value(ulong_max);
                attr.set_value(double_max);
                attr.set_value(double_min);
                attr.set_value(float_max);
                attr.set_value(float_min);
                
                #ifdef PUGIXML_HAS_LONG_LONG
                // Test long long types if supported
                attr.set_value(llong_val);
                attr.set_value(ullong_val);
                attr.set_value(llong_max);
                attr.set_value(llong_min);
                attr.set_value(ullong_max);
                #endif
                
                // Now retrieve values using as_* conversion functions
                int retrieved_int = attr.as_int();
                unsigned int retrieved_uint = attr.as_uint();
                double retrieved_double = attr.as_double();
                float retrieved_float = attr.as_float();
                bool retrieved_bool = attr.as_bool();
                
                #ifdef PUGIXML_HAS_LONG_LONG
                long long retrieved_llong = attr.as_llong();
                unsigned long long retrieved_ullong = attr.as_ullong();
                #endif
                
                // Use retrieved values to avoid unused variable warnings
                (void)retrieved_int;
                (void)retrieved_uint;
                (void)retrieved_double;
                (void)retrieved_float;
                (void)retrieved_bool;
                #ifdef PUGIXML_HAS_LONG_LONG
                (void)retrieved_llong;
                (void)retrieved_ullong;
                #endif
                
                break;
            }
            
            case 2: {
                // Test both operator= and set_value, mixing them
                
                // Set values using operator=
                attr = int_val;
                int retrieved_int1 = attr.as_int();
                
                // Change using set_value
                attr.set_value(uint_val);
                unsigned int retrieved_uint1 = attr.as_uint();
                
                // Back to operator=
                attr = double_val;
                double retrieved_double1 = attr.as_double();
                
                // set_value with precision
                attr.set_value(float_val, 4);
                float retrieved_float1 = attr.as_float();
                
                // operator= for bool
                attr = bool_val;
                bool retrieved_bool1 = attr.as_bool();
                
                // Test all numeric type conversions on the same attribute
                // This tests conversion between different numeric types
                attr = int_val;
                unsigned int as_uint_from_int = attr.as_uint();
                double as_double_from_int = attr.as_double();
                float as_float_from_int = attr.as_float();
                bool as_bool_from_int = attr.as_bool();
                
                attr = double_val;
                int as_int_from_double = attr.as_int();
                unsigned int as_uint_from_double = attr.as_uint();
                float as_float_from_double = attr.as_float();
                
                attr = bool_val;
                int as_int_from_bool = attr.as_int();
                double as_double_from_bool = attr.as_double();
                
                #ifdef PUGIXML_HAS_LONG_LONG
                // Test long long conversions
                attr = llong_val;
                long long retrieved_llong1 = attr.as_llong();
                unsigned long long as_ullong_from_llong = attr.as_ullong();
                int as_int_from_llong = attr.as_int();
                double as_double_from_llong = attr.as_double();
                
                attr = ullong_val;
                unsigned long long retrieved_ullong1 = attr.as_ullong();
                long long as_llong_from_ullong = attr.as_llong();
                #endif
                
                // Test invalid numeric strings (edge cases)
                // Set attribute to non-numeric string, then try conversion
                attr = "not_a_number";
                int as_int_invalid = attr.as_int(42);  // With default value
                double as_double_invalid = attr.as_double(3.14);
                bool as_bool_invalid = attr.as_bool(true);
                
                // Test empty attribute
                pugi::xml_attribute empty_attr;
                if (attributes.size() > 1) {
                    empty_attr = attributes[1];
                    empty_attr = "";  // Set to empty string
                    int as_int_empty = empty_attr.as_int(99);
                    double as_double_empty = empty_attr.as_double(2.71);
                    bool as_bool_empty = empty_attr.as_bool(false);
                }
                
                // Use retrieved values to avoid unused variable warnings
                (void)retrieved_int1;
                (void)retrieved_uint1;
                (void)retrieved_double1;
                (void)retrieved_float1;
                (void)retrieved_bool1;
                (void)as_uint_from_int;
                (void)as_double_from_int;
                (void)as_float_from_int;
                (void)as_bool_from_int;
                (void)as_int_from_double;
                (void)as_uint_from_double;
                (void)as_float_from_double;
                (void)as_int_from_bool;
                (void)as_double_from_bool;
                (void)as_int_invalid;
                (void)as_double_invalid;
                (void)as_bool_invalid;
                
                #ifdef PUGIXML_HAS_LONG_LONG
                (void)retrieved_llong1;
                (void)retrieved_ullong1;
                (void)as_ullong_from_llong;
                (void)as_int_from_llong;
                (void)as_double_from_llong;
                (void)as_llong_from_ullong;
                #endif
                
                break;
            }
        }
        
        // Test remaining attributes with simpler operations
        for (size_t i = 1; i < attributes.size(); i++) {
            pugi::xml_attribute& current_attr = attributes[i];
            
            // Use different numeric types for different attributes
            switch (i % 7) {
                case 0:
                    {
                        // Safe addition to avoid signed integer overflow
                        long long safe_sum = static_cast<long long>(int_val) + static_cast<long long>(i);
                        if (safe_sum <= INT_MAX && safe_sum >= INT_MIN) {
                            current_attr = static_cast<int>(safe_sum);
                        } else {
                            // Handle overflow case - use modulo to stay within valid range
                            current_attr = static_cast<int>((int_val ^ i) % 1000); // Alternative safe value
                        }
                        (void)current_attr.as_int();
                    }
                    break;
                case 1:
                    {
                        // Unsigned addition is safe from overflow due to wrap-around being defined
                        current_attr = uint_val + static_cast<unsigned int>(i);
                        (void)current_attr.as_uint();
                    }
                    break;
                case 2:
                    {
                        // Double addition is safe from overflow (results in INF which is defined)
                        current_attr = double_val + static_cast<double>(i);
                        (void)current_attr.as_double();
                    }
                    break;
                case 3:
                    {
                        // Float addition is safe from overflow (results in INF which is defined)
                        current_attr = float_val + static_cast<float>(i);
                        (void)current_attr.as_float();
                    }
                    break;
                case 4:
                    current_attr = bool_val ^ (i % 2);
                    (void)current_attr.as_bool();
                    break;
                case 5:
                    {
                        // Safe addition for long to avoid overflow
                        long long safe_sum_long = static_cast<long long>(long_val) + static_cast<long long>(i);
                        if (safe_sum_long <= LONG_MAX && safe_sum_long >= LONG_MIN) {
                            current_attr.set_value(static_cast<long>(safe_sum_long));
                        } else {
                            // Handle overflow case
                            current_attr.set_value(static_cast<long>((long_val ^ i) % 1000));
                        }
                        (void)current_attr.as_int();  // Test cross-type conversion
                    }
                    break;
                case 6:
                    {
                        // Unsigned long addition is safe from overflow due to wrap-around being defined
                        current_attr.set_value(ulong_val + static_cast<unsigned long>(i));
                        (void)current_attr.as_uint();  // Test cross-type conversion
                    }
                    break;
            }
        }
    }
    
    // Step 8: xml_document destructor automatically cleans up resources
    // No explicit cleanup needed - RAII handles it
    
    return 0;
}

} // namespace harness_020
