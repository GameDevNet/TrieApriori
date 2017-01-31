#include "AprioriTrie.h"

//////////////////////////////////////////////////////////////////////////
//                      APRIORI TRIE FUNCTIONS                          //
//////////////////////////////////////////////////////////////////////////

AprioriTrie::AprioriTrie()
{
    mRootNode = new AprioriTrieNode();
}

AprioriTrie::~AprioriTrie()
{
    delete mRootNode;
}

/*     Pre:  Transaction.
*     Post:  Transaction added.
*  Purpose:  Adds tranaction.
*************************************************************************/
void AprioriTrie::addTransaction( const LinkedList<ItemID>& transaction )
{
    mRootNode->addTransaction( transaction, 0, 1 );
}

/*     Pre:  Itemset.
*     Post:  Support returned.
*  Purpose:  Returns support for given itemset.
*************************************************************************/
int AprioriTrie::getItemsetSupport( LinkedList<ItemID> itemIDs )
{
    return mRootNode->getItemsetSupport( itemIDs );
}

/*     Pre:  
*     Post:  Frequent itemsets reported.
*  Purpose:  Reports frequent itemsets.
*************************************************************************/
void AprioriTrie::reportIfFrequent( float supportThresholdPercent, int minItemsetSize, int maxItemsetSize )
{
    LinkedList<ItemID> itemset;
    int supportThreshold = mRootNode->getSupport() * supportThresholdPercent;
    mRootNode->reportIfFrequent( supportThreshold, minItemsetSize, maxItemsetSize, itemset );
}

/*     Pre:  
*     Post:  All itemsets reported.
*  Purpose:  Reports all itemsets.
*************************************************************************/
void AprioriTrie::reportAll()
{
    reportIfFrequent( 0.0, 1, INT_MAX );
}

//////////////////////////////////////////////////////////////////////////
//                     APRIORI TRIE NODE FUNCTIONS                      //
//////////////////////////////////////////////////////////////////////////

//should only be used for the root.
AprioriTrie::AprioriTrieNode::AprioriTrieNode()
{
    mEdges = AprioriTrieLinkedList();
    mKey = -1;
    mSupport = 0;
}

AprioriTrie::AprioriTrieNode::AprioriTrieNode( ItemID key )
{
    mEdges = AprioriTrieLinkedList();
    mKey = key;
    mSupport = 0;
}

AprioriTrie::AprioriTrieNode::~AprioriTrieNode()
{
    int edgeCount = mEdges.getCount();

    for( int i = 0; i < edgeCount; ++i )
    {
        delete mEdges[0];
        mEdges.removeAt( 0 );
    }
}

/*     Pre:  
*     Post:  Transaction added.
*  Purpose:  Recursive. Adds transaction
*************************************************************************/
void AprioriTrie::AprioriTrieNode::addTransaction( const LinkedList<ItemID>& transaction,
                                                   short currProgressInTransaction, short currDepth )
{
    const int TRANSACTION_LENGTH = transaction.getCount();
    
    if( TRANSACTION_LENGTH == 0 )
    {
        return;
    }

    ++mSupport;

    //ignores itemsets that are too big.
    if( currDepth > MAX_FREQUENT_ITEMSET_SIZE )
    {
        return;
    }

    for( short i = currProgressInTransaction; i < TRANSACTION_LENGTH; ++i )
    {
        AprioriTrieNode* foundEdge = mEdges.findByKey( transaction[i] );

        if( foundEdge == nullptr )
        {
            foundEdge = new AprioriTrieNode( transaction[i] );
            mEdges.insert( foundEdge );
        }

        foundEdge->addTransaction( transaction, i + 1, currDepth + 1 );
    }
}

/*     Pre:  Itemset
*     Post:  Support returned.
*  Purpose:  Returns support for a given itemset.
*************************************************************************/
int AprioriTrie::AprioriTrieNode::getItemsetSupport( LinkedList<ItemID> itemIDs )
{
    if( itemIDs.getCount() == 0 )
    {
        return -1;
    }

    if( itemIDs.getCount() == 1 && itemIDs[0] == mKey )
    {
        return mSupport;
    }

    itemIDs.removeAt( 0 );
    
    int edgeCount = mEdges.getCount();

    for( int i = 0; i < edgeCount; ++i )
    {
        if( itemIDs[0] == mEdges[i]->mKey )
        {
            return mEdges[i]->getItemsetSupport( itemIDs );
        }
    }

    return -1;
}

