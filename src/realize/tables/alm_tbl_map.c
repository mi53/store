/*******************************************************************************
Description    : 表格映射对象，便于操作。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_tbl_map.h"
#include "alm_tools.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/* 清理ALM_TBL_MAP_RSLT_STRU对象 */
ALM_VOID ALM_StClsTblMapRslt(INOUT ALM_TBL_MAP_RSLT_STRU* pstRslt)
{
    if (pstRslt != NULL)
    {
        ALM_OsBaseMemFree(pstRslt->pObjsArray);
        (ALM_VOID)ALM_memset_s(pstRslt, sizeof(ALM_TBL_MAP_RSLT_STRU), 0,
            sizeof(ALM_TBL_MAP_RSLT_STRU));
    }
}

/* 校验映射对象的有效性 */
ALM_INT32 ALM_TblMapCheck(CONST ALM_TBL_MAP_STRU* pstTblMap)
{
    CONST ALM_TBL_DIM_STRU* pstTblDim;
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdx2;
    ALM_TBL_MAP_COLS_STRU* pstColMaps;
    ALM_BOOL bVec1;
    ALM_BOOL bVec2;
    ALM_BOOL bVec3;
    ALM_TBL_COL_IDX_ENUM eColStart;
    ALM_TBL_COL_IDX_ENUM eColEnd;

    ALM_CHK_RET(pstTblMap != NULL, ALM_ERR_NULL_PTR, (NULL));

    /* 1. 校验表格 */
    pstTblDim = ALM_TblsGetDim(pstTblMap->eTbl);
    ALM_CHK_RET(pstTblDim != NULL, ALM_ERR_UNKNOW, ("Invalid table: %d.",
        pstTblMap->eTbl));

    eColStart = (ALM_TBL_COL_IDX_ENUM)pstTblDim->uiStartIdx;
    eColEnd = (ALM_TBL_COL_IDX_ENUM)(pstTblDim->uiStartIdx
        + pstTblDim->uiColNum - 1);

    /* 2. 校验列映射 */
    ALM_CHK_RET(pstTblMap->pstColMaps != NULL
        && pstTblMap->pstColMaps->pstArray != NULL
        && pstTblMap->pstColMaps->uiNum > 0,
        ALM_ERR_UNKNOW,
        ("Not appoint col map."));

    if (pstTblMap->pstColMaps->bNeedCheck)
    {
        pstColMaps = pstTblMap->pstColMaps;

        for (uiIdx = 0; uiIdx < pstColMaps->uiNum; ++uiIdx)
        {
            /* 必须为指定表格的列 */
            ALM_CHK_RET(pstColMaps->pstArray[uiIdx].eCol >= eColStart
                && pstColMaps->pstArray[uiIdx].eCol <= eColEnd,
                ALM_ERR_UNKNOW,
                ("Col %d not exist in table %d.", uiIdx, pstTblMap->eTbl));

            /* 列大小与列定义的期望大小保持一致 */
            uiIdx2 = pstColMaps->pstArray[uiIdx].eCol;
            ALM_CHK_RET(pstTblDim->pstColDim[uiIdx2].uiNeedSize
                == pstColMaps->pstArray[uiIdx].uiSize,
                ALM_ERR_UNKNOW,
                ("Col size is differ with dim, col is: %d.", uiIdx));

            /* 不能超出存储空间 */
            ALM_CHK_RET(pstColMaps->pstArray[uiIdx].uiOffset
                + pstColMaps->pstArray[uiIdx].uiSize <= pstTblMap->uiObjSize,
                ALM_ERR_UNKNOW,
                ("Col size is overflow to obj buffer, col is: %d.", uiIdx));

            /* 列空间不能与其他列存在交汇，且不允许存在重复列 */
            for (uiIdx2 = 0; uiIdx2 < uiIdx; ++uiIdx2)
            {
                ALM_CHK_RET(pstColMaps->pstArray[uiIdx].eCol
                    != pstColMaps->pstArray[uiIdx2].eCol,
                    ALM_ERR_UNKNOW,
                    ("Exist same col: %d - %d.", uiIdx, uiIdx2));

                bVec1 = pstColMaps->pstArray[uiIdx].uiOffset
                    < pstColMaps->pstArray[uiIdx2].uiOffset ? TRUE : FALSE;

                bVec2 = pstColMaps->pstArray[uiIdx].uiOffset
                    < (pstColMaps->pstArray[uiIdx2].uiOffset
                    + pstColMaps->pstArray[uiIdx2].uiSize)
                    ? TRUE : FALSE;

                bVec3 = (pstColMaps->pstArray[uiIdx].uiOffset
                    + pstColMaps->pstArray[uiIdx].uiSize)
                    <= pstColMaps->pstArray[uiIdx2].uiOffset ? TRUE : FALSE;

                ALM_CHK_RET(bVec1 == bVec2 && bVec1 == bVec3,
                    ALM_ERR_UNKNOW,
                    ("Col exist accross with %d and %d.", uiIdx, uiIdx2));
            }
        }

        pstColMaps->bNeedCheck = FALSE;
    }

    /* 3. 校验条件列 */
    ALM_CHK_RET(0 == pstTblMap->uiCdtColNum || pstTblMap->pstCdtCols != NULL,
        ALM_ERR_UNKNOW,
        ("Include cdt cols, but array is null."));
    for (uiIdx = 0; uiIdx < pstTblMap->uiCdtColNum; ++uiIdx)
    {
        /* 必须为指定表格的列 */
        ALM_CHK_RET(pstTblMap->pstCdtCols[uiIdx] >= eColStart
            && pstTblMap->pstCdtCols[uiIdx] <= eColEnd,
            ALM_ERR_UNKNOW,
            ("Col %d not exist in table %d.", uiIdx, pstTblMap->eTbl));
    }

    /* 4. 校验当前涉及列 */
    ALM_CHK_RET(0 == pstTblMap->uiColNum || pstTblMap->pstCols != NULL,
        ALM_ERR_UNKNOW,
        ("Include cols, but array is null."));
    for (uiIdx = 0; uiIdx < pstTblMap->uiColNum; ++uiIdx)
    {
        /* 必须为指定表格的列 */
        ALM_CHK_RET(pstTblMap->pstCols[uiIdx] >= eColStart
            && pstTblMap->pstCols[uiIdx] <= eColEnd,
            ALM_ERR_UNKNOW,
            ("Col %d not exist in table %d.", uiIdx, pstTblMap->eTbl));
    }

    return ALM_OK;
}

