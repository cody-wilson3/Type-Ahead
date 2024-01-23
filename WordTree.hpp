
#pragma once

#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

class TreeNode
{
  public:
    bool endOfWord;
    std::array<std::shared_ptr<TreeNode>, 26> children;

    TreeNode() :
        endOfWord(false)
    {
        // Initialize the array with nullptr
        children.fill(nullptr);
    }
};

class WordTree
{
  private:
    std::shared_ptr<TreeNode> rootNode;

  public:
    WordTree() :
        rootNode(std::make_shared<TreeNode>()) {}

    void add(std::string word);

    bool find(std::string word);

    std::size_t size();

    std::vector<std::string> predict(std::string partial, std::uint8_t howMany);

    std::vector<std::string> breadthFirstSearch(std::shared_ptr<TreeNode> startNode, std::vector<std::string> predictions, std::uint8_t howMany, std::string partial);

    void printTree();

    void printTreeHelper(std::shared_ptr<TreeNode> node, int depth, char Label);
};

std::shared_ptr<WordTree> readDictionary(std::string filename);
