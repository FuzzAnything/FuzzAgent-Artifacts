#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>

namespace harness_002 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) return 0;  // Minimum size check for meaningful operations
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Parse XML from input data
    // Split input: use first part for XML content, rest for manipulation parameters
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(0, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, create a simple document for tree manipulation testing using fuzzed data
    if (!parse_result) {
        doc.reset();
        // Use fuzzed data for root node name
        std::string root_name = fdp.remaining_bytes() > 5 ? fdp.ConsumeRandomLengthString(10) : "doc";
        pugi::xml_node root = doc.append_child(pugi::node_element);
        root.set_name(root_name.c_str());
        
        // Add some initial structure for manipulation using fuzzed data
        if (fdp.remaining_bytes() > 10) {
            std::string child1_name = fdp.ConsumeRandomLengthString(10);
            pugi::xml_node child1 = root.append_child(child1_name.c_str());
            std::string attr1_name = fdp.ConsumeRandomLengthString(8);
            std::string attr1_value = fdp.ConsumeRandomLengthString(20);
            child1.append_attribute(attr1_name.c_str()).set_value(attr1_value.c_str());
        }
        
        if (fdp.remaining_bytes() > 10) {
            std::string child2_name = fdp.ConsumeRandomLengthString(10);
            pugi::xml_node child2 = root.append_child(child2_name.c_str());
            std::string attr2_name = fdp.ConsumeRandomLengthString(8);
            std::string attr2_value = fdp.ConsumeRandomLengthString(20);
            child2.append_attribute(attr2_name.c_str()).set_value(attr2_value.c_str());
        }
        
        if (fdp.remaining_bytes() > 10) {
            std::string child3_name = fdp.ConsumeRandomLengthString(10);
            pugi::xml_node child3 = root.append_child(child3_name.c_str());
            std::string attr3_name = fdp.ConsumeRandomLengthString(8);
            std::string attr3_value = fdp.ConsumeRandomLengthString(20);
            child3.append_attribute(attr3_name.c_str()).set_value(attr3_value.c_str());
        }
    }
    
    // Step 2: Get root node
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        root = doc.first_child();
        if (root.empty()) {
            return 0;  // No document structure to manipulate
        }
    }
    
    // Step 3: Tree manipulation operations
    
    // 3.1 Create base children using append_child and prepend_child
    // Use fuzzed data for node names and types
    int child_count = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    std::vector<pugi::xml_node> created_nodes;
    
    for (int i = 0; i < child_count && fdp.remaining_bytes() > 10; ++i) {
        // Consume node name from fuzzed data
        std::string node_name = fdp.ConsumeRandomLengthString(20);
        
        // Decide whether to append or prepend
        bool prepend = fdp.ConsumeBool();
        pugi::xml_node new_child;
        
        if (prepend) {
            new_child = root.prepend_child(node_name.c_str());
        } else {
            new_child = root.append_child(node_name.c_str());
        }
        
        if (!new_child.empty()) {
            created_nodes.push_back(new_child);
            
            // Add some attributes to the new child
            int attr_count = fdp.ConsumeIntegralInRange<int>(0, 5);
            for (int j = 0; j < attr_count && fdp.remaining_bytes() > 5; ++j) {
                std::string attr_name = fdp.ConsumeRandomLengthString(10);
                std::string attr_value = fdp.ConsumeRandomLengthString(50);
                new_child.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
            }
            
            // Add text content using xml_text::set
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                std::string text_content = fdp.ConsumeRandomLengthString(100);
                new_child.text().set(text_content.c_str());
            }
            
            // Set node value using xml_node::set_value
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                std::string node_value = fdp.ConsumeRandomLengthString(100);
                new_child.set_value(node_value.c_str());
            }
        }
    }
    
    // 3.2 Test insert_child_before and insert_child_after operations
    if (!created_nodes.empty() && fdp.remaining_bytes() > 10) {
        // Pick a reference node to insert before/after
        size_t ref_index = fdp.ConsumeIntegralInRange<size_t>(0, created_nodes.size() - 1);
        pugi::xml_node& ref_node = created_nodes[ref_index];
        
        // Create new nodes to insert
        std::string before_name = fdp.ConsumeRandomLengthString(15);
        std::string after_name = fdp.ConsumeRandomLengthString(15);
        
        pugi::xml_node before_node = root.insert_child_before(before_name.c_str(), ref_node);
        pugi::xml_node after_node = root.insert_child_after(after_name.c_str(), ref_node);
        
        if (!before_node.empty()) {
            created_nodes.push_back(before_node);
            // Add text to inserted nodes
            if (fdp.ConsumeBool()) {
                std::string before_text = fdp.ConsumeRandomLengthString(80);
                before_node.text().set(before_text.c_str());
            }
        }
        
        if (!after_node.empty()) {
            created_nodes.push_back(after_node);
            // Add attributes to inserted nodes
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                std::string attr_name = fdp.ConsumeRandomLengthString(10);
                std::string attr_value = fdp.ConsumeRandomLengthString(30);
                after_node.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
            }
        }
    }
    
    // 3.3 Test remove_attribute and remove_child operations
    if (!created_nodes.empty() && fdp.remaining_bytes() > 5) {
        // Remove some attributes
        for (auto& node : created_nodes) {
            if (!node.empty() && fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
                // Remove first attribute if exists
                pugi::xml_attribute first_attr = node.first_attribute();
                if (first_attr) {
                    node.remove_attribute(first_attr);
                }
                
                // Also test removing attribute by name
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                    std::string attr_to_remove = fdp.ConsumeRandomLengthString(10);
                    node.remove_attribute(attr_to_remove.c_str());
                }
            }
        }
        
        // Remove some children
        int nodes_to_remove = fdp.ConsumeIntegralInRange<int>(0, created_nodes.size());
        for (int i = 0; i < nodes_to_remove && !created_nodes.empty() && fdp.remaining_bytes() > 2; ++i) {
            size_t remove_index = fdp.ConsumeIntegralInRange<size_t>(0, created_nodes.size() - 1);
            pugi::xml_node node_to_remove = created_nodes[remove_index];
            
            if (!node_to_remove.empty()) {
                // Remove by node reference
                root.remove_child(node_to_remove);
                
                // Also test removing child by name
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                    std::string child_name_to_remove = fdp.ConsumeRandomLengthString(15);
                    root.remove_child(child_name_to_remove.c_str());
                }
                
                // Remove from our tracking vector
                created_nodes.erase(created_nodes.begin() + remove_index);
            }
        }
    }
    
    // 3.4 Test xml_text::set with various data types
    if (!created_nodes.empty() && fdp.remaining_bytes() > 20) {
        for (auto& node : created_nodes) {
            if (!node.empty() && fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                // Choose text set method based on fuzzed data
                int text_method = fdp.ConsumeIntegralInRange<int>(0, 5);
                
                switch (text_method) {
                    case 0: {  // Set string
                        std::string text_str = fdp.ConsumeRandomLengthString(50);
                        node.text().set(text_str.c_str());
                        break;
                    }
                    case 1: {  // Set integer
                        int int_val = fdp.ConsumeIntegral<int>();
                        node.text().set(int_val);
                        break;
                    }
                    case 2: {  // Set unsigned integer
                        unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                        node.text().set(uint_val);
                        break;
                    }
                    case 3: {  // Set double
                        double double_val = fdp.ConsumeFloatingPoint<double>();
                        node.text().set(double_val);
                        break;
                    }
                    case 4: {  // Set boolean
                        bool bool_val = fdp.ConsumeBool();
                        node.text().set(bool_val);
                        break;
                    }
                    case 5: {  // Set with precision
                        double double_val = fdp.ConsumeFloatingPoint<double>();
                        int precision = fdp.ConsumeIntegralInRange<int>(0, 10);
                        node.text().set(double_val, precision);
                        break;
                    }
                }
            }
        }
    }
    
    // 3.5 Test xml_node::set_value
    if (!created_nodes.empty() && fdp.remaining_bytes() > 10) {
        for (auto& node : created_nodes) {
            if (!node.empty() && fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                std::string node_value = fdp.ConsumeRandomLengthString(100);
                node.set_value(node_value.c_str());
            }
        }
    }
    
    // Step 4: Clean up (xml_document::reset is called automatically when doc goes out of scope)
    // The destructor will handle cleanup
    
    return 0;
}

} // namespace harness_002
