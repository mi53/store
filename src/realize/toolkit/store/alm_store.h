/*******************************************************************************
Description    : Head file that use to store Data
FileName       : alm_store.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/

#ifndef __ALM_STORE_H__
#define __ALM_STORE_H__

#include "alm_intf_constant.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/
    
/************************************************************************/
/*                      存储器定义相关数据结构                               */
/************************************************************************/

/* 初始化存储数据元 */
typedef ALM_INT32 (*ALM_STORE_INFO_BASE_INIT_DATA_FUNC)(OUT ALM_VOID* pData,
    CONST ALM_VOID* pEx);

/* 清理存储数据元 */
typedef ALM_VOID (*ALM_STORE_INFO_BASE_CLS_DATA_FUNC)(INOUT ALM_VOID* pData,
    CONST ALM_VOID* pEx);

/* 拷贝存储数据元 */
typedef ALM_INT32 (*ALM_STORE_INFO_BASE_CPY_DATA_FUNC)(OUT ALM_VOID* pDestData,
    CONST ALM_VOID* pSrcData,
    CONST ALM_VOID* pEx);

/* 比较MARK与DATA大小 */
typedef ALM_INT32 (*ALM_STORE_INFO_SORT_CMP_MARK_DATA_FUNC)(CONST ALM_VOID* pMark,
    CONST ALM_VOID* pData,
    OUT ALM_INT32* piRslt,
    CONST ALM_VOID* pEx);

/**
* 描述：存储器自定义之基础部分。
* 成员：
* 1. fnDataInit - 初始化数据对象，若没有指定使用仅做内存清零处理。
* 2. fnDataCls - 清理数据对象，若没有指定使用仅做内存清零处理。。
* 3. fnDataCpy - 拷贝数据对象，若没有指定使用memcpy。
* 4. fnMarkDataCmp - 比较MARK与DATA大小，若没有指定不支持排序且不支持依据MARK查找。
*/
typedef struct
{
    ALM_STORE_INFO_BASE_INIT_DATA_FUNC fnDataInit;
    ALM_STORE_INFO_BASE_CLS_DATA_FUNC fnDataCls;
    ALM_STORE_INFO_BASE_CPY_DATA_FUNC fnDataCpy;
    ALM_STORE_INFO_SORT_CMP_MARK_DATA_FUNC fnMarkDataCmp;
}ALM_STORE_INFO_BASE_STRU;

/* 定义时附带节点类型 */
#define ALM_STORE_STRU_T(type) ALM_STORE_STRU

/* 创建MARK对象，失败返回NULL */
typedef ALM_VOID* (*ALM_STORE_INFO_SORT_MARK_NEW_FUNC)(CONST ALM_VOID* pData,
    CONST ALM_VOID* pEx);

/* 删除MARK对象 */
typedef ALM_VOID (*ALM_STORE_INFO_SORT_MARK_FREE_FUNC)(INOUT ALM_VOID* pMark,
    CONST ALM_VOID* pEx);

/**
* 描述：
* 存储器自定义之排序部分。若存储器需要实现排序功能需要定义这部分内容。
* 只有MARK排序与DATA排序保持一致才能正常工作，否则查询只能查询到部分内容。
* 成员：
* 1. fnDataMarkNew - 依据DATA创建MARK对象。
* 2. fnDataMarkFree - 删除MARK对象，若没有指定使用free函数。
*/
typedef struct 
{
    ALM_STORE_INFO_SORT_MARK_NEW_FUNC fnDataMarkNew;
    ALM_STORE_INFO_SORT_MARK_FREE_FUNC fnDataMarkFree;
}ALM_STORE_INFO_SORT_STRU;

/**
* 描述：存储操作信息结构体。
* 成员:
* 1. uiUnitSize - 数据单元大小。
* 2. bIncSame - 是否允许添加重复数据。若为false必须指定为排序存储器。
* 3. stBaseFuncs - 数据单元初始化、清理等函数。
* 4. stSortFuncs - 存储器实现排序必须指定的一些函数，若为NULL则不排序。
* 5. pEx - 自定义附加信息，数据元操作排序等可能需要使用该扩展内容。
*/
typedef struct ALM_STORE_INFO_STRU
{
    ALM_INT32 uiUnitSize;
    ALM_BOOL bIncSame;

    ALM_STORE_INFO_BASE_STRU stBaseFuncs;
    ALM_STORE_INFO_SORT_STRU stSortFuncs;
    CONST ALM_VOID* pEx;
} ALM_STORE_INFO_STRU;