/* 使用映射对象实现添加数据 */
ALM_INT32 ALM_TblMapAdd(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObj,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet;
    CONST ALM_TBL_DIM_STRU* pstTblDim;
    ALM_TBL_SROW_STRU stNew = {0};
    ALM_TBL_DATA_STRU stTmp;
    CONST ALM_TBL_COL_DIM_EX_STRU* pstColDim;

    ALM_CHK_RET(pstTblMap != NULL && pObj != NULL, ALM_ERR_NULL_PTR, (NULL));

    pstTblDim = ALM_TblsGetDim(pstTblMap->eTbl);
    ALM_CHK_RET(pstTblDim != NULL, ALM_ERR_UNKNOW, ("Invalid table: %d.",
        pstTblMap->eTbl));

    /* 得到新数据行 */
    iRet = ALMTblMapObjChangeToSRow(pstTblMap, NULL, pObj, &stNew);
    ALM_CALL_RET(iRet, (NULL));

    /* 添加附加列 */
    for (uiIdx = 0; uiIdx < uiExNum; ++uiIdx)
    {
        pstColDim = ALM_TblsGetColDim(pstColEx[uiIdx].eCol);
        ALM_CHK_LAB(
            !(NULL == pstTblDim && (iRet = ALM_ERR_INVALID_PARA) != ALM_OK),
            LAB_END, ("Invalid col: %d.", uiIdx));

        stTmp.eType = pstColDim->stCol.eDataType;
        stTmp.pData = (ALM_VOID*)pstColEx[uiIdx].pData;/* 不会写入 */
        stTmp.uiDataSize = pstColEx[uiIdx].uiSize;

        iRet = ALM_TblSRowsAdd(&stNew, pstColDim->stCol.pcColName, &stTmp, NULL);
        ALM_CALL_LAB(LAB_END, (NULL));
    }

    /* 添加数据 */
    iRet = ALM_TblInsert(pstTblDim->pcTblName, &stNew);
    ALM_CALL_LAB(LAB_END, (NULL));

LAB_END:
    ALM_StClsTblSRow(&stNew);
    return iRet;
}