/*     Pre:  itemsSoFar should be empty for initial call.
*     Post:  Frequent itemsets reported.
*  Purpose:  Recursive. Reports frequent itemsets.
*************************************************************************/
void AprioriTrie::AprioriTrieNode::reportIfFrequent( unsigned short supportThreshold, int minItemsetSize, int maxItemsetSize, LinkedList<ItemID> itemsSoFar )
{
    if( mKey != -1 ) //only the root would have a -1 key.
    {
        itemsSoFar.insert( mKey );
    }

    if( mSupport < supportThreshold )
    {
        return;
    }

    int itemsetSizeSoFar = itemsSoFar.getCount();

    if( itemsetSizeSoFar >= minItemsetSize )
    {
        std::stringstream ss;

        ss << "Support Value: " << mSupport << " Items: ";

        for( int i = 0; i < itemsetSizeSoFar; ++i )
        {
            ss << itemsSoFar[i] << ' ';
        }

        ss << "\n";
        std::cout << ss.str();
        ++gOutputCount;
    }

    if( itemsetSizeSoFar + 1 <= maxItemsetSize )
    {
        int edgeCount = mEdges.getCount();

        for( int i = 0; i < edgeCount; ++i )
        {
            mEdges[i]->reportIfFrequent( supportThreshold, minItemsetSize, maxItemsetSize, itemsSoFar );
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//                APRIORI TRIE LINKED LIST FUNCTIONS                    //
//////////////////////////////////////////////////////////////////////////

/*     Pre:  data.
*     Post:  Data inserted.
*  Purpose:  Insert data into ATLL. Sorted by data contained in pointer, not pointer itself.
*************************************************************************/
bool AprioriTrie::AprioriTrieLinkedList::insert( AprioriTrieNode* data )
{
    Node<AprioriTrieNode*>* nodeToInsert = new Node<AprioriTrieNode*>( data );

    if( mCount == 0 )
    {
        mpHead = nodeToInsert;
        mpTail = nodeToInsert;
        ++mCount;

        return true;
    }

    if( isExist( data ) )
    {
        delete nodeToInsert;
        return false;
    }

    if( data->getKey() < mpHead->mData->getKey() )
    {
        nodeToInsert->mpNext = mpHead;
        mpHead = nodeToInsert;
        ++mCount;

        return true;
    }

    if( data->getKey() > mpTail->mData->getKey() )
    {
        mpTail->mpNext = nodeToInsert;
        mpTail = nodeToInsert;
        ++mCount;

        return true;
    }

    Node<AprioriTrieNode*>* currNode = mpHead;

    for( int i = 0; i < mCount - 1; ++i )
    {
        if( data->getKey() > currNode->mData->getKey() && data->getKey() < currNode->mpNext->mData->getKey() )
        {
            nodeToInsert->mpNext = currNode->mpNext;
            currNode->mpNext = nodeToInsert;
            ++mCount;

            return true;
        }

        currNode->mData;
        currNode = currNode->mpNext;
    }

    delete nodeToInsert;
    return false;
}

/*     Pre:  ItemID key
*     Post:  Node returned.
*  Purpose:  Finds and returns node by ItemID.
*************************************************************************/
AprioriTrie::AprioriTrieNode* AprioriTrie::AprioriTrieLinkedList::findByKey( ItemID key )
{
    if( mpHead == nullptr )
    {
        return nullptr;
    }

    Node<AprioriTrieNode*>* node = mpHead;

    while( node->mpNext != nullptr && node->mData->getKey() < key )
    {
        node = node->mpNext;
    }

    if( node != nullptr && node->mData != nullptr && node->mData->getKey() == key )
    {
        return node->mData;
    }

    return nullptr;
}