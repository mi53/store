/*******************************************************************************
Description    : 定义表格数据行。
FileName       : alm_talbe_rows.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_rows.h"
#include "alm_table_row_store.h"
#include "alm_tools.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/* 初始化表格数据行对象，应使用函数ALM_StClsTblRows清理 */
ALM_INT32 ALM_TblRowsInit(OUT ALM_TBL_ROWS_STRU* pstRows,
    CONST ALM_TBL_COLS_STRU* pstCols)
{
    ALM_INT32 iRet;
    ALM_STORE_INFO_STRU stRowsStoreInfo = {0};

    ALM_CHK_RET(pstRows != NULL && pstCols != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(pstCols->uiColNum > 0 && pstCols->pstCols != NULL,
        ALM_ERR_INVALID_PARA, ("No col to init rows."));

    /* 使用外部定义cols */
    pstRows->pstCols = pstCols;

    /* 指定row store初始化信息 */
    stRowsStoreInfo.stBaseFuncs.fnDataInit = ALM_TblRowStoreInit;
    stRowsStoreInfo.stBaseFuncs.fnDataCls = ALM_TblRowStoreCls;
    stRowsStoreInfo.stBaseFuncs.fnDataCpy = ALM_TblRowStoreCpy;
    stRowsStoreInfo.stBaseFuncs.fnMarkDataCmp = ALM_TblRowStoreMarkCmp;

    stRowsStoreInfo.stSortFuncs.fnDataMarkNew = NULL;
    stRowsStoreInfo.stSortFuncs.fnDataMarkFree = NULL;

    stRowsStoreInfo.bIncSame = TRUE;
    stRowsStoreInfo.uiUnitSize = sizeof(ALM_TBL_ROW_STRU);
    stRowsStoreInfo.pEx = (CONST ALM_VOID*)pstCols;

    /* 先用List实现基本功能，后期优化存储结构 ???? */
    iRet = ALM_StoreInit(&pstRows->stRows,
        ALM_STORE_TYPE_LIST,
        &stRowsStoreInfo);
    ALM_CALL_RET(iRet, ("Init rows store fail."));

    return ALM_OK;
}


/* 清理已初始化的数据行，清理后应重新初始化再使用 */
ALM_VOID ALM_StClsTblRows(INOUT ALM_TBL_ROWS_STRU* pstRows)
{
    if (pstRows != NULL)
    {
        ALM_StoreCls(&pstRows->stRows);
        (ALM_VOID)ALM_memset_s(pstRows, sizeof(ALM_TBL_ROWS_STRU), 0,
            sizeof(ALM_TBL_ROWS_STRU));
    }
}


/**
* 描述: 向rows对象中插入数据。
* 参数:
* 1. pstRows - 待插入数据的rows对象。
* 2. pstNewRow - 指定新插入数据行中数据列及列对应的值。
*/
ALM_INT32 ALM_TblRowsInsert(INOUT ALM_TBL_ROWS_STRU* pstRows,
    CONST ALM_TBL_SROW_STRU* pstNewRow)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_TBL_ROW_STRU stTmpRow = {0};

    ALM_CHK_RET(pstRows != NULL && pstNewRow != NULL
        && pstRows->pstCols != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_TblRowInit(&stTmpRow, pstRows->pstCols);
    ALM_CALL_LAB(LAB_END, (NULL));

    iRet = ALM_TblSRowsToUpdataRow(pstNewRow,
        pstRows->pstCols,
        &stTmpRow);
    ALM_CALL_LAB(LAB_END, (NULL));

    iRet = ALM_StoreAddData(&pstRows->stRows, &stTmpRow);
    ALM_CALL_LAB(LAB_END, (NULL));
    
LAB_END:
    ALM_StClsTblRow(&stTmpRow);
    return iRet;
}

/**
* 描述: 删除rows对象中符合要求的数据行。
* 参数:
* 1. pstRows - 待删除数据的rows对象。
* 2. pstCdt - 待删除的数据行应符合的条件，若传入NULL删除全部数据。
*/
ALM_INT32 ALM_TblRowsDel(INOUT ALM_TBL_ROWS_STRU* pstRows,
    CONST ALM_TBL_CDT_STRU* pstCdt)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstRows != NULL, ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_StoreDelData(&pstRows->stRows, (CONST ALM_VOID*)pstCdt);
    ALM_CALL_RET(iRet, ("Row delete fail."));

    return ALM_OK;
}

