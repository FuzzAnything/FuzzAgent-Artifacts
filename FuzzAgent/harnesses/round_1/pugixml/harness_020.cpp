#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <limits>

namespace harness_020 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for various integer operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: DOCUMENT SETUP ====================
    // Create XML document with structure for integer manipulation testing
    pugi::xml_document doc;
    
    // Create root element
    pugi::xml_node root = doc.append_child("integer_manipulation_root");
    
    // Create specialized nodes for different integer type tests
    pugi::xml_node int_node = root.append_child("int_tests");
    pugi::xml_node uint_node = root.append_child("uint_tests");
    pugi::xml_node long_node = root.append_child("long_tests");
    pugi::xml_node ulong_node = root.append_child("ulong_tests");
    pugi::xml_node edge_case_node = root.append_child("edge_cases");
    pugi::xml_node conversion_node = root.append_child("conversions");
    pugi::xml_node operator_node = root.append_child("operator_tests");
    
    // Get text objects for manipulation
    pugi::xml_text int_text = int_node.text();
    pugi::xml_text uint_text = uint_node.text();
    pugi::xml_text long_text = long_node.text();
    pugi::xml_text ulong_text = ulong_node.text();
    pugi::xml_text edge_text = edge_case_node.text();
    pugi::xml_text conv_text = conversion_node.text();
    pugi::xml_text op_text = operator_node.text();
    
    // ==================== PHASE 2: INTEGER SET OPERATIONS ====================
    // TARGET: Integer-specific set() functions in xml_text class
    
    while (fdp.remaining_bytes() > 32) {
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 30;
        
        switch (operation) {
            // ========== INTEGER SET OPERATIONS ==========
            case 0: { // Test int set operations
                int int_val = fdp.ConsumeIntegral<int>();
                bool set_result = int_text.set(int_val);
                (void)set_result;
                break;
            }
            case 1: { // Test unsigned int set operations
                unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                bool set_result = uint_text.set(uint_val);
                (void)set_result;
                break;
            }
            case 2: { // Test long set operations
                long long_val = fdp.ConsumeIntegral<long>();
                bool set_result = long_text.set(long_val);
                (void)set_result;
                break;
            }
            case 3: { // Test unsigned long set operations
                unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
                bool set_result = ulong_text.set(ulong_val);
                (void)set_result;
                break;
            }
            
            #ifdef PUGIXML_HAS_LONG_LONG
            case 4: { // Test long long set operations
                long long llong_val = fdp.ConsumeIntegral<long long>();
                bool set_result = conv_text.set(llong_val);
                (void)set_result;
                break;
            }
            case 5: { // Test unsigned long long set operations
                unsigned long long ullong_val = fdp.ConsumeIntegral<unsigned long long>();
                bool set_result = conv_text.set(ullong_val);
                (void)set_result;
                break;
            }
            #endif
            
            // ========== INTEGER OPERATOR= OVERLOADS ==========
            case 6: { // Test operator= with int
                int int_val = fdp.ConsumeIntegral<int>();
                op_text = int_val;
                break;
            }
            case 7: { // Test operator= with unsigned int
                unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                op_text = uint_val;
                break;
            }
            case 8: { // Test operator= with long
                long long_val = fdp.ConsumeIntegral<long>();
                op_text = long_val;
                break;
            }
            case 9: { // Test operator= with unsigned long
                unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
                op_text = ulong_val;
                break;
            }
            
            #ifdef PUGIXML_HAS_LONG_LONG
            case 10: { // Test operator= with long long
                long long llong_val = fdp.ConsumeIntegral<long long>();
                op_text = llong_val;
                break;
            }
            case 11: { // Test operator= with unsigned long long
                unsigned long long ullong_val = fdp.ConsumeIntegral<unsigned long long>();
                op_text = ullong_val;
                break;
            }
            #endif
            
            // ========== EDGE CASE INTEGER VALUES ==========
            case 12: { // Test with minimum int value
                int min_val = std::numeric_limits<int>::min();
                bool set_result = edge_text.set(min_val);
                (void)set_result;
                break;
            }
            case 13: { // Test with maximum int value
                int max_val = std::numeric_limits<int>::max();
                bool set_result = edge_text.set(max_val);
                (void)set_result;
                break;
            }
            case 14: { // Test with zero
                int zero_val = 0;
                bool set_result = edge_text.set(zero_val);
                (void)set_result;
                break;
            }
            case 15: { // Test with negative one
                int neg_one = -1;
                bool set_result = edge_text.set(neg_one);
                (void)set_result;
                break;
            }
            case 16: { // Test with maximum unsigned int
                unsigned int max_uint = std::numeric_limits<unsigned int>::max();
                bool set_result = edge_text.set(max_uint);
                (void)set_result;
                break;
            }
            
            // ========== INTEGER CONVERSION PATTERNS ==========
            case 17: { // Set integer, then convert back
                int original_val = fdp.ConsumeIntegral<int>();
                bool set_result = conv_text.set(original_val);
                if (set_result) {
                    int retrieved_val = conv_text.as_int(-1);
                    (void)retrieved_val;
                }
                break;
            }
            case 18: { // Set unsigned int, then convert back
                unsigned int original_val = fdp.ConsumeIntegral<unsigned int>();
                bool set_result = conv_text.set(original_val);
                if (set_result) {
                    unsigned int retrieved_val = conv_text.as_uint(0);
                    (void)retrieved_val;
                }
                break;
            }
            case 19: { // Test chain of integer operations
                int val1 = fdp.ConsumeIntegral<int>();
                unsigned int val2 = fdp.ConsumeIntegral<unsigned int>();
                long val3 = fdp.ConsumeIntegral<long>();
                
                conv_text.set(val1);
                int retrieved1 = conv_text.as_int(0);
                
                conv_text.set(val2);
                unsigned int retrieved2 = conv_text.as_uint(0);
                
                conv_text.set(val3);
                #ifdef PUGIXML_HAS_LONG_LONG
                long long retrieved3 = conv_text.as_llong(0);
                #else
                long retrieved3 = conv_text.as_int(0);
                #endif
                
                (void)retrieved1; (void)retrieved2; (void)retrieved3;
                break;
            }
            
            // ========== MIXED TYPE OPERATIONS ==========
            case 20: { // Test setting int then reading as unsigned
                int signed_val = fdp.ConsumeIntegral<int>();
                conv_text.set(signed_val);
                unsigned int as_unsigned = conv_text.as_uint(0);
                (void)as_unsigned;
                break;
            }
            case 21: { // Test setting unsigned then reading as signed
                unsigned int unsigned_val = fdp.ConsumeIntegral<unsigned int>();
                conv_text.set(unsigned_val);
                int as_signed = conv_text.as_int(-1);
                (void)as_signed;
                break;
            }
            
            // ========== INTEGER ARRAY/SEQUENCE TESTING ==========
            case 22: { // Test multiple integer values in sequence
                size_t num_ints = fdp.ConsumeIntegralInRange<size_t>(1, 10);
                for (size_t i = 0; i < num_ints && fdp.remaining_bytes() >= sizeof(int); ++i) {
                    int seq_val = fdp.ConsumeIntegral<int>();
                    conv_text.set(seq_val);
                    int retrieved = conv_text.as_int(0);
                    (void)retrieved;
                }
                break;
            }
            
            // ========== BOOLEAN INTEGER OPERATIONS ==========
            case 23: { // Test boolean conversion with integers
                bool bool_val = fdp.ConsumeBool();
                conv_text.set(bool_val);
                bool as_bool = conv_text.as_bool(false);
                (void)as_bool;
                break;
            }
            case 24: { // Test integer 0/1 as boolean
                int int_bool = fdp.ConsumeIntegral<int>() % 2;
                conv_text.set(int_bool);
                bool as_bool = conv_text.as_bool(false);
                (void)as_bool;
                break;
            }
            
            // ========== PRECISION AND FORMAT TESTING ==========
            case 25: { // Test integer formatting with different values
                // Create multiple nodes with different integer formats
                pugi::xml_node format_node = root.append_child("format_test");
                pugi::xml_text format_text = format_node.text();
                
                int format_val = fdp.ConsumeIntegral<int>();
                format_text.set(format_val);
                
                // Read back as string to verify formatting
                std::string formatted = format_text.as_string("");
                (void)formatted;
                break;
            }
            
            // ========== INTEGER ATTRIBUTE OPERATIONS ==========
            case 26: { // Test integer operations on attributes too
                pugi::xml_node attr_node = root.append_child("attr_test");
                pugi::xml_attribute attr = attr_node.append_attribute("int_attr");
                
                int attr_val = fdp.ConsumeIntegral<int>();
                attr.set_value(attr_val);
                
                int retrieved_attr = attr.as_int(0);
                (void)retrieved_attr;
                break;
            }
            
            // ========== EMPTY TEXT INTEGER OPERATIONS ==========
            case 27: { // Test integer operations on empty text objects
                pugi::xml_text empty_text;
                int default_val = fdp.ConsumeIntegral<int>();
                int retrieved = empty_text.as_int(default_val);
                (void)retrieved;
                break;
            }
            case 28: { // Test setting on potentially empty text
                pugi::xml_node temp_node = root.append_child("temp");
                pugi::xml_text temp_text = temp_node.text();
                
                if (fdp.ConsumeBool()) {
                    int temp_val = fdp.ConsumeIntegral<int>();
                    bool set_result = temp_text.set(temp_val);
                    (void)set_result;
                }
                break;
            }
            
            // ========== INTEGER BOUNDARY TESTING ==========
            case 29: { // Test boundary values for all integer types
                // Test various boundary conditions
                std::vector<int> boundaries = {
                    0, 1, -1, 100, -100,
                    std::numeric_limits<int>::min(),
                    std::numeric_limits<int>::max()
                };
                
                for (int boundary : boundaries) {
                    conv_text.set(boundary);
                    int retrieved = conv_text.as_int(0);
                    (void)retrieved;
                }
                break;
            }
            
            default:
                break;
        }
    }
    
    // ==================== PHASE 3: VERIFICATION AND CLEANUP ====================
    // Verify integer values were set correctly by reading them back
    std::string int_str = int_text.as_string("");
    std::string uint_str = uint_text.as_string("");
    std::string long_str = long_text.as_string("");
    std::string ulong_str = ulong_text.as_string("");
    
    (void)int_str; (void)uint_str; (void)long_str; (void)ulong_str;
    
    return 0;
}

} // namespace harness_020
