#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>

namespace harness_011 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for move operations and string_view tests
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Basic node move operations within same document
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create initial structure with multiple nodes
            size_t node_count = fdp.ConsumeIntegralInRange<size_t>(3, 8);
            std::vector<pugi::xml_node> nodes;
            
            for (size_t i = 0; i < node_count && fdp.remaining_bytes() > 20; ++i) {
                std::string node_name = "node" + std::to_string(i);
                pugi::xml_node node = root.append_child(node_name.c_str());
                
                // Add text content
                std::string text = fdp.ConsumeRandomLengthString(32);
                node.append_child(pugi::node_pcdata).set_value(text.c_str());
                
                // Add attributes
                size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                for (size_t j = 0; j < attr_count && fdp.remaining_bytes() > 10; ++j) {
                    std::string attr_name = "attr" + std::to_string(j);
                    std::string attr_value = fdp.ConsumeRandomLengthString(16);
                    node.append_attribute(attr_name.c_str()) = attr_value.c_str();
                }
                
                nodes.push_back(node);
            }
            
            if (nodes.size() >= 3) {
                // Test append_move - move first node to the end
                pugi::xml_node moved1 = root.append_move(nodes[0]);
                
                // Test prepend_move - move last node to the beginning
                pugi::xml_node moved2 = root.prepend_move(nodes[nodes.size() - 1]);
                
                // Test insert_move_after - move middle node after another node
                if (root.first_child() && root.first_child().next_sibling()) {
                    pugi::xml_node middle_node = nodes[nodes.size() / 2];
                    pugi::xml_node reference = root.first_child();
                    pugi::xml_node moved3 = root.insert_move_after(middle_node, reference);
                }
                
                // Test insert_move_before - move node before another node
                if (root.first_child() && root.first_child().next_sibling()) {
                    pugi::xml_node target_node = root.first_child().next_sibling();
                    pugi::xml_node to_move = root.last_child();
                    pugi::xml_node moved4 = root.insert_move_before(to_move, target_node);
                }
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Cross-document node move operations
            pugi::xml_document doc1;
            pugi::xml_document doc2;
            
            // Create structure in first document
            pugi::xml_node root1 = doc1.append_child("doc1_root");
            size_t nodes_in_doc1 = fdp.ConsumeIntegralInRange<size_t>(2, 5);
            
            for (size_t i = 0; i < nodes_in_doc1 && fdp.remaining_bytes() > 20; ++i) {
                std::string node_name = "doc1_node" + std::to_string(i);
                pugi::xml_node node = root1.append_child(node_name.c_str());
                
                std::string text = fdp.ConsumeRandomLengthString(32);
                node.append_child(pugi::node_pcdata).set_value(text.c_str());
                
                // Add attributes with string_view
                #ifdef PUGIXML_HAS_STRING_VIEW
                size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 2);
                for (size_t j = 0; j < attr_count && fdp.remaining_bytes() > 10; ++j) {
                    std::string attr_name_str = "attr" + std::to_string(j);
                    std::string attr_value_str = fdp.ConsumeRandomLengthString(16);
                    std::string_view attr_name_sv(attr_name_str);
                    std::string_view attr_value_sv(attr_value_str);
                    
                    pugi::xml_attribute attr = node.append_attribute(attr_name_sv);
                    attr.set_value(attr_value_sv);
                }
                #endif
            }
            
            // Create structure in second document
            pugi::xml_node root2 = doc2.append_child("doc2_root");
            
            // Attempt to move nodes between documents (should fail - cross-document moves not allowed)
            if (root1.first_child()) {
                // This should return empty node since cross-document moves aren't allowed
                pugi::xml_node attempted_move = root2.append_move(root1.first_child());
            }
            
            // Test moving nodes within each document
            if (root1.first_child() && root1.first_child().next_sibling()) {
                root1.insert_move_after(root1.first_child(), root1.first_child().next_sibling());
            }
            
            if (root2.first_child()) {
                // Add a child to root2 to have something to move
                std::string new_node_name = fdp.ConsumeRandomLengthString(16);
                pugi::xml_node new_node = root2.append_child(new_node_name.c_str());
                
                if (root2.first_child() && new_node) {
                    root2.append_move(root2.first_child());
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Complex move operations with nested hierarchies
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create a complex hierarchy
            size_t depth = fdp.ConsumeIntegralInRange<size_t>(2, 4);
            pugi::xml_node current = root;
            
            for (size_t level = 0; level < depth && fdp.remaining_bytes() > 20; ++level) {
                std::string level_name = "level" + std::to_string(level);
                pugi::xml_node child = current.append_child(level_name.c_str());
                
                // Add siblings at each level
                size_t siblings = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                for (size_t s = 0; s < siblings && fdp.remaining_bytes() > 10; ++s) {
                    std::string sibling_name = level_name + "_sib" + std::to_string(s);
                    current.append_child(sibling_name.c_str());
                }
                
                current = child;
            }
            
            // Now perform various move operations on this hierarchy
            if (root.first_child()) {
                pugi::xml_node first_child = root.first_child();
                
                // Move a deep node to root level
                if (current && current != root) {
                    root.append_move(current);
                }
                
                // Move root's first child to be a child of another node
                if (root.first_child() && root.first_child().next_sibling()) {
                    pugi::xml_node second_child = root.first_child().next_sibling();
                    if (second_child.first_child()) {
                        second_child.append_move(root.first_child());
                    }
                }
                
                // Move node before another node
                if (first_child && first_child.next_sibling() && first_child.next_sibling().next_sibling()) {
                    root.insert_move_before(first_child.next_sibling(), first_child.next_sibling().next_sibling());
                }
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Move operations with string_view variant removal APIs
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create nodes with attributes
            size_t node_count = fdp.ConsumeIntegralInRange<size_t>(3, 6);
            
            for (size_t i = 0; i < node_count && fdp.remaining_bytes() > 20; ++i) {
                std::string node_name = "move_node" + std::to_string(i);
                pugi::xml_node node = root.append_child(node_name.c_str());
                
                #ifdef PUGIXML_HAS_STRING_VIEW
                // Add attributes using string_view
                size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(1, 4);
                for (size_t j = 0; j < attr_count && fdp.remaining_bytes() > 10; ++j) {
                    std::string attr_name_str = "attr" + std::to_string(j);
                    std::string attr_value_str = fdp.ConsumeRandomLengthString(16);
                    std::string_view attr_name_sv(attr_name_str);
                    std::string_view attr_value_sv(attr_value_str);
                    
                    pugi::xml_attribute attr = node.append_attribute(attr_name_sv);
                    attr.set_value(attr_value_sv);
                }
                #endif
            }
            
            // Test move operations combined with string_view removal
            if (root.first_child() && root.first_child().next_sibling()) {
                pugi::xml_node first = root.first_child();
                pugi::xml_node second = first.next_sibling();
                
                // Move first child to after second child
                root.insert_move_after(first, second);
                
                #ifdef PUGIXML_HAS_STRING_VIEW
                // Test remove_attribute with string_view on moved node
                if (first.first_attribute()) {
                    std::string attr_to_remove = "attr0";
                    std::string_view attr_to_remove_sv(attr_to_remove);
                    first.remove_attribute(attr_to_remove_sv);
                }
                
                // Test remove_child with string_view
                if (second.first_child()) {
                    std::string child_to_remove = "child0";
                    std::string_view child_to_remove_sv(child_to_remove);
                    second.remove_child(child_to_remove_sv);
                }
                #endif
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Edge cases for move operations
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Add a couple of nodes
            pugi::xml_node node1 = root.append_child("node1");
            pugi::xml_node node2 = root.append_child("node2");
            pugi::xml_node node3 = root.append_child("node3");
            
            // Test edge cases:
            
            // 1. Move node to itself (should fail)
            pugi::xml_node self_move = root.append_move(node1);
            if (self_move.empty()) {
                // Expected failure - can't move node to itself
            }
            
            // 2. Move empty node
            pugi::xml_node empty_node;
            pugi::xml_node empty_move = root.append_move(empty_node);
            
            // 3. Move node to its parent (already there, should be no-op or fail)
            if (node2) {
                pugi::xml_node parent_move = root.append_move(node2);
            }
            
            // 4. Move node before/after itself
            if (node3) {
                pugi::xml_node self_before = root.insert_move_before(node3, node3);
                pugi::xml_node self_after = root.insert_move_after(node3, node3);
            }
            
            // 5. Test with document node
            pugi::xml_node doc_move = doc.append_move(doc);
            
            break;
        }
        
        case 5: {
            // Scenario 5: Move operations with text nodes and mixed content
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create mixed content: elements, text nodes, comments
            pugi::xml_node elem1 = root.append_child("element1");
            elem1.append_child(pugi::node_pcdata).set_value("Text content 1");
            
            pugi::xml_node elem2 = root.append_child("element2");
            elem2.append_child(pugi::node_pcdata).set_value("Text content 2");
            
            pugi::xml_node comment = root.append_child(pugi::node_comment);
            comment.set_value("This is a comment");
            
            pugi::xml_node cdata = root.append_child(pugi::node_cdata);
            cdata.set_value("<![CDATA[This is CDATA]]>");
            
            // Move text nodes around
            if (elem1.first_child() && elem2.first_child()) {
                // Move text from elem1 to elem2
                elem2.append_move(elem1.first_child());
                
                // Move comment before element1
                root.insert_move_before(comment, elem1);
                
                // Move CDATA after element2
                root.insert_move_after(cdata, elem2);
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Advanced string_view variants not covered in harness_008
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Test string_view variants for methods that might not have been covered
            #ifdef PUGIXML_HAS_STRING_VIEW
            
            // Create nodes using string_view with various edge cases
            std::string empty_str = "";
            std::string_view empty_sv(empty_str);
            
            // Test with empty string_view
            pugi::xml_node empty_node = root.append_child(empty_sv);
            
            // Test with very long string_view
            if (fdp.remaining_bytes() > 100) {
                std::string long_str = fdp.ConsumeRandomLengthString(100);
                std::string_view long_sv(long_str);
                pugi::xml_node long_node = root.append_child(long_sv);
                
                // Set value with string_view containing special characters
                std::string special_chars = fdp.ConsumeRandomLengthString(50);
                // Add some control characters
                for (char& c : special_chars) {
                    if (fdp.ConsumeBool()) {
                        c = static_cast<char>(fdp.ConsumeIntegralInRange<int>(1, 31));
                    }
                }
                std::string_view special_sv(special_chars);
                long_node.set_value(special_sv);
            }
            
            // Test find_child with lambda using string_view comparison
            std::string search_name = "search_target";
            std::string_view search_sv(search_name);
            pugi::xml_node target = root.append_child(search_sv);
            
            // This would be the find_child usage pattern, though we can't easily test predicates
            #endif
            
            break;
        }
        
        case 7: {
            // Scenario 7: Move operations in combination with copy operations
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create source nodes
            pugi::xml_node source1 = root.append_child("source1");
            source1.append_child(pugi::node_pcdata).set_value("Source 1 text");
            
            pugi::xml_node source2 = root.append_child("source2");
            source2.append_child(pugi::node_pcdata).set_value("Source 2 text");
            
            // Create a container node
            pugi::xml_node container = root.append_child("container");
            
            // Copy vs Move comparison:
            // Copy creates a new node, Move relocates existing node
            
            // First copy a node
            pugi::xml_node copied = container.append_copy(source1);
            
            // Then move a node
            pugi::xml_node moved = container.append_move(source2);
            
            // Verify copied node exists in both places
            // Verify moved node only exists in new location
            
            // Test moving a copied node
            if (copied) {
                root.append_move(copied);
            }
            break;
        }
        
        case 8: {
            // Scenario 8: Stress test - many sequential move operations
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create many nodes
            size_t node_count = fdp.ConsumeIntegralInRange<size_t>(10, 20);
            std::vector<pugi::xml_node> nodes;
            
            for (size_t i = 0; i < node_count && fdp.remaining_bytes() > 10; ++i) {
                std::string node_name = "stress_node" + std::to_string(i);
                pugi::xml_node node = root.append_child(node_name.c_str());
                nodes.push_back(node);
            }
            
            // Perform many move operations
            for (size_t i = 0; i < nodes.size() && fdp.remaining_bytes() > 5; ++i) {
                if (nodes[i] && root.first_child()) {
                    if (fdp.ConsumeBool()) {
                        root.append_move(nodes[i]);
                    } else if (fdp.ConsumeBool()) {
                        root.prepend_move(nodes[i]);
                    } else if (root.first_child().next_sibling()) {
                        root.insert_move_after(nodes[i], root.first_child());
                    }
                }
            }
            break;
        }
        
        case 9: {
            // Scenario 9: Move operations with attribute manipulation
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create nodes with attributes
            pugi::xml_node node1 = root.append_child("node_with_attrs");
            pugi::xml_node node2 = root.append_child("target_node");
            
            // Add attributes to node1 using string_view
            #ifdef PUGIXML_HAS_STRING_VIEW
            size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(2, 5);
            for (size_t i = 0; i < attr_count && fdp.remaining_bytes() > 10; ++i) {
                std::string attr_name_str = "attr" + std::to_string(i);
                std::string attr_value_str = fdp.ConsumeRandomLengthString(16);
                std::string_view attr_name_sv(attr_name_str);
                std::string_view attr_value_sv(attr_value_str);
                
                pugi::xml_attribute attr = node1.append_attribute(attr_name_sv);
                attr.set_value(attr_value_sv);
            }
            #endif
            
            // Move node with attributes
            pugi::xml_node moved = root.append_move(node1);
            
            // After move, attributes should still be accessible
            if (moved) {
                for (pugi::xml_attribute attr = moved.first_attribute(); attr; attr = attr.next_attribute()) {
                    attr.name();
                    attr.value();
                }
                
                // Test removing attributes after move with string_view
                #ifdef PUGIXML_HAS_STRING_VIEW
                if (moved.first_attribute()) {
                    std::string attr_to_remove = "attr0";
                    std::string_view attr_to_remove_sv(attr_to_remove);
                    moved.remove_attribute(attr_to_remove_sv);
                }
                #endif
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_011
