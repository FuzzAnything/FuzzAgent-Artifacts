#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include "pugixml.hpp"

namespace harness_007 {


// Advanced custom tree walker specifically targeting xml_node::traverse API
// Implements more complex logic than existing harness_003 to explore deeper coverage
class AdvancedTreeWalker : public pugi::xml_tree_walker {
private:
    int traversal_depth;
    int element_count;
    int attribute_count;
    int text_node_count;
    bool should_skip_elements;
    bool collect_statistics;
    
public:
    AdvancedTreeWalker(bool skip_elements = false, bool collect_stats = true) 
        : traversal_depth(0), element_count(0), attribute_count(0), 
          text_node_count(0), should_skip_elements(skip_elements), 
          collect_statistics(collect_stats) {}
    
    virtual bool begin(pugi::xml_node& node) override {
        // Reset counters at the beginning of traversal
        traversal_depth = 0;
        element_count = 0;
        attribute_count = 0;
        text_node_count = 0;
        
        // Test begin() return value - returning false should stop traversal
        // but we want to test both paths, so we'll use fuzzer input to decide
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Update statistics if enabled
        if (collect_statistics) {
            if (node.type() == pugi::node_element) {
                element_count++;
                
                // Count attributes on this element
                for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
                    attribute_count++;
                }
            } else if (node.type() == pugi::node_pcdata || node.type() == pugi::node_cdata) {
                text_node_count++;
            }
        }
        
        // Test the depth() method from xml_tree_walker base class
        traversal_depth = std::max(traversal_depth, depth());
        // Test attribute retrieval with reference parameter - PRIMARY TARGET API
        // xml_node::attribute(const char*, xml_attribute&) variant
        if (node.type() == pugi::node_element) {
            pugi::xml_attribute found_attr;
            // Attribute names will be passed from fuzzer input in the main function
            // The actual attribute testing happens in the main function where we have
            // access to the fuzzer-provided attribute names
        }
        if (should_skip_elements && node.type() == pugi::node_element) {
            // For elements we want to skip, return true to continue traversal
            // but we won't process their children (the walker handles this internally)
            return true;
        }
        
        return true; // Continue traversal
    }
    virtual bool end(pugi::xml_node& node) override {
        // Final processing after traversal
        return true;
    }
    
    // Statistics accessors
    int getElementCount() const { return element_count; }
    int getAttributeCount() const { return attribute_count; }
    int getTextNodeCount() const { return text_node_count; }
    int getMaxDepth() const { return traversal_depth; }
};

// Secondary custom walker with different behavior for comparison
class StatisticsTreeWalker : public pugi::xml_tree_walker {
private:
    std::vector<std::string> element_names;
    std::vector<std::string> attribute_names;
    
public:
    StatisticsTreeWalker() {}
    
