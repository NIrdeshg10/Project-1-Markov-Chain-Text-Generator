
#include "markov.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

int main()
{
    srand(time(0));

    const int MAX_WORDS = 5000;

    string words[MAX_WORDS];
    string prefixes[MAX_WORDS];
    string suffixes[MAX_WORDS];

    string filename;
    int order;
    int numWords;

    cout << "Enter input filename: ";
    cin >> filename;

    cout << "Enter order (1, 2, or 3): ";

    while (!(cin >> order) || order < 1 || order > 3)
    {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid order. Enter 1, 2, or 3: ";
    }

    cout << "Enter maximum number of words to generate: ";

    while (!(cin >> numWords) || numWords < order)
    {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid number. Enter at least "
             << order << ": ";
    }

    int numWordsRead = readWordsFromFile(filename, words, MAX_WORDS);

    if (numWordsRead == -1)
    {
        cout << "Error: Could not open the file." << endl;
        return 1;
    }

    if (numWordsRead <= order)
    {
        cout << "Error: The file needs at least "
             << order + 1 << " words." << endl;
        return 1;
    }

    if (numWordsRead == MAX_WORDS)
    {
        cout << "The program used at most "
             << MAX_WORDS
             << " words. Additional words may have been ignored."
             << endl;
    }

    int chainSize = buildMarkovChain(words, numWordsRead, order,
                                     prefixes, suffixes, MAX_WORDS);

    if (chainSize <= 0)
    {
        cout << "Error: Could not build the Markov chain." << endl;
        return 1;
    }

    string output = generateText(prefixes, suffixes,
                                 chainSize, order, numWords);

    cout << endl;
    cout << output << endl;

    // Count the actual number of words generated.
    int actualCount = 0;
    string word;
    stringstream counter(output);

    while (counter >> word)
    {
        actualCount++;
    }

    cout << endl;
    cout << "Generated " << actualCount
         << " of at most " << numWords << " words.";

    if (actualCount < numWords)
    {
        cout << " Stopped early: no successor for the current prefix.";
    }

    cout << endl;

    return 0;
}