/* 使用映射对象实现删除数据 */
/* pObjCdt - 若为NULL，不限制条件 */
ALM_INT32 ALM_TblMapRm(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjCdt,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum)
{
    ALM_INT32 iRet;
    CONST ALM_TBL_DIM_STRU* pstTblDim;
    ALM_TBL_CDT_STRU stCdt = {0};

    ALM_CHK_RET(pstTblMap != NULL, ALM_ERR_NULL_PTR, (NULL));

    pstTblDim = ALM_TblsGetDim(pstTblMap->eTbl);
    ALM_CHK_RET(pstTblDim != NULL, ALM_ERR_UNKNOW, ("Invalid table: %d.",
        pstTblMap->eTbl));

    /* 得到条件 */
    iRet = ALMTblMapObjToCdt(pstTblMap, pObjCdt, pstColEx, uiExNum, &stCdt);
    ALM_CALL_RET(iRet, (NULL));

    /* 移除数据 */
    iRet = ALM_TblRmRows(pstTblDim->pcTblName, &stCdt);
    ALM_CALL_LAB(LAB_END, (NULL));

LAB_END:
    ALM_StClsTblCdt(&stCdt);
    return iRet;
}

/* 使用映射对象实现更新，附加列只作条件 */
ALM_INT32 ALM_TblMapUpdata(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjNew,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum)
{
    ALM_INT32 iRet;
    CONST ALM_TBL_DIM_STRU* pstTblDim;
    ALM_TBL_SROW_STRU stNew = {0};
    ALM_TBL_CDT_STRU stCdt = {0};

    ALM_CHK_RET(pstTblMap != NULL && pObjNew != NULL, ALM_ERR_NULL_PTR, (NULL));

    pstTblDim = ALM_TblsGetDim(pstTblMap->eTbl);
    ALM_CHK_RET(pstTblDim != NULL, ALM_ERR_UNKNOW, ("Invalid table: %d.",
        pstTblMap->eTbl));

    /* 得到条件 */
    iRet = ALMTblMapObjToCdt(pstTblMap, pObjNew, pstColEx, uiExNum, &stCdt);
    ALM_CALL_LAB(LAB_END, (NULL));

    /* 得到新数据行 */
    iRet = ALMTblMapObjChangeToSRow(pstTblMap, NULL, pObjNew, &stNew);
    ALM_CALL_LAB(LAB_END, (NULL));

    /* 添加数据 */
    iRet = ALM_TblUpdata(pstTblDim->pcTblName, &stCdt, &stNew);
    ALM_CALL_LAB(LAB_END, (NULL));

LAB_END:
    ALM_StClsTblCdt(&stCdt);
    ALM_StClsTblSRow(&stNew);
    return iRet;
}

