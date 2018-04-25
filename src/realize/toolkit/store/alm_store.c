/*******************************************************************************
Description    : Head file that use to store Data
FileName       : alm_store.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_store.h"
#include "alm_store_list.h"
#include "alm_tools.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/



/* 校验store info的有效性 */
ALM_BOOL ALM_StoreInfoCheck(CONST ALM_STORE_INFO_STRU* pstStoreInfo)
{
    ALM_BOOL bSort = FALSE;

    if (NULL == pstStoreInfo)
    {
        ALM_LOG_ERROR(("null para."));
        return FALSE;
    }

    if (0 == pstStoreInfo->uiUnitSize)
    {
        ALM_LOG_ERROR(("Unit size is zero."));
        return FALSE;
    }

    if (NULL != pstStoreInfo->stSortFuncs.fnDataMarkNew)
    {
        if (NULL == pstStoreInfo->stBaseFuncs.fnMarkDataCmp)
        {
            ALM_LOG_ERROR(("Appoint mark new, but no cmp function."));
            return FALSE;
        }
        bSort = TRUE;
    }

    if (!pstStoreInfo->bIncSame && !bSort)
    {
        ALM_LOG_ERROR(("Can not include same data, but not support sort."));
        return FALSE;
    }

    return TRUE;
}

/* 判断是否支持排序 */
ALM_BOOL ALM_StoreInfoCheckSort(CONST ALM_STORE_INFO_STRU* pstStoreInfo)
{
    if (NULL == pstStoreInfo)
    {
        ALM_LOG_ERROR(("null para."));
        return FALSE;
    }

    if (NULL != pstStoreInfo->stSortFuncs.fnDataMarkNew)
    {
        return TRUE;
    }

    return FALSE;
}

/* 初始化存储查询结果 */
ALM_INT32 ALM_StoreQueryInit(OUT ALM_STORE_QUERY_STRU* pstStoreQuery,
    CONST ALM_STORE_STRU* pstStore)
{
    ALM_CHK_RET(NULL != pstStoreQuery, ALM_ERR_NULL_PTR, (NULL));

    pstStoreQuery->uiCount = 0;
    pstStoreQuery->stDataHead.pstNext = NULL;
    pstStoreQuery->pstStore = pstStore;

    return ALM_OK;
}

/* 清除存储查询结果 */
ALM_VOID ALM_StoreQueryCls(INOUT ALM_STORE_QUERY_STRU* pstStoreQuery)
{
    ALM_STORE_QUERY_NODE_STRU* pstDel;
    ALM_STORE_QUERY_NODE_STRU* pstItr;
    ALM_STORE_INFO_BASE_CLS_DATA_FUNC fnDataCls;

    ALM_CHK_RET_VOID(NULL != pstStoreQuery, (NULL));
    ALM_CHK_RET_VOID(NULL != pstStoreQuery->pstStore,
        ("invalid store query"));

    fnDataCls = pstStoreQuery->pstStore->stStoreInfo.stBaseFuncs.fnDataCls;
    pstItr = pstStoreQuery->stDataHead.pstNext;
    while (pstItr != NULL)
    {
        pstDel = pstItr;
        pstItr = pstItr->pstNext;

        if (NULL != fnDataCls)
        {
            fnDataCls(pstDel->pData,
                pstStoreQuery->pstStore->stStoreInfo.pEx);
        }
        else
        {
            ALM_memset_s(pstDel->pData,
                pstStoreQuery->pstStore->stStoreInfo.uiUnitSize,
                0,
                pstStoreQuery->pstStore->stStoreInfo.uiUnitSize);
        }
        ALM_OsBaseMemFree(pstDel->pData);
        ALM_OsBaseMemFree(pstDel);
    }

    pstStoreQuery->uiCount = 0;
    pstStoreQuery->stDataHead.pstNext = NULL;
}