/* 校验store info的有效性 */
ALM_BOOL ALM_StoreInfoCheck(CONST ALM_STORE_INFO_STRU* pstStoreInfo);

/* 判断是否支持排序 */
ALM_BOOL ALM_StoreInfoCheckSort(CONST ALM_STORE_INFO_STRU* pstStoreInfo);
/************************************************************************/
/*                      存储器实现及操作                                   */
/************************************************************************/

/**
* 描述：存储类型
* 成员：
* 1.ALM_STORE_TYPE_LIST - 链表类型，不支持排序。
* 2.ALM_STORE_TYPE_TREE - 树类型
* 3.ALM_STORE_TYPE_ARRAY - 数组类型
*/
typedef enum ALM_STORE_TYPE_ENUM
{
    ALM_STORE_TYPE_LIST,
    ALM_STORE_TYPE_TREE,
    ALM_STORE_TYPE_ARRAY
} ALM_STORE_TYPE_ENUM;

/**
* 描述：存储器。
* 成员：
* 1. eStoreType - 存储器类型。
* 2. stStoreInfo - 存储器自定义信息。
* 3. pStore - 存储堆，具体存储结构与存储器类型相关。
*/
typedef struct 
{
    ALM_STORE_TYPE_ENUM eStoreType;
    ALM_STORE_INFO_STRU stStoreInfo;
    ALM_VOID* pStore;
}ALM_STORE_STRU;

/**
* 描述：存储查询结果链表
* 成员：
* 1.pData - 节点数据
* 2.pstNext - 节点指针域
*/
typedef struct TAG_ALM_STORE_QUERY_NODE_STRU
{
    ALM_VOID* pData;
    struct TAG_ALM_STORE_QUERY_NODE_STRU* pstNext;
} ALM_STORE_QUERY_NODE_STRU;

/**
* 描述：存储查询结果结构体
* 成员：
* 1.uiCount - 数据的个数
* 2.stDataHead - 存储查询结果的链表起始结点，头结点为不存储数据。
*/
typedef struct ALM_STORE_QUERY_STRU
{
    ALM_UINT32 uiCount;
    ALM_STORE_QUERY_NODE_STRU stDataHead;
    CONST ALM_STORE_STRU* pstStore;
} ALM_STORE_QUERY_STRU;

/* 初始化存储查询结果 */
ALM_INT32 ALM_StoreQueryInit(OUT ALM_STORE_QUERY_STRU* pstStoreQuery,
    CONST ALM_STORE_STRU* pstStore);

/* 清除存储查询结果 */
ALM_VOID ALM_StoreQueryCls(INOUT ALM_STORE_QUERY_STRU* pstStoreQuery);

/* 添加存储查询结果 */
ALM_INT32 ALM_StoreQueryAdd(INOUT ALM_STORE_QUERY_STRU* pstStoreQuery,
    CONST ALM_VOID* pData);

/* 初始化存储器 */
ALM_INT32 ALM_StoreInit(OUT ALM_STORE_STRU* pstStore,
                  ALM_STORE_TYPE_ENUM eStoreType,
                  CONST ALM_STORE_INFO_STRU* pstStoreInfo);

/* 清除存储器 */
ALM_VOID ALM_StoreCls(INOUT ALM_STORE_STRU* pstStore);

/* 得到已存储数据元得数目 */
ALM_INT32 ALM_StoreGetTotalCnt(CONST ALM_STORE_STRU* pstStore, ALM_UINT32* puiCnt);

/* 添加新数据的拷贝 */
ALM_INT32 ALM_StoreAddData(ALM_STORE_STRU* pstStores, CONST ALM_VOID* pData);

/* 删除指定数据 */
ALM_INT32 ALM_StoreDelData(ALM_STORE_STRU* pstStore, CONST ALM_VOID* pMark);

/* 得到存储器中数据的拷贝 */
ALM_INT32 ALM_StoreGetData(CONST ALM_STORE_STRU* pstStore,
                     CONST ALM_VOID* pMark,
                     OUT ALM_STORE_QUERY_STRU* pstStoreQuery);

#ifdef __cplusplus
#if __cplusplus
}
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/

#endif /* __ALM_STORE_H__ */
