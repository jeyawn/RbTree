/*
 * =============================================================================
 *
 *       Filename:  rbtree_api.c
 *
 *    Description:  api for rbtree to use.
 *
 *        Created:  09/30/2019 11:39:34 PM
 *
 *         Author:  Ye jianyong, jeyawn@126.com
 *
 * =============================================================================
 */

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "rbtree_api.h"
//#define DEBUG
#ifdef DEBUG
#define RECORD_MEM(isMalloc, addr) \
    do {\
        if(isMalloc) {\
            printf("malloc(%p)\n", addr);}\
        else {\
            printf("free(%p)\n", addr);}\
        }while(0)
#define API_DEBUG   printf
#else
#define RECORD_MEM(isMalloc, addr)
#define API_DEBUG(formate...) 
#endif
typedef struct UserRbTreeImpl {
	struct rb_root tree;
    CompareKeyFun compareKeyFun;
    int nodeNum;
}UserRbTreeImpl_t;

typedef struct UserNodeImpl {
	struct rb_node node;
	UserKey key;
	UserValue value;
}UserNodeImpl_t;

static int compareKeyDefault(UserKey key1, UserKey key2) {
    if(key1 > key2) {
        return 1;
    }
    else if(key1 < key2) {
        return -1;
    }
    else {
        return 0;
    }
}


UserRbTree RbTreeCreate(CompareKeyFun compareFun) {
	UserRbTreeImpl_t *userRbTreeImpl = malloc(sizeof(UserRbTreeImpl_t));
	if(userRbTreeImpl) {
        RECORD_MEM(1, userRbTreeImpl);
		userRbTreeImpl->tree = RB_ROOT;
        if(compareFun) {
            userRbTreeImpl->compareKeyFun = compareFun;
        }
        else {
            userRbTreeImpl->compareKeyFun = compareKeyDefault;
        }
        userRbTreeImpl->nodeNum = 0;
	}
	return userRbTreeImpl;
}

static void deleteNode(struct rb_node *node) {
    if(node) {
        deleteNode(node->rb_left);
        deleteNode(node->rb_right);
        RECORD_MEM(0, node);
        free(node);
    }
}

void RbTreeDestory(UserRbTree userRbTree) {
	UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
	if(pUserRbTreeImpl) {
        struct rb_node *node = pUserRbTreeImpl->tree.rb_node;
        deleteNode(node);
        RECORD_MEM(0, pUserRbTreeImpl);
		free(pUserRbTreeImpl);
	}
}

void RbTreeClear(UserRbTree userRbTree)
{
	UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
	if(pUserRbTreeImpl) {
        struct rb_node *node = pUserRbTreeImpl->tree.rb_node;
        deleteNode(node);
        pUserRbTreeImpl->tree = RB_ROOT;
        pUserRbTreeImpl->nodeNum = 0;
	}    
}

int RbTreeSearch(UserRbTree userRbTree, UserKey userKey, UserNode *pUserNode)
{
	if(userRbTree && pUserNode) {
		UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
	  	struct rb_node *node = pUserRbTreeImpl->tree.rb_node;

	  	while (node) {
	  		UserNodeImpl_t *data = (UserNodeImpl_t *)node;
			int result;

			result = pUserRbTreeImpl->compareKeyFun(userKey, data->key);

			if (result < 0)
	  			node = node->rb_left;
			else if (result > 0)
	  			node = node->rb_right;
			else {
				*pUserNode = data;
				return 0;
			}
		}
	}
	return -1;
}

int RbTreeInsert(UserRbTree userRbTree, UserKey userKey, UserValue userValue)
{
    if(userRbTree) {
        UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
        struct rb_node **newNode = &(pUserRbTreeImpl->tree.rb_node), *parent = NULL;
        UserNodeImpl_t *rbTreeData = NULL;
      	/* Figure out where to put new node */
      	while (*newNode) {
            UserNodeImpl_t *data = (UserNodeImpl_t *)(*newNode);

      		int result = pUserRbTreeImpl->compareKeyFun(userKey, data->key);

    		parent = *newNode;
      		if (result < 0)
      			newNode = &((*newNode)->rb_left);
      		else if (result > 0)
      			newNode = &((*newNode)->rb_right);
      		else
      			return 0;
      	} 
        rbTreeData = malloc(sizeof(UserNodeImpl_t));
        if(rbTreeData == NULL) {
            return -2;
        }
        RECORD_MEM(1, rbTreeData);
        rbTreeData->key = userKey;
        rbTreeData->value = userValue;
        pUserRbTreeImpl->nodeNum++;
        
      	/* Add new node and rebalance tree. */
      	rb_link_node(&rbTreeData->node, parent, newNode);
      	rb_insert_color(&rbTreeData->node, &pUserRbTreeImpl->tree);

    	return 1;
    }

	return -1;
}

