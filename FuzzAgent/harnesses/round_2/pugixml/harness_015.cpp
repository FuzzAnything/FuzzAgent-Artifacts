#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <string_view>
#include <cstring>

namespace harness_015 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for comprehensive buffer and explicit-length operation testing
    if (size < 128) return 0;  // Need enough data for multiple buffer operations
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create a base XML document for testing
    pugi::xml_document doc;
    
    // Create root node with fuzzed name using explicit length variant
    std::string root_name_str = fdp.ConsumeRandomLengthString(50);
    pugi::xml_node root = doc.append_child(pugi::node_element);
    // TARGET: set_name with explicit length parameter
    root.set_name(root_name_str.c_str(), root_name_str.size());
    
    // Create initial structure for testing
    pugi::xml_node test_node = root.append_child("test_node");
    pugi::xml_node append_target = root.append_child("append_target");
    pugi::xml_node buffer_node = root.append_child("buffer_node");
    
    // Step 2: Test append_buffer operations with various encodings and buffer sizes
    // This is the primary target: append_buffer API with different encodings
    
    // Consume buffer data for append_buffer operations
    size_t buffer_data_size = fdp.ConsumeIntegralInRange<size_t>(16, fdp.remaining_bytes() / 2);
    std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_data_size);
    
    // TARGET: Test append_buffer with different encoding options
    unsigned int encoding_choice = fdp.ConsumeIntegralInRange<unsigned int>(0, 5);
    pugi::xml_encoding encoding = pugi::encoding_auto;
    switch (encoding_choice) {
        case 0: encoding = pugi::encoding_auto; break;
        case 1: encoding = pugi::encoding_utf8; break;
        case 2: encoding = pugi::encoding_utf16_le; break;
        case 3: encoding = pugi::encoding_utf16_be; break;
        case 4: encoding = pugi::encoding_utf32_le; break;
        case 5: encoding = pugi::encoding_utf32_be; break;
    }
    
    // TARGET: append_buffer with various options
    unsigned int parse_options = fdp.ConsumeIntegral<unsigned int>();
    pugi::xml_parse_result append_result;
    
    // Test append_buffer on different target nodes
    if (fdp.ConsumeBool()) {
        append_result = append_target.append_buffer(buffer_data.data(), buffer_data.size(), 
                                                    parse_options, encoding);
    } else {
        append_result = buffer_node.append_buffer(buffer_data.data(), buffer_data.size(), 
                                                  parse_options, encoding);
    }
    
    // Test edge cases for append_buffer:
    // 1. Empty buffer
    if (fdp.ConsumeBool()) {
        std::vector<uint8_t> empty_buffer;
        append_target.append_buffer(empty_buffer.data(), 0, pugi::parse_default, encoding);
    }
    
    // 2. Very small buffer
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 1) {
        std::vector<uint8_t> small_buffer = fdp.ConsumeBytes<uint8_t>(1);
        buffer_node.append_buffer(small_buffer.data(), small_buffer.size(), 
                                  pugi::parse_minimal, encoding);
    }
    
    // Step 3: Test set_value and set_name with explicit length parameters (size variants)
    // These APIs are currently low-coverage targets
    
    // Create nodes for explicit-length testing
    pugi::xml_node len_test_node = root.append_child("len_test");
    pugi::xml_attribute len_test_attr = len_test_node.append_attribute("test_attr");
    
    // Consume test strings for explicit-length operations
    if (fdp.remaining_bytes() > 200) {
        std::string node_name_with_len = fdp.ConsumeRandomLengthString(100);
        std::string node_value_with_len = fdp.ConsumeRandomLengthString(100);
        
        // TARGET: set_name with explicit length parameter
        bool name_set_result = len_test_node.set_name(node_name_with_len.c_str(), 
                                                      node_name_with_len.size());
        (void)name_set_result; // Use to avoid unused variable warning
        
        // TARGET: set_value with explicit length parameter (node version)
        bool node_value_set_result = len_test_node.set_value(node_value_with_len.c_str(), 
                                                            node_value_with_len.size());
        (void)node_value_set_result;
        
        // TARGET: set_value with explicit length parameter (attribute version)
        if (fdp.remaining_bytes() > 100) {
            std::string attr_value_with_len = fdp.ConsumeRandomLengthString(100);
            bool attr_value_set_result = len_test_attr.set_value(attr_value_with_len.c_str(), 
                                                                attr_value_with_len.size());
            (void)attr_value_set_result;
        }
    }
    
    // Test edge cases for explicit-length APIs:
    // 1. Zero-length strings
    len_test_node.set_name("", 0);
    len_test_node.set_value("", 0);
    len_test_attr.set_value("", 0);
    
    // 2. Partial string consumption
    if (fdp.remaining_bytes() > 50) {
        std::string partial_str = fdp.ConsumeRandomLengthString(50);
        // Use only first half of the string
        size_t partial_len = partial_str.size() / 2;
        if (partial_len > 0) {
            len_test_node.set_name(partial_str.c_str(), partial_len);
        }
    }
    
    // Step 4: Test string_view attribute insertion functions
    // These are currently 0% covered according to the Manager's analysis
    
