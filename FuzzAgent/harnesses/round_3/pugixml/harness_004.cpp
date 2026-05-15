#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>

namespace harness_004 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic text operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create XML document with a text node for manipulation
    pugi::xml_document doc;
    
    // Determine test scenario from fuzzer input
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Create root element with a text node
    pugi::xml_node root = doc.append_child("root");
    
    // Add initial text content with fuzzed data
    std::string initial_text = fdp.ConsumeRandomLengthString(512);
    pugi::xml_text text_obj = root.append_child(pugi::node_pcdata).text();
    
    // Test different xml_text manipulation scenarios
    switch (scenario) {
        case 0: {
            // Scenario 0: Basic text getter/setter operations
            // Test empty() method
            bool is_empty = text_obj.empty();
            
            // Test set() with various data types
            if (fdp.ConsumeBool()) {
                text_obj.set(initial_text.c_str());
            } else {
                // Use assignment operator
                text_obj = initial_text.c_str();
            }
            
            // Test get() method
            const char* retrieved_text = text_obj.get();
            
            // Test as_string() with default value
            const char* as_string_default = text_obj.as_string("default");
            
            // Consume more data for additional operations
            if (fdp.remaining_bytes() > 20) {
                // Test set with size parameter
                size_t text_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                std::string sized_text = fdp.ConsumeRandomLengthString(text_size);
                text_obj.set(sized_text.c_str(), sized_text.size());
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Numeric type conversions and assignments
            // Set text using numeric assignment operators
            uint8_t num_type = fdp.ConsumeIntegral<uint8_t>() % 8;
            
            switch (num_type) {
                case 0:
                    text_obj = fdp.ConsumeIntegral<int>();
                    break;
                case 1:
                    text_obj = fdp.ConsumeIntegral<unsigned int>();
                    break;
                case 2:
                    text_obj = fdp.ConsumeIntegral<long>();
                    break;
                case 3:
                    text_obj = fdp.ConsumeIntegral<unsigned long>();
                    break;
                case 4:
                    text_obj = fdp.ConsumeFloatingPoint<double>();
                    break;
                case 5:
                    text_obj = fdp.ConsumeFloatingPoint<float>();
                    break;
                case 6:
                    text_obj = fdp.ConsumeBool();
                    break;
                case 7: {
                    // Test set() with precision for floating point
                    double dbl_val = fdp.ConsumeFloatingPoint<double>();
                    int precision = fdp.ConsumeIntegralInRange<int>(0, 10);
                    text_obj.set(dbl_val, precision);
                    break;
                }
            }
            
            // Test numeric getters with conversion
            if (!text_obj.empty()) {
                // Test all numeric conversion methods
                int as_int = text_obj.as_int();
                unsigned int as_uint = text_obj.as_uint();
                double as_double = text_obj.as_double();
                float as_float = text_obj.as_float();
                bool as_bool = text_obj.as_bool();
                
                // Test with custom default values
                int as_int_def = text_obj.as_int(999);
                unsigned int as_uint_def = text_obj.as_uint(888);
                double as_double_def = text_obj.as_double(777.777);
                float as_float_def = text_obj.as_float(666.666f);
                bool as_bool_def = text_obj.as_bool(true);
                
                // Test long long conversions if available
                #ifdef PUGIXML_HAS_LONG_LONG
                if (fdp.remaining_bytes() > 16) {
                    text_obj = fdp.ConsumeIntegral<long long>();
                    long long as_llong = text_obj.as_llong();
                    unsigned long long as_ullong = text_obj.as_ullong();
                }
                #endif
            }
            break;
        }
        
        case 2: {
            // Scenario 2: String operations and edge cases
            // Test with empty string
            text_obj = "";
            
            // Test empty() and get() on empty text
            bool empty_on_empty = text_obj.empty();
            const char* get_on_empty = text_obj.get();
            const char* as_string_on_empty = text_obj.as_string("fallback");
            
            // Test with various string patterns
            if (fdp.remaining_bytes() > 50) {
                std::string pattern_type = fdp.ConsumeRandomLengthString(10);
                
                if (pattern_type.find("num") != std::string::npos) {
                    // Numeric string - generate from fuzzed data
                    double num_val = fdp.ConsumeFloatingPoint<double>();
                    text_obj = std::to_string(num_val).c_str();
                } else if (pattern_type.find("bool") != std::string::npos) {
                    // Boolean-like strings - generate from fuzzed data
                    text_obj = (fdp.ConsumeBool() ? "true" : "false");
                } else if (pattern_type.find("special") != std::string::npos) {
                    // Special characters - generate from fuzzed data
                    std::string special_chars = fdp.ConsumeRandomLengthString(20);
                    text_obj = special_chars.c_str();
                } else {
                    // Random text
                    std::string random_text = fdp.ConsumeRandomLengthString(200);
                    text_obj = random_text.c_str();
                }
            }
            
            // Test string_view operations if available
            #ifdef PUGIXML_HAS_STRING_VIEW
            if (fdp.remaining_bytes() > 30) {
                // Store string in variable first to avoid dangling string_view
                std::string temp_str1 = fdp.ConsumeRandomLengthString(100);
                std::string_view sv = temp_str1;
                text_obj = sv;
                
                // Test set with string_view
                std::string temp_str2 = fdp.ConsumeRandomLengthString(50);
                std::string_view sv2 = temp_str2;
                text_obj.set(sv2);
            }
            #endif
            break;
        }
        
        case 3: {
            // Scenario 3: Multiple text operations on different nodes
            // Create multiple nodes with different text operations
            size_t node_count = fdp.ConsumeIntegralInRange<size_t>(1, 8);
            
            for (size_t i = 0; i < node_count && fdp.remaining_bytes() > 20; ++i) {
                std::string node_name = "node" + std::to_string(i);
                pugi::xml_node node = root.append_child(node_name.c_str());
                pugi::xml_text node_text = node.append_child(pugi::node_pcdata).text();
                
                // Different operations on different nodes
                switch (i % 4) {
                    case 0:
                        // Integer assignment
                        node_text = fdp.ConsumeIntegral<int>();
                        break;
                    case 1: {
                        // String assignment
                        std::string temp_str = fdp.ConsumeRandomLengthString(64);
                        node_text = temp_str.c_str();
                        break;
                    }
                    case 2:
                        // Floating point
                        node_text = fdp.ConsumeFloatingPoint<double>();
                        break;
                    case 3:
                        // Boolean
                        node_text = fdp.ConsumeBool();
                        break;
                }
                
                // Test conversions on each node
                if (!node_text.empty()) {
                    node_text.as_int();
                    node_text.as_double();
                    node_text.as_bool();
                }
            }
            
            // Test text retrieval from different nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                pugi::xml_text child_text = child.text();
                if (!child_text.empty()) {
                    child_text.get();
                    child_text.as_string();
                }
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Text manipulation with error conditions
            // Test on empty text object (no node)
            pugi::xml_text empty_text;
            
            // All these should handle empty case gracefully
            bool empty_check = empty_text.empty();
            bool not_operator = !empty_text;
            const char* empty_get = empty_text.get();
            const char* empty_as_string = empty_text.as_string("default_for_empty");
            int empty_as_int = empty_text.as_int(-1);
            double empty_as_double = empty_text.as_double(-1.0);
            bool empty_as_bool = empty_text.as_bool(false);
            
            // Test set() on empty text (should fail)
            bool set_on_empty = empty_text.set("test_fail"); // Hardcoded but minimal impact
            
            // Now work with valid text
            if (fdp.remaining_bytes() > 30) {
                // Set valid text
                text_obj.set(initial_text.c_str());
                
                // Test conversions on potentially invalid text
                std::string invalid_text = fdp.ConsumeRandomLengthString(100);
                pugi::xml_node invalid_node = root.append_child("invalid");
                pugi::xml_text invalid_text_obj = invalid_node.append_child(pugi::node_pcdata).text();
                invalid_text_obj = invalid_text.c_str();
                
                // These conversions might fail or return defaults
                invalid_text_obj.as_int(999);
                invalid_text_obj.as_double(888.888);
                invalid_text_obj.as_bool(true);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Complex text lifecycle and reset operations
            // Full lifecycle: set -> modify -> read -> reset -> set again
            
            // Phase 1: Initial set - use fuzzed data
            std::string init_val = fdp.ConsumeRandomLengthString(50);
            text_obj.set(init_val.c_str());
            std::string phase1 = text_obj.as_string();
            
            // Phase 2: Modify with assignment operator
            if (fdp.ConsumeBool()) {
                text_obj = fdp.ConsumeIntegral<int>();
            } else {
                std::string temp_str = fdp.ConsumeRandomLengthString(100);
                text_obj = temp_str.c_str();
            }
            std::string phase2 = text_obj.as_string();
            
            // Phase 3: Convert and read in different formats
            if (!text_obj.empty()) {
                int as_int_phase3 = text_obj.as_int();
                double as_double_phase3 = text_obj.as_double();
                bool as_bool_phase3 = text_obj.as_bool();
                
                // Phase 4: Reset with set() using different type
                if (fdp.ConsumeBool()) {
                    text_obj.set(fdp.ConsumeFloatingPoint<double>());
                } else {
                    text_obj.set(fdp.ConsumeBool());
                }
                
                // Phase 5: Final conversions
                if (!text_obj.empty()) {
                    text_obj.as_string();
                    text_obj.as_int(text_obj.as_int() + 1); // Test with computed default
                }
            }
            
            // Test text on attribute nodes (should be empty)
            pugi::xml_node attr_node = root.append_child("attr_node");
            attr_node.append_attribute("test_attr") = "attribute_value";
            pugi::xml_text attr_text = attr_node.text();
            bool attr_text_empty = attr_text.empty();
            
            // Test text on comment nodes (should be empty or have special behavior)
            pugi::xml_node comment_node = root.append_child(pugi::node_comment);
            comment_node.set_value("comment text");
            pugi::xml_text comment_text = comment_node.text();
            bool comment_text_empty = comment_text.empty();
            
            break;
        }
    }
    
    // Final cleanup: test boolean operators on text objects
    pugi::xml_text final_text = root.text();
    
    // Test && and || operators if available
    #ifndef PUGIXML_NO_XPATH
    // These operators are defined for xml_text
    bool and_result = final_text && true;
    bool or_result = final_text || false;
    #endif
    
    // Test safe bool conversion
    if (final_text) {
        // Text is valid (non-empty)
        final_text.get();
    }
    
    return 0;
}

} // namespace harness_004
