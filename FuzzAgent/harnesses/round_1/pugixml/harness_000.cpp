#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace harness_000 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create XML document
    pugi::xml_document doc;
    
    // Try loading XML with different parsing options
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, size));
    
    // Parse with different options to exercise different code paths
    pugi::xml_parse_result result_default = doc.load_buffer(xml_buffer.data(), xml_buffer.size());
    pugi::xml_parse_result result_minimal = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_minimal);
    pugi::xml_parse_result result_full = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing succeeded, exercise node traversal and manipulation
    if (result_default || result_minimal || result_full) {
        // Get document element
        pugi::xml_node root = doc.document_element();
        
        if (root) {
            // Exercise child node access
            pugi::xml_node first_child = root.first_child();
            pugi::xml_node last_child = root.last_child();
            
            // Traverse children
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                // Get node name and value
                const char* name = child.name();
                const char* value = child.value();
                
                // Get node type
                pugi::xml_node_type type = child.type();
                
                // Exercise attribute access
                pugi::xml_attribute attr = child.first_attribute();
                while (attr) {
                    const char* attr_name = attr.name();
                    const char* attr_value = attr.value();
                    attr = attr.next_attribute();
                }
            }
            
            // Exercise text access
            pugi::xml_text root_text = root.text();
            std::string text_str = root_text.as_string();
            
            // Find nodes by path
            std::string path = fdp.ConsumeRandomLengthString(100);
            pugi::xml_node found_by_path = root.first_element_by_path(path.c_str());
            
            // Find child by attribute
            std::string attr_name = fdp.ConsumeRandomLengthString(50);
            std::string attr_value = fdp.ConsumeRandomLengthString(50);
            pugi::xml_node found_by_attr = root.find_child_by_attribute(attr_name.c_str(), attr_value.c_str());
            
            // Try creating new nodes if we have enough input
            if (fdp.remaining_bytes() > 10) {
                std::string new_node_name = fdp.ConsumeRandomLengthString(20);
                pugi::xml_node new_node = root.append_child(new_node_name.c_str());
                
                if (new_node) {
                    // Add attributes to new node
                    std::string new_attr_name = fdp.ConsumeRandomLengthString(15);
                    std::string new_attr_value = fdp.ConsumeRandomLengthString(30);
                    new_node.append_attribute(new_attr_name.c_str()) = new_attr_value.c_str();
                    
                    // Add text to new node
                    std::string new_text = fdp.ConsumeRandomLengthString(50);
                    new_node.text() = new_text.c_str();
                }
            }
        }
    }
    
    // Exercise XPath functionality if enabled and we have remaining input
#ifndef PUGIXML_NO_XPATH
    if (fdp.remaining_bytes() > 20) {
        try {
            pugi::xpath_variable_set vars;
            
            // Add some variables
            size_t var_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
            for (size_t i = 0; i < var_count && fdp.remaining_bytes() > 10; ++i) {
                std::string var_name = fdp.ConsumeRandomLengthString(20);
                int xpath_value_type_count = pugi::xpath_type_boolean + 1;
                pugi::xpath_value_type value_type = static_cast<pugi::xpath_value_type>(
                    fdp.ConsumeIntegralInRange(0, xpath_value_type_count - 1));
                vars.add(var_name.c_str(), value_type);
            }
            
            // Create XPath query
            std::string xpath_expr = fdp.ConsumeRandomLengthString(100);
            pugi::xpath_query query(xpath_expr.c_str(), &vars);
            
            // Evaluate query on document
            if (doc) {
                query.evaluate_boolean(doc);
                query.evaluate_number(doc);
                query.evaluate_string(doc);
                query.evaluate_node(doc);
                query.evaluate_node_set(doc);
            }
        }
        catch (pugi::xpath_exception&) {
            // XPath parsing failed, which is expected for invalid expressions
        }
    }
#endif
    
    return 0;
}

} // namespace harness_000
