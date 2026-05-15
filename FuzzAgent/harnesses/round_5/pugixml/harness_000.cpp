#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "pugixml.hpp"

namespace harness_000 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for at least some basic operations
    if (size < 4) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first
    uint8_t parse_mode = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 different modes to test
    bool test_xpath = fdp.ConsumeBool();
    bool test_inplace = fdp.ConsumeBool();
    
    // Consume node name for potential XPath queries (max 64 chars)
    std::string node_name = fdp.ConsumeRandomLengthString(64);
    
    // Use remaining bytes as XML content
    std::vector<uint8_t> xml_content = fdp.ConsumeRemainingBytes<uint8_t>();
    if (xml_content.empty()) return 0;
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    
    // Test parsing with different modes
    switch (parse_mode) {
        case 0:
            result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_default);
            break;
        case 1:
            result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_minimal);
            break;
        case 2:
            result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_full);
            break;
        case 3:
            result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_cdata | pugi::parse_escapes);
            break;
        case 4:
            result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_pi | pugi::parse_comments);
            break;
        case 5:
            result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_declaration | pugi::parse_doctype);
            break;
        case 6:
            result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_ws_pcdata | pugi::parse_eol);
            break;
        case 7:
            // Test fragment parsing
            result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_fragment);
            break;
    }
    
    // If parsing succeeded, try document operations
    if (result) {
        // Get document element
        pugi::xml_node root = doc.document_element();
        
        // Basic document traversal
        if (root) {
            // Traverse child nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                // Access node properties
                const char* name = child.name();
                const char* value = child.value();
                
                // Traverse attributes
                for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
                    const char* attr_name = attr.name();
                    const char* attr_value = attr.value();
                }
            }
            
            // Test XPath if requested
            if (test_xpath && !node_name.empty()) {
                try {
                    pugi::xpath_node_set nodes = root.select_nodes(node_name.c_str());
                    
                    // Process found nodes
                    for (pugi::xpath_node_set::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
                        pugi::xml_node node = it->node();
                        if (node) {
                            // Access node properties
                            const char* node_name_str = node.name();
                            const char* node_value_str = node.value();
                        }
                    }
                } catch (...) {
                    // Ignore XPath exceptions
                }
            }
        }
        
        // Test additional parsing APIs
        if (test_inplace && xml_content.size() > 0) {
            // Try in-place parsing with a copy of the buffer
            std::vector<uint8_t> mutable_buffer = xml_content;
            pugi::xml_document doc2;
            doc2.load_buffer_inplace(mutable_buffer.data(), mutable_buffer.size(), pugi::parse_default);
        }
        
        // Try load_string API
        if (xml_content.size() > 0) {
            std::string xml_string(reinterpret_cast<const char*>(xml_content.data()),
                                  std::min(xml_content.size(), (size_t)1024));
            pugi::xml_document doc3;
            doc3.load_string(xml_string.c_str(), pugi::parse_default);
        }
    }
    
    // Reset document
    doc.reset();
    
    return 0;
}

} // namespace harness_000