/* 使用映射对象实现查询，附加列只作条件，注意清理输出 */
/* pObjCdt - 若为NULL，不限制条件 */
ALM_INT32 ALM_TblMapQuery(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjCdt,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum,
    OUT ALM_TBL_MAP_RSLT_STRU* pstRslt)
{
    ALM_INT32 iRet;
    CONST ALM_TBL_DIM_STRU* pstTblDim;
    ALM_TBL_CDT_STRU stCdt = {0};
    ALM_TBL_RSLT_STRU stRslt = {0};
    ALM_TBL_RSLT_NODE_STRU* pstItr;

    ALM_CHK_RET(pstTblMap != NULL, ALM_ERR_NULL_PTR, (NULL));

    (ALM_VOID)ALM_memset_s(pstRslt, sizeof(ALM_TBL_MAP_RSLT_STRU), 0,
        sizeof(ALM_TBL_MAP_RSLT_STRU));

    pstTblDim = ALM_TblsGetDim(pstTblMap->eTbl);
    ALM_CHK_RET(pstTblDim != NULL, ALM_ERR_UNKNOW, ("Invalid table: %d.",
        pstTblMap->eTbl));

    /* 得到条件 */
    iRet = ALMTblMapObjToCdt(pstTblMap, pObjCdt, pstColEx, uiExNum, &stCdt);
    ALM_CALL_RET(iRet, (NULL));

    /* 添加需要查询的列 */
    iRet = ALMTblMapAddColToRslt(pstTblMap, &stRslt);
    ALM_CALL_LAB(LAB_END, (NULL));

    /* 查询数据 */
    iRet = ALM_TblQuery(pstTblDim->pcTblName, &stCdt, &stRslt);
    ALM_CALL_LAB(LAB_END, (NULL));

    /* 结果转换 */
    if (stRslt.uiRsltCount > 0)
    {
        iRet = ALM_ToolArrayExpandAuto((ALM_VOID**)&pstRslt->pObjsArray,
            pstTblMap->uiObjSize,
            pstRslt->uiNum,
            &pstRslt->uiCap,
            stRslt.uiRsltCount);
        ALM_CALL_LAB(LAB_END, (NULL));

        pstItr = stRslt.stEmptHead.pstNext;
        while (pstItr != NULL)
        {
            iRet = ALMTblMapRsltToObj(pstTblMap,
                (ALM_UINT8*)pstRslt->pObjsArray
                + pstTblMap->uiObjSize * pstRslt->uiNum,
                pstItr);
            ALM_CALL_LAB(LAB_END, (NULL));

            ++pstRslt->uiNum;
            pstItr = pstItr->pstNext;
        }
    }

LAB_END:
    ALM_StClsTblRslt(&stRslt);
    ALM_StClsTblCdt(&stCdt);
    return iRet;
}

