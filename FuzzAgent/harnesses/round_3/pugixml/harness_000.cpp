#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>

namespace harness_000 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parsing options
    unsigned int parse_options = fdp.ConsumeIntegral<unsigned int>();
    
    // Consume XML data for parsing
    std::string xml_data = fdp.ConsumeRandomLengthString(4096);
    
    // Test different parsing methods
    pugi::xml_document doc1;
    pugi::xml_parse_result result1 = doc1.load_string(xml_data.c_str(), parse_options);
    
    // Test load_buffer with the same data
    pugi::xml_document doc2;
    pugi::xml_parse_result result2 = doc2.load_buffer(xml_data.c_str(), xml_data.size(), parse_options);
    
    // Only proceed if parsing was successful for at least one document
    if (!result1 && !result2) {
        return 0;
    }
    
    // Use the successfully parsed document (prefer doc1 if both succeeded)
    pugi::xml_document& doc = result1 ? doc1 : doc2;
    
    // Consume operation type to determine what to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (operation) {
        case 0: {
            // Test node traversal
            for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling()) {
                // Get node name and value
                const char* name = child.name();
                const char* value = child.value();
                
                // Traverse attributes
                for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
                    const char* attr_name = attr.name();
                    const char* attr_value = attr.value();
                }
                
                // Traverse children recursively
                for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                    // Just access to ensure code coverage
                    grandchild.name();
                    grandchild.value();
                }
            }
            break;
        }
        
        case 1: {
            // Test document manipulation - create new nodes
            pugi::xml_node root = doc.document_element();
            if (root) {
                // Consume node name and value for new node
                std::string node_name = fdp.ConsumeRandomLengthString(64);
                std::string node_value = fdp.ConsumeRandomLengthString(256);
                
                // Create a new child node
                pugi::xml_node new_node = root.append_child(node_name.c_str());
                new_node.append_child(pugi::node_pcdata).set_value(node_value.c_str());
                
                // Add some attributes
                std::string attr_name = fdp.ConsumeRandomLengthString(32);
                std::string attr_value = fdp.ConsumeRandomLengthString(128);
                new_node.append_attribute(attr_name.c_str()) = attr_value.c_str();
            }
            break;
        }
        
        case 2: {
            // Test finding nodes by name
            std::string search_name = fdp.ConsumeRandomLengthString(64);
            pugi::xml_node found = doc.child(search_name.c_str());
            
            // If found, traverse its attributes
            if (found) {
                for (pugi::xml_attribute attr = found.first_attribute(); attr; attr = attr.next_attribute()) {
                    attr.name();
                    attr.value();
                }
            }
            break;
        }
        
        case 3: {
            // Test text nodes
            pugi::xml_node root = doc.document_element();
            if (root) {
                pugi::xml_text text = root.text();
                std::string text_value = text.get();
                
                // Set new text if we have enough input
                if (fdp.remaining_bytes() > 10) {
                    std::string new_text = fdp.ConsumeRandomLengthString(128);
                    text.set(new_text.c_str());
                }
            }
            break;
        }
        
        case 4: {
            // Test removing nodes and attributes
            pugi::xml_node root = doc.document_element();
            if (root) {
                // Remove first attribute if exists
                pugi::xml_attribute first_attr = root.first_attribute();
                if (first_attr) {
                    root.remove_attribute(first_attr);
                }
                
                // Remove first child if exists
                pugi::xml_node first_child = root.first_child();
                if (first_child) {
                    root.remove_child(first_child);
                }
            }
            break;
        }
        
        case 5: {
            // Test XPath queries if enabled
            #ifndef PUGIXML_NO_XPATH
            try {
                std::string xpath_query = fdp.ConsumeRandomLengthString(256);
                pugi::xpath_query query(xpath_query.c_str());
                
                // Evaluate on document
                pugi::xpath_node_set nodes = query.evaluate_node_set(doc);
                
                // Iterate through results
                for (pugi::xpath_node_set::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
                    pugi::xpath_node node = *it;
                    if (node.node()) {
                        node.node().name();
                        node.node().value();
                    }
                    if (node.attribute()) {
                        node.attribute().name();
                        node.attribute().value();
                    }
                }
            } catch (pugi::xpath_exception&) {
                // XPath parsing errors are expected with random input
            }
            #endif
            break;
        }
        
        case 6: {
            // Test saving document to string
            #ifndef PUGIXML_NO_STL
            std::ostringstream oss;
            doc.save(oss);
            std::string saved_xml = oss.str();
            #endif
            break;
        }
        
        case 7: {
            // Test memory operations - load_buffer_inplace
            if (fdp.remaining_bytes() > 100) {
                std::vector<char> buffer = fdp.ConsumeBytes<char>(fdp.ConsumeIntegralInRange<size_t>(50, 500));
                if (!buffer.empty()) {
                    pugi::xml_document inplace_doc;
                    pugi::xml_parse_result inplace_result = 
                        inplace_doc.load_buffer_inplace(buffer.data(), buffer.size(), parse_options);
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_000
