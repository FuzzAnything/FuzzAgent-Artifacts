#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <sstream>
#include <iostream>

namespace harness_019 {


#ifndef PUGIXML_NO_STL
#endif

// Custom temporary file manager for wide character file operations
class TempFileManager {
private:
    std::vector<std::string> temp_files;
    
public:
    ~TempFileManager() {
        for (const auto& file : temp_files) {
            std::remove(file.c_str());
        }
    }
    
    std::string create_temp_file(const std::string& prefix) {
        char template_str[] = "/tmp/pugixml_wide_XXXXXX";
        int fd = mkstemp(template_str);
        if (fd != -1) {
            close(fd);
            std::string filename(template_str);
            temp_files.push_back(filename);
            return filename;
        }
        return "";
    }
    
    void cleanup() {
        for (const auto& file : temp_files) {
            std::remove(file.c_str());
        }
        temp_files.clear();
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML content and wide character operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    TempFileManager temp_files;
    
    // ==================== PHASE 1: ENCODING CONVERSION TESTING ====================
    // Target: as_wide() and as_utf8() functions for Unicode encoding conversion
    
    // Test as_wide() with various UTF-8 inputs
    if (fdp.remaining_bytes() > 10) {
        // Ensure we have at least 1 byte for the max parameter
        size_t max_utf8_len = std::min(fdp.remaining_bytes(), size_t(256));
        if (max_utf8_len >= 1) {
            size_t utf8_len = fdp.ConsumeIntegralInRange<size_t>(1, max_utf8_len);
            std::string utf8_input = fdp.ConsumeBytesAsString(utf8_len);
            
            // TARGET: as_wide(const char* str)
            try {
                std::wstring wide_result = pugi::as_wide(utf8_input.c_str());
                (void)wide_result;
            } catch (...) {
                // Ignore exceptions from invalid UTF-8
            }
            
            // TARGET: as_wide(const std::basic_string<char>& str)
            try {
                std::wstring wide_result2 = pugi::as_wide(utf8_input);
                (void)wide_result2;
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Test as_utf8() with various wide character inputs
    if (fdp.remaining_bytes() > 10) {
        // Create wide string from consumed bytes (treat as raw wchar_t data)
        // Ensure we have at least sizeof(wchar_t) bytes to get at least 1 wchar_t
        size_t max_wide_chars = std::min(fdp.remaining_bytes() / sizeof(wchar_t), size_t(128));
        if (max_wide_chars >= 1) {
            size_t wide_len = fdp.ConsumeIntegralInRange<size_t>(1, max_wide_chars);
            std::vector<wchar_t> wide_buffer(wide_len);
            for (size_t i = 0; i < wide_len && fdp.remaining_bytes() >= sizeof(wchar_t); ++i) {
                wide_buffer[i] = fdp.ConsumeIntegral<wchar_t>();
            }
            
            if (wide_len > 0) {
                std::wstring wide_input(wide_buffer.data(), wide_len);
                
                // TARGET: as_utf8(const wchar_t* str)
                try {
                    std::string utf8_result = pugi::as_utf8(wide_input.c_str());
                    (void)utf8_result;
                } catch (...) {
                    // Ignore exceptions from invalid wide character data
                }
                
                // TARGET: as_utf8(const std::basic_string<wchar_t>& str)
                try {
                    std::string utf8_result2 = pugi::as_utf8(wide_input);
                    (void)utf8_result2;
                } catch (...) {
                    // Ignore exceptions
                }
            }
        }
    }
    
    // ==================== PHASE 2: XML DOCUMENT CREATION WITH UNICODE ====================
    // Create XML document with Unicode content
    pugi::xml_document doc;
    
    // Consume XML content from fuzzer input
    size_t max_xml_size = std::min(fdp.remaining_bytes(), size_t(4096));
    if (max_xml_size >= 32) {
        size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(32, max_xml_size);
        std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
        
        // Parse XML with different encoding options
        pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_default);
        
        // If parsing failed, create a document with Unicode content
        if (!parse_result && fdp.remaining_bytes() > 100) {
            doc.reset();
            pugi::xml_node root = doc.append_child("unicode_test_root");
            
            // Add Unicode element names and attributes
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 30; ++i) {
                // Create element name with potential Unicode characters
                std::string elem_name_base = "element_";
                std::string elem_name_suffix = fdp.ConsumeRandomLengthString(10);
                std::string elem_name = elem_name_base + elem_name_suffix;
                
                pugi::xml_node child = root.append_child(elem_name.c_str());
                
                // Add attributes with Unicode values
                std::string attr_name = "unicode_attr_" + std::to_string(i);
                std::string attr_value = fdp.ConsumeRandomLengthString(50);
                child.append_attribute(attr_name.c_str()) = attr_value.c_str();
                
                // Add text content with Unicode
                std::string text_content = fdp.ConsumeRandomLengthString(100);
                child.text() = text_content.c_str();
                
                // Add CDATA section with Unicode
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
                    std::string cdata_content = fdp.ConsumeRandomLengthString(50);
                    pugi::xml_node cdata_node = child.append_child(pugi::node_cdata);
                    cdata_node.set_value(cdata_content.c_str());
                }
            }
        }
    }
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }
    