/* 使用映射对象实现更新 */
/* pbChange - 若不为NULL，将得到是否存在数据变更的信息 */
ALM_INT32 ALM_TblMapUpdataByChange(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjOld,
    CONST ALM_VOID* pObjNew,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum,
    OUT ALM_BOOL* pbChange)
{
    ALM_INT32 iRet;
    CONST ALM_TBL_DIM_STRU* pstTblDim;
    ALM_TBL_SROW_STRU stNew = {0};
    ALM_TBL_CDT_STRU stCdt = {0};

    ALM_CHK_RET(pstTblMap != NULL && pObjNew != NULL && pObjOld != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    pstTblDim = ALM_TblsGetDim(pstTblMap->eTbl);
    ALM_CHK_RET(pstTblDim != NULL, ALM_ERR_UNKNOW, ("Invalid table: %d.",
        pstTblMap->eTbl));

    /* 得到条件 */
    iRet = ALMTblMapObjToCdt(pstTblMap, pObjNew, pstColEx, uiExNum, &stCdt);
    ALM_CALL_LAB(LAB_END, (NULL));

    /* 得到新数据行 */
    iRet = ALMTblMapObjChangeToSRow(pstTblMap, pObjOld, pObjNew, &stNew);
    ALM_CALL_LAB(LAB_END, (NULL));

    /* 添加数据 */
    if (stNew.uiMapsNum > 0)
    {
        iRet = ALM_TblUpdata(pstTblDim->pcTblName, &stCdt, &stNew);
        ALM_CALL_LAB(LAB_END, (NULL));
    }

    if (pbChange != NULL)
    {
        if (stNew.uiMapsNum > 0)
        {
            *pbChange = TRUE;
        }
        else
        {
            *pbChange = FALSE;
        }
    }

LAB_END:
    ALM_StClsTblCdt(&stCdt);
    ALM_StClsTblSRow(&stNew);
    return iRet;
}

/* 将obj转换为条件 */
/* pObj - 若为NULL不限制条件 */
static ALM_INT32 ALMTblMapObjToCdt(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObj,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum,
    INOUT ALM_TBL_CDT_STRU* pstCdt)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiIdx;
    CONST ALM_TBL_COL_DIM_EX_STRU* pstColDim;
    CONST ALM_TBL_MAP_COL_STRU* pstColMap;
    CONST ALM_TBL_MAP_COL_EX_STRU* pstEx;
    ALM_TBL_DATA_STRU stDataTmp;

    ALM_CHK_RET(pstTblMap != NULL && pstCdt != NULL, ALM_ERR_NULL_PTR, (NULL));
    ALM_CHK_RET(0 == pstTblMap->uiCdtColNum || pObj != NULL || pstColEx != NULL,
        ALM_ERR_INVALID_PARA, (NULL));

    for (uiIdx = 0; uiIdx < pstTblMap->uiCdtColNum; ++uiIdx)
    {
        pstColDim = ALM_TblsGetColDim(pstTblMap->pstCdtCols[uiIdx]);
        ALM_CHK_LAB(!(NULL == pstColDim && (iRet = ALM_ERR_UNKNOW) != ALM_OK),
            LAB_END, (NULL));

        /* 先查找附加列 */
        pstEx = ALMTblMapExGetCol(pstColEx, uiExNum, pstTblMap->pstCdtCols[uiIdx]);
        if (pstEx != NULL)
        {
            stDataTmp.eType = pstColDim->stCol.eDataType;
            stDataTmp.pData = (ALM_VOID*)pstEx->pData;/* 不会写入 */
            stDataTmp.uiDataSize = pstEx->uiSize;
        }
        else
        {
            /* 查找映射列 */
            pstColMap = ALMTblMapGetCol(pstTblMap, pstTblMap->pstCdtCols[uiIdx]);
            ALM_CHK_LAB(!(NULL == pstColMap && (iRet = ALM_ERR_UNKNOW) != ALM_OK),
                LAB_END, (NULL));

            stDataTmp.eType = pstColDim->stCol.eDataType;
            stDataTmp.pData = (ALM_CHAR*)pObj + pstColMap->uiOffset;
            stDataTmp.uiDataSize = pstColMap->uiSize;
        }

        iRet = ALM_TblCdtAdd(pstCdt, pstColDim->stCol.pcColName, &stDataTmp);
        ALM_CALL_LAB(LAB_END, (NULL));
    }

LAB_END:
    if (iRet != ALM_OK)
    {
        ALM_StClsTblCdt(pstCdt);
    }
    return iRet;
}

