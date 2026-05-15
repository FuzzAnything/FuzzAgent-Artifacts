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
#include <memory>

namespace harness_009 {


// Custom xml_writer implementations for comprehensive testing
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

class NullXmlWriter : public pugi::xml_writer {
public:
    virtual void write(const void* data, size_t size) override {
        // Discard everything - test that API works without side effects
        (void)data;
        (void)size;
    }
};

class ErrorSimulatingXmlWriter : public pugi::xml_writer {
private:
    size_t max_writes;
    size_t write_count;
public:
    ErrorSimulatingXmlWriter(size_t max_writes = 1000) : max_writes(max_writes), write_count(0) {}
    
    virtual void write(const void* data, size_t size) override {
        write_count++;
        if (write_count > max_writes) {
            // Simulate error by not writing
            return;
        }
        // Actually discard data
        (void)data;
        (void)size;
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create diverse XML documents for testing save operations
    // We'll create multiple document variations to test different structures
    
    std::vector<std::shared_ptr<pugi::xml_document>> documents;
    
    // Create at least 3 different document types
    for (int doc_type = 0; doc_type < 3 && fdp.remaining_bytes() > 100; doc_type++) {
        auto doc = std::make_shared<pugi::xml_document>();
        
        // Consume parse options
        unsigned int parse_options = fdp.ConsumeIntegral<unsigned int>();
        
        // Try to parse from input
        size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes() / 2);
        std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
        
        pugi::xml_parse_result parse_result = doc->load_buffer(
            xml_buffer.data(), 
            xml_buffer.size(), 
            parse_options, 
            pugi::encoding_auto
        );
        
        // If parsing failed or we want to create synthetic documents
        if (!parse_result || fdp.ConsumeBool()) {
            doc->reset();
            pugi::xml_node root = doc->append_child(pugi::node_element);
            
            // Consume root name
            std::string root_name = fdp.ConsumeRandomLengthString(20);
            if (root_name.empty()) root_name = "root";
            root.set_name(root_name.c_str());
            
            // Add different node types based on document type
            switch (doc_type) {
                case 0: { // Simple document
                    for (int i = 0; i < 3 && fdp.remaining_bytes() > 20; i++) {
                        pugi::xml_node child = root.append_child(pugi::node_element);
                        std::string child_name = fdp.ConsumeRandomLengthString(15);
                        child.set_name(child_name.c_str());
                        
                        if (fdp.ConsumeBool()) {
                            std::string text = fdp.ConsumeRandomLengthString(50);
                            child.append_child(pugi::node_pcdata).set_value(text.c_str());
                        }
                    }
                    break;
                }
                case 1: { // Document with attributes
                    for (int i = 0; i < 5 && fdp.remaining_bytes() > 30; i++) {
                        pugi::xml_node child = root.append_child(pugi::node_element);
                        std::string child_name = fdp.ConsumeRandomLengthString(15);
                        child.set_name(child_name.c_str());
                        
                        // Add attributes
                        int attr_count = fdp.ConsumeIntegralInRange<int>(0, 5);
                        for (int j = 0; j < attr_count && fdp.remaining_bytes() > 10; j++) {
                            std::string attr_name = fdp.ConsumeRandomLengthString(10);
                            std::string attr_value = fdp.ConsumeRandomLengthString(30);
                            child.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
                        }
                    }
                    break;
                }
                case 2: { // Document with mixed content and special nodes
                    // Add comment
                    if (fdp.ConsumeBool()) {
                        std::string comment = fdp.ConsumeRandomLengthString(100);
                        root.append_child(pugi::node_comment).set_value(comment.c_str());
                    }
                    
                    // Add processing instruction
                    if (fdp.ConsumeBool()) {
                        pugi::xml_node pi = root.append_child(pugi::node_pi);
                        std::string pi_target = fdp.ConsumeRandomLengthString(10);
                        std::string pi_data = fdp.ConsumeRandomLengthString(50);
                        pi.set_name(pi_target.c_str());
                        pi.set_value(pi_data.c_str());
                    }
                    
                    // Add CDATA section
                    if (fdp.ConsumeBool()) {
                        pugi::xml_node cdata = root.append_child(pugi::node_cdata);
                        std::string cdata_content = fdp.ConsumeRandomLengthString(100);
                        cdata.set_value(cdata_content.c_str());
                    }
                    
                    // Add element with namespace-like name
                    pugi::xml_node ns_elem = root.append_child(pugi::node_element);
                    std::string ns_name = fdp.ConsumeRandomLengthString(25);
                    ns_elem.set_name(ns_name.c_str());
                    break;
                }
            }
        }
        
        documents.push_back(doc);
    }
    