    // ==================== PHASE 3: WIDE CHARACTER FILE OPERATIONS ====================
    // Test file operations with wide character paths
    
    if (fdp.remaining_bytes() > 20) {
        // Create temp file for wide character operations
        std::string temp_file = temp_files.create_temp_file("pugixml_wide_");
        if (!temp_file.empty()) {
            // Convert to wide string for wide character APIs
            std::wstring wtemp_file(temp_file.begin(), temp_file.end());
            
            // Test different encoding combinations
            pugi::xml_encoding encodings[] = {
                pugi::encoding_auto,
                pugi::encoding_utf8,
                pugi::encoding_utf16_le,
                pugi::encoding_utf16_be,
                pugi::encoding_utf32_le,
                pugi::encoding_utf32_be,
                pugi::encoding_latin1
            };
            
            // Fixed valid ranges: 0-6 (7 encodings)
            size_t encoding_idx = fdp.ConsumeIntegralInRange<size_t>(0, 6);
            pugi::xml_encoding encoding = encodings[encoding_idx];
            
            unsigned int flags_combinations[] = {
                pugi::format_default,
                pugi::format_indent,
                pugi::format_no_declaration,
                pugi::format_no_escapes,
                pugi::format_save_file_text,
                pugi::format_write_bom,
                pugi::format_raw
            };
            // Fixed valid ranges: 0-6 (7 flags)
            size_t flags_idx = fdp.ConsumeIntegralInRange<size_t>(0, 6);
            unsigned int flags = flags_combinations[flags_idx];
            
            // TARGET: save_file(const wchar_t* path, ...)
            bool save_result = doc.save_file(wtemp_file.c_str(), 
                                           PUGIXML_TEXT("\t"),
                                           flags,
                                           encoding);
            (void)save_result;
            
            // If save succeeded, try loading back with wide character API
            if (save_result) {
                // TARGET: load_file(const wchar_t* path, ...)
                pugi::xml_document verify_doc;
                pugi::xml_parse_result load_result = verify_doc.load_file(wtemp_file.c_str(), 
                                                                         pugi::parse_default,
                                                                         encoding);
                (void)load_result;
                
                // Also try loading with different encoding to test conversion
                if (load_result) {
                    pugi::xml_document verify_doc2;
                    pugi::xml_encoding alt_encoding = (encoding == pugi::encoding_utf8) ? 
                                                      pugi::encoding_utf16_le : pugi::encoding_utf8;
                    pugi::xml_parse_result load_result2 = verify_doc2.load_file(wtemp_file.c_str(),
                                                                               pugi::parse_default,
                                                                               alt_encoding);
                    (void)load_result2;
                }
            }
            
            // Test with char* path for comparison
            bool save_result_char = doc.save_file(temp_file.c_str(),
                                                 "\t",
                                                 flags,
                                                 encoding);
            (void)save_result_char;
        }
    }
    
    // ==================== PHASE 4: WIDE CHARACTER STREAM OPERATIONS ====================
#ifndef PUGIXML_NO_STL
    if (fdp.remaining_bytes() > 50) {
        // Test wide character stream operations
        
        // Create wide string stream
        std::wstringstream wss;
        
        // TARGET: save(std::basic_ostream<wchar_t>& stream, ...)
        try {
            doc.save(wss, PUGIXML_TEXT("  "), pugi::format_indent);
            
            // Try to parse from the wide stream
            wss.seekg(0);
            pugi::xml_document stream_doc;
            
            // TARGET: load(std::basic_istream<wchar_t>& stream, ...)
            pugi::xml_parse_result stream_result = stream_doc.load(wss);
            (void)stream_result;
            
        } catch (...) {
            // Ignore stream exceptions
        }
        
        // Test with different format flags
        std::wstringstream wss2;
        try {
            unsigned int format_flags = fdp.ConsumeIntegral<unsigned int>();
            doc.save(wss2, PUGIXML_TEXT("\t"), format_flags);
        } catch (...) {
            // Ignore exceptions
        }
    }
#endif
    
