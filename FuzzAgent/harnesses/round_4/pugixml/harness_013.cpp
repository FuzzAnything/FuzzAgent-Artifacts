#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>

namespace harness_013 {

// Temporary file utilities for file I/O testing
class TemporaryFile {
private:
    std::string path_;
    bool wide_path_;

public:
    TemporaryFile(const std::string& base_name) : wide_path_(false) {
        // Create a unique temporary file path
        static int counter = 0;
        path_ = "/tmp/pugixml_fuzz_" + std::to_string(counter++) + "_" + base_name;
    }
    
    TemporaryFile(const std::wstring& base_name) : wide_path_(true) {
        static int counter = 0;
        // For wide paths, we'll use ASCII names to avoid encoding issues
        path_ = "/tmp/pugixml_fuzz_" + std::to_string(counter++) + "_wide";
    }
    
    ~TemporaryFile() {
        // Clean up temporary file
        std::remove(path_.c_str());
    }
    
    const char* narrow_path() const { 
        return path_.c_str(); 
    }
    
    const wchar_t* wide_path() const { 
        // Create a static wide string copy
        static std::wstring wide_path;
        wide_path = std::wstring(path_.begin(), path_.end());
        return wide_path.c_str();
    }
    
    bool is_wide() const { 
        return wide_path_; 
    }
    
    bool write_content(const std::string& content) {
        std::ofstream file(path_, std::ios::binary);
        if (!file) return false;
        file.write(content.c_str(), content.size());
        return file.good();
    }
    
    bool write_content(const std::vector<uint8_t>& content) {
        std::ofstream file(path_, std::ios::binary);
        if (!file) return false;
        file.write(reinterpret_cast<const char*>(content.data()), content.size());
        return file.good();
    }
    
