#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

namespace harness_015 {


#ifndef PUGIXML_NO_STL
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML structure and attribute operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: DOCUMENT SETUP WITH ATTRIBUTES ====================
    // Create XML document for attribute copy/insert testing
    pugi::xml_document doc;
    
    // Create root element
    pugi::xml_node root = doc.append_child("attribute_test_root");
    
    // Create source and target elements for attribute copying
    pugi::xml_node source_elem = root.append_child("source_element");
    pugi::xml_node target_elem = root.append_child("target_element");
    pugi::xml_node container_elem = root.append_child("container_element");
    pugi::xml_node sibling_elem = root.append_child("sibling_element");
    
    // Create initial attributes on source element with diverse types
    pugi::xml_attribute src_attr1 = source_elem.append_attribute("id");
    src_attr1.set_value("source_123");
    
    pugi::xml_attribute src_attr2 = source_elem.append_attribute("count");
    src_attr2.set_value(42);
    
    pugi::xml_attribute src_attr3 = source_elem.append_attribute("price");
    src_attr3.set_value(3.14159);
    
    pugi::xml_attribute src_attr4 = source_elem.append_attribute("active");
    src_attr4.set_value(true);
    
    // Create initial attributes on target element
    pugi::xml_attribute tgt_attr1 = target_elem.append_attribute("target_id");
    tgt_attr1.set_value("target_456");
    
    pugi::xml_attribute tgt_attr2 = target_elem.append_attribute("version");
    tgt_attr2.set_value(1);
    
    // Create attribute chain on container element for insert operations
    pugi::xml_attribute chain_attr1 = container_elem.append_attribute("chain_first");
    chain_attr1.set_value("first_value");
    
    pugi::xml_attribute chain_attr2 = container_elem.append_attribute("chain_second");
    chain_attr2.set_value("second_value");
    
    pugi::xml_attribute chain_attr3 = container_elem.append_attribute("chain_third");
    chain_attr3.set_value("third_value");
    
    // ==================== PHASE 2: ATTRIBUTE COPY OPERATIONS ====================
    // TARGET: xml_node::append_copy, xml_node::prepend_copy, 
    //         xml_node::insert_copy_after, xml_node::insert_copy_before
    
