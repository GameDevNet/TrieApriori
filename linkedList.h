/*      Author:  Wei Kian Chen
*  Modified By:  Nedim "Net" Ibrahimkadic
*  Last Update:  September 10, 2014
*        Class:  CSI-281
*     Filename:  linkedList.h
*  Description:  Sorted singly linked list
**********************************************************************/
#pragma once

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <utility>

template <typename T>
class LinkedList
{
    protected:
    template <typename T>
    struct Node
    {
        T        mData;
        Node<T>* mpNext;

        //Default Constructor
        Node()
        {
            mData = T();
            mpNext = nullptr;
        }

        //Constructor
        Node( T data )
        {
            mData = data;
            mpNext = nullptr;
        }
    };

    Node<T> *mpHead,
            *mpTail;
    unsigned short int mCount;

    Node<T>* getNodeAtIndex( int index ) const;
    void swap( LinkedList& rhs );
    
    public:
    LinkedList();
    LinkedList( const LinkedList& rhs );
    ~LinkedList();

    int getCount() const;
    T    getData( int index ) const;
    void setData( int index, T data );

    void clear();
    void display() const;
    bool insert( T data );
    bool isEmpty() const;
    bool isExist( T searchKey ) const;
    bool remove( T searchKey );
    T    removeAt( int index );

    LinkedList& operator=( LinkedList rhs ); //Pass-by-value to optimize Copy-and-swap idiom.
    const T& operator[]( int index ) const;
    friend std::ostream& operator<<( std::ostream& os, const LinkedList<T>& linkedList );
};

//Default Constructor
template <typename T>
LinkedList<T>::LinkedList()
{
    mpHead = nullptr;
    mpTail = nullptr;
    mCount = 0;
}

//Copy Constructor
template <typename T>
LinkedList<T>::LinkedList( const LinkedList& rhs )
{
    mpHead = new Node<T>();
    mCount = rhs.mCount;

    Node<T> *pLhsNode = mpHead,
        *pRhsNode = rhs.mpHead;

    for( int i = 0; i < mCount; ++i )
    {
        pLhsNode->mData = pRhsNode->mData;

        if( pRhsNode != rhs.mpTail )
        {
            pLhsNode->mpNext = new Node<T>();
            pLhsNode = pLhsNode->mpNext;
            pRhsNode = pRhsNode->mpNext;
        }
    }

    mpTail = pLhsNode;
}

//Destructor
template <typename T>
LinkedList<T>::~LinkedList()
{
    // remove all nodes
    clear();

    // reinitialize the pointers
    mpHead = nullptr;
    mpTail = nullptr;

    // reinitialize count
    mCount = 0;
}

///Operator Overloads

//Copy Assignment
template <typename T>
LinkedList<T>& LinkedList<T>::operator=( LinkedList<T> rhs )
{
    swap( rhs );
    return *this;
}

//Const Subscript
template <typename T>
const T& LinkedList<T>::operator[]( int index ) const
{
    return getNodeAtIndex( index )->mData;
}

//Insertion
template <typename T>
std::ostream& operator<<( std::ostream& os, const LinkedList<T>& linkedList )
{
    LinkedList<T>::Node<T>* pCurrNode = linkedList.mpHead;

    for( int i = 0; i < linkedList.mCount; ++i )
    {
        os << pCurrNode->mData << " ";
        pCurrNode = pCurrNode->mpNext;
    }

    return os;
}

/*     Pre:  The object is instantiated
*     Post:  The number of nodes in the linked list is returned to the caller
*  Purpose:  To retrieve the number of nodes in the list
*****************************************************************************/
template <typename T>
int LinkedList<T>::getCount() const
{
    return mCount;
}

/*     Pre:  The list is instantiated and the index is valid
*     Post:  The data in the specified index is returned to the caller
*  Purpose:  To retrieve the specified nodes in the list
*****************************************************************************/
template <typename T>
T LinkedList<T>::getData( int index ) const
{
    Node<T>* node = getNodeAtIndex( index );

    if( node != nullptr )
    {
        return node->mData;
    }

    return nullptr;
}

/*     Pre:  The list is instantiated, the index is valid and the data is
*            available
*     Post:  The node in the specified index is returned.
*  Purpose:  Returns the node at the specified index. Returns null on out of range.
*****************************************************************************/
template <typename T>
typename LinkedList<T>::Node<T>* LinkedList<T>::getNodeAtIndex( int index ) const
{
    if( index >= mCount || index < 0 )
    {
        return nullptr;
    }

    Node<T>* pCurrNode = mpHead;

    for( int i = 0; i < index; ++i )
    {
        pCurrNode = pCurrNode->mpNext;
    }

    return pCurrNode;
}

/*     Pre:  The list is instantiated, the index is valid and the data is
*            available
*     Post:  The data in the specified index is updated with the specified
*            data
*  Purpose:  To update the specified nodes in the list
*****************************************************************************/
template <typename T>
void LinkedList<T>::setData( int index, T data )
{
    Node<T>* node = getNodeAtIndex( index );

    if( node != nullptr )
    {
        node->mData = data;
    }
}

