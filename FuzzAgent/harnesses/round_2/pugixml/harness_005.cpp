#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

namespace harness_005 {

// Custom xml_writer implementation that discards output (for testing)
class DiscardingXmlWriter : public pugi::xml_writer {
public:
    virtual void write(const void* data, size_t size) override {
        // Just discard the data - we're testing that the save API works
        (void)data;
        (void)size;
    }
};

// Custom xml_writer that counts bytes written
class CountingXmlWriter : public pugi::xml_writer {
private:
    size_t total_bytes;
public:
    CountingXmlWriter() : total_bytes(0) {}
    
    virtual void write(const void* data, size_t size) override {
        total_bytes += size;
        (void)data; // Discard data, just count
    }
    
    size_t get_total_bytes() const { return total_bytes; }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Parse XML input using xml_document::load_buffer()
    // Consume a portion of input for XML content
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(16, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(
        xml_buffer.data(), 
        xml_buffer.size(), 
        pugi::parse_default, 
        pugi::encoding_auto
    );
    
    // If parsing failed, create a simple document for testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child(pugi::node_element);
        root.set_name("root");
        
        // Add some elements to make the document more interesting
        int element_count = fdp.ConsumeIntegralInRange<int>(1, 10);
        for (int i = 0; i < element_count && fdp.remaining_bytes() > 10; i++) {
            pugi::xml_node child = root.append_child(pugi::node_element);
            
            // Consume element name
            std::string elem_name = fdp.ConsumeRandomLengthString(50);
            child.set_name(elem_name.c_str());
            
            // Add text content
            if (fdp.ConsumeBool()) {
                std::string text = fdp.ConsumeRandomLengthString(100);
                child.append_child(pugi::node_pcdata).set_value(text.c_str());
            }
            
            // Add attributes
            int attr_count = fdp.ConsumeIntegralInRange<int>(0, 5);
            for (int j = 0; j < attr_count && fdp.remaining_bytes() > 10; j++) {
                std::string attr_name = fdp.ConsumeRandomLengthString(30);
                std::string attr_value = fdp.ConsumeRandomLengthString(50);
                child.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
            }
        }
    }
    
    // Step 2: Optionally modify the document tree using covered APIs
    // Add some random modifications to test different document structures
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        pugi::xml_node root = doc.document_element();
        if (root.empty()) {
            root = doc.first_child();
        }
        
        if (!root.empty()) {
            // Add a comment
            if (fdp.ConsumeBool()) {
                std::string comment = fdp.ConsumeRandomLengthString(100);
                root.append_child(pugi::node_comment).set_value(comment.c_str());
            }
            
            // Add a processing instruction
            if (fdp.ConsumeBool()) {
                std::string pi_target = fdp.ConsumeRandomLengthString(20);
                std::string pi_data = fdp.ConsumeRandomLengthString(50);
                pugi::xml_node pi = root.append_child(pugi::node_pi);
                pi.set_name(pi_target.c_str());
                pi.set_value(pi_data.c_str());
            }
        }
    }
    
    // Step 3: Test save to file with different encodings and flags
    // Create temporary files for testing
    char temp_file_path[] = "/tmp/pugixml_fuzz_XXXXXX";
    int temp_fd = mkstemp(temp_file_path);
    if (temp_fd != -1) {
        close(temp_fd); // We'll use C++ streams to write
        
        // Test save_file with char* path
        const char* indent = fdp.ConsumeBool() ? "  " : "\t";
        unsigned int flags = 0;
        
        // Consume different flag combinations
        if (fdp.ConsumeBool()) flags |= pugi::format_indent;
        if (fdp.ConsumeBool()) flags |= pugi::format_write_bom;
        if (fdp.ConsumeBool()) flags |= pugi::format_raw;
        if (fdp.ConsumeBool()) flags |= pugi::format_no_declaration;
        if (fdp.ConsumeBool()) flags |= pugi::format_no_escapes;
        if (fdp.ConsumeBool()) flags |= pugi::format_save_file_text;
        
        // Consume encoding
        pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(
            fdp.ConsumeIntegralInRange<int>(0, 4)
        );
        
        bool save_result = doc.save_file(temp_file_path, indent, flags, encoding);
        (void)save_result; // Result may be false for invalid paths, that's OK
        
        // Test save_file with wchar_t* path if available
        #ifdef _WIN32
        wchar_t wtemp_file_path[] = L"/tmp/pugixml_fuzz_XXXXXX";
        _wmktemp(wtemp_file_path);
        doc.save_file(wtemp_file_path, L"\t", flags, encoding);
        #endif
        
        // Clean up temp file
        std::remove(temp_file_path);
    }
    
