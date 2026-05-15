#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>

namespace harness_004 {


// Custom xml_tree_walker implementation that uses fuzzed data to decide traversal behavior
class FuzzingTreeWalker : public pugi::xml_tree_walker {
private:
    FuzzedDataProvider* fdp;
    int max_depth;
    int node_count;
    int element_count;
    int attribute_count;
    int text_count;
    bool should_modify_nodes;
    bool should_collect_stats;
    
public:
    FuzzingTreeWalker(FuzzedDataProvider* provider, bool modify, bool collect) 
        : fdp(provider), max_depth(0), node_count(0), element_count(0), 
          attribute_count(0), text_count(0), should_modify_nodes(modify),
          should_collect_stats(collect) {
    }
    
    // Override begin() method - called when traversal begins
    virtual bool begin(pugi::xml_node& node) override {
        if (should_collect_stats) {
            // Reset statistics
            max_depth = 0;
            node_count = 0;
            element_count = 0;
            attribute_count = 0;
            text_count = 0;
        }
        
        // Return true to continue traversal, false to stop
        return fdp->ConsumeBool();
    }
    
    // Override for_each() method - called for each node during traversal
    virtual bool for_each(pugi::xml_node& node) override {
        if (should_collect_stats) {
            node_count++;
            
            // Update max depth
            int current_depth = depth();
            if (current_depth > max_depth) {
                max_depth = current_depth;
            }
            
            // Count node types
            pugi::xml_node_type type = node.type();
            switch (type) {
                case pugi::node_element:
                    element_count++;
                    
                    // Count attributes on element nodes
                    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
                        attribute_count++;
                    }
                    break;
                    
                case pugi::node_pcdata:
                case pugi::node_cdata:
                    text_count++;
                    break;
                    
                default:
                    // Other node types (comment, pi, declaration, etc.)
                    break;
            }
        }
        
        if (should_modify_nodes && node.type() == pugi::node_element) {
            // Occasionally modify nodes during traversal based on fuzzed input
            if (fdp->ConsumeBool() && fdp->remaining_bytes() > 10) {
                // Add a fuzzed attribute
                std::string attr_name = "fuzzed_" + std::to_string(node_count);
                std::string attr_value = fdp->ConsumeRandomLengthString(50);
                node.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
            }
            
            // Occasionally modify text content
            if (fdp->ConsumeBool() && fdp->remaining_bytes() > 10 && node.first_child().type() == pugi::node_pcdata) {
                std::string new_text = fdp->ConsumeRandomLengthString(100);
                node.text().set(new_text.c_str());
            }
        }
        
        // Return true to continue traversal to next node, false to stop
        return fdp->ConsumeBool();
    }
    
    // Override end() method - called when traversal ends
    virtual bool end(pugi::xml_node& node) override {
        // Use the statistics if collected
        if (should_collect_stats) {
            // Statistics are now available in member variables
            (void)max_depth; (void)node_count; (void)element_count;
            (void)attribute_count; (void)text_count;
        }
        
        // Return value doesn't matter for end() but follow API convention
        return true;
    }
    
    // Getter methods for statistics (if collected)
    int getMaxDepth() const { return max_depth; }
    int getNodeCount() const { return node_count; }
    int getElementCount() const { return element_count; }
    int getAttributeCount() const { return attribute_count; }
    int getTextCount() const { return text_count; }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) return 0;  // Minimum size for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Parse XML input using xml_document::load_buffer
    // Split input: use first part for XML content
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(0, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, create a simple XML structure for traversal testing
    if (!parse_result) {
        doc.reset();
        
        // Create root element
        pugi::xml_node root = doc.append_child(pugi::node_element);
        root.set_name("root");
        
        // Add some structure for traversal
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 5; i++) {
            std::string child_name = "child" + std::to_string(i);
            pugi::xml_node child = root.append_child(child_name.c_str());
            
            // Add some attributes
            for (int j = 0; j < 3 && fdp.remaining_bytes() > 5; j++) {
                std::string attr_name = "attr" + std::to_string(j);
                std::string attr_value = "value" + std::to_string(i * 10 + j);
                child.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
            }
            
            // Add text content
            std::string text_content = "Text content " + std::to_string(i);
            child.append_child(pugi::node_pcdata).set_value(text_content.c_str());
            
            // Add nested children for deeper traversal
            if (i % 2 == 0 && fdp.remaining_bytes() > 5) {
                std::string nested_name = "nested" + std::to_string(i);
                pugi::xml_node nested = child.append_child(nested_name.c_str());
                nested.append_child(pugi::node_pcdata).set_value("Nested text");
            }
        }
    }
    
    // Step 2: Get root node using xml_document::document_element()
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        root = doc.first_child();
        if (root.empty()) {
            // No nodes to traverse
            doc.reset();
            return 0;
        }
    }
    
    // Step 3: Create custom xml_tree_walker derived class instance
    // Use fuzzed data to decide walker behavior
    bool should_modify_nodes = fdp.ConsumeBool();
    bool should_collect_stats = fdp.ConsumeBool();
    
    FuzzingTreeWalker walker(&fdp, should_modify_nodes, should_collect_stats);
    
    // Step 4: Call xml_node::traverse(custom_walker) on the root node
    bool traversal_result = root.traverse(walker);
    (void)traversal_result;  // Use result to avoid unused variable warning
    
    // Step 5: The custom walker's begin(), for_each(), and end() methods 
    // were called during traversal automatically
    
    // Optional: Perform additional traversals with different walkers
    if (fdp.remaining_bytes() > 10) {
        // Create a second walker with different behavior
        bool second_modify = fdp.ConsumeBool();
        bool second_collect = fdp.ConsumeBool();
        FuzzingTreeWalker walker2(&fdp, second_modify, second_collect);
        
        // Traverse from a different starting point if available
        if (!root.first_child().empty()) {
            pugi::xml_node child = root.first_child();
            child.traverse(walker2);
        }
    }
    
    // Step 6: Test traversal on different node types
    // Try traversing from document node as well
    if (fdp.ConsumeBool()) {
        FuzzingTreeWalker doc_walker(&fdp, false, true);  // Don't modify, just collect
        doc.traverse(doc_walker);
    }
    
    // Step 7: Clean up using xml_document::reset()
    doc.reset();
    
    return 0;
}

} // namespace harness_004
