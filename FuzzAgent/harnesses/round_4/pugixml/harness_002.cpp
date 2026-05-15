#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

namespace harness_002 {

// Custom xml_tree_walker implementation for fuzzing
class FuzzingTreeWalker : public pugi::xml_tree_walker {
private:
    FuzzedDataProvider* fdp;
    int max_depth;
    int nodes_processed;
    
public:
    FuzzingTreeWalker(FuzzedDataProvider* provider, int max_depth_limit = 10) 
        : fdp(provider), max_depth(max_depth_limit), nodes_processed(0) {}
    
    virtual bool begin(pugi::xml_node& node) override {
        // Randomly decide whether to continue traversal
        if (fdp->ConsumeBool()) {
            nodes_processed = 0;
            return true;
        }
        return false;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Limit traversal depth to prevent infinite recursion
        if (depth() > max_depth) {
            return false;
        }
        
        // Random operations on each node based on fuzzer input
        uint8_t operation = fdp->ConsumeIntegral<uint8_t>() % 8;
        
        switch (operation) {
            case 0:
                // Access node properties
                node.name();
                node.value();
                node.type();
                break;
            case 1:
                // Modify node value
                if (fdp->ConsumeBool()) {
                    std::string new_value = fdp->ConsumeRandomLengthString(256);
                    node.text().set(new_value.c_str());
                }
                break;
            case 2:
                // Add attribute
                if (fdp->ConsumeBool()) {
                    std::string attr_name = fdp->ConsumeRandomLengthString(64);
                    std::string attr_value = fdp->ConsumeRandomLengthString(128);
                    node.append_attribute(attr_name.c_str()) = attr_value.c_str();
                }
                break;
            case 3:
                // Remove attribute
                if (fdp->ConsumeBool() && node.first_attribute()) {
                    node.remove_attribute(node.first_attribute());
                }
                break;
            case 4:
                // Add child node
                if (fdp->ConsumeBool()) {
                    std::string child_name = fdp->ConsumeRandomLengthString(64);
                    node.append_child(child_name.c_str());
                }
                break;
            case 5:
                // Remove child node
                if (fdp->ConsumeBool() && node.first_child()) {
                    node.remove_child(node.first_child());
                }
                break;
            case 6:
                // Access parent and siblings
                node.parent();
                node.next_sibling();
                node.previous_sibling();
                break;
            case 7:
                // Access children
                node.first_child();
                node.last_child();
                node.child(fdp->ConsumeRandomLengthString(32).c_str());
                break;
        }
        
        nodes_processed++;
        
        // Randomly decide whether to continue traversal
        return fdp->ConsumeBool();
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Cleanup or final operations
        return true;
    }
    
    int getNodesProcessed() const { return nodes_processed; }
};

// Custom xml_writer implementation for fuzzing
class FuzzingXmlWriter : public pugi::xml_writer {
private:
    std::vector<char> buffer;
    
public:
    virtual void write(const void* data, size_t size) override {
        const char* char_data = static_cast<const char*>(data);
        buffer.insert(buffer.end(), char_data, char_data + size);
    }
    
    const std::vector<char>& getBuffer() const { return buffer; }
    void clear() { buffer.clear(); }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t walker_operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint8_t writer_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint8_t format_flags = fdp.ConsumeIntegral<uint8_t>();
    uint8_t encoding_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume variable-size data
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    std::string indent_str = fdp.ConsumeRandomLengthString(8);
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    // Parse XML based on test_mode
    switch (test_mode) {
        case 0:
            parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
            break;
        case 1:
            parse_result = doc.load_string(xml_content.c_str(), parse_options);
            break;
        case 2:
            // Use pugixml allocation for in-place parsing
            if (xml_content.size() > 0) {
                pugi::allocation_function alloc = pugi::get_memory_allocation_function();
                char* buffer = static_cast<char*>(alloc(xml_content.size()));
                if (buffer) {
                    memcpy(buffer, xml_content.c_str(), xml_content.size());
                    parse_result = doc.load_buffer_inplace_own(buffer, xml_content.size(), parse_options);
                }
            }
            break;
        case 3:
            // Create simple document for testing
            doc.reset();
            pugi::xml_node root = doc.append_child("root");
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
                std::string node_name = fdp.ConsumeRandomLengthString(32);
                pugi::xml_node child = root.append_child(node_name.c_str());
                
                std::string attr_name = fdp.ConsumeRandomLengthString(32);
                std::string attr_value = fdp.ConsumeRandomLengthString(64);
                child.append_attribute(attr_name.c_str()) = attr_value.c_str();
                
                std::string text_value = fdp.ConsumeRandomLengthString(128);
                child.text().set(text_value.c_str());
            }
            parse_result.status = pugi::status_ok;
            break;
    }
    
