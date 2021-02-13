#include "InvertedIndex.hpp"

#include <fstream>

InvertedIndex::InvertedIndex() {

}

InvertedIndex::InvertedIndex(const InvertedIndex &obj) {

    this->invertedIndex = obj.invertedIndex; // uses operator= of BST
}

InvertedIndex::~InvertedIndex() {

}

BST<IIData> &InvertedIndex::getInvertedIndex() {

    return invertedIndex;
}

InvertedIndex &InvertedIndex::addDocument(const std::string &documentName) {

    std::ifstream file(documentName.c_str(), std::ifstream::in);

    std::string word;
    for (int position = 1; (file >> word); ++position) {
        BSTNode<IIData> *theNode = invertedIndex.find(IIData(word));

        /* TODO */
        if( theNode == NULL )
        {
          IIData tmp(word);
          tmp.addOccurrence(documentName, position);
          invertedIndex.insert(tmp);
        }
        else
        {
          theNode->data.addOccurrence(documentName, position);
        }
    }

    file.close();

    return *this;
}

InvertedIndex &InvertedIndex::removeDocument(const std::string &documentName) {
    /* TODO */
    std::ifstream file(documentName.c_str(), std::ifstream::in);

    std::string word;
    for (int position = 1; (file >> word); ++position) {
        BSTNode<IIData> *theNode = invertedIndex.find(IIData(word));

        if( theNode == NULL )
        {
          continue;
        }

        theNode->data.removeOccurrences(documentName);

        if( theNode->data.getOccurrences().size() == 0 )
        {
          invertedIndex.remove( theNode->data );
        }
    }

    file.close();

    return *this;
}

InvertedIndex &InvertedIndex::searchWord(const std::string &word) {

    BSTNode<IIData> *theNode = invertedIndex.find(IIData(word));

    if (theNode == NULL) {
        std::cout << "The word \"" << word << "\" does not exist in inverted index." << std::endl;
    } else {
        std::cout << theNode->data << std::endl;
    }

    return *this;
}

InvertedIndex &InvertedIndex::printInvertedIndex() {

    invertedIndex.printInorder();

    return *this;
}

InvertedIndex &InvertedIndex::operator=(const InvertedIndex &rhs) {

    this->invertedIndex = rhs.invertedIndex; // uses operator= of BST

    return *this;
}
