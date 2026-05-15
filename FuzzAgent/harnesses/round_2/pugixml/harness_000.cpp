#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>

namespace harness_000 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) return 0;  // Minimum size check
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parsing options
    unsigned int parse_options = fdp.ConsumeIntegral<unsigned int>();
    
    // Consume encoding options
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
    
    // Test various load methods with the input data
    pugi::xml_document doc;
    
    // Test load_buffer with different options
    pugi::xml_parse_result result1 = doc.load_buffer(data, size, parse_options, encoding);
    
    // Clear and test with minimal parsing
    doc.reset();
    pugi::xml_parse_result result2 = doc.load_buffer(data, size, pugi::parse_minimal, encoding);
    
    // Clear and test with full parsing  
    doc.reset();
    pugi::xml_parse_result result3 = doc.load_buffer(data, size, pugi::parse_full, encoding);
    
    // If any parsing succeeded, test traversal and manipulation
    if (result1 || result2 || result3) {
        // Reset to last successful parse (result3 from parse_full)
        doc.reset();
        doc.load_buffer(data, size, pugi::parse_full, encoding);
        
        pugi::xml_node root = doc.document_element();
        
        if (!root.empty()) {
            // Traverse all nodes in the document
            for (pugi::xml_node node = root; node; node = node.next_sibling()) {
                // Get node properties
                pugi::xml_node_type type = node.type();
                const char* name = node.name();
                const char* value = node.value();
                
                // Traverse attributes
                for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
                    const char* attr_name = attr.name();
                    const char* attr_value = attr.value();
                    
                    // Test attribute value conversions
                    std::string str_val = attr.as_string();
                    int int_val = attr.as_int();
                    double double_val = attr.as_double();
                    bool bool_val = attr.as_bool();
                }
                
                // Traverse children
                for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                    // Access child properties
                    const char* child_name = child.name();
                    const char* child_value = child.value();
                    std::string child_text = child.child_value();
                }
            }
            
            // Test node manipulation with remaining fuzzed data
            if (fdp.remaining_bytes() > 20) {
                // Create a new child node
                pugi::xml_node new_child = root.append_child(pugi::node_element);
                
                // Set node name from fuzzed data
                std::string node_name = fdp.ConsumeRandomLengthString(20);
                new_child.set_name(node_name.c_str());
                
                // Add an attribute
                pugi::xml_attribute new_attr = new_child.append_attribute("fuzzed_attr");
                std::string attr_value = fdp.ConsumeRandomLengthString(50);
                new_attr.set_value(attr_value.c_str());
                
                // Add text content
                pugi::xml_node text_node = new_child.append_child(pugi::node_pcdata);
                std::string text_content = fdp.ConsumeRandomLengthString(100);
                text_node.set_value(text_content.c_str());
                
                // Test finding nodes and attributes
                pugi::xml_node found_child = root.child(node_name.c_str());
                pugi::xml_attribute found_attr = new_child.attribute("fuzzed_attr");
                
                // Test removing nodes
                if (!found_child.empty()) {
                    root.remove_child(found_child);
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_000