int RbTreeRemove(UserRbTree userRbTree, UserNode node)
{
    if(userRbTree && node) {
        UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
        UserNodeImpl_t * pUserNodeImpl = (UserNodeImpl_t *)node;
        rb_erase(&pUserNodeImpl->node, &pUserRbTreeImpl->tree);
        RECORD_MEM(0, pUserNodeImpl);
        free(pUserNodeImpl);
        pUserRbTreeImpl->nodeNum--;
    }

    return 0;
}

UserValue RbTreeGetValueFromNode(UserNode node)
{
    UserNodeImpl_t * pUserNodeImpl = (UserNodeImpl_t *)node;
    return pUserNodeImpl->value;
}

UserKey RbTreeGetKeyFromNode(UserNode node) 
{
    UserNodeImpl_t * pUserNodeImpl = (UserNodeImpl_t *)node;
    return pUserNodeImpl->key;    
}

int RbTreeGetSize(UserRbTree userRbTree) {
    if(userRbTree) {
        UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
        return pUserRbTreeImpl->nodeNum;
    }
    return 0;
}

void RbTreeDump(UserRbTree userRbTree) {
    if(userRbTree) {
        UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
        struct rb_node *node;
        for (node = rb_first(&pUserRbTreeImpl->tree); node; node = rb_next(node)) {
            
            printf("key = %ld\n", (long)((UserNodeImpl_t *)node)->key);

        }

    }
    
}

int RbTreeGetMinKey(UserRbTree userRbTree, UserKey *pUserKey)
{
    if(userRbTree) {
        UserNodeImpl_t * pUserNodeImpl = NULL;
        UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
        struct rb_node *node = pUserRbTreeImpl->tree.rb_node;
        if(node) {
            while (node->rb_left) {
                node = node->rb_left;
            }
            pUserNodeImpl = (UserNodeImpl_t *)node;
            *pUserKey = pUserNodeImpl->key;
            return 0;
        }
        else {
            return -1;
        }
    }
    return -2;
}

int RbTreeGetMaxKey(UserRbTree userRbTree, UserKey *pUserKey)
{
    if(userRbTree) {
        UserNodeImpl_t * pUserNodeImpl = NULL;
        UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
        struct rb_node *node = pUserRbTreeImpl->tree.rb_node;
        if(node) {
            while (node->rb_right) {
                node = node->rb_right;
            }
            pUserNodeImpl = (UserNodeImpl_t *)node;
            *pUserKey = pUserNodeImpl->key;
            return 0;
        }
        else {
            return -1;
        }
    }
    return -2;

}

int RbTreeGetNodeListBetween(UserRbTree userRbTree, UserNode *userNodeList, int size, UserKey userKeyMin, UserKey userKeyMax)
{
    if(userRbTree && userNodeList && size > 0) {
        UserRbTreeImpl_t *pUserRbTreeImpl = userRbTree;
        struct rb_node **newNode = &(pUserRbTreeImpl->tree.rb_node), *parent = NULL;
        UserNodeImpl_t *rbTreeData = NULL;
        int result = -2;
        int curSize = 0;
      	/* Figure out where to put new node */
      	while (*newNode) {
            UserNodeImpl_t *data = (UserNodeImpl_t *)(*newNode);

      		result = pUserRbTreeImpl->compareKeyFun(userKeyMin, data->key);
            parent = *newNode;

    		rbTreeData = (UserNodeImpl_t *)(*newNode);
            API_DEBUG("####rbTreeData-->key:%ld, result:%d\n", (long)rbTreeData->key, result);
      		if (result <= 0) {
      			newNode = &((*newNode)->rb_left);
                
            }
      		else if (result > 0) {
      			newNode = &((*newNode)->rb_right);

            }  
      	} 
        if(parent) {
            if(result <= 0) {
                
            }
            else {
                parent = rb_next(parent);
            }
        }
        else {
            parent = rb_first(&pUserRbTreeImpl->tree);
        }

        struct rb_node *node;
        for (node = parent; node; node = rb_next(node)) {
            result = pUserRbTreeImpl->compareKeyFun(userKeyMax, ((UserNodeImpl_t *)node)->key);
            
            if(result >= 0) {
                API_DEBUG("====>key = %ld, result:%d\n", (long)((UserNodeImpl_t *)node)->key, result);
                if(curSize < size) {
                    userNodeList[curSize] = (UserNode)node;
                    curSize++;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }

        }
        return curSize;

    }
    return -1;
}