/*     Pre:  The list is initiated
*     Post:  All the nodes in the list is deleted
*  Purpose:  To remove all the nodes in the list
*****************************************************************************/
template <typename T>
void LinkedList<T>::clear()
{
    Node<T> *toDelete,
        *pNextNode = mpHead;

    for( int i = 0; i < mCount; ++i )
    {
        toDelete = pNextNode;
        pNextNode = toDelete->mpNext;

        toDelete->mpNext = nullptr;
        delete toDelete;
    }

    mpHead = nullptr;
    mpTail = nullptr;
    mCount = 0;
}

/*     Pre:  The list is instantiated
*     Post:  The entire list is displayed on the screen
*  Purpose:  To show the content of the list
*****************************************************************************/
template <typename T>
void LinkedList<T>::display() const
{
    if( mCount == 0 )
    {
        std::cout << "Empty.";
    }
    else
    {
        std::cout << *( this );
    }
}

/*     Pre:  The list is instantiated and the data is available
*     Post:  The data is inserted in ascending order
*  Purpose:  To insert a data into the list in ascending order.  However, if
*            the data already existed in the list, it will not be added again
*****************************************************************************/
template <typename T>
bool LinkedList<T>::insert( T data )
{
    Node<T>* nodeToInsert = new Node<T>( data );

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

    if( data < mpHead->mData )
    {
        nodeToInsert->mpNext = mpHead;
        mpHead = nodeToInsert;
        ++mCount;

        return true;
    }

    if( data > mpTail->mData )
    {
        mpTail->mpNext = nodeToInsert;
        mpTail = nodeToInsert;
        ++mCount;

        return true;
    }

    Node<T>* currNode = mpHead;

    for( int i = 0; i < mCount - 1; ++i )
    {
        if( data > currNode->mData && data < currNode->mpNext->mData )
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

/*     Pre:  The list is instantiated
*     Post:  The function returns true is the list is empty; false otherwise
*  Purpose:  To determine if the list is empty
*****************************************************************************/
template <typename T>
bool LinkedList<T>::isEmpty() const
{
    return mCount == 0;
}

/*     Pre:  The list is instantiated and the searchKey is available
*     Post:  The function returns true if the search key exists in the list;
*            otherwise false
*  Purpose:  To determine if a specific value exists in the list or not
*****************************************************************************/
template <typename T>
bool LinkedList<T>::isExist( T searchKey ) const
{
    if( mCount == 0 )
    {
        return false;
    }

    Node<T>* currNode = mpHead;

    for( int i = 0; i < mCount; ++i )
    {
        if( currNode->mData == searchKey )
        {
            return true;
        }

        currNode = currNode->mpNext;
    }

    return false;
}

/*     Pre:  The list is instantiated and the searchKey is available
*     Post:  If the searchKey exists, removes it from the list and the
*            function returns true; otherwise the function does nothing
*            and returns false
*  Purpose:  To remove a specific value from the list
*****************************************************************************/
template <typename T>
bool LinkedList<T>::remove( T searchKey )
{
    if( mCount == 0 )
    {
        return false;
    }

    if( mCount == 1 )
    {
        delete mpHead;

        mpTail = nullptr;
        mpHead = nullptr;
        mCount = 0;

        return true;
    }

    if( mpHead->mData = searchKey )
    {
        Node<T>* mOldHead = mpHead;

        mpHead = mOldHead->mpNext;

        mOldHead->mpNext = nullptr;
        delete mOldHead;

        --mCount;

        return true;
    }

    Node<T>* currNode = mpHead;

    for( int i = 0; i < mCount - 1; ++i )
    {
        if( currNode->mpNext->mData == searchKey )
        {
            Node<T>* pNodeToDelete = currNode->mpNext;

            currNode->mpNext = pNodeToDelete->mpNext;

            pNodeToDelete->mpNext = nullptr;
            delete pNodeToDelete;

            --mCount;

            return true;
        }

        currNode = currNode->mpNext;
    }

    return false;
}

/*     Pre:  The list is instantiated and the index is valid
*     Post:  Remove the element in the specified index location and returns
*            its content to the caller.  If the index location is invalid, the
*            function returns the default value
*  Purpose:  To remove an item in the specified index location
*****************************************************************************/
template <typename T>
T LinkedList<T>::removeAt( int index )
{
    T data = T();

    if( index == 0 )
    {
        Node<T>* pNodeToDelete = mpHead;

        mpHead = mpHead->mpNext;
        data = pNodeToDelete->mData;

        pNodeToDelete->mpNext = nullptr;
        delete pNodeToDelete;

        --mCount;
    }
    else
    {
        Node<T>* prevNode = getNodeAtIndex( index - 1 );

        if( prevNode != nullptr && prevNode->mpNext != nullptr )
        {
            Node<T>* nodeToDelete = prevNode->mpNext;

            prevNode->mpNext = prevNode->mpNext->mpNext;

            data = nodeToDelete->mData;

            nodeToDelete->mpNext = nullptr;
            delete nodeToDelete;

            --mCount;

            if( index == mCount - 1 )
            {
                mpTail = prevNode;
            }
        }
    }

    return data;
}

/*     Pre:  Initialized linkedlist.
*     Post:  Swaps out the contents of this object and the right hand side one.
*  Purpose:  Used for the swap portion of the copy-and-swap idiom utilized in
*            the copy assignment operator.
*****************************************************************************/
template <typename T>
void LinkedList<T>::swap( LinkedList& rhs )
{
    std::swap( mpHead, rhs.mpHead );
    std::swap( mpTail, rhs.mpTail );
    std::swap( mCount, rhs.mCount );
}

#endif