    // Step 4: Test save to std::ostream and std::wostream
    #ifndef PUGIXML_NO_STL
    if (fdp.remaining_bytes() > 10) {
        // Test with std::ostringstream (char)
        std::ostringstream oss;
        
        // Consume different formatting options
        std::string stream_indent = fdp.ConsumeRandomLengthString(4);
        if (stream_indent.empty()) stream_indent = "  ";
        
        unsigned int stream_flags = 0;
        if (fdp.ConsumeBool()) stream_flags |= pugi::format_indent;
        if (fdp.ConsumeBool()) stream_flags |= pugi::format_write_bom;
        if (fdp.ConsumeBool()) stream_flags |= pugi::format_no_declaration;
        
        pugi::xml_encoding stream_encoding = static_cast<pugi::xml_encoding>(
            fdp.ConsumeIntegralInRange<int>(0, 4)
        );
        
        doc.save(oss, stream_indent.c_str(), stream_flags, stream_encoding);
        
        // Test with std::wostringstream (wchar_t)
        // Use PUGIXML_TEXT macro to handle both char and wchar_t modes
        if (fdp.remaining_bytes() > 10) {
            std::wostringstream woss;
            
            // Use appropriate string type based on PUGIXML_WCHAR_MODE
            #ifdef PUGIXML_WCHAR_MODE
            std::wstring windent = L"\t";
            if (fdp.ConsumeBool()) windent = L"  ";
            #else
            std::string windent = "\t";
            if (fdp.ConsumeBool()) windent = "  ";
            #endif
            
            unsigned int wflags = 0;
            if (fdp.ConsumeBool()) wflags |= pugi::format_indent;
            if (fdp.ConsumeBool()) wflags |= pugi::format_write_bom;
            
            doc.save(woss, windent.c_str(), wflags);
        }
    }
    #endif
    
    // Step 5: Test save to custom xml_writer implementations
    if (fdp.remaining_bytes() > 10) {
        // Test with discarding writer
        DiscardingXmlWriter discarding_writer;
        
        std::string writer_indent = fdp.ConsumeRandomLengthString(4);
        if (writer_indent.empty()) writer_indent = "\t";
        
        unsigned int writer_flags = 0;
        if (fdp.ConsumeBool()) writer_flags |= pugi::format_indent;
        if (fdp.ConsumeBool()) writer_flags |= pugi::format_raw;
        
        pugi::xml_encoding writer_encoding = static_cast<pugi::xml_encoding>(
            fdp.ConsumeIntegralInRange<int>(0, 4)
        );
        
        doc.save(discarding_writer, writer_indent.c_str(), writer_flags, writer_encoding);
        
        // Test with counting writer
        CountingXmlWriter counting_writer;
        doc.save(counting_writer, "\t", pugi::format_default, pugi::encoding_auto);
        
        // The count is available but we don't need to use it
        (void)counting_writer.get_total_bytes();
    }
    
    // Step 6: Test save to xml_writer_stream and xml_writer_file
    #ifndef PUGIXML_NO_STL
    if (fdp.remaining_bytes() > 10) {
        // Create a stringstream and xml_writer_stream
        std::ostringstream test_stream;
        pugi::xml_writer_stream stream_writer(test_stream);
        
        std::string stream_indent2 = fdp.ConsumeRandomLengthString(4);
        if (stream_indent2.empty()) stream_indent2 = "  ";
        
        doc.save(stream_writer, stream_indent2.c_str(), pugi::format_default, pugi::encoding_auto);
    }
    #endif
    
    // Create another temporary file for xml_writer_file testing
    char temp_file_path2[] = "/tmp/pugixml_fuzz2_XXXXXX";
    int temp_fd2 = mkstemp(temp_file_path2);
    if (temp_fd2 != -1) {
        FILE* temp_file = fdopen(temp_fd2, "w");
        if (temp_file) {
            pugi::xml_writer_file file_writer(temp_file);
            
            std::string file_indent = fdp.ConsumeRandomLengthString(4);
            if (file_indent.empty()) file_indent = "\t";
            
            doc.save(file_writer, file_indent.c_str(), pugi::format_default, pugi::encoding_auto);
            
            fclose(temp_file);
        } else {
            close(temp_fd2);
        }
        
        std::remove(temp_file_path2);
    }
    
    // Step 7: Clean up
    // xml_document destructor will handle cleanup automatically
    // Optionally call reset() explicitly
    if (fdp.ConsumeBool()) {
        doc.reset();
    }
    
    return 0;
}

} // namespace harness_005
