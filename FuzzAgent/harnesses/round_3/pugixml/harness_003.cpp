#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <iostream>
#include <algorithm>

namespace harness_003 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic XML structure and save options
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create XML document with fuzzed structure
    pugi::xml_document doc;
    
    // Determine document structure complexity
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Create root element with fuzzed name
    std::string root_name = fdp.ConsumeRandomLengthString(32);
    if (root_name.empty()) root_name = "root";
    pugi::xml_node root = doc.append_child(root_name.c_str());
    
    // Add attributes to root with fuzzed names/values
    size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 8);
    for (size_t i = 0; i < attr_count && fdp.remaining_bytes() > 10; ++i) {
        std::string attr_name = fdp.ConsumeRandomLengthString(16);
        std::string attr_value = fdp.ConsumeRandomLengthString(64);
        root.append_attribute(attr_name.c_str()) = attr_value.c_str();
    }
    
    // Add child nodes based on structure type
    size_t child_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < child_count && fdp.remaining_bytes() > 20; ++i) {
        std::string child_name = fdp.ConsumeRandomLengthString(24);
        pugi::xml_node child = root.append_child(child_name.c_str());
        
        // Add text content with potential special characters
        if (fdp.ConsumeBool()) {
            std::string text_content = fdp.ConsumeRandomLengthString(128);
            child.append_child(pugi::node_pcdata).set_value(text_content.c_str());
        }
        
        // Add attributes to child
        size_t child_attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 4);
        for (size_t j = 0; j < child_attr_count && fdp.remaining_bytes() > 10; ++j) {
            std::string child_attr_name = fdp.ConsumeRandomLengthString(12);
            std::string child_attr_value = fdp.ConsumeRandomLengthString(48);
            child.append_attribute(child_attr_name.c_str()) = child_attr_value.c_str();
        }
        
        // Add nested grandchildren for complex structures
        if (structure_type >= 2 && fdp.ConsumeBool() && fdp.remaining_bytes() > 30) {
            size_t grandchild_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
            for (size_t k = 0; k < grandchild_count; ++k) {
                std::string grandchild_name = fdp.ConsumeRandomLengthString(20);
                pugi::xml_node grandchild = child.append_child(grandchild_name.c_str());
                
                if (fdp.ConsumeBool()) {
                    std::string grandchild_text = fdp.ConsumeRandomLengthString(96);
                    grandchild.append_child(pugi::node_pcdata).set_value(grandchild_text.c_str());
                }
            }
        }
    }
    
    // Add special node types for more complex testing
    if (structure_type >= 1) {
        // Add comment node
        if (fdp.ConsumeBool()) {
            std::string comment_text = fdp.ConsumeRandomLengthString(100);
            root.append_child(pugi::node_comment).set_value(comment_text.c_str());
        }
        
        // Add CDATA node
        if (fdp.ConsumeBool()) {
            std::string cdata_text = fdp.ConsumeRandomLengthString(150);
            root.append_child(pugi::node_cdata).set_value(cdata_text.c_str());
        }
        
        // Add processing instruction
        if (fdp.ConsumeBool()) {
            std::string pi_name = fdp.ConsumeRandomLengthString(20);
            std::string pi_value = fdp.ConsumeRandomLengthString(80);
            pugi::xml_node pi = root.append_child(pugi::node_pi);
            pi.set_name(pi_name.c_str());
            pi.set_value(pi_value.c_str());
        }
    }
    
    // Consume save options from fuzzer input
    unsigned int format_flags = fdp.ConsumeIntegral<unsigned int>();
    
    // Limit flags to valid combinations (mask with known flag values)
    const unsigned int valid_flags_mask = 
        pugi::format_indent | 
        pugi::format_write_bom | 
        pugi::format_raw | 
        pugi::format_no_declaration | 
        pugi::format_no_escapes | 
        pugi::format_save_file_text | 
        pugi::format_indent_attributes | 
        pugi::format_no_empty_element_tags | 
        pugi::format_skip_control_chars | 
        pugi::format_attribute_single_quote;
    
    format_flags &= valid_flags_mask;
    
    // Consume indentation string
    std::string indent_str = fdp.ConsumeRandomLengthString(8);
    if (indent_str.empty()) indent_str = "\t";
    
    // Consume encoding type
    uint8_t encoding_val = fdp.ConsumeIntegral<uint8_t>() % 7;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    switch (encoding_val) {
        case 0: encoding = pugi::encoding_utf8; break;
        case 1: encoding = pugi::encoding_utf16_le; break;
        case 2: encoding = pugi::encoding_utf16_be; break;
        case 3: encoding = pugi::encoding_utf32_le; break;
        case 4: encoding = pugi::encoding_utf32_be; break;
        case 5: encoding = pugi::encoding_latin1; break;
        case 6: encoding = pugi::encoding_auto; break;
    }
    
    // Determine which save operations to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Test different save operations based on operation value
    switch (operation) {
        case 0: {
            // Test save to string stream (std::ostream)
            #ifndef PUGIXML_NO_STL
            try {
                std::stringstream ss;
                doc.save(ss, indent_str.c_str(), format_flags, encoding);
                
                // Also test with wide stream if enough bytes left
                if (fdp.remaining_bytes() > 10 && fdp.ConsumeBool()) {
                    std::wstringstream wss;
                    doc.save(wss, indent_str.c_str(), format_flags);
                }
            } catch (...) {
                // Ignore exceptions from fuzzing
            }
            #endif
            break;
        }
        
        case 1: {
            // Test save to file with generated filename
            std::string filename = fdp.ConsumeRandomLengthString(64);
            // Ensure filename is not empty and doesn't contain problematic characters
            if (filename.empty()) filename = "test.xml";
            
            // Clean filename of characters that might cause issues
            std::replace(filename.begin(), filename.end(), '/', '_');
            std::replace(filename.begin(), filename.end(), '\\', '_');
            std::replace(filename.begin(), filename.end(), ':', '_');
            std::replace(filename.begin(), filename.end(), '*', '_');
            std::replace(filename.begin(), filename.end(), '?', '_');
            std::replace(filename.begin(), filename.end(), '"', '_');
            std::replace(filename.begin(), filename.end(), '<', '_');
            std::replace(filename.begin(), filename.end(), '>', '_');
            std::replace(filename.begin(), filename.end(), '|', '_');
            
            try {
                bool save_result = doc.save_file(filename.c_str(), indent_str.c_str(), format_flags, encoding);
                // Result can be false for various reasons (permissions, invalid path, etc.)
                // We don't care about the result for fuzzing purposes
            } catch (...) {
                // Ignore exceptions
            }
            break;
        }
        
        case 2: {
            // Test save with custom xml_writer (string writer)
            #ifndef PUGIXML_NO_STL
            class string_writer : public pugi::xml_writer {
            public:
                std::string result;
                
                virtual void write(const void* data, size_t size) {
                    result.append(static_cast<const char*>(data), size);
                }
            };
            
            string_writer writer;
            try {
                doc.save(writer, indent_str.c_str(), format_flags, encoding);
            } catch (...) {
                // Ignore exceptions
            }
            #endif
            break;
        }
        
        case 3: {
            // Test save with various format flag combinations
            // Test a subset of interesting flag combinations
            unsigned int test_flags[] = {
                pugi::format_default,
                pugi::format_raw,
                pugi::format_indent,
                pugi::format_indent | pugi::format_indent_attributes,
                pugi::format_raw | pugi::format_no_declaration,
                pugi::format_indent | pugi::format_no_escapes,
                pugi::format_indent | pugi::format_save_file_text,
                pugi::format_indent | pugi::format_skip_control_chars,
                pugi::format_indent | pugi::format_attribute_single_quote,
                format_flags  // Use fuzzed flags
            };
            
            #ifndef PUGIXML_NO_STL
            for (unsigned int test_flag : test_flags) {
                if (fdp.remaining_bytes() < 10) break;
                
                std::stringstream ss;
                try {
                    doc.save(ss, indent_str.c_str(), test_flag, encoding);
                } catch (...) {
                    // Continue with next flag combination
                }
            }
            #endif
            break;
        }
        
        case 4: {
            // Test save with different encodings
            #ifndef PUGIXML_NO_STL
            pugi::xml_encoding encodings_to_test[] = {
                pugi::encoding_utf8,
                pugi::encoding_utf16_le,
                pugi::encoding_utf16_be,
                pugi::encoding_utf32_le,
                pugi::encoding_utf32_be,
                pugi::encoding_latin1,
                pugi::encoding_auto
            };
            
            for (pugi::xml_encoding test_encoding : encodings_to_test) {
                if (fdp.remaining_bytes() < 10) break;
                
                std::stringstream ss;
                try {
                    doc.save(ss, indent_str.c_str(), format_flags, test_encoding);
                } catch (...) {
                    // Continue with next encoding
                }
            }
            #endif
            break;
        }
        
        case 5: {
            // Test save with problematic content (edge cases)
            // Add potentially problematic content to document
            if (fdp.remaining_bytes() > 50) {
                std::string problematic_text = fdp.ConsumeRandomLengthString(200);
                
                // Add node with control characters, null bytes, etc.
                pugi::xml_node problem_node = root.append_child("problem");
                problem_node.append_child(pugi::node_pcdata).set_value(problematic_text.c_str());
                
                // Also add attributes with problematic values
                std::string problematic_attr_value = fdp.ConsumeRandomLengthString(100);
                problem_node.append_attribute("badattr") = problematic_attr_value.c_str();
            }
            
            // Now save the document
            #ifndef PUGIXML_NO_STL
            std::stringstream ss;
            try {
                doc.save(ss, indent_str.c_str(), format_flags, encoding);
            } catch (...) {
                // Expected for some problematic content
            }
            #endif
            break;
        }
        
        case 6: {
            // Test save after document modifications
            // Make some modifications to the document
            if (!root.empty()) {
                // Remove some nodes
                size_t nodes_to_remove = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                pugi::xml_node child = root.first_child();
                for (size_t i = 0; i < nodes_to_remove && child; ++i) {
                    pugi::xml_node next = child.next_sibling();
                    root.remove_child(child);
                    child = next;
                }
                
                // Modify attributes
                for (pugi::xml_attribute attr = root.first_attribute(); attr; attr = attr.next_attribute()) {
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                        std::string new_value = fdp.ConsumeRandomLengthString(50);
                        attr.set_value(new_value.c_str());
                    }
                }
                
                // Add new nodes
                if (fdp.remaining_bytes() > 20) {
                    size_t new_nodes = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                    for (size_t i = 0; i < new_nodes; ++i) {
                        std::string new_node_name = fdp.ConsumeRandomLengthString(20);
                        root.append_child(new_node_name.c_str());
                    }
                }
            }
            
            // Save modified document
            #ifndef PUGIXML_NO_STL
            std::stringstream ss;
            try {
                doc.save(ss, indent_str.c_str(), format_flags, encoding);
            } catch (...) {
                // Ignore exceptions
            }
            #endif
            break;
        }
        
        case 7: {
            // Test save with empty or minimal documents
            pugi::xml_document empty_doc;
            
            // Sometimes create truly empty document, sometimes with just declaration
            if (fdp.ConsumeBool()) {
                // Add XML declaration
                pugi::xml_node decl = empty_doc.append_child(pugi::node_declaration);
                decl.append_attribute("version") = "1.0";
                if (fdp.ConsumeBool()) {
                    decl.append_attribute("encoding") = "UTF-8";
                }
            }
            
            // Save empty document
            #ifndef PUGIXML_NO_STL
            std::stringstream ss;
            try {
                empty_doc.save(ss, indent_str.c_str(), format_flags, encoding);
            } catch (...) {
                // Ignore exceptions
            }
            #endif
            break;
        }
    }
    
    return 0;
}

} // namespace harness_003
