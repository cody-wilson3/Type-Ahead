#include "WordTree.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

void WordTree::add(std::string word)
{
    if (!word.empty() && std::all_of(word.begin(), word.end(), [](unsigned char c)
                                     {
                                         return std::isalpha(c);
                                     }))
    {

        // changing each character in current word/word partial into lowercase
        std::transform(word.begin(), word.end(), word.begin(), [](char c)
                       {
                           return static_cast<char>(std::tolower(c));
                       });

        // starting at root node
        std::shared_ptr<TreeNode>
            current = rootNode;

        for (char letter : word)
        {
            // if letter is b then index is 1, c then 2 etc
            int index = letter - 'a';

            // same as (*current).children[index]
            if (current->children[index] == nullptr)
            {
                current->children[index] = std::make_shared<TreeNode>();
            }
            current = current->children[index];
        }

        current->endOfWord = true;
    }
    // printTreeHelper(rootNode, 0, ' ');
}

bool WordTree::find(std::string word)
{
    // Ensuring word is alphabetic
    if (!word.empty() && std::all_of(word.begin(), word.end(), [](unsigned char c)
                                     {
                                         return std::isalpha(c);
                                     }))
    {

        // Changing word to lowercase
        std::transform(word.begin(), word.end(), word.begin(), [](char c)
                       {
                           return static_cast<char>(std::tolower(c));
                       });

        std::shared_ptr<TreeNode> current = rootNode;

        for (char letter : word)
        {

            // if letter is b then index is 1, c then 2 etc
            int index = letter - 'a';

            if (current->children[index] == nullptr)
            {
                return false;
            }

            current = current->children[index];
        }

        return current->endOfWord;
    }
    else
    {
        return false;
    }
}

std::vector<std::string> WordTree::predict(std::string partial, std::uint8_t howMany)
{
    std::vector<std::string> predictions;

    // Ensuring word is alphabetic
    if (!partial.empty() && std::all_of(partial.begin(), partial.end(), [](unsigned char c)
                                        {
                                            return std::isalpha(c);
                                        }))
    {

        // Changing word to lowercase
        std::transform(partial.begin(), partial.end(), partial.begin(), [](char c)
                       {
                           return static_cast<char>(std::tolower(c));
                       });

        std::shared_ptr<TreeNode> current = rootNode;

        for (char letter : partial)
        {
            int index = letter - 'a';

            if (current->children[index] == nullptr)
            {
                // If the path for the partial input is not found, return an empty prediction
                return predictions;
            }

            current = current->children[index];
        }

        // Perform a breadth-first search to predict words
        std::vector<std::string> predictionsList = breadthFirstSearch(current, predictions, howMany, partial);
        return predictionsList;
    }
    else
    {
        return predictions;
    }
}

// Public method to get the size of the tree
std::size_t WordTree::size()
{
    std::size_t count = 0;
    std::queue<std::shared_ptr<TreeNode>> nodeQueue;
    nodeQueue.push(rootNode);

    while (!nodeQueue.empty())
    {
        std::shared_ptr<TreeNode> currentNode = nodeQueue.front();
        nodeQueue.pop();

        if (currentNode != nullptr)
        {
            if (currentNode->endOfWord)
            {
                ++count;
            }

            for (int i = 0; i < 26; i++)
            {
                if (currentNode->children[i] != nullptr)
                {
                    nodeQueue.push(currentNode->children[i]);
                }
            }
        }
    }

    return count;
}

// Helper function for breadth-first search
std::vector<std::string> WordTree::breadthFirstSearch(std::shared_ptr<TreeNode> startNode, std::vector<std::string> predictions, std::uint8_t howMany, std::string partial)
{
    if (!startNode)
    {
        return predictions;
    }

    std::queue<std::pair<std::shared_ptr<TreeNode>, std::string>> nodeQueue;
    nodeQueue.push({ startNode, partial });

    while (!(nodeQueue.empty()) && predictions.size() < howMany)
    {
        std::pair<std::shared_ptr<TreeNode>, std::string> currentPair = nodeQueue.front();
        nodeQueue.pop();

        std::shared_ptr<TreeNode> currentNode = currentPair.first;
        std::string currentWord = currentPair.second;

        // Check if the current node marks the end of a word
        if (currentNode->endOfWord && currentWord != partial)
        {
            predictions.push_back(currentWord);
        }

        // Enqueue children for further exploration
        for (char letter = 'a'; letter <= 'z'; letter++)
        {
            int index = letter - 'a';
            if (currentNode->children[index] != nullptr)
            {
                nodeQueue.push({ currentNode->children[index], currentWord + (static_cast<char>(index + 'a')) });
            }
        }
    }
    return predictions;
}

std::shared_ptr<WordTree> readDictionary(std::string filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream inFile = std::ifstream(filename, std::ios::in);

    // Put this in myself ---------------------------------
    /*   if (!inFile.is_open())
       {
           std::cerr << "Error opening file: " << filename << std::endl;
           return wordTree;
       }*/
    // end -------------------------------------------

    while (!inFile.eof())
    {
        std::string word;
        std::getline(inFile, word);

        // Need to consume the carriage return character for some systems, if it exists

        if (!word.empty() && word[word.size() - 1] == '\r')
        {
            word.erase(word.end() - 1);
        }
        // Keep only if everything is an alphabetic character -- Have to send isalpha an unsigned char or
        // it will throw exception on negative values; e.g., characters with accent marks.
        if (std::all_of(word.begin(), word.end(), [](unsigned char c)
                        {
                            return std::isalpha(c);
                        }))
        {
            std::transform(word.begin(), word.end(), word.begin(), [](char c)
                           {
                               return static_cast<char>(std::tolower(c));
                           });
            wordTree->add(word);
        }
        // wordTree->printTree();
    }
    return wordTree;
}

void WordTree::printTree()
{
    std::cout << " ------------------ \n"
              << std::endl;
    printTreeHelper(rootNode, 0, ' ');
}

void WordTree::printTreeHelper(std::shared_ptr<TreeNode> node, int depth, char Label)
{
    if (node != nullptr)
    {
        // Print current node
        std::cout << std::setw(depth * 4) << Label << " ";
        if (node->endOfWord)
        {
            std::cout << "(*)";
        }
        std::cout << std::endl;

        // Print children
        for (int i = 0; i < 26; ++i)
        {
            if (node->children[i] != nullptr)
            {
                printTreeHelper(node->children[i], depth + 1, static_cast<char>(i + 'a'));
            }
        }
    }
}
