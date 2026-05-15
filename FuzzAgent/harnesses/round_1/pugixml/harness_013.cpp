#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <iostream>

namespace harness_013 {


#ifndef PUGIXML_NO_STL
#endif

// Custom temporary file manager for save_file testing
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
        char template_str[] = "/tmp/pugixml_XXXXXX";
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
    // Minimum size check: need enough for XML content, file operations, and debugging
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    TempFileManager temp_files;
    
    // ==================== PHASE 1: XML DOCUMENT CREATION & PARSING ====================
    // Create XML document for persistence testing
    pugi::xml_document doc;
    
    // Consume XML content from fuzzer input
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(32, std::min(size / 2, size_t(4096)));
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    // Parse XML with different options to get various parse results
    pugi::xml_parse_result parse_results[4];
    parse_results[0] = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_default);
    parse_results[1] = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_minimal);
    parse_results[2] = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    parse_results[3] = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_ws_pcdata | pugi::parse_eol);
    
    // If all parsing attempts failed, create a structured document for testing
    bool any_success = false;
    for (int i = 0; i < 4; i++) {
        if (parse_results[i]) {
            any_success = true;
            break;
        }
    }
    
    if (!any_success && fdp.remaining_bytes() > 50) {
        doc.reset();
        pugi::xml_node root = doc.append_child("persistence_test_root");
        
        // Add structured content for persistence testing
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 20; ++i) {
            std::string child_name = "child_" + std::to_string(i);
            pugi::xml_node child = root.append_child(child_name.c_str());
            
            // Add attributes
            child.append_attribute("id") = std::to_string(i).c_str();
            child.append_attribute("type") = (i % 2 == 0) ? "even" : "odd";
            
            // Add text content
            std::string text = fdp.ConsumeRandomLengthString(50);
            child.text() = text.c_str();
            
            // Add attributes with special characters for encoding testing
            if (fdp.remaining_bytes() > 10) {
                std::string special_attr = "special_" + std::to_string(i);
                std::string special_value = fdp.ConsumeRandomLengthString(30);
                child.append_attribute(special_attr.c_str()) = special_value.c_str();
            }
        }
        
        // Re-parse to get a successful parse result
#ifndef PUGIXML_NO_STL
        std::stringstream ss;
        doc.save(ss);
        std::string xml_str = ss.str();
        parse_results[0] = doc.load_string(xml_str.c_str());
