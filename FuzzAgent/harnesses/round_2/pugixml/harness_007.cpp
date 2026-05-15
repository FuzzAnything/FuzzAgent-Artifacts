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
#include <cwchar>

namespace harness_007 {


// Custom xml_writer implementation for testing save() API
class TestXmlWriter : public pugi::xml_writer {
private:
    size_t bytes_written;
public:
    TestXmlWriter() : bytes_written(0) {}
    
    virtual void write(const void* data, size_t size) override {
        bytes_written += size;
        // Just discard the data - we're testing that the API works
        (void)data;
    }
    
    size_t get_bytes_written() const { return bytes_written; }
};

// Custom xml_writer that accumulates data in a string
class StringXmlWriter : public pugi::xml_writer {
private:
    std::string buffer;
public:
    virtual void write(const void* data, size_t size) override {
        buffer.append(static_cast<const char*>(data), size);
    }
    
    const std::string& get_buffer() const { return buffer; }
    void clear() { buffer.clear(); }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Parse XML input using xml_document::load_buffer() or load_string()
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    // Choose loading method based on input
    bool use_load_string = fdp.ConsumeBool();
    if (use_load_string && fdp.remaining_bytes() > 10) {
        // Use load_string
        std::string xml_content = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
        parse_result = doc.load_string(xml_content.c_str());
    } else {
        // Use load_buffer
        size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(16, size / 2);
        std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
        parse_result = doc.load_buffer(
            xml_buffer.data(), 
            xml_buffer.size(), 
            pugi::parse_default, 
            pugi::encoding_auto
        );
    }
    
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
    
    // Step 2: Optionally manipulate the document tree
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
            
