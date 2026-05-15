#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "pugixml.hpp"

namespace harness_004 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 different file I/O scenarios
    bool test_save_file = fdp.ConsumeBool();
    bool test_load_file = fdp.ConsumeBool();
    bool test_stream_io = fdp.ConsumeBool();
    bool test_file_roundtrip = fdp.ConsumeBool();
    
    // Consume formatting flags
    unsigned int format_flags = 0;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_indent;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_write_bom;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_raw;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_no_declaration;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_no_escapes;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_save_file_text;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_indent_attributes;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_no_empty_element_tags;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_skip_control_chars;
    if (fdp.ConsumeBool()) format_flags |= pugi::format_attribute_single_quote;
    
    // Consume encoding options
    uint8_t encoding_option = fdp.ConsumeIntegral<uint8_t>() % 6;  // 6 encoding options
    
    // Consume indent string (max 16 chars)
    std::string indent_str = fdp.ConsumeRandomLengthString(16);
    if (indent_str.empty()) {
        indent_str = "\t";  // Default indent
    }
    
    // Consume temporary filename (max 32 chars)
    std::string filename = fdp.ConsumeRandomLengthString(32);
    if (filename.empty()) {
        filename = "fuzz_temp.xml";
    }
    
    // Use remaining bytes as XML content
    std::vector<uint8_t> xml_content = fdp.ConsumeRemainingBytes<uint8_t>();
    if (xml_content.empty()) return 0;
    
    // Create XML document for testing
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    // Try parsing the XML content with different options
    uint8_t parse_option = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (parse_option) {
        case 0:
            parse_result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_default);
            break;
        case 1:
            parse_result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_minimal);
            break;
        case 2:
            parse_result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_full);
            break;
        case 3:
            parse_result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_fragment);
            break;
    }
    
    // If parsing succeeded, test file I/O operations
    if (parse_result) {
        // Test 1: save_file operation
        if (test_save_file) {
            pugi::xml_encoding encoding;
            switch (encoding_option) {
                case 0: encoding = pugi::encoding_auto; break;
                case 1: encoding = pugi::encoding_utf8; break;
                case 2: encoding = pugi::encoding_utf16_le; break;
                case 3: encoding = pugi::encoding_utf16_be; break;
                case 4: encoding = pugi::encoding_utf32_le; break;
                case 5: encoding = pugi::encoding_utf32_be; break;
                default: encoding = pugi::encoding_auto; break;
            }
            
            // Try saving to file
            bool save_result = doc.save_file(filename.c_str(), 
                                            indent_str.c_str(), 
                                            format_flags, 
                                            encoding);
            (void)save_result; // Avoid unused variable warning
            
            // Clean up the file if it was created
            std::remove(filename.c_str());
        }
        
        // Test 2: load_file operation (if we have a valid file to load)
        if (test_load_file) {
            // First save a document to create a file to load
            pugi::xml_document temp_doc;
            pugi::xml_node root = temp_doc.append_child("fuzz_root");
            root.append_child("child1").append_attribute("attr") = "value1";
            root.append_child("child2").append_attribute("attr") = "value2";
            
            // Save to temporary file
            std::string load_filename = "fuzz_load_temp.xml";
            if (temp_doc.save_file(load_filename.c_str())) {
                // Now try to load it back
                pugi::xml_document loaded_doc;
                pugi::xml_parse_result load_result;
                
                uint8_t load_option = fdp.ConsumeIntegral<uint8_t>() % 3;
                switch (load_option) {
                    case 0:
                        load_result = loaded_doc.load_file(load_filename.c_str(), pugi::parse_default);
                        break;
                    case 1:
                        load_result = loaded_doc.load_file(load_filename.c_str(), pugi::parse_minimal);
                        break;
                    case 2:
                        load_result = loaded_doc.load_file(load_filename.c_str(), pugi::parse_full);
                        break;
                }
                
                (void)load_result; // Avoid unused variable warning
                
                // Clean up temporary file
                std::remove(load_filename.c_str());
            }
        }
        
        // Test 3: Stream-based save operations (only if STL is available)
        #ifndef PUGIXML_NO_STL
        if (test_stream_io) {
            // Test saving to string stream
            std::stringstream ss;
            doc.save(ss, indent_str.c_str(), format_flags);
            
            // Test saving to ostringstream with different encodings
            std::ostringstream oss;
            pugi::xml_encoding stream_encoding;
            switch (encoding_option) {
                case 0: stream_encoding = pugi::encoding_auto; break;
                case 1: stream_encoding = pugi::encoding_utf8; break;
                default: stream_encoding = pugi::encoding_utf8; break;
            }
            
            doc.save(oss, indent_str.c_str(), format_flags, stream_encoding);
            
            // Test loading from string stream (if we have valid XML)
            std::string saved_xml = oss.str();
            if (!saved_xml.empty()) {
                std::istringstream iss(saved_xml);
                pugi::xml_document stream_doc;
                pugi::xml_parse_result stream_result;
                
                uint8_t stream_option = fdp.ConsumeIntegral<uint8_t>() % 2;
                if (stream_option == 0) {
                    stream_result = stream_doc.load(iss);
                } else {
                    stream_result = stream_doc.load(iss, pugi::parse_full);
                }
                
                (void)stream_result; // Avoid unused variable warning
            }
        }
        #endif
        
        // Test 4: File roundtrip (save then load)
        if (test_file_roundtrip) {
            std::string roundtrip_filename = "fuzz_roundtrip_temp.xml";
            
            // Save document to file
            if (doc.save_file(roundtrip_filename.c_str())) {
                // Load it back
                pugi::xml_document roundtrip_doc;
                pugi::xml_parse_result roundtrip_result;
                
                uint8_t roundtrip_option = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (roundtrip_option) {
                    case 0:
                        roundtrip_result = roundtrip_doc.load_file(roundtrip_filename.c_str(), pugi::parse_default);
                        break;
                    case 1:
                        roundtrip_result = roundtrip_doc.load_file(roundtrip_filename.c_str(), pugi::parse_minimal);
                        break;
                    case 2:
                        roundtrip_result = roundtrip_doc.load_file(roundtrip_filename.c_str(), pugi::parse_full);
                        break;
                    case 3:
                        roundtrip_result = roundtrip_doc.load_file(roundtrip_filename.c_str(), pugi::parse_fragment);
                        break;
                }
                
                (void)roundtrip_result; // Avoid unused variable warning
                
                // Clean up temporary file
                std::remove(roundtrip_filename.c_str());
            }
        }
        
        // Test 5: Different operation modes based on operation_mode
        switch (operation_mode) {
            case 0: // Test save_file with different indent strings
            {
                std::string custom_indent = fdp.ConsumeRandomLengthString(8);
                if (custom_indent.empty()) custom_indent = "  ";
                
                std::string custom_filename = "fuzz_custom_indent.xml";
                bool custom_save = doc.save_file(custom_filename.c_str(), 
                                                custom_indent.c_str(), 
                                                format_flags);
                (void)custom_save;
                std::remove(custom_filename.c_str());
                break;
            }
            
            case 1: // Test save_file with minimal formatting
            {
                std::string minimal_filename = "fuzz_minimal.xml";
                bool minimal_save = doc.save_file(minimal_filename.c_str(), 
                                                 "",  // No indent
                                                 pugi::format_raw);  // Raw formatting
                (void)minimal_save;
                std::remove(minimal_filename.c_str());
                break;
            }
            
            case 2: // Test save_file with all formatting flags
            {
                std::string full_filename = "fuzz_full_format.xml";
                unsigned int full_flags = pugi::format_indent | 
                                         pugi::format_write_bom | 
                                         pugi::format_no_declaration | 
                                         pugi::format_indent_attributes;
                
                bool full_save = doc.save_file(full_filename.c_str(), 
                                              indent_str.c_str(), 
                                              full_flags);
                (void)full_save;
                std::remove(full_filename.c_str());
                break;
            }
            
            case 3: // Test error conditions for save_file
            {
                // Try saving to invalid path
                bool invalid_save = doc.save_file("/invalid/path/fuzz_test.xml");
                (void)invalid_save;  // Should fail
                break;
            }
            
            case 4: // Test error conditions for load_file
            {
                pugi::xml_document error_doc;
                pugi::xml_parse_result error_result = error_doc.load_file("non_existent_file.xml");
                (void)error_result;  // Should fail with status_file_not_found
                break;
            }
            
            case 5: // Test document reset and file operations
            {
                pugi::xml_document fresh_doc;
                fresh_doc.reset(doc);  // Copy document
                
                std::string reset_filename = "fuzz_reset.xml";
                bool reset_save = fresh_doc.save_file(reset_filename.c_str());
                (void)reset_save;
                std::remove(reset_filename.c_str());
                break;
            }
            
            case 6: // Test empty document file operations
            {
                pugi::xml_document empty_doc;
                std::string empty_filename = "fuzz_empty.xml";
                bool empty_save = empty_doc.save_file(empty_filename.c_str());
                (void)empty_save;
                std::remove(empty_filename.c_str());
                break;
            }
            
            case 7: // Test complex document with various node types
            {
                pugi::xml_document complex_doc;
                pugi::xml_node root = complex_doc.append_child("complex_root");
                
                // Add different node types
                root.append_child(pugi::node_comment).set_value("Test comment");
                pugi::xml_node pi_node = root.append_child(pugi::node_pi);
                pi_node.set_name("target");
                pi_node.set_value("processing instruction");
                root.append_child(pugi::node_cdata).set_value("<cdata content>");
                
                pugi::xml_node element = root.append_child("element");
                element.append_attribute("attr1") = "value1";
                element.append_attribute("attr2") = "value2";
                element.append_child(pugi::node_pcdata).set_value("Text content");
                
                std::string complex_filename = "fuzz_complex.xml";
                bool complex_save = complex_doc.save_file(complex_filename.c_str());
                (void)complex_save;
                std::remove(complex_filename.c_str());
                break;
            }
        }
    }
    
    // Clean up any remaining temporary files
    const char* temp_files[] = {
        "fuzz_temp.xml",
        "fuzz_load_temp.xml",
        "fuzz_roundtrip_temp.xml",
        "fuzz_custom_indent.xml",
        "fuzz_minimal.xml",
        "fuzz_full_format.xml",
        "fuzz_reset.xml",
        "fuzz_empty.xml",
        "fuzz_complex.xml"
    };
    
    for (const char* file : temp_files) {
        std::remove(file);
    }
    
    return 0;
}

} // namespace harness_004
