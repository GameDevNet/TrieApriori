#pragma once

#ifndef APRIORI_TRIE_H_
#define APRIORI_TRIE_H_

#include "linkedList.h"

#include <sstream>
#include <fstream>
#include <chrono>
#include <iomanip>

const char* const FILE_OUT_NAME = "output.txt";
const int MAX_FREQUENT_ITEMSET_SIZE = 3;

typedef short ItemID;

class AprioriTrie
{
    private:
    class AprioriTrieNode; //forward declaration for AprioriTrieLinkedList.

    class AprioriTrieLinkedList : public LinkedList<AprioriTrieNode*>
    {
        public:
        bool insert( AprioriTrieNode* data );
        AprioriTrieNode* findByKey( ItemID key );
    };

    class AprioriTrieNode
    {
        private:
        AprioriTrieLinkedList mEdges;
        ItemID mKey;
        unsigned short mSupport;

        public:
        AprioriTrieNode();
        AprioriTrieNode( ItemID key );
        ~AprioriTrieNode();
        void addTransaction( const LinkedList<ItemID>& transaction, short currProgressInTransaction, short currDepth );
        int  getItemsetSupport( LinkedList<ItemID> itemIDs );
        inline ItemID getKey() { return mKey; }
        inline unsigned short getSupport() { return mSupport; }
        void reportIfFrequent( unsigned short supportThreshold, int minItemsetSize, int maxItemsetSize, LinkedList<ItemID> itemsSoFar );
    };

    AprioriTrieNode* mRootNode;

    public:

    AprioriTrie();
    ~AprioriTrie();

    void addTransaction( const LinkedList<ItemID>& transaction );
    int  getItemsetSupport( LinkedList<ItemID> itemIDs );
    void reportIfFrequent( float supportThresholdPercent, int minItemsetSize, int maxItemsetSize );
    void reportAll();
};

extern unsigned int gOutputCount;
#endif