    virtual bool begin(pugi::xml_node& node) override {
        element_names.clear();
        attribute_names.clear();
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        if (node.type() == pugi::node_element) {
            const char* name = node.name();
            if (name) {
                element_names.push_back(name);
            }
            
            for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
                const char* attr_name = attr.name();
                if (attr_name) {
                    attribute_names.push_back(attr_name);
                }
            }
        }
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        return true;
    }
    
    size_t getUniqueElementCount() const {
        std::vector<std::string> unique = element_names;
        std::sort(unique.begin(), unique.end());
        unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
        return unique.size();
    }
    
    size_t getUniqueAttributeCount() const {
        std::vector<std::string> unique = attribute_names;
        std::sort(unique.begin(), unique.end());
        unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
        return unique.size();
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for XML content and control values
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first - determine which APIs to test
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool test_traverse = fdp.ConsumeBool();
    bool test_select_node_xpath_query = fdp.ConsumeBool();
    bool test_attribute_ref_variant = fdp.ConsumeBool();
    bool test_find_child_by_attribute = fdp.ConsumeBool();
    bool test_insert_operations = fdp.ConsumeBool();
    bool test_append_with_string_view = fdp.ConsumeBool();
    bool use_advanced_walker = fdp.ConsumeBool();
    bool collect_statistics = fdp.ConsumeBool();
    
    // Consume parameters for tree walker behavior
    bool skip_elements = fdp.ConsumeBool();
    uint8_t max_traversal_depth = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
    
    // Consume strings for node/attribute names and values
    std::string target_element_name = fdp.ConsumeRandomLengthString(32);
    std::string target_attribute_name = fdp.ConsumeRandomLengthString(32);
    std::string target_attribute_value = fdp.ConsumeRandomLengthString(64);
    std::string new_child_name = fdp.ConsumeRandomLengthString(32);
    std::string new_attribute_name = fdp.ConsumeRandomLengthString(32);
    std::string new_attribute_value = fdp.ConsumeRandomLengthString(64);
    
    // Consume XPath query strings
    std::string xpath_query1 = fdp.ConsumeRandomLengthString(128);
    std::string xpath_query2 = fdp.ConsumeRandomLengthString(128);
    
    // Consume additional strings for common attribute names and other values
    std::string common_attr1 = fdp.ConsumeRandomLengthString(16);  // For attribute names like "id"
    std::string common_attr2 = fdp.ConsumeRandomLengthString(16);  // For attribute names like "name"  
    std::string common_attr3 = fdp.ConsumeRandomLengthString(16);  // For attribute names like "class"
    std::string common_attr4 = fdp.ConsumeRandomLengthString(16);  // For attribute names like "version"
    std::string comment_text = fdp.ConsumeRandomLengthString(64);
    std::string pi_name = fdp.ConsumeRandomLengthString(16);
    std::string pi_value = fdp.ConsumeRandomLengthString(64);
    std::string simple_xpath_query = fdp.ConsumeRandomLengthString(32);
    std::string inserted_child_name = fdp.ConsumeRandomLengthString(16);
    
    // Use remaining bytes as XML content
    std::vector<uint8_t> xml_content = fdp.ConsumeRemainingBytes<uint8_t>();
    if (xml_content.empty()) return 0;
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_default);
    
    // If parsing succeeded, test advanced tree traversal and manipulation APIs
    if (result) {
        // Get document root for operations
        pugi::xml_node root = doc.document_element();
        if (!root) {
            root = doc.first_child();
        }
            if (test_traverse) {
                if (use_advanced_walker) {
                    AdvancedTreeWalker walker(skip_elements, collect_statistics);
                    bool traversal_result = root.traverse(walker);
                    
                    // Access statistics to ensure they're computed
                    int elements = walker.getElementCount();
                    int attributes = walker.getAttributeCount();
                    int text_nodes = walker.getTextNodeCount();
                    int max_depth = walker.getMaxDepth();
                } else {
                    StatisticsTreeWalker stats_walker;
                    bool stats_result = root.traverse(stats_walker);
                    
                    size_t unique_elements = stats_walker.getUniqueElementCount();
                    size_t unique_attributes = stats_walker.getUniqueAttributeCount();
                }
            }
            
            // SCENARIO 2: Test xml_node::select_node(const xpath_query&) - PRIMARY TARGET
            if (test_select_node_xpath_query) {
                try {
                    // Create xpath_query objects (not just string evaluation)
                    pugi::xpath_query query_obj1(xpath_query1.c_str());
                    pugi::xpath_query query_obj2(xpath_query2.c_str());
                    
                    // Test select_node with xpath_query objects
                    pugi::xpath_node selected1 = root.select_node(query_obj1);
                    pugi::xpath_node selected2 = root.select_node(query_obj2);
                    
                    // Also test on child nodes if selections were successful
                    if (selected1.node()) {
                        pugi::xml_node node1 = selected1.node();
                        // Test further operations on selected node
                        pugi::xpath_node nested_selection = node1.select_node(query_obj2);
                    }
                    
                    if (selected2.node()) {
                        pugi::xml_node node2 = selected2.node();
                        // Test attribute selection on the found node
                        if (test_attribute_ref_variant) {
                            pugi::xml_attribute attr_ref;
                            bool has_attr = node2.attribute(common_attr1.empty() ? "id" : common_attr1.c_str(), attr_ref);
                            if (has_attr && attr_ref) {
                                const char* attr_value = attr_ref.value();
                            }
                        }
                    }
                } catch (...) {
                    // XPath query parsing can throw exceptions
                }
            }
            
            // SCENARIO 3: Test xml_node::attribute(const char*, xml_attribute&) - PRIMARY TARGET
            if (test_attribute_ref_variant) {
                // Test attribute retrieval with reference parameter on multiple nodes
                std::vector<pugi::xml_node> elements_to_check;
                
                // Collect some elements to test
                for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                    if (child.type() == pugi::node_element) {
                        elements_to_check.push_back(child);
                        if (elements_to_check.size() >= 5) break;
                    }
                }
                
                // Test reference attribute retrieval on each element
                for (pugi::xml_node& elem : elements_to_check) {
                    pugi::xml_attribute found_attr;
                    
                    // Test with common attribute names using fuzzer-provided strings
                    bool has_id = elem.attribute(common_attr1.empty() ? "id" : common_attr1.c_str(), found_attr);
                    bool has_name = elem.attribute(common_attr2.empty() ? "name" : common_attr2.c_str(), found_attr);
                    bool has_class = elem.attribute(common_attr3.empty() ? "class" : common_attr3.c_str(), found_attr);
                    // Test with the target attribute name from fuzzer input
                    bool has_target = elem.attribute(target_attribute_name.c_str(), found_attr);
                    
                    // Test with dynamically generated names
                    for (int i = 0; i < 3; i++) {
                        std::string dynamic_name = "attr_" + std::to_string(i);
                        bool has_dynamic = elem.attribute(dynamic_name.c_str(), found_attr);
                    }
                    
                    // If attribute was found, test its value
                    if (found_attr) {
                        const char* attr_value = found_attr.value();
                        bool empty_value = (attr_value == nullptr || attr_value[0] == '\0');
                    }
                }
            }
            
            // SCENARIO 4: Test find_child_by_attribute variants - SECONDARY TARGET
            if (test_find_child_by_attribute && !target_element_name.empty() && 
                !target_attribute_name.empty() && !target_attribute_value.empty()) {
                
                // Test three-parameter variant: name, attr_name, attr_value
                pugi::xml_node found_by_name_attr = root.find_child_by_attribute(
                    target_element_name.c_str(), 
                    target_attribute_name.c_str(), 
                    target_attribute_value.c_str());
                
                // Test two-parameter variant: attr_name, attr_value
                pugi::xml_node found_by_attr = root.find_child_by_attribute(
                    target_attribute_name.c_str(), 
                    target_attribute_value.c_str());
                
                // Test with empty/null strings and fuzzer-provided strings (edge cases)
                pugi::xml_node found_empty1 = root.find_child_by_attribute("", common_attr1.empty() ? "test" : common_attr1.c_str(), target_attribute_value.empty() ? "value" : target_attribute_value.c_str());
                pugi::xml_node found_empty2 = root.find_child_by_attribute(common_attr1.empty() ? "test" : common_attr1.c_str(), "");
                // If nodes were found, perform additional operations on them
                if (found_by_name_attr) {
                    // Test attribute reference variant on found node
                    if (test_attribute_ref_variant) {
                        pugi::xml_attribute attr_ref;
                        bool has_attr = found_by_name_attr.attribute(target_attribute_name.c_str(), attr_ref);
                    }
                }
                
                if (found_by_attr) {
                    // Test traversal on found node
                    if (test_traverse) {
                        AdvancedTreeWalker walker(false, true);
                        found_by_attr.traverse(walker);
                    }
                }
            }
            
            // SCENARIO 5: Test insert_child_after/before with string_view - SECONDARY TARGET
            if (test_insert_operations && !new_child_name.empty()) {
                // Find a reference child to insert before/after
                pugi::xml_node reference_child;
                for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                    if (child.type() == pugi::node_element) {
                        reference_child = child;
                        break;
                    }
                }
                
                if (reference_child) {
                    #ifdef PUGIXML_HAS_STRING_VIEW
                    // Test with string_view parameters if supported
                    std::string_view child_name_sv(new_child_name);
                    
                    // Insert before reference child
                    pugi::xml_node inserted_before = root.insert_child_before(child_name_sv, reference_child);
                    
                    // Insert after reference child  
                    pugi::xml_node inserted_after = root.insert_child_after(child_name_sv, reference_child);
                    
                    // Test append_child with string_view
                    pugi::xml_node appended = root.append_child(child_name_sv);
                    
                    // Add attributes to inserted nodes using string_view
                    if (inserted_before && !new_attribute_name.empty()) {
                        std::string_view attr_name_sv(new_attribute_name);
                        std::string_view attr_value_sv(new_attribute_value);
                        pugi::xml_attribute attr = inserted_before.append_attribute(attr_name_sv);
                        attr.set_value(attr_value_sv);
                    }
                    #else
                    // Fallback to const char* if string_view not available
                    pugi::xml_node inserted_before = root.insert_child_before(new_child_name.c_str(), reference_child);
                    pugi::xml_node inserted_after = root.insert_child_after(new_child_name.c_str(), reference_child);
                    pugi::xml_node appended = root.append_child(new_child_name.c_str());
                    
                    if (inserted_before && !new_attribute_name.empty()) {
                        pugi::xml_attribute attr = inserted_before.append_attribute(new_attribute_name.c_str());
                        attr.set_value(new_attribute_value.c_str());
                    }
                    #endif
                    
                    // Test traversal on modified tree
                    if (test_traverse) {
                        StatisticsTreeWalker walker;
                        root.traverse(walker);
                    }
                }
            }
            
            // SCENARIO 6: Test append_child with string_view - SECONDARY TARGET
            if (test_append_with_string_view && !new_child_name.empty()) {
                #ifdef PUGIXML_HAS_STRING_VIEW
                std::string_view child_name_sv(new_child_name);
                pugi::xml_node appended = root.append_child(child_name_sv);
                
                // Also test with different node types
                pugi::xml_node comment_node = root.append_child(pugi::node_comment);
                if (comment_node) {
                    comment_node.set_value(comment_text.empty() ? "Fuzzer-generated comment" : comment_text.c_str());
                }
                
                pugi::xml_node pi_node = root.append_child(pugi::node_pi);
                if (pi_node) {
                    pi_node.set_name(pi_name.empty() ? "fuzzer" : pi_name.c_str());
                    pi_node.set_value(pi_value.empty() ? "processing instruction" : pi_value.c_str());
                }
                #else
                pugi::xml_node appended = root.append_child(new_child_name.c_str());
                
                pugi::xml_node comment_node = root.append_child(pugi::node_comment);
                if (comment_node) {
                    comment_node.set_value(comment_text.empty() ? "Fuzzer-generated comment" : comment_text.c_str());
                }
                
                pugi::xml_node pi_node = root.append_child(pugi::node_pi);
                if (pi_node) {
                    pi_node.set_name(pi_name.empty() ? "fuzzer" : pi_name.c_str());
                    pi_node.set_value(pi_value.empty() ? "processing instruction" : pi_value.c_str());
                }
                #endif
            }
            
            // SCENARIO 7: Combined operations - test interaction between APIs
            if (test_scenario == 7) {
                // Create a complex test scenario combining multiple APIs
                
                // 1. First traverse the tree to understand its structure
                AdvancedTreeWalker structure_walker(false, true);
                root.traverse(structure_walker);
                
                // 2. Use find_child_by_attribute to locate specific nodes
                if (!target_attribute_name.empty() && !target_attribute_value.empty()) {
                    pugi::xml_node found = root.find_child_by_attribute(
                        target_attribute_name.c_str(), target_attribute_value.c_str());
                    
                    if (found) {
                        // 3. Test attribute reference variant on found node
                        pugi::xml_attribute attr_ref;
                        bool has_attr = found.attribute(target_attribute_name.c_str(), attr_ref);
                        
                        try {
                            pugi::xpath_query query(simple_xpath_query.empty() ? "child::*" : simple_xpath_query.c_str());
                            pugi::xpath_node selected = found.select_node(query);
                            
                            if (selected.node()) {
                                // 5. Insert new child relative to selected node
                                #ifdef PUGIXML_HAS_STRING_VIEW
                                std::string_view new_name_sv(inserted_child_name.empty() ? "inserted_child" : inserted_child_name.c_str());
                                pugi::xml_node inserted = found.insert_child_before(new_name_sv, selected.node());
                                #else
                                pugi::xml_node inserted = found.insert_child_before(inserted_child_name.empty() ? "inserted_child" : inserted_child_name.c_str(), selected.node());
                                #endif
                            }
                        } catch (...) {
                            // XPath parsing can fail
                        }
                }
            }
        }
        
        // Test attribute reference variant on document itself
        if (test_attribute_ref_variant) {
            pugi::xml_attribute doc_attr_ref;
            bool doc_has_attr = doc.attribute(common_attr4.empty() ? "version" : common_attr4.c_str(), doc_attr_ref);
        }
    }
    
    // Always clean up
    doc.reset();
    
    return 0;
}

} // namespace harness_007