/* 将涉及列添加到查询结果中 */
static ALM_INT32 ALMTblMapAddColToRslt(CONST ALM_TBL_MAP_STRU* pstTblMap,
    INOUT ALM_TBL_RSLT_STRU* pstRslt)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiIdx;
    CONST ALM_TBL_COL_DIM_EX_STRU* pstColDim;
    ALM_TBL_COL_IDX_ENUM eColIdx;
    ALM_UINT32 uiNum;

    ALM_CHK_RET(pstTblMap != NULL && pstRslt != NULL, ALM_ERR_NULL_PTR,
        (NULL));
    ALM_CHK_RET(pstTblMap->pstColMaps != NULL, ALM_ERR_UNKNOW, (NULL));

    /* 若没有指定当前列，使用全部映射列，否则只添加涉及列 */
    uiNum = pstTblMap->pstCols != NULL ? pstTblMap->uiColNum
        : pstTblMap->pstColMaps->uiNum;
    for (uiIdx = 0; uiIdx < uiNum; ++uiIdx)
    {
        if (NULL == pstTblMap->pstCols)
        {
            eColIdx = pstTblMap->pstColMaps->pstArray[uiIdx].eCol;
        }
        else
        {
            eColIdx = pstTblMap->pstCols[uiIdx];
        }

        pstColDim = ALM_TblsGetColDim(eColIdx);
        ALM_CHK_LAB(!(NULL == pstColDim && (iRet = ALM_ERR_UNKNOW) != ALM_OK),
            LAB_END, ("Invalid col, idx - col: %d - %d.", uiIdx, eColIdx));

        iRet = ALM_TblRsltAddCol(pstRslt,
            pstColDim->stCol.pcColName,
            pstColDim->stCol.eDataType);
        ALM_CALL_LAB(LAB_END, (NULL));
    }

LAB_END:
    if (iRet != ALM_OK)
    {
        ALM_StClsTblRslt(pstRslt);
    }
    return iRet;
}

/* 将obj变动新转换为插入或更新数据行 */
static ALM_INT32 ALMTblMapObjChangeToSRow(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjOld,
    CONST ALM_VOID* pObjNew,
    OUT ALM_TBL_SROW_STRU* pstRow)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiNum;
    CONST ALM_TBL_MAP_COL_STRU* pstColMap;
    CONST ALM_TBL_COL_DIM_EX_STRU* pstColDim;
    ALM_TBL_DATA_STRU stDataNew;
    ALM_TBL_DATA_STRU stDataOld;
    ALM_INT32 iCmp;

    ALM_CHK_RET(pstTblMap != NULL && pObjNew != NULL && pstRow != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(pstTblMap->pstColMaps != NULL, ALM_ERR_UNKNOW, (NULL));
    
    /* 若没有指定当前列，使用全部映射列 */
    uiNum = pstTblMap->pstCols != NULL ? pstTblMap->uiColNum
        : pstTblMap->pstColMaps->uiNum;
    for (uiIdx = 0; uiIdx < uiNum; ++uiIdx)
    {
        if (NULL == pstTblMap->pstCols)
        {
            pstColMap = &pstTblMap->pstColMaps->pstArray[uiIdx];
        }
        else
        {
            pstColMap = ALMTblMapGetCol(pstTblMap, pstTblMap->pstCols[uiIdx]);
            ALM_CHK_LAB(
                !(NULL == pstColMap && (iRet = ALM_ERR_UNKNOW) != ALM_OK),
                LAB_END, (NULL));
        }

        pstColDim = ALM_TblsGetColDim(pstColMap->eCol);
        ALM_CHK_LAB(!(NULL == pstColDim && (iRet = ALM_ERR_UNKNOW) != ALM_OK),
            LAB_END,
            ("Invalid col, idx - col: %d - %d.", uiIdx, pstColMap->eCol));

        iCmp = -1;
        stDataNew.eType = pstColDim->stCol.eDataType;
        stDataNew.pData = (ALM_CHAR*)pObjNew + pstColMap->uiOffset;
        stDataNew.uiDataSize = pstColMap->uiSize;

        if (pObjOld != NULL)
        {
            stDataOld.eType = pstColDim->stCol.eDataType;
            stDataOld.pData = (ALM_CHAR*)pObjOld + pstColMap->uiOffset;
            stDataOld.uiDataSize = pstColMap->uiSize;

            iRet = ALM_TblDataCmp(&stDataNew, &stDataOld, &iCmp);
            ALM_CALL_LAB(LAB_END, (NULL));
        }

        if (iCmp != 0)
        {
            iRet = ALM_TblSRowsAdd(pstRow,
                pstColDim->stCol.pcColName,
                &stDataNew,
                NULL);
            ALM_CALL_LAB(LAB_END, (NULL));
        }
    }

LAB_END:
    if (iRet != ALM_OK)
    {
        ALM_StClsTblSRow(pstRow);
    }
    return iRet;
}

