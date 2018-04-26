/*******************************************************************************
Description    : 定义表格列信息。
FileName       : alm_talbe_col.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_COL_H__
#define __ALM_TBLE_COL_H__

#include "alm_table_cols.h"
#include "alm_table_tool.h"
#include "alm_tools.h"

#include <string.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */


/* 描述: 初始化单个列，不再使用应使用函数ALMStClsTblCol清理。可以不指定默认值。*/
static ALM_INT32 ALMTblColInit(OUT ALM_TBL_COL_STRU* pstCol,
    CONST ALM_TBL_COL_DIM_STRU* pstColDim)
{
    ALM_UINT32 uiColNameLen;
    ALM_INT32 iRet;

    ALM_CHK_RET(pstCol != NULL && pstColDim != NULL,
        ALM_ERR_NULL_PTR,
        (NULL));

    (ALM_VOID)ALM_memset_s(pstCol, sizeof(ALM_TBL_COL_STRU), 0,
        sizeof(ALM_TBL_COL_STRU));

    /* 保存col name */
    uiColNameLen = ALM_Strlen(pstColDim->pcColName, sizeof(pstCol->acColName));

    ALM_CHK_RET(uiColNameLen > 0 && uiColNameLen < sizeof(pstCol->acColName),
        ALM_ERR_INVALID_PARA,
        ("Col name len is invalid."));

    (ALM_VOID)ALM_strncpy_s(pstCol->acColName,
        sizeof(pstCol->acColName) - 1,
        pstColDim->pcColName,
        uiColNameLen);

    pstCol->acColName[sizeof(pstCol->acColName) - 1] = '\0';

    /* 保存col type */
    ALM_CHK_RET(ALM_TblDataTypeCheck(pstColDim->eDataType),
        ALM_ERR_INVALID_PARA,
        ("Col data type is invalid."));
    pstCol->eDataType = pstColDim->eDataType;

    /* 保存默认值 */
    if (NULL != pstColDim->pcDfltVal)
    {
        iRet = ALM_TblDataInitByStrVal(&pstCol->stDfltVal,
            pstCol->eDataType,
            pstColDim->pcDfltVal);
        ALM_CALL_RET(iRet, ("Col deafult value is invalid."));

        pstCol->bExistDflt = TRUE;
    }
    else
    {
        pstCol->bExistDflt = FALSE;
    }

    return ALM_OK;
}

/* 清理已初始的列，清理后应重新初始化才能使用 */
static ALM_VOID ALMStClsTblCol(INOUT ALM_TBL_COL_STRU* pstCol)
{
    if (pstCol != NULL)
    {
        if (pstCol->bExistDflt)
        {
            ALM_StClsTblData(&pstCol->stDfltVal);
        }

        ALM_memset_s(pstCol, sizeof(ALM_TBL_COL_STRU), 0,
            sizeof(ALM_TBL_COL_STRU));
    }
}




/* 初始化全部列定义，不再使用应使用函数ALM_TblColsCls清理。 */
ALM_INT32 ALM_TblColsInit(OUT ALM_TBL_COLS_STRU* pstCols)
{
    ALM_CHK_RET(pstCols != NULL, ALM_ERR_NULL_PTR, (NULL));

    pstCols->uiColNum = 0;
    pstCols->pstCols = NULL;

    return ALM_OK;
}

/* 清理已初始的列，清理后应重新初始化才能使用 */
ALM_VOID ALMStClsTblCols(INOUT ALM_TBL_COLS_STRU* pstCols)
{
    ALM_UINT32 uiIdx;

    if (NULL != pstCols)
    {
        if (NULL != pstCols->pstCols)
        {
            for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
            {
                ALMStClsTblCol(pstCols->pstCols + uiIdx);
            }
            ALM_OsBaseMemFree(pstCols->pstCols);
        }

        pstCols->pstCols = NULL;
        pstCols->uiColNum = 0;
    }
}

/* 增加列 */
ALM_INT32 ALM_TblColsAddCol(INOUT ALM_TBL_COLS_STRU* pstCols,
    ALM_UINT32 uiColNum,
    CONST ALM_TBL_COL_DIM_STRU* pstColsDim)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiAddIdx = 0;

    ALM_CHK_RET(pstCols != NULL && pstColsDim != NULL && uiColNum > 0,
        ALM_ERR_NULL_PTR,
        (NULL));

    /* 扩增存储空间 */
    iRet = ALM_ToolArrayExpand((ALM_VOID**)&pstCols->pstCols,
        sizeof(ALM_TBL_COL_STRU),
        pstCols->uiColNum,
        uiColNum);
    ALM_CALL_RET(iRet, ("Expand col store fail."));

    /* 增加新列 */
    uiAddIdx = pstCols->uiColNum;
    for (uiIdx = 0; uiIdx < uiColNum; ++uiIdx)
    {
        iRet = ALMTblColInit(pstCols->pstCols + uiAddIdx, pstColsDim + uiIdx);
        ALM_CALL_LAB(LAB_END, ("Add invalid col."));

        ++uiAddIdx;
    }

    pstCols->uiColNum += uiColNum;


LAB_END:

    /* 移除新增列，额外扩增的内存不会被访问可以不用释放 */
    if (iRet != ALM_OK)
    {
        for (uiIdx = pstCols->uiColNum; uiIdx < uiAddIdx; ++uiIdx)
        {
            ALMStClsTblCol(pstCols->pstCols + uiIdx);
        }
    }

    return iRet;
}


/**
* 描述:获取列索引。
* 参数:
* 1. pstCols - 列存储对象。
* 2. pcColName - 待获取的列面。
* 3. piColIdx - 得到的对应列的索引。列不存在得到负值。
*/
ALM_INT32 ALM_TblColsGetColIdx(CONST ALM_TBL_COLS_STRU* pstCols,
    CONST ALM_CHAR* pcColName,
    OUT ALM_INT32* piColIdx)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iCmp;

    ALM_CHK_RET(pstCols != NULL && pcColName != NULL && piColIdx != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    *piColIdx = -1;

    for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
    {
        iCmp = strcmp(pstCols->pstCols[uiIdx].acColName, pcColName);
        if (0 == iCmp)
        {
            *piColIdx = (ALM_INT32)uiIdx;
            break;
        }
    }

    return ALM_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_COL_H__ */