            // Modify existing nodes
            for (pugi::xml_node child = root.first_child(); child && fdp.remaining_bytes() > 10; child = child.next_sibling()) {
                if (fdp.ConsumeBool()) {
                    std::string new_value = fdp.ConsumeRandomLengthString(50);
                    child.set_value(new_value.c_str());
                }
            }
        }
    }
    
    // Step 3: Test save_file with char* path
    if (fdp.remaining_bytes() > 10) {
        char temp_file_path[] = "/tmp/pugixml_fuzz_XXXXXX";
        int temp_fd = mkstemp(temp_file_path);
        if (temp_fd != -1) {
            close(temp_fd);
            
            // Consume formatting options
            std::string indent = fdp.ConsumeRandomLengthString(4);
            if (indent.empty()) indent = "  ";
            
            unsigned int flags = 0;
            if (fdp.ConsumeBool()) flags |= pugi::format_indent;
            if (fdp.ConsumeBool()) flags |= pugi::format_write_bom;
            if (fdp.ConsumeBool()) flags |= pugi::format_raw;
            if (fdp.ConsumeBool()) flags |= pugi::format_no_declaration;
            if (fdp.ConsumeBool()) flags |= pugi::format_no_escapes;
            if (fdp.ConsumeBool()) flags |= pugi::format_save_file_text;
            
            pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(
                fdp.ConsumeIntegralInRange<int>(0, 4)
            );
            
            // Target API 1: xml_document::save_file (char* path)
            bool save_result = doc.save_file(temp_file_path, indent.c_str(), flags, encoding);
            (void)save_result; // Result may be false, that's OK for fuzzing
            
            // Clean up temp file
            std::remove(temp_file_path);
        }
    }
    
    // Step 4: Test save_file with wchar_t* path if supported
    #ifdef _WIN32
    if (fdp.remaining_bytes() > 10) {
        wchar_t wtemp_file_path[] = L"/tmp/pugixml_fuzz_XXXXXX";
        _wmktemp(wtemp_file_path);
        
        // Consume formatting options
        std::string windent = fdp.ConsumeRandomLengthString(4);
        if (windent.empty()) windent = "  ";
        
        unsigned int wflags = 0;
        if (fdp.ConsumeBool()) wflags |= pugi::format_indent;
        if (fdp.ConsumeBool()) wflags |= pugi::format_write_bom;
        
        pugi::xml_encoding wencoding = static_cast<pugi::xml_encoding>(
            fdp.ConsumeIntegralInRange<int>(0, 4)
        );
        
        // Target API 2: xml_document::save_file (wchar_t* path)
        doc.save_file(wtemp_file_path, windent.c_str(), wflags, wencoding);
        
        // Clean up temp file
        _wremove(wtemp_file_path);
    }
    #endif
    
    // Step 5: Test save to xml_writer
    if (fdp.remaining_bytes() > 10) {
        // Test with different writer implementations
        
        // Target API 3: xml_document::save to xml_writer
        TestXmlWriter test_writer;
        std::string writer_indent = fdp.ConsumeRandomLengthString(4);
        if (writer_indent.empty()) writer_indent = "\t";
        
        unsigned int writer_flags = 0;
        if (fdp.ConsumeBool()) writer_flags |= pugi::format_indent;
        if (fdp.ConsumeBool()) writer_flags |= pugi::format_write_bom;
        
        pugi::xml_encoding writer_encoding = static_cast<pugi::xml_encoding>(
            fdp.ConsumeIntegralInRange<int>(0, 4)
        );
        
        doc.save(test_writer, writer_indent.c_str(), writer_flags, writer_encoding);
        
        // Also test with string writer
        StringXmlWriter string_writer;
        doc.save(string_writer, "\t", pugi::format_default, pugi::encoding_auto);
    }
    
    // Step 6: Test save to std::ostream and std::wostream (if STL is enabled)
    #ifndef PUGIXML_NO_STL
    if (fdp.remaining_bytes() > 10) {
        // Target API 4: xml_document::save to std::ostream
        std::ostringstream oss;
        
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
        
        // Target API 5: xml_document::save to std::wostream
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
            
            unsigned int wstream_flags = 0;
            if (fdp.ConsumeBool()) wstream_flags |= pugi::format_indent;
            if (fdp.ConsumeBool()) wstream_flags |= pugi::format_write_bom;
            
            doc.save(woss, windent.c_str(), wstream_flags);
        }
    }
    #endif
    
    // Step 7: Test xml_node::print operations
    if (fdp.remaining_bytes() > 10) {
        // Get a node to print (document element or first child)
        pugi::xml_node node_to_print = doc.document_element();
        if (node_to_print.empty()) {
            node_to_print = doc.first_child();
        }
        
        if (!node_to_print.empty()) {
            // Target API 6: xml_node::print to xml_writer
            TestXmlWriter print_writer;
            std::string print_indent = fdp.ConsumeRandomLengthString(4);
            if (print_indent.empty()) print_indent = "\t";
            
            unsigned int print_flags = 0;
            if (fdp.ConsumeBool()) print_flags |= pugi::format_indent;
            if (fdp.ConsumeBool()) print_flags |= pugi::format_write_bom;
            if (fdp.ConsumeBool()) print_flags |= pugi::format_no_escapes;
            
            pugi::xml_encoding print_encoding = static_cast<pugi::xml_encoding>(
                fdp.ConsumeIntegralInRange<int>(0, 4)
            );
            
            unsigned int depth = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
            
            node_to_print.print(print_writer, print_indent.c_str(), print_flags, print_encoding, depth);
            
            // Target API 7: xml_node::print to std::ostream (if STL is enabled)
            #ifndef PUGIXML_NO_STL
            if (fdp.remaining_bytes() > 10) {
                std::ostringstream print_oss;
                
                std::string node_print_indent = fdp.ConsumeRandomLengthString(4);
                if (node_print_indent.empty()) node_print_indent = "  ";
                
                unsigned int node_print_flags = 0;
                if (fdp.ConsumeBool()) node_print_flags |= pugi::format_indent;
                if (fdp.ConsumeBool()) node_print_flags |= pugi::format_write_bom;
                
                pugi::xml_encoding node_print_encoding = static_cast<pugi::xml_encoding>(
                    fdp.ConsumeIntegralInRange<int>(0, 4)
                );
                
                unsigned int node_depth = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
                
                node_to_print.print(print_oss, node_print_indent.c_str(), node_print_flags, node_print_encoding, node_depth);
                
                // Also test print to std::wostream
                if (fdp.remaining_bytes() > 10) {
                    std::wostringstream print_woss;
                    
                    #ifdef PUGIXML_WCHAR_MODE
                    std::wstring wprint_indent = L"\t";
                    if (fdp.ConsumeBool()) wprint_indent = L"  ";
                    #else
                    std::string wprint_indent = "\t";
                    if (fdp.ConsumeBool()) wprint_indent = "  ";
                    #endif
                    
                    unsigned int wprint_flags = 0;
                    if (fdp.ConsumeBool()) wprint_flags |= pugi::format_indent;
                    if (fdp.ConsumeBool()) wprint_flags |= pugi::format_write_bom;
                    
                    unsigned int wprint_depth = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
                    
                    node_to_print.print(print_woss, wprint_indent.c_str(), wprint_flags, wprint_depth);
                }
            }
            #endif
            
            // Test printing different nodes in the document
            for (pugi::xml_node child = node_to_print.first_child(); child && fdp.remaining_bytes() > 10; child = child.next_sibling()) {
                if (fdp.ConsumeBool()) {
                    TestXmlWriter child_writer;
                    child.print(child_writer, "\t", pugi::format_default, pugi::encoding_auto, 0);
                }
            }
        }
    }
    
    // Step 8: Clean up
    doc.reset();
    
    return 0;
}

} // namespace harness_007