#endif
    }
    
    pugi::xml_node root = doc.document_element();
    // ==================== PHASE 2: PARSE RESULT DIAGNOSTICS TESTING ====================
    // TARGET: xml_parse_result::description() - 36 undiscovered branches
    
    for (int i = 0; i < 4; i++) {
        // Get error description from parse result
        const char* error_desc = parse_results[i].description();
        (void)error_desc; // Use to avoid compiler warnings
        
        // Test parse result properties
        pugi::xml_parse_status status = parse_results[i].status;
        ptrdiff_t offset = parse_results[i].offset;
        pugi::xml_encoding encoding = parse_results[i].encoding;
        
        // Test bool conversion
        bool is_valid = static_cast<bool>(parse_results[i]);
        (void)is_valid;
    }
    
    if (!root) {
        // Even without valid document, we can still test some persistence APIs
        // with empty document
        pugi::xml_document empty_doc;
        
        // Test save_file with empty document
        std::string temp_file = temp_files.create_temp_file("empty_");
        if (!temp_file.empty()) {
            bool save_result = empty_doc.save_file(temp_file.c_str());
            (void)save_result;
        }
        
        return 0;
    }
    
    // ==================== PHASE 3: DEBUG INFORMATION EXTRACTION ====================
    // TARGET: xml_node::offset_debug() - 39 undiscovered branches
    
    // Traverse document and collect debug offsets
    std::vector<ptrdiff_t> debug_offsets;
    
    // Get debug offset from root
    ptrdiff_t root_offset = root.offset_debug();
    debug_offsets.push_back(root_offset);
    
    // Traverse children and get their debug offsets
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        ptrdiff_t child_offset = child.offset_debug();
        debug_offsets.push_back(child_offset);
        
        // Get offsets from attributes
        for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
            // Note: xml_attribute doesn't have offset_debug, but we can test other debug info
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
            (void)attr_name;
            (void)attr_value;
        }
        
        // Get offsets from grandchildren
        for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
            ptrdiff_t grandchild_offset = grandchild.offset_debug();
            debug_offsets.push_back(grandchild_offset);
        }
    }
    
    // ==================== PHASE 4: DOCUMENT PERSISTENCE OPERATIONS ====================
    // TARGET: save_file variants and stream save - 27 undiscovered branches total
    
    // Test different save_file configurations
    while (fdp.remaining_bytes() > 100) {
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        // Create temp file for this operation
        std::string temp_file = temp_files.create_temp_file("pugixml_");
        if (temp_file.empty()) {
            continue;
        }
        
        switch (operation) {
            case 0: {
                // TARGET: save_file(const char* path, ...)
                std::string indent = fdp.ConsumeRandomLengthString(4);
                unsigned int flags = fdp.ConsumeIntegral<unsigned int>();
                pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(
                    fdp.ConsumeIntegralInRange<int>(0, 5));
                
                bool result = doc.save_file(temp_file.c_str(), 
                                           indent.empty() ? "\t" : indent.c_str(),
                                           flags,
                                           encoding);
                (void)result;
                break;
            }
                
            case 1: {
                // TARGET: save_file(const wchar_t* path, ...) - wide char version
                std::wstring wtemp_file(temp_file.begin(), temp_file.end());
                std::string indent = fdp.ConsumeRandomLengthString(4);
                unsigned int flags = fdp.ConsumeIntegral<unsigned int>();
                pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(
                    fdp.ConsumeIntegralInRange<int>(0, 5));
                
                bool result = doc.save_file(wtemp_file.c_str(),
                                           indent.empty() ? PUGIXML_TEXT("\t") : 
                                           reinterpret_cast<const pugi::char_t*>(indent.c_str()),
                                           flags,
                                           encoding);
                (void)result;
                break;
            }
                
            case 2: {
                // Test with different indent styles
                const char* indents[] = {"", "  ", "\t", "    ", "  "};
                size_t indent_idx = fdp.ConsumeIntegralInRange<size_t>(0, 4);
                unsigned int flags = fdp.ConsumeIntegral<unsigned int>();
                
                bool result = doc.save_file(temp_file.c_str(),
                                           indents[indent_idx],
                                           flags,
                                           pugi::encoding_auto);
                (void)result;
                break;
            }
                
            case 3: {
                // Test with format flags combinations
                unsigned int flags_combinations[] = {
                    pugi::format_default,
                    pugi::format_indent,
                    pugi::format_no_declaration,
                    pugi::format_no_escapes,
                    pugi::format_save_file_text,
                    pugi::format_write_bom,
                    pugi::format_raw
                };
                size_t flags_idx = fdp.ConsumeIntegralInRange<size_t>(0, 6);
                pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(
                    fdp.ConsumeIntegralInRange<int>(0, 5));
                
                bool result = doc.save_file(temp_file.c_str(),
                                           "\t",
                                           flags_combinations[flags_idx],
                                           encoding);
                (void)result;
                break;
            }
                
            case 4: {
                // Test different encodings
                pugi::xml_encoding encodings[] = {
                    pugi::encoding_auto,
                    pugi::encoding_utf8,
                    pugi::encoding_utf16_le,
                    pugi::encoding_utf16_be,
                    pugi::encoding_utf32_le,
                    pugi::encoding_utf32_be,
                    pugi::encoding_latin1
                };
                size_t encoding_idx = fdp.ConsumeIntegralInRange<size_t>(0, 6);
                unsigned int flags = fdp.ConsumeIntegral<unsigned int>();
                
                bool result = doc.save_file(temp_file.c_str(),
                                           "\t",
                                           flags,
                                           encodings[encoding_idx]);
                (void)result;
                break;
            }
                
            case 5: {
                // Test with very long file path
                std::string long_path = temp_file + "_" + fdp.ConsumeRandomLengthString(100);
                bool result = doc.save_file(long_path.c_str());
                (void)result;
                // Clean up if file was created
                std::remove(long_path.c_str());
                break;
            }
                
            case 6: {
                // Test save to string (alternative to file)
#ifndef PUGIXML_NO_STL
                std::stringstream ss;
                doc.save(ss);
                std::string xml_string = ss.str();
                // Verify we can re-parse the saved string
                pugi::xml_document verify_doc;
                pugi::xml_parse_result verify_result = verify_doc.load_string(xml_string.c_str());
                (void)verify_result;
#endif
                break;
            }
                
            case 7: {
#ifndef PUGIXML_NO_STL
                // TARGET: save to std::ostream
                std::stringstream ss;
                unsigned int flags = fdp.ConsumeIntegral<unsigned int>();
                pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(
                    fdp.ConsumeIntegralInRange<int>(0, 5));
                
                doc.save(ss, "\t", flags, encoding);
                
                // Test wide character stream as well
                std::wstringstream wss;
                doc.save(wss);
                break;
#endif
            }
        }
        
        // Test reading back the saved file
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
            pugi::xml_document verify_doc;
            pugi::xml_parse_result load_result = verify_doc.load_file(temp_file.c_str());
            
            // Get description of load result for error testing
            const char* load_error_desc = load_result.description();
            (void)load_error_desc;
            
            // If loaded successfully, verify structure
            if (load_result) {
                pugi::xml_node verify_root = verify_doc.document_element();
                if (verify_root) {
                    // Get debug offset of loaded document
                    ptrdiff_t loaded_offset = verify_root.offset_debug();
                    (void)loaded_offset;
                    
                    // Compare some basic properties
                    const char* original_name = root.name();
                    const char* loaded_name = verify_root.name();
                    (void)original_name;
                    (void)loaded_name;
                }
            }
        }
    }
    
    // ==================== PHASE 5: ERROR PATH TESTING ====================
    // Test save_file with invalid paths to trigger error paths
    if (fdp.remaining_bytes() > 50) {
        // Test with empty path
        bool empty_result = doc.save_file("");
        (void)empty_result;
        
        // Test with NULL path
        // Note: This might crash, so we need to be careful
        // bool null_result = doc.save_file(nullptr);
        // (void)null_result;
        
        // Test with path that's likely to fail (directory path)
        std::string dir_path = "/tmp/pugixml_test_dir_" + fdp.ConsumeRandomLengthString(10);
        bool dir_result = doc.save_file(dir_path.c_str());
        (void)dir_result;
        
        // Clean up directory if created
        std::remove(dir_path.c_str());
    }
    
    // ==================== PHASE 6: MEMORY PERSISTENCE TESTING ====================
    // Test in-memory persistence alternatives
    if (fdp.remaining_bytes() > 100) {
        // Create a custom xml_writer for memory persistence
        class MemoryWriter : public pugi::xml_writer {
        public:
            std::vector<char> buffer;
            
            virtual void write(const void* data, size_t size) {
                const char* ptr = static_cast<const char*>(data);
                buffer.insert(buffer.end(), ptr, ptr + size);
            }
        };
        
        MemoryWriter writer;
        unsigned int flags = fdp.ConsumeIntegral<unsigned int>();
        pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(
            fdp.ConsumeIntegralInRange<int>(0, 5));
        
        doc.save(writer, "\t", flags, encoding);
        
        // Verify the written data can be parsed back
        if (!writer.buffer.empty()) {
            pugi::xml_document memory_doc;
            pugi::xml_parse_result memory_result = memory_doc.load_buffer(
                writer.buffer.data(), writer.buffer.size());
            
            // Test parse result diagnostics
            const char* memory_error_desc = memory_result.description();
            (void)memory_error_desc;
        }
    }
    
    // ==================== PHASE 7: DOCUMENT MANIPULATION & RE-SAVE ====================
    // Modify document and test persistence of changes
    if (fdp.remaining_bytes() > 50 && root) {
        // Add some new nodes based on fuzzer input
        size_t num_new_nodes = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        for (size_t i = 0; i < num_new_nodes && fdp.remaining_bytes() > 20; ++i) {
            std::string node_name = "fuzz_node_" + std::to_string(i);
            std::string node_text = fdp.ConsumeRandomLengthString(30);
            
            pugi::xml_node new_node = root.append_child(node_name.c_str());
            new_node.text() = node_text.c_str();
            
            // Add attributes
            size_t num_attrs = fdp.ConsumeIntegralInRange<size_t>(0, 3);
            for (size_t j = 0; j < num_attrs && fdp.remaining_bytes() > 10; ++j) {
                std::string attr_name = "attr_" + std::to_string(j);
                std::string attr_value = fdp.ConsumeRandomLengthString(20);
                new_node.append_attribute(attr_name.c_str()) = attr_value.c_str();
            }
        }
        
        // Save modified document
        std::string modified_temp = temp_files.create_temp_file("modified_");
        if (!modified_temp.empty()) {
            bool modified_result = doc.save_file(modified_temp.c_str());
            (void)modified_result;
            
            // Get debug offset after modification
            ptrdiff_t modified_offset = root.offset_debug();
            (void)modified_offset;
        }
    }
    
    // Clean up all temp files (handled by TempFileManager destructor)
    temp_files.cleanup();
    
    return 0;
}

} // namespace harness_013