    // Consume operation type and parameters from fuzzer input
    while (fdp.remaining_bytes() > 16) {
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 20;
        
        switch (operation) {
            case 0: { // Test append_copy - copy attribute to end of target's attribute list
                pugi::xml_attribute copy_result = target_elem.append_copy(src_attr1);
                (void)copy_result; // Use result to avoid compiler warnings
                
                // Try copying different attribute types
                pugi::xml_attribute copy_result2 = container_elem.append_copy(src_attr2);
                (void)copy_result2;
                
                pugi::xml_attribute copy_result3 = sibling_elem.append_copy(src_attr3);
                (void)copy_result3;
                break;
            }
            
            case 1: { // Test prepend_copy - copy attribute to beginning of target's attribute list
                pugi::xml_attribute copy_result = target_elem.prepend_copy(src_attr2);
                (void)copy_result;
                
                // Test with different target elements
                pugi::xml_attribute copy_result2 = container_elem.prepend_copy(tgt_attr1);
                (void)copy_result2;
                break;
            }
            
            case 2: { // Test insert_copy_after - copy attribute after specific attribute
                pugi::xml_attribute copy_result = target_elem.insert_copy_after(src_attr3, tgt_attr1);
                (void)copy_result;
                
                // Test inserting in the middle of attribute chain
                pugi::xml_attribute copy_result2 = container_elem.insert_copy_after(src_attr4, chain_attr2);
                (void)copy_result2;
                
                // Test edge case: insert after last attribute
                pugi::xml_attribute copy_result3 = container_elem.insert_copy_after(tgt_attr2, chain_attr3);
                (void)copy_result3;
                break;
            }
            
            case 3: { // Test insert_copy_before - copy attribute before specific attribute
                pugi::xml_attribute copy_result = target_elem.insert_copy_before(src_attr4, tgt_attr2);
                (void)copy_result;
                
                // Test inserting before first attribute
                pugi::xml_attribute copy_result2 = container_elem.insert_copy_before(tgt_attr1, chain_attr1);
                (void)copy_result2;
                
                // Test inserting in the middle
                pugi::xml_attribute copy_result3 = container_elem.insert_copy_before(src_attr1, chain_attr2);
                (void)copy_result3;
                break;
            }
            
            case 4: { // Test attribute insertion with new names (not copies)
                // TARGET: xml_node::insert_attribute_after, xml_node::insert_attribute_before
                
                // Consume attribute name from fuzzer input
                std::string new_attr_name = fdp.ConsumeRandomLengthString(30);
                
                pugi::xml_attribute new_attr = target_elem.insert_attribute_after(new_attr_name.c_str(), tgt_attr1);
                if (new_attr) {
                    // Set value based on fuzzer input
                    std::string attr_value = fdp.ConsumeRandomLengthString(50);
                    new_attr.set_value(attr_value.c_str());
                }
                
                // Test insert_attribute_before
                std::string new_attr_name2 = fdp.ConsumeRandomLengthString(30);
                pugi::xml_attribute new_attr2 = container_elem.insert_attribute_before(new_attr_name2.c_str(), chain_attr2);
                if (new_attr2) {
                    double numeric_value = fdp.ConsumeFloatingPoint<double>();
                    new_attr2.set_value(numeric_value);
                }
                break;
            }
            
            case 5: { // Test attribute traversal and manipulation
                // Iterate through all attributes and modify some
                for (pugi::xml_attribute attr = source_elem.first_attribute(); attr; attr = attr.next_attribute()) {
                    // Consume operation for each attribute
                    if (fdp.remaining_bytes() > 8) {
                        uint8_t attr_op = fdp.ConsumeIntegral<uint8_t>() % 4;
                        
                        switch (attr_op) {
                            case 0: { // Rename attribute
                                if (fdp.remaining_bytes() > 10) {
                                    std::string new_name = "renamed_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                                    attr.set_name(new_name.c_str());
                                }
                                break;
                            }
                                
                            case 1: { // Change attribute value type
                                if (fdp.ConsumeBool()) {
                                    attr.set_value(fdp.ConsumeIntegral<int>());
                                } else {
                                    std::string new_val = fdp.ConsumeRandomLengthString(40);
                                    attr.set_value(new_val.c_str());
                                }
                                break;
                            }
                                
                            case 2: { // Copy this attribute to another element
                                pugi::xml_attribute copy = sibling_elem.append_copy(attr);
                                (void)copy;
                                break;
                            }
                                
                            case 3: { // Get and use attribute value
                                std::string str_val = attr.value();
                                double num_val = attr.as_double(0.0);
                                bool bool_val = attr.as_bool(false);
                                (void)str_val;
                                (void)num_val;
                                (void)bool_val;
                                break;
                            }
                        }
                    }
                }
                break;
            }
            
            case 6: { // Test attribute removal and reinsertion
                // Remove an attribute
                bool removed = target_elem.remove_attribute(tgt_attr1);
                (void)removed;
                
                // Try to copy removed attribute (should fail or create empty attribute)
                pugi::xml_attribute copy_attempt = container_elem.append_copy(tgt_attr1);
                (void)copy_attempt;
                
                // Re-add attribute with same name
                pugi::xml_attribute new_attr = target_elem.append_attribute("target_id");
                new_attr.set_value("recreated_value");
                
                // Test removing by name
                bool removed_by_name = source_elem.remove_attribute("id");
                (void)removed_by_name;
                
                // Test remove_attributes (clear all attributes)
                if (fdp.ConsumeBool()) {
                    bool all_removed = sibling_elem.remove_attributes();
                    (void)all_removed;
                }
                break;
            }
            
            case 7: { // Test complex attribute chains
                // Create a new element with many attributes
                pugi::xml_node complex_elem = root.append_child("complex_element");
                
                // Add multiple attributes in sequence
                int num_attrs = fdp.ConsumeIntegralInRange<int>(1, 10);
                pugi::xml_attribute prev_attr;
                
                for (int i = 0; i < num_attrs && fdp.remaining_bytes() > 10; ++i) {
                    std::string attr_name = "attr_" + std::to_string(i);
                    
                    if (i == 0) {
                        prev_attr = complex_elem.append_attribute(attr_name.c_str());
                    } else {
                        prev_attr = complex_elem.insert_attribute_after(attr_name.c_str(), prev_attr);
                    }
                    
                    if (prev_attr) {
                        // Set diverse value types
                        switch (i % 5) {
                            case 0: prev_attr.set_value(fdp.ConsumeIntegral<int>()); break;
                            case 1: prev_attr.set_value(fdp.ConsumeFloatingPoint<double>()); break;
                            case 2: prev_attr.set_value(fdp.ConsumeBool()); break;
                            case 3: {
                                std::string str_val = fdp.ConsumeRandomLengthString(30);
                                prev_attr.set_value(str_val.c_str());
                                break;
                            }
                            case 4: prev_attr.set_value(static_cast<long long>(fdp.ConsumeIntegral<int64_t>())); break;
                        }
                    }
                }
                
                // Now copy the entire chain to another element
                pugi::xml_node copy_elem = root.append_child("copy_of_complex");
                for (pugi::xml_attribute attr = complex_elem.first_attribute(); attr; attr = attr.next_attribute()) {
                    pugi::xml_attribute copied = copy_elem.append_copy(attr);
                    (void)copied;
                }
                break;
            }
            
            case 8: { // Test attribute iteration and predicate finding
                // Use find_attribute with different predicates
                
                // Find attribute by name contains substring
                pugi::xml_attribute found1 = source_elem.find_attribute(
                    [](pugi::xml_attribute attr) {
                        std::string name = attr.name();
                        return name.find("id") != std::string::npos;
                    }
                );
                (void)found1;
                
                // Find attribute by numeric value
                pugi::xml_attribute found2 = source_elem.find_attribute(
                    [](pugi::xml_attribute attr) {
                        return attr.as_int(-1) > 0;
                    }
                );
                (void)found2;
                
                // Find attribute by boolean value
                pugi::xml_attribute found3 = source_elem.find_attribute(
                    [](pugi::xml_attribute attr) {
                        return attr.as_bool(false) == true;
                    }
                );
                (void)found3;
                
                // Find attribute with string value containing specific text
                std::string search_text = fdp.ConsumeRandomLengthString(10);
                pugi::xml_attribute found4 = source_elem.find_attribute(
                    [&search_text](pugi::xml_attribute attr) {
                        std::string value = attr.value();
                        return value.find(search_text) != std::string::npos;
                    }
                );
                (void)found4;
                break;
            }
            
            case 9: { // Test attribute copying between different documents
                // Create a second document
                pugi::xml_document doc2;
                pugi::xml_node root2 = doc2.append_child("second_doc_root");
                pugi::xml_node elem2 = root2.append_child("element_in_doc2");
                
                // Try copying attributes between documents (should work)
                pugi::xml_attribute cross_doc_copy = elem2.append_copy(src_attr1);
                (void)cross_doc_copy;
                
                // Add attribute to second document and copy back
                pugi::xml_attribute doc2_attr = elem2.append_attribute("doc2_attribute");
                doc2_attr.set_value("from_doc2");
                
                pugi::xml_attribute copy_back = source_elem.append_copy(doc2_attr);
                (void)copy_back;
                break;
            }
            
            case 10: { // Test attribute order preservation and manipulation
                // Verify attribute order after various operations
                std::vector<std::string> original_order;
                for (pugi::xml_attribute attr = container_elem.first_attribute(); attr; attr = attr.next_attribute()) {
                    original_order.push_back(attr.name());
                }
                
                // Perform some insertions that should maintain order
                if (!original_order.empty() && fdp.remaining_bytes() > 20) {
                    // Find middle attribute
                    size_t middle_idx = original_order.size() / 2;
                    pugi::xml_attribute middle_attr;
                    size_t current_idx = 0;
                    
                    for (pugi::xml_attribute attr = container_elem.first_attribute(); attr; attr = attr.next_attribute()) {
                        if (current_idx == middle_idx) {
                            middle_attr = attr;
                            break;
                        }
                        current_idx++;
                    }
                    
                    if (middle_attr) {
                        // Insert new attribute before middle
                        std::string new_name = "inserted_before_middle";
                        pugi::xml_attribute inserted = container_elem.insert_attribute_before(new_name.c_str(), middle_attr);
                        if (inserted) {
                            inserted.set_value("inserted_value");
                        }
                        
                        // Copy an attribute and insert after middle
                        pugi::xml_attribute copied_insert = container_elem.insert_copy_after(src_attr1, middle_attr);
                        (void)copied_insert;
                    }
                }
                
                // Verify we can still traverse all attributes
                int attr_count = 0;
                for (pugi::xml_attribute attr = container_elem.first_attribute(); attr; attr = attr.next_attribute()) {
                    attr_count++;
                    // Access attribute properties
                    std::string name = attr.name();
                    std::string value = attr.value();
                    (void)name;
                    (void)value;
                }
                (void)attr_count;
                break;
            }
            
            case 11: { // Test attribute lifecycle with temporary objects
                // Create temporary attributes and copy them
                {
                    pugi::xml_node temp_node = root.append_child("temp_node");
                    pugi::xml_attribute temp_attr = temp_node.append_attribute("temp_attr");
                    temp_attr.set_value("temporary_value");
                    
                    // Copy temporary attribute before node is potentially removed
                    pugi::xml_attribute persistent_copy = target_elem.append_copy(temp_attr);
                    (void)persistent_copy;
                    
                    // Remove the temporary node
                    root.remove_child(temp_node);
                    
                    // The copied attribute should still be valid
                    if (persistent_copy) {
                        std::string copied_value = persistent_copy.value();
                        (void)copied_value;
                    }
                }
                
                // Test with attribute created in inner scope
                pugi::xml_attribute scope_test_attr;
                {
                    pugi::xml_node inner_node = root.append_child("inner_scope_node");
                    scope_test_attr = inner_node.append_attribute("inner_attr");
                    scope_test_attr.set_value(3.14);
                    
                    // Copy attribute while still in scope
                    pugi::xml_attribute scope_copy = target_elem.append_copy(scope_test_attr);
                    (void)scope_copy;
                }
                
                // The original attribute handle is still valid even though node exists
                if (scope_test_attr) {
                    double val = scope_test_attr.as_double(0.0);
                    (void)val;
                }
                break;
            }
            
            default: {
                // Test miscellaneous attribute operations
                
                // Test attribute comparison operators
                bool eq1 = (src_attr1 == src_attr1);
                bool eq2 = (src_attr1 != src_attr2);
                bool lt1 = (src_attr1 < src_attr2);
                bool gt1 = (src_attr1 > src_attr2);
                (void)eq1; (void)eq2; (void)lt1; (void)gt1;
                
                // Test empty attribute handling
                pugi::xml_attribute empty_attr;
                bool is_empty = empty_attr.empty();
                std::string empty_name = empty_attr.name();
                std::string empty_value = empty_attr.value();
                (void)is_empty; (void)empty_name; (void)empty_value;
                
                // Test next_attribute and previous_attribute traversal
                if (container_elem.first_attribute()) {
                    pugi::xml_attribute first = container_elem.first_attribute();
                    pugi::xml_attribute second = first.next_attribute();
                    if (second) {
                        pugi::xml_attribute back_to_first = second.previous_attribute();
                        (void)back_to_first;
                    }
                }
                
                // Test attribute with special characters in name/value
                if (fdp.remaining_bytes() > 20) {
                    std::string special_name = fdp.ConsumeRandomLengthString(50);
                    std::string special_value = fdp.ConsumeRandomLengthString(100);
                    
                    pugi::xml_attribute special_attr = sibling_elem.append_attribute(special_name.c_str());
                    if (special_attr) {
                        special_attr.set_value(special_value.c_str());
                        
                        // Copy special attribute
                        pugi::xml_attribute special_copy = target_elem.append_copy(special_attr);
                        (void)special_copy;
                    }
                }
                break;
            }
        }
        
        // Check if we should break early
        if (fdp.remaining_bytes() < 16) {
            break;
        }
    }
    
    // ==================== PHASE 3: VALIDATION AND CLEANUP ====================
    // Final validation that document is still structurally sound
#ifndef PUGIXML_NO_STL
    std::stringstream xml_stream;
    doc.save(xml_stream);
    std::string xml_output = xml_stream.str();
#endif
    
    // Count total attributes in document
    size_t total_attrs = 0;
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            total_attrs++;
        }
    }
    (void)total_attrs;
    
    return 0;
}

} // namespace harness_015
