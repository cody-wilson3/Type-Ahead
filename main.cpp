#include "WordTree.hpp"
#include "rlutil.h"

int main()

{

    // Clear the console and initialize variables
    rlutil::cls();
    std::shared_ptr<WordTree> wordTree = readDictionary("dictionary.txt");
    std::string partialWord;
    std::string sentence;
    int count = 0;

    while (true)
    {
        // Get user input, update partialWord and sentence with new key
        int key = rlutil::getkey();

        if (key == rlutil::KEY_ESCAPE || (key == rlutil::KEY_BACKSPACE && sentence == ""))
        {
            break;
        }

        // if space key
        else if (key == rlutil::KEY_SPACE)
        {
            count++;
            sentence = sentence + " ";
            partialWord.clear();
            rlutil::cls();
            rlutil::locate(sentence.length(), 0);
            std::cout << sentence << std::endl;
            rlutil::locate(3, 3);
            std::cout << "--- prediction ---";
            rlutil::locate(sentence.length() + 1, 1);
        }

        // if backspace
        else if (key == rlutil::KEY_BACKSPACE)
        {
            // if we're backspacing through a space
            if (!partialWord.empty())
            {
                partialWord.pop_back();
                sentence.pop_back();
                rlutil::cls();
                std::vector<std::string> predictions = wordTree->predict(sentence.substr(sentence.find_last_of(" ") + 1), rlutil::trows() - 4);
                rlutil::locate(0, 0);
                rlutil::locate(3, 3);
                std::cout << "--- prediction ---";
                for (int i = 0; i < predictions.size(); i++)
                {
                    rlutil::locate(2, 4 + i);
                    std::cout << predictions[i];
                }
                rlutil::locate(1, 1);
                std::cout << sentence;
                rlutil::locate(sentence.length() + 1, 1);
            }
            else
            {
                sentence.pop_back();
                rlutil::cls();
                rlutil::locate(3, 3);
                std::cout << "--- prediction ---";
                rlutil::locate(1, 1);
                if (!sentence.ends_with(" "))
                {
                    partialWord = sentence.substr(sentence.find_last_of(" ") + 1);
                }
                std::cout << sentence << std::endl;
                rlutil::locate(sentence.length() + 1, 1);
            }
        }
        else
        {
            char ch = static_cast<char>(key);
            partialWord.push_back(ch); // Add the character to the sentence
            sentence.push_back(ch);
            rlutil::cls();
            std::vector<std::string> predictions = wordTree->predict(sentence.substr(sentence.find_last_of(" ") + 1), rlutil::trows() - 4);
            // Display predicted words
            rlutil::locate(3, 3);
            std::cout << "--- prediction ---";
            for (int i = 0; i < predictions.size(); i++)
            {
                rlutil::locate(2, 4 + i);
                std::cout << predictions[i];
            }
            rlutil::locate(1, 1);
            std::cout << sentence << std::endl;
            rlutil::locate(sentence.length() + 1, 1);
        }
    }
}
