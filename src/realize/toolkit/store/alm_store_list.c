/*******************************************************************************
Description    : Head file that use to store list Data
FileName       : alm_store_list.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_store_list.h"
#include "alm_tools.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/

/*
* 描述:
* 初始化链表，本函数只保存pstStoreInfo的指针，不存储实际内容。
* 创建List对象者应保证pstStoreInfo与List的生命周期一致性。
*/
ALM_INT32 ALM_StoreListInit(OUT ALM_STORE_LIST_STRU* pstList,
    CONST ALM_STORE_INFO_STRU* pstStoreInfo)
{
    ALM_CHK_RET(pstList != NULL && pstStoreInfo != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(ALM_StoreInfoCheck(pstStoreInfo),
        ALM_ERR_INVALID_PARA, (NULL));

    ALM_CHK_RET(!ALM_StoreInfoCheckSort(pstStoreInfo), ALM_ERR_INVALID_PARA,
        ("Simple list can not support sort."));

    pstList->pstStoreInfo = pstStoreInfo;
    pstList->uiCntNum = 0;
    pstList->stHeadNode.pData = NULL;
    pstList->stHeadNode.pstNext = NULL;

    return ALM_OK;
}

/* 清除链表 */
ALM_VOID ALM_StClsStoreList(INOUT ALM_STORE_LIST_STRU* pstList)
{
    ALM_STORE_LIST_NODE_STRU* pstDelNode;
    ALM_STORE_LIST_NODE_STRU* pstItrNode;

    ALM_CHK_RET_VOID(NULL != pstList, (NULL));

    pstItrNode = pstList->stHeadNode.pstNext;
    while (pstItrNode != NULL)
    {
        pstDelNode = pstItrNode;
        pstItrNode = pstItrNode->pstNext;

        ALMStoreListNodeFree(pstDelNode, pstList->pstStoreInfo);
    }

    pstList->stHeadNode.pstNext = NULL;
    pstList->uiCntNum = 0;
}

/* 添加数据元 */
ALM_INT32 ALM_StoreListAdd(INOUT ALM_STORE_LIST_STRU* pstList,
    CONST ALM_VOID* pData)
{
    ALM_INT32 iRet;
    ALM_STORE_LIST_NODE_STRU* pstNew;
    ALM_STORE_LIST_NODE_STRU* pstLast;

    ALM_CHK_RET(pstList != NULL && pData != NULL, ALM_ERR_NULL_PTR,
        (NULL));
    
    /* 得到插入的上一结点，非排序List在头结点处添加 */
    pstLast = &(pstList->stHeadNode);
    pstNew = NULL;

    /* 得到新的结点 */
    iRet = ALMStoreListNodeNew(&pstNew, pstList->pstStoreInfo, pData);
    ALM_CALL_RET(iRet, (NULL));

    /* 添加结点 */
    pstNew->pstNext = pstLast->pstNext;
    pstLast->pstNext = pstNew;
    ++pstList->uiCntNum;

    return iRet;
}

/* 获取链表中数据元总数量 */
ALM_INT32 ALM_StoreListGetTotal(CONST ALM_STORE_LIST_STRU* pstList,
    ALM_UINT32* puiCnt)
{
    ALM_CHK_RET(pstList != NULL && puiCnt != NULL,
        ALM_ERR_NULL_PTR, (NULL));
    
    *puiCnt = pstList->uiCntNum;

    return ALM_OK;
}

/* 删除链表中指定的节点，必须指定mark data比较函数，若mark为NULL清除全部 */
ALM_INT32 ALM_StoreListDel(INOUT ALM_STORE_LIST_STRU* pstList,
    CONST ALM_VOID* pMark)
{
    ALM_INT32 iRet;
    ALM_INT32 iCmp;
    ALM_STORE_LIST_NODE_STRU* pstDel;
    ALM_STORE_LIST_NODE_STRU* pstLast;

    ALM_CHK_RET(pstList != NULL, ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(pstList->pstStoreInfo->stBaseFuncs.fnMarkDataCmp != NULL,
        ALM_ERR_INVALID_PARA,
        ("Delete data by mark, but not cmpare function."));

    pstLast = &(pstList->stHeadNode);
    pstDel = pstLast->pstNext;
    while (pstDel != NULL)
    {
        iCmp = 0;
        if (pMark != NULL)
        {
            iRet = pstList->pstStoreInfo->stBaseFuncs.fnMarkDataCmp(pMark,
                pstDel->pData,
                &iCmp,
                pstList->pstStoreInfo->pEx);
            ALM_CALL_RET(iRet, (NULL));
        }

        if (0 == iCmp)
        {
            pstLast->pstNext = pstDel->pstNext;
            --pstList->uiCntNum;

            ALMStoreListNodeFree(pstDel, pstList->pstStoreInfo);
        }
        else
        {
            pstLast = pstDel;
        }

        pstDel = pstLast->pstNext;
    }
    
    return ALM_OK;
}

/* 查找数据，得到数据的拷贝，必须指定mark data比较函数 */
ALM_INT32 ALM_StoreListGet(CONST ALM_STORE_LIST_STRU* pstList,
    CONST ALM_VOID* pMark,
    INOUT ALM_STORE_QUERY_STRU* pstStoreQuery)
{
    ALM_INT32 iRet;
    ALM_INT32 iCmp;
    ALM_STORE_LIST_NODE_STRU* pstItr;

    ALM_CHK_RET(pstList != NULL && pstStoreQuery != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(pstList->pstStoreInfo->stBaseFuncs.fnMarkDataCmp != NULL,
        ALM_ERR_INVALID_PARA, ("Get data by mark, but not cmpare function."));
    
    pstItr = pstList->stHeadNode.pstNext;
    while (pstItr != NULL)
    {
        iRet = pstList->pstStoreInfo->stBaseFuncs.fnMarkDataCmp(pMark,
            pstItr->pData,
            &iCmp,
            pstList->pstStoreInfo->pEx);
        ALM_CALL_RET(iRet, (NULL));

        if (0 == iCmp)
        {   
            iRet = ALM_StoreQueryAdd(pstStoreQuery, pstItr->pData);
            ALM_CALL_RET(iRet, (NULL));
        }

        pstItr = pstItr->pstNext;
    }

    return ALM_OK;
}

/* 创建节点 */
static ALM_INT32 ALMStoreListNodeNew(OUT ALM_STORE_LIST_NODE_STRU** ppstNewNode,
    CONST ALM_STORE_INFO_STRU* pstStoreInfo,
    CONST ALM_VOID* pData)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_LIST_NODE_STRU* pstNewNode;
    ALM_STORE_INFO_BASE_CPY_DATA_FUNC fnCpy;

    ALM_CHK_RET(ppstNewNode != NULL && pstStoreInfo != NULL
        && pData != NULL, ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(pstStoreInfo->uiUnitSize > 0, ALM_ERR_INVALID_PARA,
        ("The size of the store unit data is zero."));

    *ppstNewNode = NULL;

    /* 分配存储空间 */
    pstNewNode = (ALM_STORE_LIST_NODE_STRU*)ALM_OsBaseMemAlloc(
        sizeof(ALM_STORE_LIST_NODE_STRU));
    ALM_CHK_RET(pstNewNode != NULL, ALM_ERR_OS_MALLOC_FAIL, (NULL));

    pstNewNode->pstNext = NULL;
    pstNewNode->pData = ALM_OsBaseMemAlloc(pstStoreInfo->uiUnitSize);
    iRet = ALM_ERR_OS_MALLOC_FAIL;
    ALM_CHK_LAB(pstNewNode->pData != NULL, LAB_FREE_NODE, (NULL));

    /* 拷贝数据 */
    fnCpy = pstStoreInfo->stBaseFuncs.fnDataCpy;
    if (fnCpy != NULL)
    {
        iRet = fnCpy(pstNewNode->pData, pData, pstStoreInfo->pEx);
        ALM_CALL_LAB(LAB_FREE_ALL, (NULL));
    }
    else
    {
        ALM_memcpy_s(pstNewNode->pData,
            pstStoreInfo->uiUnitSize,
            pData,
            pstStoreInfo->uiUnitSize);
    }

    *ppstNewNode = pstNewNode;

    return ALM_OK;

LAB_FREE_ALL:
    ALM_OsBaseMemFree(pstNewNode->pData);

LAB_FREE_NODE:
    ALM_OsBaseMemFree(pstNewNode);
    return iRet;
}

/* 释放节点 */
static ALM_VOID ALMStoreListNodeFree(ALM_STORE_LIST_NODE_STRU* pstDelNode,
    CONST  ALM_STORE_INFO_STRU* pstStoreInfo)
{
    ALM_STORE_INFO_BASE_CLS_DATA_FUNC fnDataCls = NULL;

    ALM_CHK_RET_VOID(pstDelNode != NULL && pstStoreInfo != NULL,
        ("free null data."));

    fnDataCls = pstStoreInfo->stBaseFuncs.fnDataCls;
    if (pstDelNode->pData != NULL)
    {
        if (fnDataCls != NULL)
        {
            fnDataCls(pstDelNode->pData, pstStoreInfo->pEx);
        }
        else
        {
            ALM_memset_s(pstDelNode->pData, pstStoreInfo->uiUnitSize, 0,
                pstStoreInfo->uiUnitSize);
        }
        ALM_OsBaseMemFree(pstDelNode->pData);
    }

    pstDelNode->pData = NULL;
    pstDelNode->pstNext = NULL;
    ALM_OsBaseMemFree(pstDelNode->pData);
    ALM_OsBaseMemFree(pstDelNode);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/