    std::string read_content() {
        std::ifstream file(path_, std::ios::binary | std::ios::ate);
        if (!file) return "";
        
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::vector<char> buffer(size);
        if (file.read(buffer.data(), size)) {
            return std::string(buffer.data(), buffer.size());
        }
        return "";
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for file I/O operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t file_operation_mode = fdp.ConsumeIntegral<uint8_t>() % 6;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t save_flags = fdp.ConsumeIntegral<uint8_t>();
    uint8_t encoding_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    bool use_wide_path = fdp.ConsumeBool();
    bool test_append_buffer = fdp.ConsumeBool();
    bool test_wide_stream = fdp.ConsumeBool();
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    
    // Consume data for node manipulation
    std::string node_name = fdp.ConsumeRandomLengthString(128);
    std::string attr_name = fdp.ConsumeRandomLengthString(128);
    std::string attr_value = fdp.ConsumeRandomLengthString(256);
    std::string node_value = fdp.ConsumeRandomLengthString(512);
    
    // Create temporary files for testing
    TemporaryFile temp_file("test.xml");
    TemporaryFile temp_file_save("save.xml");
    
    // Write initial XML content to temporary file
    if (!temp_file.write_content(xml_content)) {
        return 0; // Failed to create temp file
    }
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    
    // TARGET API 1 & 2: Load from file (narrow and wide versions)
    if (use_wide_path) {
        // Use wide path version
        result = doc.load_file(temp_file.wide_path(), parse_options, 
                              static_cast<pugi::xml_encoding>(encoding_type));
    } else {
        // Use narrow path version
        result = doc.load_file(temp_file.narrow_path(), parse_options,
                              static_cast<pugi::xml_encoding>(encoding_type));
    }
    
    // If file loading fails, create a simple document for testing
    if (!result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        for (int i = 0; i < 3; i++) {
            pugi::xml_node child = root.append_child(("node" + std::to_string(i)).c_str());
            child.append_attribute(("attr" + std::to_string(i)).c_str()) = 
                ("value" + std::to_string(i)).c_str();
            child.text().set(("text" + std::to_string(i)).c_str());
        }
    }
    
    // Optional node manipulation before saving
    pugi::xml_node root = doc.document_element();
    if (root && !node_name.empty()) {
        // Add a new child node with fuzzed data
        pugi::xml_node new_child = root.append_child(node_name.c_str());
        if (new_child && !attr_name.empty() && !attr_value.empty()) {
            new_child.append_attribute(attr_name.c_str()) = attr_value.c_str();
        }
        if (new_child && !node_value.empty()) {
            new_child.text().set(node_value.c_str());
        }
    }
    
    // TARGET API 3 & 4: Save to file (narrow and wide versions)
    bool save_success = false;
    if (use_wide_path) {
        // Use wide path version for saving
        save_success = doc.save_file(temp_file_save.wide_path(), 
                                    PUGIXML_TEXT("\t"), 
                                    save_flags,
                                    static_cast<pugi::xml_encoding>(encoding_type));
    } else {
        // Use narrow path version for saving
        save_success = doc.save_file(temp_file_save.narrow_path(),
                                    PUGIXML_TEXT("\t"),
                                    save_flags,
                                    static_cast<pugi::xml_encoding>(encoding_type));
    }
    
    // Verify the saved file can be loaded back
    if (save_success) {
        pugi::xml_document verify_doc;
        pugi::xml_parse_result verify_result;
        
        if (use_wide_path) {
            verify_result = verify_doc.load_file(temp_file_save.wide_path());
        } else {
            verify_result = verify_doc.load_file(temp_file_save.narrow_path());
        }
        
        // Verify round-trip worked
        if (!verify_result) {
            // Round-trip failed, but that's OK for fuzzing
        }
    }
    
    // TARGET API 5: Test append_buffer if enabled
    if (test_append_buffer && root) {
        // Create some additional XML content to append
        std::string append_content = fdp.ConsumeRandomLengthString(1024);
        if (!append_content.empty()) {
            pugi::xml_parse_result append_result = 
                root.append_buffer(append_content.c_str(), 
                                  append_content.size(),
                                  parse_options,
                                  static_cast<pugi::xml_encoding>(encoding_type));
            
            // If append succeeded, save the updated document
            if (append_result) {
                TemporaryFile temp_append("append.xml");
                doc.save_file(temp_append.narrow_path());
            }
        }
    }
    
    // TARGET API 6: Test save to wide stream if enabled
    if (test_wide_stream) {
#ifndef PUGIXML_NO_STL
        std::wstringstream wss;
        doc.save(wss);
        
        // Verify we can parse from the stream
        std::wstring saved_content = wss.str();
        if (!saved_content.empty()) {
            pugi::xml_document stream_doc;
            // Create a string from wide stream for parsing
            std::string narrow_content(saved_content.begin(), saved_content.end());
            stream_doc.load_buffer(narrow_content.c_str(), narrow_content.size());
        }
#endif
    }
    
    // Test different file operation modes
    switch (file_operation_mode) {
        case 0:
            // Test load from file, modify, save to different file
            {
                TemporaryFile temp_output("output.xml");
                doc.save_file(temp_output.narrow_path());
            }
            break;
            
        case 1:
            // Test multiple save operations with different flags
            {
                TemporaryFile temp1("temp1.xml");
                TemporaryFile temp2("temp2.xml");
                
                doc.save_file(temp1.narrow_path(), PUGIXML_TEXT("  "), pugi::format_indent);
                doc.save_file(temp2.narrow_path(), PUGIXML_TEXT(""), pugi::format_raw);
            }
            break;
            
        case 2:
            // Test file loading with different encodings
            {
                pugi::xml_document encoding_doc;
                for (int i = 0; i < 3; i++) {
                    uint8_t test_encoding = fdp.ConsumeIntegral<uint8_t>() % 5;
                    encoding_doc.load_file(temp_file.narrow_path(), 
                                          parse_options,
                                          static_cast<pugi::xml_encoding>(test_encoding));
                }
            }
            break;
            
        case 3:
            // Test file saving with different indentations
            {
                std::string indent_str = fdp.ConsumeRandomLengthString(8);
                if (!indent_str.empty()) {
                    TemporaryFile temp_indent("indent.xml");
                    doc.save_file(temp_indent.narrow_path(), 
                                 indent_str.c_str(),
                                 save_flags,
                                 static_cast<pugi::xml_encoding>(encoding_type));
                }
            }
            break;
            
        case 4:
            // Test error cases with invalid file paths
            {
                pugi::xml_document error_doc;
                pugi::xml_parse_result error_result;
                
                // Try loading from non-existent file
                error_result = error_doc.load_file("/non/existent/path/file.xml");
                
                // Try saving to invalid path
                bool save_error = error_doc.save_file("/invalid/path/file.xml");
                (void)save_error; // Ignore result
            }
            break;
            
        case 5:
            // Test combined operations: load -> modify -> append -> save
            {
                if (root && test_append_buffer) {
                    // Create additional content for appending
                    std::string extra_xml = "<extra><item>test</item></extra>";
                    root.append_buffer(extra_xml.c_str(), extra_xml.size());
                    
                    TemporaryFile temp_combined("combined.xml");
                    doc.save_file(temp_combined.narrow_path());
                }
            }
            break;
    }
    
    return 0;
}

} // namespace harness_013