    // Test xml_tree_walker functionality if parsing succeeded
    if (parse_result) {
        FuzzingTreeWalker walker(&fdp);
        
        switch (walker_operation) {
            case 0:
                // Traverse entire document
                doc.traverse(walker);
                break;
            case 1:
                // Traverse from root element
                if (doc.document_element()) {
                    doc.document_element().traverse(walker);
                }
                break;
            case 2:
                // Traverse with depth limit
                {
                    FuzzingTreeWalker limited_walker(&fdp, 3);
                    doc.traverse(limited_walker);
                }
                break;
        }
    }
    
    // Test document manipulation APIs
    doc.reset();  // Reset document
    
    // Recreate document for writer testing
    if (fdp.remaining_bytes() > 100) {
        std::string new_xml = fdp.ConsumeRandomLengthString(1024);
        pugi::xml_parse_result new_result = doc.load_buffer(new_xml.c_str(), new_xml.size());
        
        if (new_result) {
            // Test xml_writer implementations
            switch (writer_type) {
                case 0: {
                    // Custom writer implementation
                    FuzzingXmlWriter custom_writer;
                    doc.save(custom_writer, indent_str.c_str(), format_flags, 
                            static_cast<pugi::xml_encoding>(encoding_type));
                    // Access the buffer to ensure it was written
                    const auto& buffer = custom_writer.getBuffer();
                    (void)buffer;
                    break;
                }
                case 1: {
                    // String stream writer (requires STL)
                    #ifndef PUGIXML_NO_STL
                    std::stringstream ss;
                    pugi::xml_writer_stream stream_writer(ss);
                    doc.save(stream_writer, indent_str.c_str(), format_flags,
                            static_cast<pugi::xml_encoding>(encoding_type));
                    #endif
                    break;
                }
                case 2: {
                    // Test print method with custom writer
                    FuzzingXmlWriter print_writer;
                    if (doc.document_element()) {
                        doc.document_element().print(print_writer, indent_str.c_str(), 
                                                     format_flags, 
                                                     static_cast<pugi::xml_encoding>(encoding_type));
                    }
                    break;
                }
            }
            
            // Test document manipulation APIs
            // 1. Document comparison
            pugi::xml_document doc2;
            doc2.load_buffer(new_xml.c_str(), new_xml.size());
            bool documents_equal = (doc == doc2);
            (void)documents_equal;
            
            // 2. Document copying
            pugi::xml_document doc_copy;
            doc_copy.reset(doc);
            
            // 3. Document swapping - removed as xml_document doesn't have swap method
            // pugi::xml_document doc_swap;
            // doc.swap(doc_swap);
            // doc.swap(doc_swap); // Swap back
            
            // 4. Test offset_debug function
            #ifdef PUGIXML_DEBUG
            pugi::xml_node debug_node = doc.document_element();
            if (debug_node) {
                debug_node.offset_debug();
            }
            #endif
        }
    }
    
    // Test memory management functions
    if (fdp.ConsumeBool()) {
        // Get memory statistics
        pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
        pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();
        (void)alloc_func;
        (void)dealloc_func;
        
        // Test setting custom memory functions
        pugi::set_memory_management_functions(alloc_func, dealloc_func);
    }
    
    // Test node range iterators
    if (doc.document_element()) {
        auto children = doc.document_element().children();
        for (auto it = children.begin(); it != children.end(); ++it) {
            // Access iterator properties
            (void)*it;
        }
        
        auto attributes = doc.document_element().attributes();
        for (auto it = attributes.begin(); it != attributes.end(); ++it) {
            // Access attribute iterator properties
            (void)*it;
        }
    }
    
    return 0;
}

} // namespace harness_002