/* 添加存储查询结果 */
ALM_INT32 ALM_StoreQueryAdd(INOUT ALM_STORE_QUERY_STRU* pstStoreQuery,
    CONST ALM_VOID* pData)
{
    ALM_STORE_QUERY_NODE_STRU* pstNewNode;
    ALM_STORE_INFO_BASE_CPY_DATA_FUNC fnDataCpy;
    ALM_INT32 iRet;

    ALM_CHK_RET(NULL != pstStoreQuery && pData != NULL,
        ALM_ERR_NULL_PTR, (NULL));
    ALM_CHK_RET(NULL != pstStoreQuery->pstStore,
        ALM_ERR_INVALID_PARA, ("invalid store query"));

    /* 分配空间 */
    pstNewNode = (ALM_STORE_QUERY_NODE_STRU*)ALM_OsBaseMemAlloc(
        sizeof(ALM_STORE_QUERY_NODE_STRU));
    ALM_CHK_RET(pstNewNode != NULL, ALM_ERR_OS_MALLOC_FAIL, (NULL));

    pstNewNode->pData = ALM_OsBaseMemAlloc(
        pstStoreQuery->pstStore->stStoreInfo.uiUnitSize);
    iRet = ALM_ERR_OS_MALLOC_FAIL;
    ALM_CHK_LAB(pstNewNode->pData != NULL, LAB_ERR_FREE_NODE, (NULL));

    /* 拷贝数据 */
    fnDataCpy = pstStoreQuery->pstStore->stStoreInfo.stBaseFuncs.fnDataCpy;
    if (fnDataCpy != NULL)
    {
        iRet = fnDataCpy(pstNewNode->pData,
            pData,
            pstStoreQuery->pstStore->stStoreInfo.pEx);
        ALM_CALL_LAB(LAB_ERR_FREE_ALL, (NULL));
    }
    else
    {
        ALM_memcpy_s(pstNewNode->pData,
            pstStoreQuery->pstStore->stStoreInfo.uiUnitSize,
            pData,
            pstStoreQuery->pstStore->stStoreInfo.uiUnitSize);
    }

    /* 在头部添加 */
    pstNewNode->pstNext = pstStoreQuery->stDataHead.pstNext;
    pstStoreQuery->stDataHead.pstNext = pstNewNode;
    ++pstStoreQuery->uiCount;

    return ALM_OK;

LAB_ERR_FREE_ALL:
    ALM_OsBaseMemFree(pstNewNode->pData);

LAB_ERR_FREE_NODE:
    ALM_OsBaseMemFree(pstNewNode);

    return iRet;
}

/* 初始化存储器 */
ALM_INT32 ALM_StoreInit(OUT ALM_STORE_STRU* pstStore,
    ALM_STORE_TYPE_ENUM eStoreType,
    CONST ALM_STORE_INFO_STRU* pstStoreInfo)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstStore != NULL && pstStoreInfo != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(ALM_StoreInfoCheck(pstStoreInfo), ALM_ERR_INVALID_PARA,
        ("store info is invalid."));

    pstStore->eStoreType = eStoreType;
    ALM_memcpy_s(&pstStore->stStoreInfo,
        sizeof(ALM_STORE_INFO_STRU),
        pstStoreInfo,
        sizeof(ALM_STORE_INFO_STRU));

    switch (eStoreType)
    {
    case ALM_STORE_TYPE_LIST:
        pstStore->pStore = ALM_OsBaseMemAlloc(sizeof(ALM_STORE_LIST_STRU));
        ALM_CHK_RET(pstStore->pStore != NULL, ALM_ERR_OS_MALLOC_FAIL,
            (NULL));

        iRet = ALM_StoreListInit((ALM_STORE_LIST_STRU*)pstStore->pStore,
            &pstStore->stStoreInfo);
        ALM_CALL_RET(iRet, (NULL));
        break;

    case ALM_STORE_TYPE_TREE:
    case ALM_STORE_TYPE_ARRAY:
        ALM_LOG_ERROR(("not support this data structure."));
        return ALM_ERR_INVALID_PARA;

    default:
        ALM_LOG_ERROR(("invalid type of the data structure."));
        return ALM_ERR_INVALID_PARA;
    }

    return ALM_OK;
}

/* 清除存储器 */
ALM_VOID ALM_StoreCls(INOUT ALM_STORE_STRU* pstStore)
{
    ALM_CHK_RET_VOID(pstStore != NULL, (NULL));

    switch (pstStore->eStoreType)
    {
    case ALM_STORE_TYPE_LIST:
        ALM_StClsStoreList((ALM_STORE_LIST_STRU*)pstStore->pStore);
        break;

    case ALM_STORE_TYPE_TREE:
    case ALM_STORE_TYPE_ARRAY:
        ALM_LOG_ERROR(("not support this data structure."));
        return ;

    default:
        ALM_LOG_ERROR(("invalid type of the data structure."));
        return ;
    }

    ALM_OsBaseMemFree(pstStore->pStore);

    return ;
}