    // ==================== PHASE 5: UNICODE CONTENT MANIPULATION ====================
    // Manipulate XML nodes with Unicode content
    
    if (root) {
        // Add Unicode content to existing nodes
        if (fdp.remaining_bytes() > 20) {
            std::string unicode_text = fdp.ConsumeRandomLengthString(100);
            pugi::xml_node unicode_node = root.append_child("unicode_content");
            unicode_node.text() = unicode_text.c_str();
            
            // Add Unicode attribute
            std::string unicode_attr_name = "unicode_" + fdp.ConsumeRandomLengthString(20);
            std::string unicode_attr_value = fdp.ConsumeRandomLengthString(50);
            unicode_node.append_attribute(unicode_attr_name.c_str()) = unicode_attr_value.c_str();
            
            // Test node value setting with Unicode
            if (fdp.remaining_bytes() > 10) {
                std::string node_value = fdp.ConsumeRandomLengthString(80);
                unicode_node.set_value(node_value.c_str());
            }
        }
        
        // Test text and attribute access with potential Unicode
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            // Get text as string (may contain Unicode)
            std::string text = child.text().as_string();
            (void)text;
            
            // Get attribute values
            for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
                std::string attr_name = attr.name();
                std::string attr_value = attr.value();
                (void)attr_name;
                (void)attr_value;
            }
        }
    }
    
    // ==================== PHASE 6: ENCODING-SPECIFIC TESTING ====================
    // Test specific encoding scenarios
    
    if (fdp.remaining_bytes() > 100) {
        // Create XML with specific encoding declaration
        pugi::xml_document encoding_doc;
        pugi::xml_node encoding_root = encoding_doc.append_child("encoding_test");
        
        // Add content that might trigger encoding-specific code paths
        std::string special_content = fdp.ConsumeRandomLengthString(200);
        encoding_root.text() = special_content.c_str();
        
        // Test saving with different encodings
        pugi::xml_encoding test_encodings[] = {
            pugi::encoding_utf8,
            pugi::encoding_utf16_le,
            pugi::encoding_utf16_be,
            pugi::encoding_latin1
        };
        
        for (int i = 0; i < 2 && fdp.remaining_bytes() > 10; ++i) {
            // Fixed valid range: 0-3 (4 encodings)
            size_t enc_idx = fdp.ConsumeIntegralInRange<size_t>(0, 3);
            pugi::xml_encoding test_enc = test_encodings[enc_idx];
            
            // Create temp file
            std::string enc_file = temp_files.create_temp_file("pugixml_enc_");
            if (!enc_file.empty()) {
                std::wstring wenc_file(enc_file.begin(), enc_file.end());
                
                // Save with specific encoding
                bool enc_save_result = encoding_doc.save_file(wenc_file.c_str(),
                                                             PUGIXML_TEXT(""),
                                                             pugi::format_default,
                                                             test_enc);
                (void)enc_save_result;
                
                // Try to load with same encoding
                if (enc_save_result) {
                    pugi::xml_document load_doc;
                    pugi::xml_parse_result enc_load_result = load_doc.load_file(wenc_file.c_str(),
                                                                               pugi::parse_default,
                                                                               test_enc);
                    (void)enc_load_result;
                }
            }
        }
    }
    
    // ==================== PHASE 7: ERROR HANDLING TESTING ====================
    // Test error cases for wide character operations
    
    if (fdp.remaining_bytes() > 10) {
        // Test with invalid wide character paths
        std::wstring invalid_path = L"";
        if (fdp.ConsumeBool()) {
            invalid_path = L"/invalid/path/that/does/not/exist/";
            invalid_path += std::to_wstring(fdp.ConsumeIntegral<uint64_t>());
            invalid_path += L".xml";
        }
        
        // TARGET: save_file with invalid path (should return false)
        bool invalid_save = doc.save_file(invalid_path.c_str(),
                                         PUGIXML_TEXT("\t"),
                                         pugi::format_default,
                                         pugi::encoding_auto);
        (void)invalid_save;
        
        // TARGET: load_file with invalid path (should return failed parse result)
        pugi::xml_document invalid_doc;
        pugi::xml_parse_result invalid_load = invalid_doc.load_file(invalid_path.c_str());
        (void)invalid_load;
    }
    
    return 0;
}

} // namespace harness_019
