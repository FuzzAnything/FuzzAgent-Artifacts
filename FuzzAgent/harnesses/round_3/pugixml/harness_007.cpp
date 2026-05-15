#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <functional>

namespace harness_007 {

// Custom xml_tree_walker implementations for testing

// Walker 1: Basic traversal walker that always continues
class BasicWalker : public pugi::xml_tree_walker {
private:
    int max_depth;
    int nodes_visited;
    bool should_continue;
    
public:
    BasicWalker(int max_depth = 100) : max_depth(max_depth), nodes_visited(0), should_continue(true) {}
    
    virtual bool begin(pugi::xml_node& node) override {
        nodes_visited = 0;
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        nodes_visited++;
        
        // Access node properties to ensure coverage
        node.name();
        node.value();
        node.type();
        
        // Check attributes
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            attr.name();
            attr.value();
        }
        
        // Continue traversal unless we hit max depth
        return (depth() < max_depth);
    }
    
    virtual bool end(pugi::xml_node& node) override {
        return true;
    }
    
    int getNodesVisited() const { return nodes_visited; }
};

// Walker 2: Conditional walker that may stop early based on fuzzer input
class ConditionalWalker : public pugi::xml_tree_walker {
private:
    int stop_after_nodes;
    int nodes_visited;
    bool stop_at_depth;
    int stop_depth;
    
public:
    ConditionalWalker(int stop_after = 10, bool stop_at_depth = false, int stop_depth = 3) 
        : stop_after_nodes(stop_after), nodes_visited(0), stop_at_depth(stop_at_depth), stop_depth(stop_depth) {}
    
    virtual bool begin(pugi::xml_node& node) override {
        nodes_visited = 0;
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        nodes_visited++;
        
        // Access node children count
        node.first_child();
        node.last_child();
        
        // Check if we should stop based on conditions
        if (nodes_visited >= stop_after_nodes) {
            return false;
        }
        
        if (stop_at_depth && depth() >= stop_depth) {
            return false;
        }
        
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        return true;
    }
};

// Walker 3: Modifying walker that changes node names/values during traversal
class ModifyingWalker : public pugi::xml_tree_walker {
private:
    std::string prefix;
    bool modify_attributes;
    
public:
    ModifyingWalker(const std::string& prefix = "modified_", bool modify_attributes = true) 
        : prefix(prefix), modify_attributes(modify_attributes) {}
    
    virtual bool begin(pugi::xml_node& node) override {
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Modify node name if it's an element
        if (node.type() == pugi::node_element) {
            std::string new_name = prefix + (node.name() ? node.name() : "unnamed");
            node.set_name(new_name.c_str());
        }
        
        // Modify node value if it has one
        if (node.value() && strlen(node.value()) > 0) {
            std::string new_value = prefix + node.value();
            node.set_value(new_value.c_str());
        }
        
        // Modify attributes if enabled
        if (modify_attributes) {
            for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
                std::string new_attr_name = prefix + (attr.name() ? attr.name() : "attr");
                attr.set_name(new_attr_name.c_str());
                
                std::string new_attr_value = prefix + (attr.value() ? attr.value() : "value");
                attr.set_value(new_attr_value.c_str());
            }
        }
        
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        return true;
    }
};

// Walker 4: Stateful walker that tracks traversal path
class StatefulWalker : public pugi::xml_tree_walker {
private:
    std::vector<std::string> node_path;
    std::vector<int> depth_history;
    bool record_path;
    
public:
    StatefulWalker(bool record_path = true) : record_path(record_path) {}
    
    virtual bool begin(pugi::xml_node& node) override {
        node_path.clear();
        depth_history.clear();
        if (record_path && node.name()) {
            node_path.push_back(node.name());
        }
        depth_history.push_back(depth());
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        if (record_path && node.name()) {
            node_path.push_back(node.name());
        }
        depth_history.push_back(depth());
        
        // Track parent relationships
        pugi::xml_node parent = node.parent();
        if (parent) {
            parent.name();
        }
        
        // Track sibling relationships
        pugi::xml_node next_sib = node.next_sibling();
        pugi::xml_node prev_sib = node.previous_sibling();
        
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        if (record_path && node.name()) {
            node_path.push_back(node.name());
        }
        depth_history.push_back(depth());
        return true;
    }
    
    const std::vector<std::string>& getNodePath() const { return node_path; }
    const std::vector<int>& getDepthHistory() const { return depth_history; }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for XML structure and walker configuration
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create XML document with complex tree structure for traversal testing
    pugi::xml_document doc;
    
