#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>
#include "pugixml.hpp"

namespace harness_002 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic operations
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 different tree manipulation modes
    bool create_from_scratch = fdp.ConsumeBool();
    bool test_attribute_ops = fdp.ConsumeBool();
    bool test_node_removal = fdp.ConsumeBool();
    bool test_node_movement = fdp.ConsumeBool();
    bool test_node_copy = fdp.ConsumeBool();
    bool test_traversal = fdp.ConsumeBool();
    
    // Consume string data for node/attribute names and values
    std::string root_name = fdp.ConsumeRandomLengthString(32);
    std::string child1_name = fdp.ConsumeRandomLengthString(32);
    std::string child2_name = fdp.ConsumeRandomLengthString(32);
    std::string attr1_name = fdp.ConsumeRandomLengthString(32);
    std::string attr2_name = fdp.ConsumeRandomLengthString(32);
    std::string attr3_name = fdp.ConsumeRandomLengthString(32);
    std::string text_content = fdp.ConsumeRandomLengthString(256);
    
    // Consume numeric data for attribute values
    int int_val = fdp.ConsumeIntegral<int>();
    double double_val = fdp.ConsumeFloatingPoint<double>();
    bool bool_val = fdp.ConsumeBool();
    
    // Create XML document
    pugi::xml_document doc;
    
    if (create_from_scratch) {
        // Create document from scratch (not parsing)
        pugi::xml_node root = doc.append_child(root_name.empty() ? "root" : root_name.c_str());
        
        // Add attributes to root
        if (test_attribute_ops) {
            pugi::xml_attribute attr1 = root.append_attribute(attr1_name.empty() ? "attr1" : attr1_name.c_str());
            attr1.set_value(int_val);
            
            pugi::xml_attribute attr2 = root.append_attribute(attr2_name.empty() ? "attr2" : attr2_name.c_str());
            attr2.set_value(double_val);
            
            pugi::xml_attribute attr3 = root.append_attribute(attr3_name.empty() ? "attr3" : attr3_name.c_str());
            attr3.set_value(bool_val);
            
            // Test attribute modification
            attr1.set_name((attr1_name + "_modified").c_str());
            attr2.set_value(double_val * 2);
        }
        
        // Add child nodes with different types based on operation mode
        switch (operation_mode) {
            case 0: // Basic element nodes
            {
                pugi::xml_node child1 = root.append_child(child1_name.empty() ? "child1" : child1_name.c_str());
                pugi::xml_node child2 = root.append_child(child2_name.empty() ? "child2" : child2_name.c_str());
                
                // Add text content
                child1.append_child(pugi::node_pcdata).set_value(text_content.c_str());
                
                // Add attributes to children
                child1.append_attribute("id") = "1";
                child2.append_attribute("id") = "2";
                break;
            }
            case 1: // Different node types
            {
                pugi::xml_node comment = root.append_child(pugi::node_comment);
                comment.set_value("Generated comment");
                
                pugi::xml_node pi = root.append_child(pugi::node_pi);
                pi.set_name("target");
                pi.set_value("processing instruction");
                
                pugi::xml_node cdata = root.append_child(pugi::node_cdata);
                cdata.set_value("<cdata content>");
                break;
            }
            case 2: // Insert operations (insert before/after)
            {
                pugi::xml_node first = root.append_child("first");
                pugi::xml_node last = root.append_child("last");
                
                pugi::xml_node middle = root.insert_child_after("middle", first);
                pugi::xml_node before_first = root.insert_child_before("before_first", first);
                break;
            }
            case 3: // Prepend operations
            {
                pugi::xml_node last = root.append_child("last");
                pugi::xml_node first = root.prepend_child("first");
                pugi::xml_node middle = root.insert_child_after("middle", first);
                break;
            }
            case 4: // Nested structure
            {
                pugi::xml_node level1 = root.append_child("level1");
                pugi::xml_node level2 = level1.append_child("level2");
                pugi::xml_node level3 = level2.append_child("level3");
                
                level1.append_attribute("depth") = 1;
                level2.append_attribute("depth") = 2;
                level3.append_attribute("depth") = 3;
                break;
            }
            case 5: // Attribute insertion operations
            {
                pugi::xml_node node = root.append_child("node");
                pugi::xml_attribute first_attr = node.append_attribute("first");
                first_attr.set_value("1");
                
                pugi::xml_attribute last_attr = node.append_attribute("last");
                last_attr.set_value("3");
                
                pugi::xml_attribute middle_attr = node.insert_attribute_after("middle", first_attr);
                middle_attr.set_value("2");
                break;
            }
            case 6: // Node removal operations
            {
                if (test_node_removal) {
                    pugi::xml_node child1 = root.append_child("child1");
                    pugi::xml_node child2 = root.append_child("child2");
                    pugi::xml_node child3 = root.append_child("child3");
                    
                    // Remove middle child
                    root.remove_child(child2);
                    
                    // Remove by name
                    root.append_child("todelete");
                    root.remove_child("todelete");
                    
                    // Remove attributes
                    child1.append_attribute("temp") = "value";
                    child1.remove_attribute("temp");
                }
                break;
            }
            case 7: // Node copy and move operations
            {
                if (test_node_copy || test_node_movement) {
                    pugi::xml_node source = root.append_child("source");
                    source.append_attribute("id") = "source_id";
                    source.append_child("source_child");
                    
                    if (test_node_copy) {
                        pugi::xml_node copy = root.append_copy(source);
                    }
                    
                    if (test_node_movement) {
                        pugi::xml_node target = doc.append_child("target");
                        target.append_move(source);
                    }
                }
                break;
            }
        }
        
        // Test tree traversal methods
        if (test_traversal && !root.empty()) {
            // Child iteration
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                const char* name = child.name();
                const char* value = child.value();
                
                // Attribute iteration
                for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
                    const char* attr_name = attr.name();
                    const char* attr_value = attr.value();
                }
            }
            
            // Find operations
            pugi::xml_node found = root.find_child([](pugi::xml_node node) { return node.name() != nullptr; });
            
            // Path-based access
            if (!root.first_child().empty()) {
                pugi::xml_node by_path = root.first_element_by_path("child1");
            }
        }
        
        // Test node/attribute modification
        if (!root.empty()) {
            pugi::xml_node first_child = root.first_child();
            if (first_child) {
                first_child.set_name("renamed_node");
                first_child.set_value("new_value");
                
                pugi::xml_attribute first_attr = first_child.first_attribute();
                if (first_attr) {
                    first_attr.set_name("renamed_attr");
                    first_attr.set_value("new_attr_value");
                }
            }
        }
        
        // Test text nodes
        pugi::xml_node text_node = doc.append_child("text_node");
        pugi::xml_text text = text_node.text();
        text.set("Some text content");
        
        // Test child_value methods
        const char* child_val = root.child_value();
        if (root.first_child()) {
            const char* named_child_val = root.child_value(root.first_child().name());
        }
        
    } else {
        // Use remaining bytes as XML content for parsing
        std::vector<uint8_t> xml_content = fdp.ConsumeRemainingBytes<uint8_t>();
        if (xml_content.empty()) return 0;
        
        pugi::xml_parse_result result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_default);
        
        if (result) {
            // Apply tree manipulation operations on parsed document
            pugi::xml_node root = doc.document_element();
            if (!root) root = doc.first_child();
            
            if (root) {
                // Add a new child to parsed document
                pugi::xml_node new_child = root.append_child("fuzzer_added");
                new_child.append_attribute("fuzzed") = true;
                new_child.append_child(pugi::node_pcdata).set_value("Added by fuzzer");
                
                // Modify existing nodes/attributes if they exist
                for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                    if (child.first_attribute()) {
                        pugi::xml_attribute attr = child.first_attribute();
                        attr.set_value("modified_by_fuzzer");
                    }
                }
                
                // Test removal operations
                if (test_node_removal) {
                    pugi::xml_node first_child = root.first_child();
                    if (first_child) {
                        root.remove_child(first_child);
                    }
                    
                    // Remove all children if there are many
                    if (root.first_child() && root.last_child() && root.first_child() != root.last_child()) {
                        root.remove_children();
                    }
                }
            }
        }
    }
    
    // Final validation: ensure document is well-formed
    if (!doc.empty()) {
        // Try to save to string (tests internal consistency)
        #ifndef PUGIXML_NO_STL
        std::ostringstream oss;
        doc.print(oss);
        std::string output = oss.str();
        #endif
        
        // Test range-based for loops if supported
        pugi::xml_node root = doc.document_element();
        if (!root) root = doc.first_child();
        
        if (root) {
            // Test children range
            for (pugi::xml_node child : root.children()) {
                (void)child;
            }
            
            // Test attributes range
            for (pugi::xml_attribute attr : root.attributes()) {
                (void)attr;
            }
        }
    }
    
    return 0;
}

} // namespace harness_002