    if (documents.empty()) {
        return 0; // No documents to test
    }
    
    // Step 2: Test all encoding options exhaustively
    const pugi::xml_encoding all_encodings[] = {
        pugi::encoding_auto,
        pugi::encoding_utf8,
        pugi::encoding_utf16_le,
        pugi::encoding_utf16_be,
        pugi::encoding_utf16,
        pugi::encoding_utf32_le,
        pugi::encoding_utf32_be,
        pugi::encoding_utf32,
        pugi::encoding_wchar,
        pugi::encoding_latin1
    };
    
    // Step 3: Test save_file operations comprehensively
    if (fdp.remaining_bytes() > 100) {
        // Create temporary file for testing
        char temp_file_path[] = "/tmp/pugixml_save_fuzz_XXXXXX";
        int temp_fd = mkstemp(temp_file_path);
        if (temp_fd != -1) {
            close(temp_fd);
            
            // Test each document with various save_file parameters
            for (size_t i = 0; i < documents.size() && fdp.remaining_bytes() > 50; i++) {
                // Consume indent string
                std::string indent_str = fdp.ConsumeRandomLengthString(8);
                if (indent_str.empty()) indent_str = "\t";
                
                // Consume flags - test all possible flag combinations
                unsigned int flags = 0;
                if (fdp.ConsumeBool()) flags |= pugi::format_indent;
                if (fdp.ConsumeBool()) flags |= pugi::format_write_bom;
                if (fdp.ConsumeBool()) flags |= pugi::format_raw;
                if (fdp.ConsumeBool()) flags |= pugi::format_no_declaration;
                if (fdp.ConsumeBool()) flags |= pugi::format_no_escapes;
                if (fdp.ConsumeBool()) flags |= pugi::format_save_file_text;
                
                // Test multiple encodings
                size_t encoding_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(all_encodings)/sizeof(all_encodings[0]) - 1);
                pugi::xml_encoding encoding = all_encodings[encoding_idx];
                
                // Target API 1: xml_document::save_file with char* path
                bool save_result = documents[i]->save_file(
                    temp_file_path,
                    indent_str.c_str(),
                    flags,
                    encoding
                );
                (void)save_result; // Result may be false, that's OK
                
                // Test with empty indent
                if (fdp.ConsumeBool()) {
                    documents[i]->save_file(
                        temp_file_path,
                        "",
                        flags & ~pugi::format_indent, // No indent when indent string is empty
                        encoding
                    );
                }
                
                // Test with minimal flags
                if (fdp.ConsumeBool()) {
                    documents[i]->save_file(
                        temp_file_path,
                        "  ",
                        pugi::format_default,
                        pugi::encoding_auto
                    );
                }
            }
            
            // Clean up
            std::remove(temp_file_path);
        }
    }
    
    // Step 4: Test save to custom xml_writer implementations
    if (fdp.remaining_bytes() > 50) {
        for (size_t i = 0; i < documents.size() && fdp.remaining_bytes() > 30; i++) {
            // Test with CountingXmlWriter
            CountingXmlWriter counting_writer;
            std::string writer_indent = fdp.ConsumeRandomLengthString(4);
            
            unsigned int writer_flags = 0;
            if (fdp.ConsumeBool()) writer_flags |= pugi::format_indent;
            if (fdp.ConsumeBool()) writer_flags |= pugi::format_write_bom;
            
            size_t encoding_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(all_encodings)/sizeof(all_encodings[0]) - 1);
            pugi::xml_encoding writer_encoding = all_encodings[encoding_idx];
            
            // Target API 2: xml_document::save to xml_writer
            documents[i]->save(
                counting_writer,
                writer_indent.c_str(),
                writer_flags,
                writer_encoding
            );
            
            // Test with StringXmlWriter
            StringXmlWriter string_writer;
            documents[i]->save(
                string_writer,
                "\t",
                pugi::format_default,
                pugi::encoding_auto
            );
            
            // Test with NullXmlWriter (edge case)
            NullXmlWriter null_writer;
            documents[i]->save(
                null_writer,
                "",
                pugi::format_raw,
                pugi::encoding_utf8
            );
            
            // Test with ErrorSimulatingXmlWriter
            ErrorSimulatingXmlWriter error_writer(5);
            documents[i]->save(
                error_writer,
                "  ",
                pugi::format_indent,
                pugi::encoding_auto
            );
        }
    }
    
    // Step 5: Test save to std::ostream (if STL is enabled)
