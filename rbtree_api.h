#ifndef __RBTREE_API_H__
#define __RBTREE_API_H__

#define MK_KEY(key) ((UserKey)key)
#define MK_VALUE(value) ((UserValue)value)

typedef void * UserRbTree;

typedef void * UserKey;
typedef void * UserValue;
typedef void * UserNode;

typedef int (*CompareKeyFun)(UserKey key1, UserKey key2);

/**
    create rbtree
**/
UserRbTree RbTreeCreate(CompareKeyFun compareFun);

/**
    destory rbtree
**/
void RbTreeDestory(UserRbTree userRbTree);
/**
	return 0: find the key
	return -1: not found the key
**/
int RbTreeSearch(UserRbTree userRbTree, UserKey userKey, UserNode *pUserNode);

/**
	return 0: insert ok
	return 1: already has key
	< 0: error occure
**/
int RbTreeInsert(UserRbTree userRbTree, UserKey userKey, UserValue userValue);

int RbTreeRemove(UserRbTree userRbTree, UserNode node);

UserValue RbTreeGetValueFromNode(UserNode node);

void RbTreeDump(UserRbTree userRbTree);

int RbTreeGetSize(UserRbTree userRbTree);

int RbTreeGetMinKey(UserRbTree userRbTree, UserKey *pUserKey);

int RbTreeGetMaxKey(UserRbTree userRbTree, UserKey *pUserKey);

int RbTreeGetNodeListBetween(UserRbTree userRbTree, UserNode *userNodeList, int size, UserKey userKeyMin, UserKey userKeyMax);

#endif