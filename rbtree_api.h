#ifndef __RBTREE_API_H__
#define __RBTREE_API_H__

#define MK_KEY(key) ((UserKey)key)
#define MK_VALUE(value) ((UserValue)value)

typedef void * UserRbTree;
typedef void * UserKey;
typedef void * UserValue;
typedef void * UserNode;
/**
    compare function for user key
**/
typedef int (*CompareKeyFun)(UserKey key1, UserKey key2);

/**
    create rbtree
    compareFun: the compare function for key.
    If it is NULL, a default compare function will be used just compare key value in format of integer

    return: the instance for rbtree
**/
UserRbTree RbTreeCreate(CompareKeyFun compareFun);

/**
    destory rbtree
    It will release all the memory and destory the tree instance.
**/
void RbTreeDestory(UserRbTree userRbTree);

/**
    search value in rbtree instance
    userKey: the key want to search
    pUserNode: the node that equal userKey. You can use RbTreeGetValueFromNode to get the value in the node
	return 0: find the key
	return -1: not found the key
**/
int RbTreeSearch(UserRbTree userRbTree, UserKey userKey, UserNode *pUserNode);

/**
    insert a key value pair to rbtree instance
    userKey: the key want to insert
    userValue: the value want to insert
	return 0: insert ok
	return 1: already has key
	< 0: error occure
**/
int RbTreeInsert(UserRbTree userRbTree, UserKey userKey, UserValue userValue);

/**
    remove a key value pair from rbtree instance
    node: node get from RbTreeSearch, invalid node might cause api crash
	return 0: remove ok
	< 0: error occure
**/
int RbTreeRemove(UserRbTree userRbTree, UserNode node);

/**
    get value from node
    node: node get from RbTreeSearch, invalid node might cause api crash
	return the value of the node
**/
UserValue RbTreeGetValueFromNode(UserNode node);

/**
    dump the rbtree 
**/
void RbTreeDump(UserRbTree userRbTree);

/**
    get the size of rbtree 
    return the size of rbtree
**/
int RbTreeGetSize(UserRbTree userRbTree);

/**
    get the min key of rbtree 
    return the min key of rbtree
**/
int RbTreeGetMinKey(UserRbTree userRbTree, UserKey *pUserKey);

/**
    get the max key of rbtree 
    return the max key of rbtree
**/
int RbTreeGetMaxKey(UserRbTree userRbTree, UserKey *pUserKey);

/**
    get the nodelist from the instance that key between userKeyMin and userKeyMax,
    the value will be save in userNodeList.
    userNodeList: the space to save result, the mem must be alloc out of api
    size: the count of node in userNodeList
    userKeyMin: the min userKey
    userKeyMax: the max userKey
	return >=0 the number of node save in userNodeList
	< 0: error occure
**/
int RbTreeGetNodeListBetween(UserRbTree userRbTree, UserNode *userNodeList, int size, UserKey userKeyMin, UserKey userKeyMax);

#endif