#ifndef PUGIXML_NO_STL
    if (fdp.remaining_bytes() > 50) {
        for (size_t i = 0; i < documents.size() && fdp.remaining_bytes() > 20; i++) {
            // Test with std::ostringstream
            std::ostringstream oss;
            
            std::string stream_indent = fdp.ConsumeRandomLengthString(4);
            unsigned int stream_flags = 0;
            if (fdp.ConsumeBool()) stream_flags |= pugi::format_indent;
            if (fdp.ConsumeBool()) stream_flags |= pugi::format_write_bom;
            
            size_t encoding_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(all_encodings)/sizeof(all_encodings[0]) - 1);
            pugi::xml_encoding stream_encoding = all_encodings[encoding_idx];
            
            // Target API 3: xml_document::save to std::ostream
            documents[i]->save(
                oss,
                stream_indent.c_str(),
                stream_flags,
                stream_encoding
            );
            
            // Test with std::wostringstream if wchar_t support is available
            if (fdp.remaining_bytes() > 10) {
                std::wostringstream woss;
                
                // Use appropriate string type
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
                
                // Target API 4: xml_document::save to std::wostream
                documents[i]->save(
                    woss,
                    windent.c_str(),
                    wstream_flags
                );
            }
        }
    }
#endif
    
    // Step 6: Test xml_node::print operations for comparison with save
    if (fdp.remaining_bytes() > 50) {
        for (size_t i = 0; i < documents.size() && fdp.remaining_bytes() > 20; i++) {
            pugi::xml_node root = documents[i]->document_element();
            if (root.empty()) {
                root = documents[i]->first_child();
            }
            
            if (!root.empty()) {
                // Test xml_node::print with various parameters
                CountingXmlWriter node_writer;
                std::string node_indent = fdp.ConsumeRandomLengthString(4);
                unsigned int node_flags = 0;
                if (fdp.ConsumeBool()) node_flags |= pugi::format_indent;
                if (fdp.ConsumeBool()) node_flags |= pugi::format_write_bom;
                
                size_t encoding_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(all_encodings)/sizeof(all_encodings[0]) - 1);
                pugi::xml_encoding node_encoding = all_encodings[encoding_idx];
                
                unsigned int depth = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
                
                // Target API 5: xml_node::print to xml_writer
                root.print(
                    node_writer,
                    node_indent.c_str(),
                    node_flags,
                    node_encoding,
                    depth
                );
                
                // Test print on child nodes
                for (pugi::xml_node child = root.first_child(); child && fdp.remaining_bytes() > 10; child = child.next_sibling()) {
                    if (fdp.ConsumeBool()) {
                        CountingXmlWriter child_writer;
                        child.print(
                            child_writer,
                            "\t",
                            pugi::format_default,
                            pugi::encoding_auto,
                            0
                        );
                    }
                }
            }
        }
    }
    
    // Step 7: Test edge cases and error conditions
    if (fdp.remaining_bytes() > 30) {
        // Test with empty document
        pugi::xml_document empty_doc;
        
        // Should handle empty document gracefully
        CountingXmlWriter empty_writer;
        empty_doc.save(empty_writer, "\t", pugi::format_default, pugi::encoding_auto);
        
        // Test document with only declaration
        pugi::xml_document decl_doc;
        decl_doc.reset();
        decl_doc.append_child(pugi::node_declaration);
        
        CountingXmlWriter decl_writer;
        decl_doc.save(decl_writer, "", pugi::format_no_declaration, pugi::encoding_utf8);
        
        // Test with maximum indent (long string)
        if (!documents.empty()) {
            std::string long_indent(100, ' ');
            CountingXmlWriter long_writer;
            documents[0]->save(
                long_writer,
                long_indent.c_str(),
                pugi::format_indent,
                pugi::encoding_auto
            );
        }
    }
    
    // Step 8: Test save operations with document modifications between saves
    if (!documents.empty() && fdp.remaining_bytes() > 100) {
        pugi::xml_document& doc = *documents[0];
        
        // Modify document
        pugi::xml_node root = doc.document_element();
        if (root.empty()) {
            root = doc.first_child();
        }
        
        if (!root.empty()) {
            // Add new nodes between saves
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 20; i++) {
                std::string new_node_name = fdp.ConsumeRandomLengthString(15);
                pugi::xml_node new_node = root.append_child(new_node_name.c_str());
                
                // Save after each modification
                CountingXmlWriter incremental_writer;
                doc.save(incremental_writer, "\t", pugi::format_default, pugi::encoding_auto);
                
                // Add content to new node
                if (fdp.ConsumeBool()) {
                    std::string new_text = fdp.ConsumeRandomLengthString(50);
                    new_node.append_child(pugi::node_pcdata).set_value(new_text.c_str());
                    
                    // Save again
                    CountingXmlWriter text_writer;
                    doc.save(text_writer, "  ", pugi::format_indent, pugi::encoding_utf8);
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_009