    // Consume tree structure parameters
    size_t max_depth = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    size_t max_children = fdp.ConsumeIntegralInRange<size_t>(1, 8);
    size_t max_attributes = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    
    // Helper function to recursively build tree
    std::function<void(pugi::xml_node&, size_t)> buildTree = 
        [&](pugi::xml_node& parent, size_t current_depth) {
            if (current_depth >= max_depth || fdp.remaining_bytes() < 20) {
                return;
            }
            
            size_t num_children = fdp.ConsumeIntegralInRange<size_t>(0, max_children);
            for (size_t i = 0; i < num_children && fdp.remaining_bytes() > 30; ++i) {
                std::string node_name = "node_" + std::to_string(current_depth) + "_" + std::to_string(i);
                pugi::xml_node child = parent.append_child(node_name.c_str());
                
                // Add attributes
                size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, max_attributes);
                for (size_t j = 0; j < attr_count && fdp.remaining_bytes() > 10; ++j) {
                    std::string attr_name = "attr_" + std::to_string(j);
                    std::string attr_value = fdp.ConsumeRandomLengthString(32);
                    child.append_attribute(attr_name.c_str()) = attr_value.c_str();
                }
                
                // Add text content
                if (fdp.ConsumeBool()) {
                    std::string text_content = fdp.ConsumeRandomLengthString(128);
                    child.append_child(pugi::node_pcdata).set_value(text_content.c_str());
                }
                
                // Add comment nodes
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
                    std::string comment_text = fdp.ConsumeRandomLengthString(100);
                    child.append_child(pugi::node_comment).set_value(comment_text.c_str());
                }
                
                // Recursively build subtree
                buildTree(child, current_depth + 1);
            }
        };
    
    // Create root node
    std::string root_name = fdp.ConsumeRandomLengthString(32);
    if (root_name.empty()) root_name = "root";
    pugi::xml_node root = doc.append_child(root_name.c_str());
    
    // Build the tree
    buildTree(root, 0);
    
    // Consume walker type selection
    uint8_t walker_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    bool traversal_result = false;
    
    // Test different walker types
    switch (walker_type) {
        case 0: {
            // BasicWalker
            int max_walker_depth = fdp.ConsumeIntegralInRange<int>(1, 20);
            BasicWalker walker(max_walker_depth);
            traversal_result = root.traverse(walker);
            break;
        }
        
        case 1: {
            // ConditionalWalker
            int stop_after = fdp.ConsumeIntegralInRange<int>(1, 50);
            bool stop_at_depth = fdp.ConsumeBool();
            int stop_depth = fdp.ConsumeIntegralInRange<int>(1, 10);
            ConditionalWalker walker(stop_after, stop_at_depth, stop_depth);
            traversal_result = root.traverse(walker);
            break;
        }
        
        case 2: {
            // ModifyingWalker
            std::string prefix = fdp.ConsumeRandomLengthString(16);
            bool modify_attrs = fdp.ConsumeBool();
            ModifyingWalker walker(prefix, modify_attrs);
            traversal_result = root.traverse(walker);
            
            // After modification, traverse again with basic walker to verify changes
            if (traversal_result) {
                BasicWalker verify_walker;
                root.traverse(verify_walker);
            }
            break;
        }
        
        case 3: {
            // StatefulWalker
            bool record_path = fdp.ConsumeBool();
            StatefulWalker walker(record_path);
            traversal_result = root.traverse(walker);
            
            // Access the recorded path and depth history
            const std::vector<std::string>& path = walker.getNodePath();
            const std::vector<int>& depths = walker.getDepthHistory();
            
            // Use the recorded data to ensure it's not optimized away
            if (!path.empty()) {
                size_t total_chars = 0;
                for (const auto& str : path) {
                    total_chars += str.length();
                }
            }
            break;
        }
    }
    
    // Test edge cases
    
    // 1. Traverse empty document
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        pugi::xml_document empty_doc;
        BasicWalker empty_walker;
        empty_doc.traverse(empty_walker);
    }
    
    // 2. Traverse with null node
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        pugi::xml_node null_node;
        BasicWalker null_walker;
        null_node.traverse(null_walker);
    }
    
    // 3. Test multiple consecutive traversals
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        BasicWalker walker1;
        ConditionalWalker walker2(5, true, 2);
        
        bool result1 = root.traverse(walker1);
        bool result2 = root.traverse(walker2);
        
        // Test that traversal doesn't interfere with document state
        pugi::xml_node test_node = root.first_child();
        if (test_node) {
            test_node.name();
            test_node.value();
        }
    }
    
    // 4. Test traversal on subtree (not entire document)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        pugi::xml_node subtree = root.first_child();
        if (subtree) {
            BasicWalker subtree_walker;
            subtree.traverse(subtree_walker);
        }
    }
    
    // 5. Test traversal interruption and resumption
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 30) {
        // Create a walker that stops early
        int stop_point = fdp.ConsumeIntegralInRange<int>(1, 10);
        ConditionalWalker interrupter(stop_point, false, 0);
        
        // First traversal that may stop early
        bool interrupted = root.traverse(interrupter);
        
        // Second traversal that should complete
        BasicWalker completer;
        bool completed = root.traverse(completer);
    }
    
    return 0;
}

} // namespace harness_007
