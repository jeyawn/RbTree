/*
 * =============================================================================
 *
 *       Filename:  api_test.c
 *
 *    Description:  test code for rbtree_api to use.
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

void doRbTreeTest()
{
    int ret, index;
    UserKey minUserKey, maxUserKey;
    UserNode userNode;
    UserRbTree userRbTree = RbTreeCreate(NULL);
    if(userRbTree == NULL) {
        return;
    }
    RbTreeInsert(userRbTree, MK_KEY(1), MK_VALUE("1"));
    RbTreeInsert(userRbTree, MK_KEY(10), MK_VALUE("10"));
    RbTreeInsert(userRbTree, MK_KEY(5), MK_VALUE("5"));
    RbTreeInsert(userRbTree, MK_KEY(7), MK_VALUE("7"));
    RbTreeInsert(userRbTree, MK_KEY(4), MK_VALUE("4"));
    RbTreeInsert(userRbTree, MK_KEY(3), MK_VALUE("3"));
    RbTreeInsert(userRbTree, MK_KEY(9), MK_VALUE("9"));
    printf("treesize: %d\n", RbTreeGetSize(userRbTree));
    RbTreeDump(userRbTree);
    RbTreeSearch(userRbTree, MK_KEY(4), &userNode);
    RbTreeRemove(userRbTree, userNode);
    printf("after delete treesize: %d\n", RbTreeGetSize(userRbTree));
    RbTreeDump(userRbTree);
    RbTreeGetMinKey(userRbTree, &minUserKey);
    RbTreeGetMaxKey(userRbTree, &maxUserKey);
    printf("minkey:%ld, maxkey:%ld\n", (long)minUserKey, (long)maxUserKey);
    UserNode *userNodeList = malloc(sizeof(UserNode) * 3);
    ret = RbTreeGetNodeListBetween(userRbTree, userNodeList, 3, (UserKey)3, (UserKey)12);
    printf("we got size:%d\n", ret);
    for(index = 0; index < ret; index++) {
        printf("---->{%ld}\n", (long)RbTreeGetKeyFromNode(userNodeList[index]));
    }

    RbTreeClear(userRbTree);
    printf("after clear treesize: %d\n", RbTreeGetSize(userRbTree));
    RbTreeInsert(userRbTree, MK_KEY(1), MK_VALUE("1"));
    RbTreeInsert(userRbTree, MK_KEY(10), MK_VALUE("10"));
    RbTreeInsert(userRbTree, MK_KEY(5), MK_VALUE("5"));
    RbTreeInsert(userRbTree, MK_KEY(7), MK_VALUE("7"));
    RbTreeInsert(userRbTree, MK_KEY(4), MK_VALUE("4"));
    RbTreeInsert(userRbTree, MK_KEY(3), MK_VALUE("3"));
    RbTreeInsert(userRbTree, MK_KEY(9), MK_VALUE("9"));
    printf("after insert treesize: %d\n", RbTreeGetSize(userRbTree));
    ret = RbTreeGetNodeListBetween(userRbTree, userNodeList, 3, 3, 12);
    printf("we got size:%d\n", ret);
    for(index = 0; index < ret; index++) {
        printf("---->{%ld}\n", (long)RbTreeGetKeyFromNode(userNodeList[index]));
    }
    free(userNodeList);
    RbTreeDestory(userRbTree);
    printf("doRbTreeTest end\n");
}

int main()
{
    doRbTreeTest();
	return 0;
}


