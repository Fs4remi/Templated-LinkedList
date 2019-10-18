//
// Created by fs4re on 10/6/2019.
//
#ifndef CHUNKARRAY_CHUNKLINKEDLIST_H
#define CHUNKARRAY_CHUNKLINKEDLIST_H
int const MAX_CAP = 12;
using namespace std;
template <class T>
struct ChunkyNode{
    int index;
    ChunkyNode<T>* next;
    T values[MAX_CAP];
};

template <class T>
class ChunkLinkedList{
private:
    ChunkyNode<T>*head, *tail;
    int length;

    bool isChunkFull(ChunkyNode<T>* objPtr)
    {   return objPtr->index == MAX_CAP; }

    bool isChunkEmpty(ChunkyNode<T>* objPtr)
    {   return objPtr->index == 0; }

    void Rearrange(ChunkyNode<T>* node, int index);
public:
    ChunkLinkedList();
    ~ChunkLinkedList();

    bool isListEmpty()
    {   return head == nullptr; }

    int GetLength()
    {   return length; }// (refers to the total number of elements, not the number of chunk nodes)

    void Append(T elem);
    T* GetIndex(int i); // (gets the item at index i)
    bool Search(T elem);
    void Print();
    void Remove(T elem);// (removes first instance of elem)
};

template <class T>
ChunkLinkedList<T>::ChunkLinkedList()
{
    head = tail = nullptr;
    length = 0;
}
template <class T>
void ChunkLinkedList<T>::Rearrange(ChunkyNode<T>* node, int index)
{
    int length = node->index;
    if(index != length-1) {//if the index is the last element in the array
        node->index--;//we don't have to shift things up
        for(int i = index ; i < length-1; i++){
            node->values[i] = node->values[i+1];
        }
    }
    node->index--;
}
//what how the func does what it does 
template <class T>
void ChunkLinkedList<T>::Remove(T elem)
{
    if(isListEmpty())
        return;
    ChunkyNode<T>* travel = head;
    ChunkyNode<T>*previous = nullptr;
    bool stopFlag = false;
    while(travel){//traversing the array in each node to find the elem given
        for(int i = 0; i < travel->index; i++){
            if(travel->values[i] == elem){ //if its found in the array
                if(travel->index > 1)
                    Rearrange(travel, i);
                else{
                    if(!previous)
                        head=head->next;
                    else
                        previous->next = travel->next;
                    delete travel;
                }
                length--;
                stopFlag = true;
            }
        }
        if(!stopFlag){
            previous = travel;
            travel = travel->next;
        }
        else
            break;
    }
}

template <class T>
bool ChunkLinkedList<T>::Search(T elem)
{
    if(isListEmpty())
        return false;
    ChunkyNode<T>* travel = head;
    while(travel){
        for(int i = 0; i < travel->index; i++){
            if(travel->values[i] == elem)
                return true;
        }
        travel = travel->next;
    }
}

template <class T>
T* ChunkLinkedList<T>::GetIndex(int i)
{
    if(i > length) //if the index i is out of bounds
        return nullptr;
    if(i < head->index) // if the index is in the head's array
        return new T(head->values[i]);
    else{ // if we have to traverse the list to find it
        ChunkyNode<T>* travel = head->next;
        i -= head->index;
        while(travel && i >= 0){
            i -= travel->index;
            travel = travel->next;
        }
        if(i<0 || !travel)
            return nullptr;
        else
            return new T(travel->values[i]);
    }

}
template <class T>
void ChunkLinkedList<T>::Print()
{
    ChunkyNode<T>* travel = head;
    int count = 1;
    while(travel){
        cout << "Chunk " << count << ": ";
        for(int i = 0; i < travel->index; i++){
            std::cout << " " << travel->values[i];
        }
        std::cout << "\n";
        count++;
        travel = travel->next;
    }
}
template <class T>
void ChunkLinkedList<T>::Append(T elem)
{
    if(isListEmpty()){
        ChunkyNode<T>* newNode = new ChunkyNode<T>;
        newNode->index = 0;
        newNode->next = nullptr;
        newNode->values[newNode->index] = elem;
        newNode->index++;
        head = tail = newNode;
    }
    else if (!isChunkFull(tail)){
        tail->values[tail->index] = elem;
        tail->index++;
    }
    else if(isChunkFull(tail)){
        ChunkyNode<T>* newNode = new ChunkyNode<T>{ 0 , nullptr};
        newNode->values[newNode->index] = elem;
        newNode->index++;
        tail->next = newNode;
        tail = newNode;
    }
    length++;
}


template <class T>
ChunkLinkedList<T>::~ChunkLinkedList()
{
    if( !isListEmpty() ){
        ChunkyNode<T>* traveller;
        while(head){
            traveller = head;
            head = head->next;
            delete traveller;
        }
    }
}

#endif //CHUNKARRAY_CHUNKLINKEDLIST_H