/**
* 描述: 更新rows对象中数据行。
* 参数:
* 1. pstRows - 待更新数据的rows对象。
* 2. pstCdt - 需要更新数据的数据行应满足的条件，若传入NULL更新全部数据。
* 3. pstNewVal - 指定待更新的列及对应的新值。
*/
ALM_INT32 ALM_TblRowsUpdata(INOUT ALM_TBL_ROWS_STRU* pstRows,
    CONST ALM_TBL_CDT_STRU* pstCdt,
    CONST ALM_TBL_SROW_STRU* pstNewVal)
{
    ALM_INT32 iRet;
    ALM_STORE_QUERY_NODE_STRU* pstItr;
    ALM_STORE_QUERY_STRU stQuery = {0};
    ALM_TBL_ROW_STRU stTmp = {0};

    ALM_CHK_RET(pstRows != NULL && pstNewVal != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    /* 找到要更新的数据 */
    iRet = ALM_StoreGetData(&pstRows->stRows,
        (CONST ALM_VOID*)pstCdt,
        &stQuery);
    ALM_CALL_RET(iRet, ("Rows updata fail."));
    if (0 == stQuery.uiCount)
    {
        return ALM_OK;
    }

    /* 删除原有数据行 */
    iRet = ALM_TblRowsDel(pstRows, pstCdt);
    ALM_CALL_LAB(LAB_ERR, (NULL));

    /* 更新数据 */
    pstItr = stQuery.stDataHead.pstNext;
    while (pstItr != NULL)
    {
        /* 保存原有数据 */
        iRet = ALM_TblRowCpy((CONST ALM_TBL_ROW_STRU*)pstItr->pData, &stTmp);
        ALM_CALL_LAB(LAB_ERR, (NULL));

        /* 更新数据行 */
        iRet = ALM_TblSRowsToUpdataRow(pstNewVal, pstRows->pstCols, &stTmp);
        ALM_CALL_LAB(LAB_ERR, (NULL));

        /* 添加新的数据行 */
        iRet = ALM_StoreAddData(&pstRows->stRows, &stTmp);
        ALM_CALL_LAB(LAB_ERR, (NULL));

        /* 迭代 */
        ALM_StClsTblRow(&stTmp);
        pstItr = pstItr->pstNext;
    }

    ALM_StoreQueryCls(&stQuery);

    return ALM_OK;

LAB_ERR:
    ALM_StClsTblRow(&stTmp);
    ALM_StoreQueryCls(&stQuery);

    return iRet;
}

/**
* 描述: 查询rows对象中数据行。调用该函数后应外部清理查询结果。
* 参数:
* 1. pstRows - 待更新数据的表格。
* 2. pstCdt - 所得数据行应满足的条件，若为NULL则无条件限制。
* 3. pstRslt - 查询得到的全部数据行，会得到指定列的对应的数据值，若没指定列得到全部列的值。
*/
ALM_INT32 ALM_TblRowsQuery(CONST ALM_TBL_ROWS_STRU* pstRows,
    CONST ALM_TBL_CDT_STRU* pstCdt,
    INOUT ALM_TBL_RSLT_STRU* pstRslt)
{
    ALM_INT32 iRet;
    ALM_STORE_QUERY_NODE_STRU* pstItr;
    ALM_STORE_QUERY_STRU stQuery = {0};

    ALM_CHK_RET(pstRows != NULL && pstRslt != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    /* 若没有指定列，指定全部列 */
    if (0 == pstRslt->uiColNum || NULL == pstRslt->pstCols)
    {
        iRet = ALM_TblRsltInitByCols(pstRslt, pstRows->pstCols);
        ALM_CALL_RET(iRet, (NULL));
    }

    /* 找到要查询的数据 */
    iRet = ALM_StoreGetData(&pstRows->stRows,
        (CONST ALM_VOID*)pstCdt,
        &stQuery);
    ALM_CALL_RET(iRet, ("Rows query fail."));

    /* 添加查询结果 */
    pstItr = stQuery.stDataHead.pstNext;
    while (pstItr != NULL)
    {
        iRet = ALM_TblRsltAddRsltByRow(pstRslt,
            pstRows->pstCols,
            (CONST ALM_TBL_ROW_STRU*)pstItr->pData);
        ALM_CALL_LAB(LAB_ERR, ("Add row to query bag fail."));

        pstItr = pstItr->pstNext;
    }
    ALM_StoreQueryCls(&stQuery);

    return ALM_OK;

LAB_ERR:
    ALM_StoreQueryCls(&stQuery);

    return iRet;
}

/* 得到表格中总数据行数。*/
ALM_INT32 ALM_TblRowsTotal(CONST ALM_TBL_ROWS_STRU* pstRows, ALM_UINT32* puiRowTotal)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstRows != NULL && puiRowTotal != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    *puiRowTotal = 0;

    iRet = ALM_StoreGetTotalCnt(&pstRows->stRows, puiRowTotal);
    ALM_CALL_RET(iRet, ("Get row count fail."));


    return ALM_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
