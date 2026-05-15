#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

namespace harness_012 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful node manipulation testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create or parse XML document for manipulation testing
    // Split input: first part for XML content, rest for manipulation parameters
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(32, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    pugi::xml_document doc1;
    pugi::xml_parse_result parse_result = doc1.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, create a structured document with many elements/attributes for manipulation testing
    if (!parse_result) {
        doc1.reset();
        
        // Create root with fuzzed name
        std::string root_name = fdp.remaining_bytes() > 5 ? fdp.ConsumeRandomLengthString(10) : "root";
        pugi::xml_node root = doc1.append_child(pugi::node_element);
        root.set_name(root_name.c_str());
        
        // Create initial children with attributes for copying/moving testing
        int initial_children = fdp.ConsumeIntegralInRange<int>(3, 8);
        for (int i = 0; i < initial_children && fdp.remaining_bytes() > 20; ++i) {
            std::string child_name = "child_" + std::to_string(i);
            if (fdp.remaining_bytes() > 5) {
                child_name = fdp.ConsumeRandomLengthString(15);
            }
            
            pugi::xml_node child = root.append_child(child_name.c_str());
            
            // Add attributes to child
            int attr_count = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int j = 0; j < attr_count && fdp.remaining_bytes() > 10; ++j) {
                std::string attr_name = "attr_" + std::to_string(j);
                std::string attr_value = "value_" + std::to_string(j);
                
                if (fdp.remaining_bytes() > 10) {
                    attr_name = fdp.ConsumeRandomLengthString(8);
                    attr_value = fdp.ConsumeRandomLengthString(20);
                }
                
                child.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
            }
            
            // Add text content
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                std::string text = fdp.ConsumeRandomLengthString(50);
                child.append_child(pugi::node_pcdata).set_value(text.c_str());
            }
        }
    }
    
    // Get root node
    pugi::xml_node root1 = doc1.document_element();
    if (root1.empty()) {
        root1 = doc1.first_child();
        if (root1.empty()) {
            return 0; // No document structure to manipulate
        }
    }
    
    // Create a second document for cross-document operations
    pugi::xml_document doc2;
    pugi::xml_node root2 = doc2.append_child("doc2_root");
    
    // Step 2: Test Node Copying Operations
    
    // 2.1 Collect nodes that can be copied
    std::vector<pugi::xml_node> nodes_to_copy;
    for (pugi::xml_node child = root1.first_child(); child; child = child.next_sibling()) {
        nodes_to_copy.push_back(child);
    }
    
    if (nodes_to_copy.empty()) {
        // Create some nodes to copy if none exist
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; ++i) {
            std::string node_name = "node_to_copy_" + std::to_string(i);
            if (fdp.remaining_bytes() > 5) {
                node_name = fdp.ConsumeRandomLengthString(10);
            }
            nodes_to_copy.push_back(root1.append_child(node_name.c_str()));
        }
    }
    
    // 2.2 Test append_copy (copy node as child)
    if (!nodes_to_copy.empty() && fdp.remaining_bytes() > 10) {
        size_t src_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_copy.size() - 1);
        pugi::xml_node copied_node = root1.append_copy(nodes_to_copy[src_idx]);
        
        // Also test copying to different document
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            pugi::xml_node cross_doc_copy = root2.append_copy(nodes_to_copy[src_idx]);
        }
    }
    
    // 2.3 Test prepend_copy
    if (!nodes_to_copy.empty() && fdp.remaining_bytes() > 10) {
        size_t src_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_copy.size() - 1);
        pugi::xml_node prepend_copied = root1.prepend_copy(nodes_to_copy[src_idx]);
    }
    
    // 2.4 Test insert_copy_before and insert_copy_after
    if (nodes_to_copy.size() >= 2 && fdp.remaining_bytes() > 20) {
        size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_copy.size() - 1);
        size_t src_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_copy.size() - 1);
        
        // Insert copy before target node
        pugi::xml_node inserted_before = root1.insert_copy_before(
            nodes_to_copy[src_idx], 
            nodes_to_copy[target_idx]
        );
        
        // Insert copy after target node (use different indices)
        size_t target_idx2 = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_copy.size() - 1);
        size_t src_idx2 = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_copy.size() - 1);
        
        pugi::xml_node inserted_after = root1.insert_copy_after(
            nodes_to_copy[src_idx2], 
            nodes_to_copy[target_idx2]
        );
    }
    
    // Step 3: Test Node Moving Operations
    
    // 3.1 Collect nodes that can be moved
    std::vector<pugi::xml_node> nodes_to_move;
    for (pugi::xml_node child = root1.first_child(); child; child = child.next_sibling()) {
        nodes_to_move.push_back(child);
    }
    
    // 3.2 Test append_move (move node to become child)
    if (nodes_to_move.size() >= 2 && fdp.remaining_bytes() > 20) {
        size_t src_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
        size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
        
        if (src_idx != target_idx) {
            // Move node to become last child of target node
            pugi::xml_node target_node = nodes_to_move[target_idx];
            pugi::xml_node moved_node = target_node.append_move(nodes_to_move[src_idx]);
        }
    }
    
    // 3.3 Test prepend_move
    if (nodes_to_move.size() >= 2 && fdp.remaining_bytes() > 20) {
        size_t src_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
        size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
        
        if (src_idx != target_idx) {
            pugi::xml_node target_node = nodes_to_move[target_idx];
            pugi::xml_node moved_node = target_node.prepend_move(nodes_to_move[src_idx]);
        }
    }
    
    // 3.4 Test insert_move_before and insert_move_after
    if (nodes_to_move.size() >= 3 && fdp.remaining_bytes() > 30) {
        // Find three distinct nodes: source, target, and reference
        size_t src_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
        size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
        size_t ref_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
        
        // Ensure distinct indices
        if (src_idx != target_idx && src_idx != ref_idx && target_idx != ref_idx) {
            pugi::xml_node target_parent = nodes_to_move[target_idx];
            pugi::xml_node reference_node = nodes_to_move[ref_idx];
            
            // Move source node before reference node under target parent
            pugi::xml_node moved_before = target_parent.insert_move_before(
                nodes_to_move[src_idx],
                reference_node
            );
            
            // Find another set for insert_move_after
            size_t src_idx2 = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
            size_t target_idx2 = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
            size_t ref_idx2 = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
            
            if (src_idx2 != target_idx2 && src_idx2 != ref_idx2 && target_idx2 != ref_idx2) {
                pugi::xml_node target_parent2 = nodes_to_move[target_idx2];
                pugi::xml_node reference_node2 = nodes_to_move[ref_idx2];
                
                pugi::xml_node moved_after = target_parent2.insert_move_after(
                    nodes_to_move[src_idx2],
                    reference_node2
                );
            }
        }
    }
    
    // 3.5 Test cross-document moving
    if (!nodes_to_move.empty() && fdp.remaining_bytes() > 10) {
        size_t src_idx = fdp.ConsumeIntegralInRange<size_t>(0, nodes_to_move.size() - 1);
        
        // Move node from doc1 to doc2
        pugi::xml_node cross_doc_moved = root2.append_move(nodes_to_move[src_idx]);
    }
    
    // Step 4: Test Attribute Manipulation Operations
    
    // 4.1 Collect elements with attributes
    std::vector<pugi::xml_node> elements_with_attrs;
    for (pugi::xml_node node = root1.first_child(); node; node = node.next_sibling()) {
        if (!node.first_attribute().empty()) {
            elements_with_attrs.push_back(node);
        }
    }
    
    // 4.2 Test attribute copying operations
    if (!elements_with_attrs.empty() && fdp.remaining_bytes() > 20) {
        size_t elem_idx = fdp.ConsumeIntegralInRange<size_t>(0, elements_with_attrs.size() - 1);
        pugi::xml_node element = elements_with_attrs[elem_idx];
        
        // Get first attribute to copy
        pugi::xml_attribute first_attr = element.first_attribute();
        if (first_attr) {
            // Test append_copy for attributes
            pugi::xml_attribute appended_copy = element.append_copy(first_attr);
            
            // Test prepend_copy for attributes
            pugi::xml_attribute prepended_copy = element.prepend_copy(first_attr);
            
            // Test insert_copy_before and insert_copy_after for attributes
            pugi::xml_attribute second_attr = first_attr.next_attribute();
            if (second_attr) {
                pugi::xml_attribute inserted_before = element.insert_copy_before(first_attr, second_attr);
                pugi::xml_attribute inserted_after = element.insert_copy_after(first_attr, second_attr);
            }
        }
    }
    
    // 4.3 Test set_name and set_value operations
    if (!elements_with_attrs.empty() && fdp.remaining_bytes() > 30) {
        size_t elem_idx = fdp.ConsumeIntegralInRange<size_t>(0, elements_with_attrs.size() - 1);
        pugi::xml_node element = elements_with_attrs[elem_idx];
        
        // Test set_name on element
        std::string new_name = fdp.ConsumeRandomLengthString(20);
        element.set_name(new_name.c_str());
        
        // Test set_value on element's text child
        pugi::xml_node text_child = element.first_child();
        if (text_child && (text_child.type() == pugi::node_pcdata || text_child.type() == pugi::node_cdata)) {
            std::string new_value = fdp.ConsumeRandomLengthString(100);
            text_child.set_value(new_value.c_str());
        }
        
        // Test set_name and set_value on attributes
        pugi::xml_attribute attr = element.first_attribute();
        if (attr) {
            std::string new_attr_name = fdp.ConsumeRandomLengthString(15);
            std::string new_attr_value = fdp.ConsumeRandomLengthString(50);
            
            // Note: pugixml doesn't have set_name on attributes directly, but we can remove and re-add
            // For this test, we'll create a new attribute with the new name/value
            pugi::xml_attribute new_attr = element.append_attribute(new_attr_name.c_str());
            new_attr.set_value(new_attr_value.c_str());
        }
    }
    
    // 4.4 Test attribute insertion operations (insert_attribute_before, insert_attribute_after)
    if (!elements_with_attrs.empty() && fdp.remaining_bytes() > 20) {
        size_t elem_idx = fdp.ConsumeIntegralInRange<size_t>(0, elements_with_attrs.size() - 1);
        pugi::xml_node element = elements_with_attrs[elem_idx];
        
        // Get reference attribute
        pugi::xml_attribute ref_attr = element.first_attribute();
        if (ref_attr) {
            // Test insert_attribute_before
            std::string before_name = fdp.ConsumeRandomLengthString(10);
            pugi::xml_attribute inserted_before = element.insert_attribute_before(before_name.c_str(), ref_attr);
            
            if (inserted_before) {
                std::string before_value = fdp.ConsumeRandomLengthString(30);
                inserted_before.set_value(before_value.c_str());
            }
            
            // Test insert_attribute_after
            std::string after_name = fdp.ConsumeRandomLengthString(10);
            pugi::xml_attribute inserted_after = element.insert_attribute_after(after_name.c_str(), ref_attr);
            
            if (inserted_after) {
                std::string after_value = fdp.ConsumeRandomLengthString(30);
                inserted_after.set_value(after_value.c_str());
            }
        }
    }
    
    // Step 5: Test comprehensive document structure after manipulations
    
    // Traverse final document structure to ensure no crashes
    if (fdp.remaining_bytes() > 10) {
        // Traverse doc1
        for (pugi::xml_node node = doc1.first_child(); node; node = node.next_sibling()) {
            pugi::xml_node_type type = node.type();
            const char* name = node.name();
            const char* value = node.value();
            
            // Traverse attributes
            for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
                const char* attr_name = attr.name();
                const char* attr_value = attr.value();
            }
            
            // Traverse children
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                const char* child_name = child.name();
                const char* child_value = child.value();
            }
        }
        
        // Traverse doc2
        for (pugi::xml_node node = doc2.first_child(); node; node = node.next_sibling()) {
            pugi::xml_node_type type = node.type();
            const char* name = node.name();
            const char* value = node.value();
        }
    }
    
    return 0;
}

} // namespace harness_012
