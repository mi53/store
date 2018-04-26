/*******************************************************************************
Description    : rows中存储row需要指定的一些函数。
FileName       : alm_talbe_row_store.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_row_store.h"
#include "alm_table_tool.h"
#include "alm_tools.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/* 初始化ROW对象 */
ALM_INT32 ALM_TblRowStoreInit(OUT ALM_VOID* pRow, CONST ALM_VOID* pCols)
{
    ALM_INT32 iRet;
    ALM_TBL_ROW_STRU* pstRow = (ALM_TBL_ROW_STRU*)pRow;
    CONST ALM_TBL_COLS_STRU* pstCols = (CONST ALM_TBL_COLS_STRU*)pCols;

    iRet = ALM_TblRowInit(pstRow, pstCols);
    ALM_CALL_RET(iRet, (NULL));

    return ALM_OK;
}

/* 清理ROW对象 */
ALM_VOID ALM_TblRowStoreCls(INOUT ALM_VOID* pRow, CONST ALM_VOID* pCols)
{
    ALM_TBL_ROW_STRU* pstRow = (ALM_TBL_ROW_STRU*)pRow;
    (ALM_VOID)pCols;

    ALM_StClsTblRow(pstRow);
}

/* Row拷贝 */
ALM_INT32 ALM_TblRowStoreCpy(OUT ALM_VOID* pDestRow,
    CONST ALM_VOID* pSrcRow,
    CONST ALM_VOID* pCols)
{
    ALM_TBL_ROW_STRU* pstDestRow = (ALM_TBL_ROW_STRU*)pDestRow;
    CONST ALM_TBL_ROW_STRU* pstSrcRow = (CONST ALM_TBL_ROW_STRU*)pSrcRow;
    ALM_INT32 iRet;

    (ALM_VOID)pCols;
    iRet = ALM_TblRowCpy(pstSrcRow, pstDestRow);
    ALM_CALL_RET(iRet, (NULL));

    return ALM_OK;
}

/* 得到条件MARK */
ALM_VOID* ALM_TblRowStoreMarkNew(CONST ALM_VOID* pRow, CONST ALM_VOID* pCols)
{
    ALM_INT32 iRet = ALM_OK;
    CONST ALM_TBL_COLS_STRU* pstCols = (CONST ALM_TBL_COLS_STRU*)pCols;
    CONST ALM_TBL_ROW_STRU* pstRow = (ALM_TBL_ROW_STRU*)pRow;
    ALM_TBL_CDT_STRU* pstRet = NULL;
    ALM_TBL_SROW_STRU stSRowsTmp = {0};
    ALM_TBL_DATA_STRU stDataTmp;
    ALM_UINT32 uiIdx;

    ALM_CHK_RET(pstCols != NULL, NULL,
        ("Get stroe mark with null para."));

    /* 创建row cdt对象 */
    pstRet = (ALM_TBL_CDT_STRU*)ALM_OsMemAllocAndFillZero(
        sizeof(ALM_TBL_CDT_STRU));
    ALM_CHK_RET(pstRet != NULL, NULL, ("malloc fail."));

    /* 得到col map 对象 */
    iRet = ALM_StInitTblSRows(&stSRowsTmp, pstCols->uiColNum);
    ALM_CHK_LAB(pstRet != NULL, LAB_END, (NULL));

    for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
    {
        stDataTmp.eType = pstCols->pstCols[uiIdx].eDataType;
        iRet = ALM_TblRowGetColData(pstRow,
            pstCols,
            uiIdx,
            &stDataTmp.uiDataSize,
            &stDataTmp.pData);
        ALM_CALL_LAB(LAB_END, (NULL));

        iRet = ALM_TblSRowsAdd(&stSRowsTmp,
            pstCols->pstCols[uiIdx].acColName,
            &stDataTmp,
            NULL);
        ALM_CALL_LAB(LAB_END, (NULL));
    }

    /* 初始化row cdt对象 */
    iRet = ALM_TblCdtAddColCdts(pstRet, &stSRowsTmp);
    ALM_CHK_LAB(pstRet != NULL, LAB_END, ("malloc fail."));
    
LAB_END:
    ALM_StClsTblSRow(&stSRowsTmp);

    if (iRet != ALM_OK)
    {
        ALM_StClsTblCdt(pstRet);
        ALM_OsBaseMemFree(pstRet);
        return NULL;
    }

    return pstRet;
}

/* 释放条件MARK */
ALM_VOID ALM_TblRowStoreMarkFree(INOUT ALM_VOID* pCdt, CONST ALM_VOID* pCols)
{
    ALM_TBL_CDT_STRU* pstCdt = (ALM_TBL_CDT_STRU*)pCdt;
    
    (ALM_VOID)pCols;

    ALM_StClsTblCdt(pstCdt);
}

/* 比较条件MARK与ROW大小，没有指定条件全部符合要求 */
ALM_INT32 ALM_TblRowStoreMarkCmp(CONST ALM_VOID* pCdt,
    CONST ALM_VOID* pRow,
    OUT ALM_INT32* piRslt,
    CONST ALM_VOID* pCols)
{
    CONST ALM_TBL_CDT_STRU* pstCdt = (CONST ALM_TBL_CDT_STRU*)pCdt;
    CONST ALM_TBL_ROW_STRU* pstRow = (ALM_TBL_ROW_STRU*)pRow;
    CONST ALM_TBL_COLS_STRU* pstCols = (CONST ALM_TBL_COLS_STRU*)pCols;

    ALM_INT32 iCmp = 0;
    ALM_INT32 iRet;
    ALM_UINT32 uiIdx;
    ALM_INT32 iColIdx;
    ALM_TBL_DATA_STRU stDataTmp; /* 仅作一个数据包使用 */

    ALM_CHK_RET(pstRow != NULL && pstCols != NULL && piRslt != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    if (NULL == pstCdt || 0 == pstCdt->stCols.uiMapsNum)
    {
        *piRslt = 0;
        return ALM_OK;
    }

    for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
    {

        /* 得到cdt中对应列 */
        iRet = ALM_TblSRowsGetIdx(&pstCdt->stCols,
            pstCols->pstCols[uiIdx].acColName,
            &iColIdx);
        ALM_CALL_RET(iRet, (NULL));

        /* 未指定列，默认符合条件 */
        if (iColIdx < 0)
        {
            continue;
        }

        /* 得到row中对应列的数据 */
        stDataTmp.eType = pstCols->pstCols[uiIdx].eDataType;
        iRet = ALM_TblRowGetColData(pstRow,
            pstCols,
            uiIdx,
            &stDataTmp.uiDataSize,
            &stDataTmp.pData);
        ALM_CALL_RET(iRet, (NULL));        

        /* stDataTmp仅为一个数据包，数据类型必须考虑字节对齐才能进行比较 */
        iRet = ALM_TblDataCmp(&(pstCdt->stCols.pstMaps[iColIdx].stVal),
            &stDataTmp,
            &iCmp);
        ALM_CALL_RET(iRet, (NULL));

        if (0 == iCmp)
        {
            continue;
        }
        else
        {
            break;
        }
    }
    *piRslt = iCmp;

    return ALM_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
