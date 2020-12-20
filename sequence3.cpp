//==================================================================
//sequence3.cpp - sequence class with linked lists implementation.
//Written by: Yussef Ibarra
//Email: yibarra000@citymail.cuny.edu
//==================================================================

//! \file sequence3.cpp
//! \brief sequence class with linked lists
//! \author Yussef Ibarra
//! \INVARIANT for the sequence class:
//  1. The number of items in the sequence is in the member varible m_many_nodes
//  2. The pointer m_head_ptr is a memeber variable pointing to the node at the beginning of the sequence. If the sequence is
//     empty, m_head_ptr is NULL.
//  3. The pointer m_cursor_ptr points to the node at the current item. If there is no current item, m_cursor_ptr is NULL.
//     m_precursor_ptr points to the node before the current item. m_precursor_ptr is NULL if there is no current item or
//     if the current item is the item in the sequence.
//  4. The pointer m_tail_ptr points to the last node in the sequence. If the sequence is NULL, m_tail_ptr is NULL.

#include <stdio.h>
#include <cstdlib>   // Provides size_t and NULL
#include <assert.h>
#include "sequence3.h"
#include "node1.h"   // Provides linked list toolkit

namespace main_savitch_5
{

//Postcondition: The sequence has been initialized as an empty sequence.
//Time analysis: O(1), constant time, same as sequece with dynamic array
sequence::sequence()
{
    m_head_ptr  = NULL;
    m_precursor_ptr = NULL;
    m_cursor_ptr = NULL;
    m_tail_ptr = NULL;
    m_many_nodes = 0;
}

//Postcondition: This is the copy constructor. Copies the sequence source to the initiated sequence.
//Time analysis: O(n), linear time, same as sequence with dynamic array
sequence::sequence(const sequence& source)
{
    m_many_nodes = source.m_many_nodes;
    //Copy sequence source to the initiated sequnce
    list_copy(source.m_head_ptr, m_head_ptr, m_tail_ptr);
    node* rand_cursor_ptr;
    m_precursor_ptr = m_head_ptr;
    //The code below will determine the pointers m_cursor_ptr ans m_precursor_ptr for the initiated sequence
    //Case where source does not have a current item.
    if(source.m_cursor_ptr==NULL){
        if(source.m_precursor_ptr==NULL){
            m_precursor_ptr = NULL;
            m_cursor_ptr = NULL;
        }else{
            m_precursor_ptr = m_tail_ptr;
            m_cursor_ptr = NULL;
        }
    }else{
        //Case where source does have a current item
        if(source.m_precursor_ptr==NULL){
            m_cursor_ptr = m_head_ptr;
            m_precursor_ptr = NULL;
        }else{
            //Hardest case: source has m_cursor_ptr ans m_precursor_ptr that are not NULL
            for(rand_cursor_ptr = source.m_head_ptr; rand_cursor_ptr != source.m_precursor_ptr;rand_cursor_ptr=rand_cursor_ptr->link()){
                m_precursor_ptr = m_precursor_ptr->link();
            }
            m_cursor_ptr = m_precursor_ptr->link();
        }
    }
}

//Postcodition: Deconstructor, releases memory when needed.
//Time analysis: O(n), linear time same as sequence with dynamic array
sequence::~sequence()
{
    list_clear(m_head_ptr);
}

//Postcondition: The first item in sequence becomes the current item
//(but if the sequence is empty, then there is no current item).
//Time analysis: O(1), constant time, same as sequence with dynamic array
void sequence::start()
{
    m_precursor_ptr = NULL;
    m_cursor_ptr = m_head_ptr;
}

//Precondition: is_item is true.
//Postcondition: If the current item was already the last item on the
//sequence, then there is no longer any current item. Otherwise, the new
//current item is the item immediately after the original current item.
//Time analysis: O(1), constant time, same as sequence with dynamic array
void sequence::advance()
{
    assert(is_item());
    m_precursor_ptr = m_cursor_ptr;
    m_cursor_ptr = m_cursor_ptr->link();
}

//Postcondition: A new copy of entry has been inserted in the sequence before
//the current item. If there was no current item, then the new entry has
//been inserted at the front of the sequence. In either case, the newly
//inserted item is now the current item of the sequence.
//Time analysis: O(1), constant time, more efficient than the sequence with a dynamic array, O(n)
void sequence::insert(const sequence::value_type& entry)
{
    //Case where there is a current item, checks if current item is the first item
    if(is_item()){
        if(m_precursor_ptr!=NULL){
            list_insert(m_precursor_ptr, entry);
            m_cursor_ptr = m_precursor_ptr->link();
        }else{
            list_head_insert(m_head_ptr, entry);
            m_precursor_ptr = NULL;
            m_cursor_ptr = m_head_ptr;
        }
    }else{
        //Case where there is not a current item, and/or the sequence is empty
        if(m_many_nodes==0){
            list_head_insert(m_head_ptr, entry);
            m_precursor_ptr = NULL;
            m_cursor_ptr = m_head_ptr;
            m_tail_ptr = m_head_ptr;
        }else{
            list_head_insert(m_head_ptr, entry);
            m_precursor_ptr = NULL;
            m_cursor_ptr = m_head_ptr;
        }
    }
    m_many_nodes++;
}

//Postcondition: A new copy of entry has been inserted in the sequence after
//the current item. If there was no current item, then the new entry has
//been attached to the end of the sequence. In either case, the newly
//inserted item is now the current item of the sequence.
//Time analysis: O(1), constant time, more efficient than the sequence with a dynamic array, O(n)
void sequence::attach(const sequence::value_type& entry)
{
    //Case where there is a current item, checks if current item is the last item
    if(is_item()){
        if(m_cursor_ptr->link()==NULL){
            list_insert(m_cursor_ptr,entry);
            m_precursor_ptr = m_cursor_ptr;
            m_cursor_ptr = m_cursor_ptr->link();
            m_tail_ptr = m_cursor_ptr;
        }else{
            list_insert(m_cursor_ptr, entry);
            m_precursor_ptr = m_cursor_ptr;
            m_cursor_ptr = m_cursor_ptr->link();
        }
    }else{
        //Case where there is not a current item, and/or the sequence is empty
        if(m_many_nodes==0){
                list_head_insert(m_head_ptr, entry);
                m_precursor_ptr = NULL;
                m_cursor_ptr = m_head_ptr;
                m_tail_ptr = m_head_ptr;
        }else{
            m_precursor_ptr = m_tail_ptr;
            list_insert(m_tail_ptr,entry);
            m_tail_ptr = m_tail_ptr->link();
            m_cursor_ptr = m_tail_ptr;
        }
    }
    m_many_nodes++;
}

//Postcondition: This is the overloaded assignment operator. Copies the contents of the sequence source to the sequence on
//the left of the = operator.
//Time analysis: O(n), linear time, same as sequence with dynamic array
void sequence::operator =(const sequence& source)
{
    //If both sequences are already the same, end function
    if(this == &source){
        return;
    }
    list_clear(m_head_ptr);
    m_many_nodes=0;
    m_many_nodes = source.m_many_nodes;
    //Copy sequence source to the initiated sequnce
    list_copy(source.m_head_ptr, m_head_ptr, m_tail_ptr);
    node* rand_cursor_ptr;
    m_precursor_ptr = m_head_ptr;
    //The code below will determine the pointers m_cursor_ptr ans m_precursor_ptr for the initiated sequence
    //Case where source does not have a current item.
    if(source.m_cursor_ptr==NULL){
        if(source.m_precursor_ptr==NULL){
            m_precursor_ptr = NULL;
            m_cursor_ptr = NULL;
        }else{
            m_precursor_ptr = m_tail_ptr;
            m_cursor_ptr = NULL;
        }
    }else{
        //Case where source does have a current item
        if(source.m_precursor_ptr==NULL){
            m_cursor_ptr = m_head_ptr;
            m_precursor_ptr = NULL;
        }else{
            //Hardest case: source has m_cursor_ptr ans m_precursor_ptr that are not NULL
           for(rand_cursor_ptr = source.m_head_ptr; rand_cursor_ptr != source.m_precursor_ptr;rand_cursor_ptr=rand_cursor_ptr->link()){
               m_precursor_ptr = m_precursor_ptr->link();
            }
            m_cursor_ptr = m_precursor_ptr->link();
        }
    }
}

//Precondition: is_item returns true.
//Postcondition: The current item has been removed from the sequence, and the
//item after this (if there is one) is now the new current item.
//Time analysis: O(1), constant time, more efficient than the sequence with a dynamic array, O(n)
void sequence::remove_current( )
{
    assert(is_item());
    --m_many_nodes;
    //Case where the current item is the first item in the sequence
    if(m_precursor_ptr==NULL){
        list_head_remove(m_head_ptr);
        m_cursor_ptr = m_head_ptr;
    }else{
        //Case where current item is after the first item in the sequence
        m_cursor_ptr=m_cursor_ptr->link();
        list_remove(m_precursor_ptr);
    }
}

//Postcondition: The return value is the number of items on the sequence.
//Time analysis: O(1), constant time, same as sequence with dynamic array
sequence::size_type sequence::size() const
{
    return m_many_nodes;
}
    
//Postcondition: A true return value indicates that there is a valid
//"current" item that may be retrieved by activating the current
//member function (listed below). A false return value indicates that
// there is no valid current item.
//Time analysis: O(1), constant time, same as sequence with dynamic array
bool sequence::is_item() const
{
    return (m_cursor_ptr!=NULL && m_many_nodes > 0);
}

//Precondition: is_item( ) returns true.
//Postcondition: The item returned is the current item on the sequence.
//Time analysis: O(1), constant time, same as sequence with dynamic array
sequence::value_type sequence::current() const
{
    assert(is_item());
    return m_cursor_ptr->data();
}
        
}