/* 得到已存储数据元得数目 */
ALM_INT32 ALM_StoreGetTotalCnt(CONST ALM_STORE_STRU* pstStore, ALM_UINT32* puiCnt)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstStore != NULL, ALM_ERR_NULL_PTR, (NULL));

    switch (pstStore->eStoreType)
    {
    case ALM_STORE_TYPE_LIST:
        iRet = ALM_StoreListGetTotal((ALM_STORE_LIST_STRU*)pstStore->pStore,
            puiCnt);
        ALM_CALL_RET(iRet, (NULL));
        break;

    case ALM_STORE_TYPE_TREE:
    case ALM_STORE_TYPE_ARRAY:
        ALM_LOG_ERROR(("not support this data structure."));
        return ALM_ERR_INVALID_PARA;

    default:
        ALM_LOG_ERROR(("invalid type of the data structure."));
        return ALM_ERR_INVALID_PARA;
    }

    return ALM_OK;
}

/* 添加新数据的拷贝 */
ALM_INT32 ALM_StoreAddData(ALM_STORE_STRU* pstStore, CONST ALM_VOID* pData)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstStore != NULL, ALM_ERR_NULL_PTR, (NULL));

    switch (pstStore->eStoreType)
    {
    case ALM_STORE_TYPE_LIST:
        iRet = ALM_StoreListAdd((ALM_STORE_LIST_STRU*)pstStore->pStore,
            pData);
        ALM_CALL_RET(iRet, (NULL));
        break;

    case ALM_STORE_TYPE_TREE:
    case ALM_STORE_TYPE_ARRAY:
        ALM_LOG_ERROR(("not support this data structure."));
        return ALM_ERR_INVALID_PARA;

    default:
        ALM_LOG_ERROR(("invalid type of the data structure."));
        return ALM_ERR_INVALID_PARA;
    }

    return ALM_OK;
}

/* 删除指定数据 */
ALM_INT32 ALM_StoreDelData(ALM_STORE_STRU* pstStore, CONST ALM_VOID* pMark)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstStore != NULL, ALM_ERR_NULL_PTR, (NULL));

    switch (pstStore->eStoreType)
    {
    case ALM_STORE_TYPE_LIST:
        iRet = ALM_StoreListDel((ALM_STORE_LIST_STRU*)pstStore->pStore,
            pMark);
        ALM_CALL_RET(iRet, (NULL));
        break;

    case ALM_STORE_TYPE_TREE:
    case ALM_STORE_TYPE_ARRAY:
        ALM_LOG_ERROR(("not support this data structure."));
        return ALM_ERR_INVALID_PARA;

    default:
        ALM_LOG_ERROR(("invalid type of the data structure."));
        return ALM_ERR_INVALID_PARA;
    }

    return ALM_OK;
}

/* 得到存储器中数据的拷贝 */
ALM_INT32 ALM_StoreGetData(CONST ALM_STORE_STRU* pstStore,
    CONST ALM_VOID* pMark,
    OUT ALM_STORE_QUERY_STRU* pstStoreQuery)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_CHK_RET(pstStore != NULL, ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_StoreQueryInit(pstStoreQuery, pstStore);
    ALM_CALL_RET(iRet, (NULL));

    switch (pstStore->eStoreType)
    {
    case ALM_STORE_TYPE_LIST:
        iRet = ALM_StoreListGet((ALM_STORE_LIST_STRU*)pstStore->pStore,
            pMark,
            pstStoreQuery);
        ALM_CALL_LAB(LAB_END, (NULL));
        break;

    case ALM_STORE_TYPE_TREE:
    case ALM_STORE_TYPE_ARRAY:
        ALM_LOG_ERROR(("not support this data structure."));
        iRet = ALM_ERR_INVALID_PARA;
        goto LAB_END;

    default:
        ALM_LOG_ERROR(("invalid type of the data structure."));
        iRet = ALM_ERR_INVALID_PARA;
        goto LAB_END;
    }
    
LAB_END:

    if (iRet != ALM_OK)
    {
        ALM_StoreQueryCls(pstStoreQuery);
    }
    return iRet;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/
