//
//  BplusTree.h
//  proj
//
//  Created by David Ellison on 5/17/16.
//
//

#ifndef BplusTree_h
#define BplusTree_h

#include <stdio.h>

int createBTree(char *table, RecordStructure key, int volatileFlag);
int dropBTree(char *btreeName);
int bTreeInsert(char *btree, char *record);
int bTreeDelete(char *btree, char *key);
int bTreeFind(char *btree, char *key, DiskAddress * page);

#endif /* BplusTree_h */