/* 将查询结果转换为obj */
static ALM_INT32 ALMTblMapRsltToObj(CONST ALM_TBL_MAP_STRU* pstTblMap,
    OUT ALM_VOID* pObj,
    CONST ALM_TBL_RSLT_NODE_STRU* pstRslt)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiIdx;
    CONST ALM_TBL_MAP_COL_STRU* pstColMap;
    CONST ALM_TBL_COL_DIM_EX_STRU* pstColDim;

    ALM_CHK_RET(pstTblMap != NULL && pObj != NULL && pstRslt != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(pstTblMap->pstColMaps != NULL, ALM_ERR_UNKNOW, (NULL));

    /* 若没有指定当前列，使用全部映射列 */
    uiIdx = 0;
    while (TRUE)
    {
        if (NULL == pstTblMap->pstCols && uiIdx < pstTblMap->pstColMaps->uiNum)
        {
            pstColMap = pstTblMap->pstColMaps->pstArray + uiIdx;
        }
        else if (NULL != pstTblMap->pstCols && uiIdx < pstTblMap->uiColNum)
        {
            pstColMap = ALMTblMapGetCol(pstTblMap, pstTblMap->pstCols[uiIdx]);
            ALM_CHK_LAB(
                !(NULL == pstColMap && (iRet = ALM_ERR_UNKNOW) != ALM_OK),
                LAB_END, (NULL));
        }
        else
        {
            break;
        }

        pstColDim = ALM_TblsGetColDim(pstColMap->eCol);
        ALM_CHK_LAB(
            !(NULL == pstColDim && (iRet = ALM_ERR_UNKNOW) != ALM_OK),
            LAB_END, (NULL));

        iRet = ALM_TblRsltGetCol(pstRslt,
            pstColDim->stCol.pcColName,
            (ALM_UINT8*)pObj + pstColMap->uiOffset,
            pstColMap->uiSize);
        ALM_CALL_LAB(LAB_END, (NULL));

        ++uiIdx;
    }


LAB_END:
    return iRet;
}


/* 得到映射列 */
static CONST ALM_TBL_MAP_COL_STRU* ALMTblMapGetCol(
    CONST ALM_TBL_MAP_STRU* pstTblMap,
    ALM_TBL_COL_IDX_ENUM eCol)
{
    ALM_UINT32 uiIdx;

    ALM_CHK_RET(pstTblMap != NULL && pstTblMap->pstColMaps != NULL, NULL,
        (NULL));

    for (uiIdx = 0; uiIdx < pstTblMap->pstColMaps->uiNum; ++uiIdx)
    {
        if (pstTblMap->pstColMaps->pstArray[uiIdx].eCol == eCol)
        {
            return &pstTblMap->pstColMaps->pstArray[uiIdx];
        }
    }

    return NULL;
}

/* 得到附加列 */
static CONST ALM_TBL_MAP_COL_EX_STRU* ALMTblMapExGetCol(
    CONST ALM_TBL_MAP_COL_EX_STRU* pstExs,
    ALM_UINT32 uiExNum,
    ALM_TBL_COL_IDX_ENUM eCol)
{
    ALM_UINT32 uiIdx;

    if (pstExs != NULL && uiExNum > 0)
    {
        for (uiIdx = 0; uiIdx < uiExNum; ++uiIdx)
        {
            if (pstExs[uiIdx].eCol == eCol)
            {
                return &pstExs[uiIdx];
            }
        }
    }


    return NULL;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