#ifdef PUGIXML_HAS_STRING_VIEW
    if (fdp.remaining_bytes() > 300) {
        // Create persistent strings for string_view operations
        std::string sv_attr_name1 = fdp.ConsumeRandomLengthString(50);
        std::string sv_attr_name2 = fdp.ConsumeRandomLengthString(50);
        std::string sv_attr_name3 = fdp.ConsumeRandomLengthString(50);
        std::string sv_attr_value1 = fdp.ConsumeRandomLengthString(100);
        std::string sv_attr_value2 = fdp.ConsumeRandomLengthString(100);
        std::string sv_attr_value3 = fdp.ConsumeRandomLengthString(100);
        
        std::string_view sv_attr_name1_view = sv_attr_name1;
        std::string_view sv_attr_name2_view = sv_attr_name2;
        std::string_view sv_attr_name3_view = sv_attr_name3;
        std::string_view sv_attr_value1_view = sv_attr_value1;
        std::string_view sv_attr_value2_view = sv_attr_value2;
        std::string_view sv_attr_value3_view = sv_attr_value3;
        
        // Create a node specifically for string_view testing
        pugi::xml_node sv_test_node = root.append_child("sv_test");
        
        // TARGET: append_attribute with string_view
        pugi::xml_attribute sv_attr1 = sv_test_node.append_attribute(sv_attr_name1_view);
        // TARGET: set_value with string_view (attribute version)
        sv_attr1.set_value(sv_attr_value1_view);
        
        // TARGET: prepend_attribute with string_view
        pugi::xml_attribute sv_attr2 = sv_test_node.prepend_attribute(sv_attr_name2_view);
        sv_attr2.set_value(sv_attr_value2_view);
        
        // Test insertion around existing attributes
        if (!sv_test_node.first_attribute()) {
            // TARGET: insert_attribute_before with string_view
            pugi::xml_attribute sv_attr3 = sv_test_node.insert_attribute_before(sv_attr_name3_view, 
                                                                               sv_attr1);
            if (sv_attr3) {
                sv_attr3.set_value(sv_attr_value3_view);
            }
        }
        
        // TARGET: remove_attribute with string_view
        sv_test_node.remove_attribute(sv_attr_name1_view);
        
        // TARGET: attribute retrieval with string_view
        pugi::xml_attribute found_attr = sv_test_node.attribute(sv_attr_name2_view);
        (void)found_attr;
    }
#endif // PUGIXML_HAS_STRING_VIEW
    
    // Step 5: Edge cases for buffer manipulation APIs
    
    // Test with invalid/non-element nodes
    pugi::xml_node comment_node = root.append_child(pugi::node_comment);
    comment_node.set_value("Test comment");
    
    // Try append_buffer on non-element node (should fail with status_append_invalid_root)
    if (fdp.remaining_bytes() > 10) {
        std::vector<uint8_t> small_buf = fdp.ConsumeBytes<uint8_t>(10);
        pugi::xml_parse_result invalid_result = comment_node.append_buffer(
            small_buf.data(), small_buf.size(), pugi::parse_default, pugi::encoding_auto);
        (void)invalid_result;
    }
    
    // Test with maximum size values
    if (fdp.ConsumeBool()) {
        size_t large_size = fdp.ConsumeIntegralInRange<size_t>(0, 10000);
        // Note: We don't actually consume this many bytes, just test the size parameter
        // with available data
        size_t actual_size = std::min(large_size, fdp.remaining_bytes());
        if (actual_size > 0) {
            std::vector<uint8_t> large_buffer = fdp.ConsumeBytes<uint8_t>(actual_size);
            buffer_node.append_buffer(large_buffer.data(), actual_size, 
                                     pugi::parse_full, encoding);
        }
    }
    
    // Test buffer manipulation with different parse options combinations
    unsigned int test_options[] = {
        pugi::parse_default,
        pugi::parse_minimal,
        pugi::parse_full,
        pugi::parse_ws_pcdata,
        pugi::parse_escapes,
        pugi::parse_eol,
        pugi::parse_wconv_attribute,
        pugi::parse_wnorm_attribute,
        pugi::parse_declaration,
        pugi::parse_comments,
        pugi::parse_pi,
        pugi::parse_cdata,
        pugi::parse_fragment
    };
    
    if (fdp.remaining_bytes() > 100) {
        std::vector<uint8_t> option_test_buffer = fdp.ConsumeBytes<uint8_t>(100);
        
        // Test a few different parse option combinations
        for (int i = 0; i < 3 && i < sizeof(test_options)/sizeof(test_options[0]); i++) {
            unsigned int opt_idx = fdp.ConsumeIntegralInRange<unsigned int>(0, 
                sizeof(test_options)/sizeof(test_options[0]) - 1);
            pugi::xml_node opt_test_node = root.append_child("opt_test");
            opt_test_node.append_buffer(option_test_buffer.data(), 
                                       option_test_buffer.size(),
                                       test_options[opt_idx],
                                       encoding);
        }
    }
    
    // Step 6: Cleanup and validation
    
    // Verify the document structure is still valid
    // Traverse and access nodes to ensure no crashes
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        const char* name = child.name();
        const char* value = child.value();
        (void)name;
        (void)value;
        
        // Check attributes
        for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
            (void)attr_name;
            (void)attr_value;
        }
    }
    
    return 0;
}

} // namespace harness_015
