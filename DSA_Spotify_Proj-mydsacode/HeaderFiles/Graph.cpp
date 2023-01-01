#pragma once
#include <iostream>
#include <string>
#include "Graph.h"
using namespace std;

Edge::Edge()
{
    artist1 = nullptr;
    artist2 = nullptr;
}
Edge::Edge(Artist *artist1, Artist *artist2)
{
    this->artist1 = artist1;
    this->artist2 = artist2;
}

bool Edge::operator==(Edge test)
{
    return (this->artist1 == artist1 && this->artist2 == artist2) ||
            (this->artist1 == artist2 && this->artist2 == artist1);
}

PointerToEdge::PointerToEdge(Artist *a1, Artist *a2)
{
    edgePointer = new Edge(a1, a2);
    next = nullptr;
}
EdgeLinkedList::EdgeLinkedList()
{
    head = nullptr;
}
void EdgeLinkedList::addTrackToEdge(Artist *a1, Artist *a2, EdgeLinkedList *e1, Track *toAdd)
{
    PointerToEdge *temp = e1->head;
    while (temp!=nullptr && temp->next != nullptr)
    {
        if ((temp->edgePointer->artist1 == a1 && temp->edgePointer->artist2 == a2) ||
            (temp->edgePointer->artist1 == a2 && temp->edgePointer->artist2 == a1))
        {
            temp->edgePointer->collabTracks->ADD(toAdd);
            return;
        }
        if (temp->next != nullptr)
            temp = temp->next;
    }
    Edge* to_AddEdge;
    if (temp==nullptr){
        e1->head = new PointerToEdge(a1, a2);
        e1->head->edgePointer->collabTracks->ADD(toAdd);
        to_AddEdge=e1->head->edgePointer;
    }
    else{
    temp->next = new PointerToEdge(a1, a2);
    temp->next->edgePointer->collabTracks->ADD(toAdd);
    to_AddEdge=temp->next->edgePointer;
    }
    a1->CollabList->ADD(to_AddEdge);
    a2->CollabList->ADD(to_AddEdge);
}

void EdgeLinkedList::createGraph(TrackHashTable *h1, EdgeLinkedList *e1)
{
    // this pointer will store head of linked list of tracks from hash table where it is found.
    TrackNode *trackNodeTemp;
    // loop iterating over track hash indexes.
    for (int i = 0; i < h1->tablesize; i++)
    {
        if (h1->isAvailable(i))
            continue;
        // if list is found assign head
        trackNodeTemp = h1->hashTableArr[i]->head; // LINKED LIST HEAD TRACK NODE

        while (trackNodeTemp != nullptr)
        { // now iterate through list and create edges

            if (trackNodeTemp->TrackPointer->ArtistsOfTrack->head->next == nullptr)
            {
                trackNodeTemp = trackNodeTemp->next;
                continue;
                // no edge needed. only solo artist.
            }

            else
            {
                // create local dynamic array to store artist pointers for edges.
                Node<Artist> *artistTemp1 = trackNodeTemp->TrackPointer->ArtistsOfTrack->head;
                Node<Artist> *artistTemp2;
                while (artistTemp1 != nullptr)
                {
                    artistTemp2 = artistTemp1->next;
                    while (artistTemp2 != nullptr)
                    {
                        addTrackToEdge(artistTemp1->object, artistTemp2->object, e1, trackNodeTemp->TrackPointer);
                    }
                    artistTemp1 = artistTemp1->next;
                }
            }
        }
    }
}

void EdgeLinkedList::printEdges(){
    int count=0;
    PointerToEdge* temp=head;
    while(temp!=nullptr){
        count++;
        temp=temp->next;
    }
    cout<<"Total Edges created: "<<count<<endl;
}