/*******************************************************************************
Description    : Head file that use to store list Data
FileName       : alm_store_list.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_STORE_LIST_H__
#define __ALM_STORE_LIST_H__

#include "alm_store.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/

/**
*描述：链表定义结构体。
*成员：
*1.pData - 数据域
*2.pstNext - 指针域
*/
typedef struct TAG_ALM_STORE_LIST_NODE_STRU
{
    ALM_VOID* pData;
    struct TAG_ALM_STORE_LIST_NODE_STRU* pstNext;
} ALM_STORE_LIST_NODE_STRU;

/**
*描述: 存储链表结构体。
*成员：
*1.uiCntNum - 当前节点数目。
*2.stHeadNode - 使用一个空头节点。
*3.stStoreInfo - store统一定义包。
*/
typedef struct 
{
    ALM_UINT32 uiCntNum;
    ALM_STORE_LIST_NODE_STRU stHeadNode;
    CONST ALM_STORE_INFO_STRU* pstStoreInfo;
} ALM_STORE_LIST_STRU;

/*
* 描述:
* 初始化链表，本函数只保存pstStoreInfo的指针，不存储实际内容。
* 创建List对象者应保证pstStoreInfo与List的生命周期一致性。
*/
ALM_INT32 ALM_StoreListInit(OUT ALM_STORE_LIST_STRU* pstList,
    CONST ALM_STORE_INFO_STRU* pstStoreInfo);

/* 清除链表 */
ALM_VOID ALM_StClsStoreList(INOUT ALM_STORE_LIST_STRU* pstList);

/* 添加数据元 */
ALM_INT32 ALM_StoreListAdd(INOUT ALM_STORE_LIST_STRU* pstList,
    CONST ALM_VOID* pData);

/* 获取链表中数据元总数量 */
ALM_INT32 ALM_StoreListGetTotal(CONST ALM_STORE_LIST_STRU* pstList,
    ALM_UINT32* puiCnt);

/* 删除链表中指定的节点，必须指定mark data比较函数，若mark为NULL清除全部 */
ALM_INT32 ALM_StoreListDel(INOUT ALM_STORE_LIST_STRU* pstList,
    CONST ALM_VOID* pMark);

/* 查找数据，得到数据的拷贝，必须指定mark data比较函数 */
ALM_INT32 ALM_StoreListGet(CONST ALM_STORE_LIST_STRU* pstList,
    CONST ALM_VOID* pMark,
    INOUT ALM_STORE_QUERY_STRU* pstStoreQuery);

/* 创建节点 */
static ALM_INT32 ALMStoreListNodeNew(ALM_STORE_LIST_NODE_STRU** pstNewNode,
    CONST ALM_STORE_INFO_STRU* pstStoreInfo,
    CONST ALM_VOID* pData);

/* 释放节点 */
static ALM_VOID ALMStoreListNodeFree(ALM_STORE_LIST_NODE_STRU* pstDelNode,
    CONST  ALM_STORE_INFO_STRU* pstStoreInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/

#endif /* __ALM_STORE_LIST_H__ */
