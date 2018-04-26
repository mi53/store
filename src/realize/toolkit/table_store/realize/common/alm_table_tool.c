/*******************************************************************************
Description    : 扩展并完善工具对象。
FileName       : alm_table_tool.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_tool.h"
#include "alm_tools.h"

#include <string.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/************************************************************************/
/*         ALM_TBL_DATA_STRU: 注意正确处理未字节对齐的整形数据。               */
/************************************************************************/

/* 确定数据类型的有效性 */
ALM_BOOL ALM_TblDataTypeCheck(ALM_TBL_DATA_TYPE_ENUM eType)
{
    switch (eType)
    {
    case ALM_TBL_DATA_TYPE_UCHAR:
    case ALM_TBL_DATA_TYPE_UINT32:
    case ALM_TBL_DATA_TYPE_UINT64:
    case ALM_TBL_DATA_TYPE_CHAR32:
    case ALM_TBL_DATA_TYPE_CHAR64:
    case ALM_TBL_DATA_TYPE_CHAR128:
    case ALM_TBL_DATA_TYPE_CHAR256:
    case ALM_TBL_DATA_TYPE_CHAR512:
        return TRUE;
    default:
        return FALSE;
    }
}

/* 获取指定数据类型大小 */
ALM_INT32 ALM_TblDataSize(ALM_TBL_DATA_TYPE_ENUM eType,
    OUT ALM_UINT32* puiSize)
{
    ALM_CHK_RET(puiSize != NULL, ALM_ERR_NULL_PTR, (NULL));

    switch (eType)
    {
    case ALM_TBL_DATA_TYPE_UCHAR:
        *puiSize = ALM_TBL_DATA_TYPE_UCHAR_LEN;
        break;

    case ALM_TBL_DATA_TYPE_UINT32:
        *puiSize = ALM_TBL_DATA_TYPE_UINT32_LEN;
        break;

    case ALM_TBL_DATA_TYPE_UINT64:
        *puiSize = ALM_TBL_DATA_TYPE_UINT64_LEN;
        break;

    case ALM_TBL_DATA_TYPE_CHAR32:
        *puiSize = ALM_TBL_DATA_TYPE_CHAR32_LEN;
        break;

    case ALM_TBL_DATA_TYPE_CHAR64:
        *puiSize = ALM_TBL_DATA_TYPE_CHAR64_LEN;
        break;

    case ALM_TBL_DATA_TYPE_CHAR128:
        *puiSize = ALM_TBL_DATA_TYPE_CHAR128_LEN;
        break;

    case ALM_TBL_DATA_TYPE_CHAR256:
        *puiSize = ALM_TBL_DATA_TYPE_CHAR256_LEN;
        break;

    case ALM_TBL_DATA_TYPE_CHAR512:
        *puiSize = ALM_TBL_DATA_TYPE_CHAR512_LEN;
        break;

    default:
        *puiSize = 0;
        return ALM_ERR_INVALID_PARA;
    }

    return ALM_OK;
}

/* 初始化数据项 */
ALM_INT32 ALM_TblDataInit(OUT ALM_TBL_DATA_STRU* pstData,
    ALM_TBL_DATA_TYPE_ENUM eType)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiSize;

    ALM_CHK_RET(pstData != NULL, ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_TblDataSize(eType, &uiSize);
    ALM_CALL_RET(iRet, ("Get data size fail."));

    pstData->eType = eType;
    pstData->uiDataSize = uiSize;
    pstData->pData = ALM_OsMemAllocAndFillZero(uiSize);

    ALM_CHK_RET(pstData->pData != NULL, ALM_ERR_OS_MALLOC_FAIL, (NULL));

    return iRet;
}

/* 依据字符串及指定数据类型，得到对应的数据对象 */
ALM_INT32 ALM_TblDataInitByStrVal(OUT ALM_TBL_DATA_STRU* pstData,
    ALM_TBL_DATA_TYPE_ENUM eType,
    CONST ALM_CHAR* pcVal)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstData != NULL && pcVal != NULL, ALM_ERR_NULL_PTR, (NULL));
    
    iRet = ALM_TblDataInit(pstData, eType);
    ALM_CALL_RET(iRet, (NULL));
    
    iRet = ALMTblDataWrite(pstData, pcVal);
    ALM_CALL_LAB(LAB_END, (NULL));

LAB_END:
    if (iRet != ALM_OK)
    {
        ALM_StClsTblData(pstData);
    }
    return iRet;
}

/* 写入数据，不会重新分配空间，非字符串类型数据大小必须一致，字符串类型容量必须足够 */
ALM_INT32 ALM_TblDataWriteByPtr(INOUT ALM_TBL_DATA_STRU* pstData,
    ALM_TBL_DATA_TYPE_ENUM eType,
    CONST ALM_VOID* pData,
    ALM_UINT32 uiDataSize)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiStrLen;

    ALM_CHK_RET(pData != NULL && uiDataSize > 0, ALM_ERR_NULL_PTR, (NULL));
    ALM_CHK_RET(ALM_TblDataTypeCheck(eType), ALM_ERR_INVALID_PARA, (NULL));

    if (ALMTblDataCheckStr(eType))
    {
        uiStrLen = ALM_Strlen((CONST ALM_CHAR*)pData, uiDataSize);

        ALM_CHK_LAB(!(uiStrLen >= pstData->uiDataSize
            && (iRet = ALM_ERR_INVALID_PARA) != ALM_OK),
            LAB_END, ("string is too long to the data."));

        (ALM_VOID)ALM_memset_s(pstData->pData, pstData->uiDataSize, 0,
            pstData->uiDataSize);

        (ALM_VOID)ALM_strncpy_s((ALM_CHAR*)pstData->pData,
            pstData->uiDataSize,
            (CONST ALM_CHAR*)pData,
            uiStrLen);
    }
    else
    {
        ALM_CHK_LAB(!(uiDataSize != pstData->uiDataSize
            && (iRet = ALM_ERR_INVALID_PARA) != ALM_OK),
            LAB_END, (NULL));

        (ALM_VOID)ALM_memcpy_s(pstData->pData, pstData->uiDataSize, pData,
            uiDataSize);
    }

LAB_END:
    if (iRet != ALM_OK)
    {
        ALM_StClsTblData(pstData);
    }
    return iRet;
}

/* 清理data对象 */
ALM_VOID ALM_StClsTblData(INOUT ALM_TBL_DATA_STRU* pstData)
{
    if (pstData != NULL)
    {
        ALM_OsBaseMemFree(pstData->pData);
        (ALM_VOID)ALM_memset_s(pstData, sizeof(ALM_TBL_DATA_STRU), 0,
            sizeof(ALM_TBL_DATA_STRU));
    }
}


/* 改变数据字节序*/
ALM_INT32 ALM_TblDataChangeEndin(INOUT ALM_TBL_DATA_STRU* pstData)
{
    ALM_CHK_RET(pstData != NULL && pstData->pData != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    if (!ALMTblDataCheckStr(pstData->eType))
    {
        ALM_ToolChangeEndin(pstData->pData, pstData->uiDataSize);
    }

    return ALM_OK;
}

/* 判断是否为字符串类型 */
static ALM_BOOL ALMTblDataCheckStr(ALM_TBL_DATA_TYPE_ENUM eType)
{
    switch (eType)
    {
    case ALM_TBL_DATA_TYPE_UCHAR:
    case ALM_TBL_DATA_TYPE_UINT32:
    case ALM_TBL_DATA_TYPE_UINT64:
        return FALSE;
    default:
        return TRUE;
    }
}

/* 将字符串转换为data对应的数据类型，并写入数据对象 */
static ALM_INT32 ALMTblDataWrite(INOUT ALM_TBL_DATA_STRU* pstData,
    CONST ALM_CHAR* pcVal)
{
    ALM_INT32 iRet;
    ALM_UINT32 uiValLen;
    ALM_UINT64 ulValTmp;
    ALM_UINT32 uiValTmp;
    ALM_CHAR ucValTmp;
    void* pNew;

    ALM_CHK_RET(pstData != NULL && pcVal != NULL, ALM_ERR_NULL_PTR, (NULL));
    ALM_CHK_RET(ALM_TblDataTypeCheck(pstData->eType), ALM_ERR_INVALID_PARA,
        (NULL));
    
    if (!ALMTblDataCheckStr(pstData->eType))
    {
        iRet = ALM_ToolAtoul(pcVal, 0, &ulValTmp);
        ALM_CALL_RET(iRet, (NULL));

        if (ALM_TBL_DATA_TYPE_UINT32 == pstData->eType)
        {
            uiValTmp = (ALM_UINT32)ulValTmp;
            pNew = &uiValTmp;
            uiValLen = sizeof(uiValTmp);
        }
        else if (ALM_TBL_DATA_TYPE_UCHAR == pstData->eType)
        {
            ucValTmp = (ALM_UCHAR)ulValTmp;
            pNew = &ucValTmp;
            uiValLen = sizeof(ucValTmp);
        }
        else
        {
            pNew = &ulValTmp;
            uiValLen = sizeof(ulValTmp);
        }

        /* 考虑数据可能未字节对齐，使用mmemcpy赋值 */
        (ALM_VOID)ALM_memcpy_s(pstData->pData, pstData->uiDataSize, pNew,
            uiValLen);
    }
    else
    {
        uiValLen = ALM_Strlen(pcVal, ALM_TBL_DATA_TYPE_MAX_LEN);
        ALM_CHK_RET(uiValLen < ALM_TBL_DATA_TYPE_MAX_LEN,
            ALM_ERR_INVALID_PARA, (NULL));

        ALM_CHK_RET(uiValLen < pstData->uiDataSize,
            ALM_ERR_INVALID_PARA,
            ("string is too ALM_INT64 to the data."));

        (ALM_VOID)ALM_strncpy_s((ALM_CHAR*)pstData->pData, pstData->uiDataSize,
            pcVal, uiValLen);
        ((ALM_CHAR*)pstData->pData)[pstData->uiDataSize - 1] = '\0';
    }

    return ALM_OK;
}


/* 比较数据大小，数据类型必须一致 */
ALM_INT32 ALM_TblDataCmp(CONST ALM_TBL_DATA_STRU* pstLeft,
    CONST ALM_TBL_DATA_STRU* pstRight,
    ALM_INT32* piRlst)
{
    ALM_UINT32 uiLen;

    ALM_CHK_RET(pstLeft != NULL && pstRight != NULL && piRlst != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(pstLeft->pData != NULL && pstRight->pData != NULL
        && pstLeft->eType == pstRight->eType
        && ALM_TblDataTypeCheck(pstLeft->eType),
        ALM_ERR_INVALID_PARA, (NULL));

    if (ALMTblDataCheckStr(pstLeft->eType))
    {
        uiLen = pstLeft->uiDataSize > pstRight->uiDataSize
            ? pstLeft->uiDataSize : pstRight->uiDataSize;
        *piRlst = strncmp((CONST ALM_CHAR*)pstLeft->pData,
            (CONST ALM_CHAR*)pstRight->pData,
            uiLen);
    }
    else
    {
        ALM_CHK_RET(pstLeft->uiDataSize == pstRight->uiDataSize
            && pstLeft->uiDataSize > 0,
            ALM_ERR_INVALID_PARA, (NULL));

        *piRlst = memcmp(pstLeft->pData, pstRight->pData, pstLeft->uiDataSize);
        if (ALM_ToolIsBigEndin())
        {
            *piRlst *= -1;
        }
    }

    return ALM_OK;
}


/* 获取数据内容，数值数据类型，buf长度必须与数据类型大小保持一致，string类型只需能存储便可 */
ALM_INT32 ALM_TblDataGet(CONST ALM_TBL_DATA_STRU* pstData,
    ALM_TBL_DATA_TYPE_ENUM eDataType,
    OUT ALM_VOID* pBuf,
    ALM_UINT32 uiBufLen)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstData != NULL && pBuf != NULL, ALM_ERR_NULL_PTR, (NULL));
    ALM_CHK_RET(ALM_TblDataTypeCheck(pstData->eType) && pstData->pData != NULL
        && pstData->eType == eDataType, ALM_ERR_INVALID_PARA, (NULL));

    if (ALMTblDataCheckStr(eDataType))
    {
        iRet = ALM_strncpy_s((ALM_CHAR*)pBuf, uiBufLen,
            (ALM_CHAR*)pstData->pData,
            pstData->uiDataSize);
        ALM_CHK_RET(iRet == EOK, ALM_ERR_INVALID_PARA,
            ("Copy col string value fail, maybe buffer is not enough."));
    }
    else
    {
        ALM_CHK_RET(pstData->uiDataSize == uiBufLen, ALM_ERR_INVALID_PARA,
            ("Get result col value with different size."));
        (ALM_VOID)ALM_memcpy_s(pBuf, uiBufLen, pstData->pData,
            pstData->uiDataSize);
    }

    return ALM_OK;
}

/************************************************************************/
/*              ALM_TBL_CDT_STRU                                    */
/************************************************************************/

/* 清理已初始化的查询条件 */
ALM_VOID ALM_StClsTblCdt(INOUT ALM_TBL_CDT_STRU* pstCdt)
{
    if (pstCdt != NULL)
    {
        ALM_StClsTblSRow(&pstCdt->stCols);
    }
}

/* 增加列条件 */
ALM_INT32 ALM_TblCdtAddColCdts(INOUT ALM_TBL_CDT_STRU* pstCdt,
    CONST ALM_TBL_SROW_STRU* pstColCdts)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet = ALM_OK;

    ALM_CHK_RET(pstCdt != NULL && pstColCdts != NULL,
        ALM_ERR_NULL_PTR, (NULL));
    ALM_CHK_RET(pstColCdts->uiMapsNum > 0 && pstColCdts->pstMaps != NULL,
        ALM_ERR_INVALID_PARA, (NULL));

    /* 扩展存储空间 */
    iRet = ALMTblSRowsExpand(&pstCdt->stCols, pstColCdts->uiMapsNum);
    ALM_CALL_RET(iRet, (NULL));

    /* 添加新的col条件 */
    for (uiIdx = 0; uiIdx < pstColCdts->uiMapsNum; ++uiIdx)
    {
        iRet = ALM_TblSRowsAdd(&pstCdt->stCols,
            pstColCdts->pstMaps[uiIdx].acColName,
            &(pstColCdts->pstMaps[uiIdx].stVal),
            NULL);
        ALM_CALL_RET(iRet, (NULL));
    }
    
    return ALM_OK;
}

/* 增加列条件 */
ALM_INT32 ALM_TblCdtAdd(INOUT ALM_TBL_CDT_STRU* pstCdt,
    CONST ALM_CHAR* pcColName,
    CONST ALM_TBL_DATA_STRU* pstVal)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_CHK_RET(pstCdt != NULL && pcColName != NULL
        && pstVal != NULL, ALM_ERR_NULL_PTR, (NULL));

    /* 扩展存储空间 */
    iRet = ALMTblSRowsExpand(&pstCdt->stCols, 1);
    ALM_CALL_RET(iRet, (NULL));

    iRet = ALM_TblSRowsAdd(&pstCdt->stCols, pcColName, pstVal, NULL);
    ALM_CALL_RET(iRet, (NULL));

    return ALM_OK;
}

/************************************************************************/
/*               ALM_TBL_COL_MAP_STRU                               */
/************************************************************************/


/* 初始化单个列条件，不再使用应使用函数ALMTblQueryColCdtCls清理 */
static ALM_INT32 ALMStInitTblSRowsCol(OUT ALM_TBL_COL_MAP_STRU* pstColMap,
    CONST ALM_CHAR* pcColName,
    CONST ALM_TBL_DATA_STRU* pstVal)
{
    ALM_UINT32 uiNameLen;
    ALM_INT32 iRet;

    ALM_CHK_RET(pstColMap != NULL && pcColName != NULL
        && pstVal != NULL, ALM_ERR_NULL_PTR, (NULL));

    uiNameLen = ALM_Strlen(pcColName, sizeof(pstColMap->acColName));
    ALM_CHK_RET(uiNameLen > 0 && uiNameLen < sizeof(pstColMap->acColName),
        ALM_ERR_INVALID_PARA, ("Col name is more than max value."));

    (ALM_VOID)ALM_strncpy_s(pstColMap->acColName,
        sizeof(pstColMap->acColName),
        pcColName,
        uiNameLen);

    iRet = ALM_TblDataInit(&pstColMap->stVal, pstVal->eType);
    ALM_CALL_RET(iRet, (NULL));

    iRet = ALM_TblDataWriteByPtr(&pstColMap->stVal,
        pstVal->eType,
        pstVal->pData,
        pstVal->uiDataSize);
    ALM_CALL_RET(iRet, (NULL));


    return ALM_OK;
}

/* 清理已初始的列，清理后应重新初始化才能使用 */
static ALM_VOID ALMStClsTblSRowsCol(INOUT ALM_TBL_COL_MAP_STRU* pstColMap)
{
    if (pstColMap != NULL)
    {
        ALM_StClsTblData(&pstColMap->stVal);
        ALM_memset_s(pstColMap, sizeof(ALM_TBL_COL_MAP_STRU), 0,
            sizeof(ALM_TBL_COL_MAP_STRU));
    }
}

/* 更新一个数据行，只更新指定列 */
ALM_INT32 ALM_TblSRowsToUpdataRow(CONST ALM_TBL_SROW_STRU* pstSRows,
    CONST ALM_TBL_COLS_STRU* pstCols,
    INOUT ALM_TBL_ROW_STRU* pstRow)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet;

    ALM_CHK_RET(pstSRows != NULL && pstCols != NULL
        && pstRow != NULL, ALM_ERR_NULL_PTR, (NULL));

    for (uiIdx = 0; uiIdx < pstSRows->uiMapsNum; ++uiIdx)
    {
        iRet = ALM_TblRowUpdata(pstRow,
            pstCols,
            pstSRows->pstMaps[uiIdx].acColName,
            &(pstSRows->pstMaps[uiIdx].stVal));
        ALM_CALL_RET(iRet, (NULL));
    }

    return ALM_OK;
}



/* 初始化单个列条件，不再使用应使用函数ALM_StClsTblSRow清理 */
ALM_INT32 ALM_StInitTblSRows(OUT ALM_TBL_SROW_STRU* pstSRows,
    ALM_UINT32 uiInitCap)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstSRows != NULL, ALM_ERR_NULL_PTR, (NULL));

    pstSRows->uiMapsCap = 0;
    pstSRows->uiMapsNum = 0;
    pstSRows->pstMaps = NULL;

    iRet = ALMTblSRowsExpand(pstSRows, uiInitCap);
    ALM_CALL_RET(iRet, (NULL));

    return ALM_OK;
}

/* 清理已初始的列，清理后应重新初始化才能使用 */
ALM_VOID ALM_StClsTblSRow(INOUT ALM_TBL_SROW_STRU* pstSRows)
{
    ALM_UINT32 uiIdx;

    if (pstSRows != NULL)
    {
        if (pstSRows->uiMapsNum > 0 && pstSRows->pstMaps != NULL)
        {
            for (uiIdx = 0; uiIdx < pstSRows->uiMapsNum; ++uiIdx)
            {
                ALMStClsTblSRowsCol(pstSRows->pstMaps + uiIdx);
            }
        }
        
        ALM_OsBaseMemFree(pstSRows->pstMaps);
        (ALM_VOID)ALM_memset_s(pstSRows, sizeof(ALM_TBL_SROW_STRU), 0,
            sizeof(ALM_TBL_SROW_STRU));
    }
}

/* 扩展COL MAPS存储空间，保证总容量与当前数目的差值大于等于指定的扩展数目 */
static ALM_INT32 ALMTblSRowsExpand(INOUT ALM_TBL_SROW_STRU* pstSRows,
    ALM_UINT32 uiExpand)
{
    ALM_INT32 iRet;

    ALM_CHK_RET(pstSRows != NULL, ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_ToolArrayExpandAuto((ALM_VOID**)&pstSRows->pstMaps,
        sizeof(ALM_TBL_COL_MAP_STRU),
        pstSRows->uiMapsNum,
        &pstSRows->uiMapsCap,
        uiExpand);
    ALM_CALL_RET(iRet, (NULL));

    return ALM_OK;
}

/* 增加列-值对，并得到添加后所在索引 */
/* piIdx - 若为NULL，不输出索引 */
ALM_INT32 ALM_TblSRowsAdd(INOUT ALM_TBL_SROW_STRU* pstSRows,
    CONST ALM_CHAR* pcColName,
    CONST ALM_TBL_DATA_STRU* pstVal,
    OUT ALM_INT32* piIdx)
{
    ALM_INT32 iRet;
    ALM_INT32 idx;

    ALM_CHK_RET(pstSRows != NULL, ALM_ERR_NULL_PTR, (NULL));
    
    iRet = ALM_TblSRowsGetIdx(pstSRows, pcColName, &idx);
    ALM_CALL_RET(iRet, (NULL));
    ALM_CHK_RET(idx < 0, ALM_ERR_INVALID_PARA, ("Col have exist."));

    iRet = ALMTblSRowsExpand(pstSRows, 1);
    ALM_CALL_RET(iRet, (NULL));

    iRet = ALMStInitTblSRowsCol(pstSRows->pstMaps + pstSRows->uiMapsNum,
        pcColName,
        pstVal);
    ALM_CALL_RET(iRet, (NULL));

    if (NULL != piIdx)
    {
        *piIdx = (ALM_INT32)pstSRows->uiMapsNum;
    }

    ++pstSRows->uiMapsNum;

    return ALM_OK;
}

/* 得到对应列的值 */
ALM_INT32 ALM_TblSRowsGet(IN CONST ALM_TBL_SROW_STRU* pstSRows,
    CONST ALM_CHAR* pcColName,
    OUT CONST ALM_TBL_DATA_STRU** ppstVal)
{
    ALM_INT32 iRet;
    ALM_INT32 idx;

    ALM_CHK_RET(pstSRows != NULL && pcColName != NULL && ppstVal != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_TblSRowsGetIdx(pstSRows, pcColName, &idx);
    ALM_CALL_RET(iRet, (NULL));
    ALM_CHK_RET(idx >= 0, ALM_ERR_INVALID_PARA, ("Col do not exist."));

    *ppstVal = &(pstSRows->pstMaps[idx].stVal);

    return ALM_OK;
}

/* 得到某一列的索引，若不存在指定列得到的索引为负值 */
ALM_INT32 ALM_TblSRowsGetIdx(CONST ALM_TBL_SROW_STRU* pstSRows,
    CONST ALM_CHAR* pcColName,
    OUT ALM_INT32* piIdx)
{
    ALM_INT32 iCmp;
    ALM_UINT32 uiIdx;

    ALM_CHK_RET(pstSRows != NULL && pcColName != NULL
        && piIdx != NULL, ALM_ERR_NULL_PTR, (NULL));

    *piIdx = -1;
    for (uiIdx = 0; uiIdx < pstSRows->uiMapsNum; ++uiIdx)
    {
        iCmp = strcmp(pcColName, pstSRows->pstMaps[uiIdx].acColName);
        if (0 == iCmp)
        {
            *piIdx = (ALM_INT32)uiIdx;
            break;
        }
    }

    return ALM_OK;
}


/************************************************************************/
/*                         ALM_TBL_RSLT_STRU                            */
/************************************************************************/

/* 清理已初始化的查询包。 */
ALM_VOID ALM_StClsTblRslt(INOUT ALM_TBL_RSLT_STRU* pstRslt)
{
    ALM_UINT32 uiIdx;
    ALM_TBL_RSLT_NODE_STRU* pstItr;
    ALM_TBL_RSLT_NODE_STRU* pstItrDel;

    if (pstRslt != NULL)
    {
        for (uiIdx = 0; uiIdx < pstRslt->uiColNum; ++uiIdx)
        {
            ALM_OsBaseMemFree(pstRslt->pstCols[uiIdx].pcColName);
        }
        ALM_OsBaseMemFree(pstRslt->pstCols);

        pstItr = pstRslt->stEmptHead.pstNext;
        while (NULL != pstItr)
        {
            pstItrDel = pstItr;
            pstItr = pstItr->pstNext;

            ALM_OsBaseMemFree(pstItrDel->pData);
            ALM_OsBaseMemFree(pstItrDel);
        }

        (ALM_VOID)ALM_memset_s(pstRslt, sizeof(ALM_TBL_RSLT_STRU), 0,
            sizeof(ALM_TBL_RSLT_STRU));
    }
}

/* 添加待查询列 */
ALM_INT32 ALM_TblRsltAddCol(INOUT ALM_TBL_RSLT_STRU* pstRslt,
    CONST ALM_CHAR* pstCol,
    ALM_TBL_DATA_TYPE_ENUM eDataType)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_CHK_RET(pstRslt != NULL && pstCol != NULL, ALM_ERR_INVALID_PARA,
        (NULL));

    /* 扩展空间 */
    iRet = ALM_ToolArrayExpandAuto((ALM_VOID**)&(pstRslt->pstCols),
        sizeof(ALM_TBL_RSLT_COL_UNIT_STRU),
        pstRslt->uiColNum,
        &pstRslt->uiColCap,
        1);
    ALM_CALL_RET(iRet, (NULL));

    /* 得到数据大小 */
    iRet = ALM_TblDataSize(eDataType,
        &pstRslt->pstCols[pstRslt->uiColNum].uiDataSize);
    ALM_CALL_RET(iRet, (NULL));

    /* 得到数据类型 */
    pstRslt->pstCols[pstRslt->uiColNum].eDataType = eDataType;

    /* 得到数据偏移 */
    if (pstRslt->uiColNum > 0)
    {
        pstRslt->pstCols[pstRslt->uiColNum].uiOffSet = 
            pstRslt->pstCols[pstRslt->uiColNum - 1].uiDataSize
            + pstRslt->pstCols[pstRslt->uiColNum - 1].uiOffSet;
    }
    else
    {
        pstRslt->pstCols[pstRslt->uiColNum].uiOffSet = 0;
    }

    /* 得到列名 */
    pstRslt->pstCols[pstRslt->uiColNum].pcColName = 
        (ALM_CHAR*)ALM_OsBaseMemAlloc(ALM_CONST_NAME_MAX_LEN + 1);

    ALM_CHK_RET(pstRslt->pstCols[pstRslt->uiColNum].pcColName != NULL,
        ALM_ERR_OS_MALLOC_FAIL, (NULL));

    (ALM_VOID)ALM_strcpy_s(pstRslt->pstCols[pstRslt->uiColNum].pcColName,
        ALM_CONST_NAME_MAX_LEN + 1,
        pstCol);

    /* 更新整体信息 */
    pstRslt->uiNodeDataLen += pstRslt->pstCols[pstRslt->uiColNum].uiDataSize;
    ++pstRslt->uiColNum;


    return iRet;
}


/* 获取列在结果集中的索引，若不存在得到负数 */
ALM_INT32 ALM_TblRsltGetColIdx(CONST ALM_TBL_RSLT_STRU* pstRslt,
    CONST ALM_CHAR* pcColName,
    OUT ALM_INT32* piColIdx)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iCmp;

    ALM_CHK_RET(pstRslt != NULL && piColIdx != NULL && pcColName != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    *piColIdx = -1;
    for (uiIdx = 0; uiIdx < pstRslt->uiColNum; ++uiIdx)
    {
        iCmp = strcmp(pstRslt->pstCols[uiIdx].pcColName, pcColName);
        if (0 == iCmp)
        {
            *piColIdx = (ALM_INT32)uiIdx;
            break;
        }
    }

    return ALM_OK;
}

/* 依据列索引得到列的值 */
ALM_INT32 ALM_TblRsltGetColByIdx(CONST ALM_TBL_RSLT_NODE_STRU* pstNode,
    ALM_INT32 iColIdx,
    OUT void* pBuf,
    ALM_UINT32 uiBufLen)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_TBL_DATA_STRU stTmp;
    ALM_TBL_RSLT_STRU* pstRslt;

    ALM_CHK_RET(pBuf != NULL && pstNode != NULL && pstNode->pstRslt != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    pstRslt = (ALM_TBL_RSLT_STRU*)pstNode->pstRslt;

    ALM_CHK_RET(iColIdx >= 0 && ((ALM_UINT32)iColIdx) < pstRslt->uiColNum,
        ALM_ERR_INVALID_PARA, (NULL));

    stTmp.pData = (ALM_CHAR*)pstNode->pData
        + pstRslt->pstCols[iColIdx].uiOffSet;

    stTmp.eType = pstRslt->pstCols[iColIdx].eDataType;

    iRet = ALM_TblDataSize(stTmp.eType, &stTmp.uiDataSize);
    ALM_CALL_RET(iRet, (NULL));

    iRet = ALM_TblDataGet(&stTmp, stTmp.eType, pBuf, uiBufLen);
    ALM_CALL_RET(iRet, (NULL));

    return ALM_OK;
}

/* 获取列在结果集中的指定列的值 */
ALM_INT32 ALM_TblRsltGetCol(CONST ALM_TBL_RSLT_NODE_STRU* pstNode,
    CONST ALM_CHAR* pcColName,
    OUT void* pBuf,
    ALM_UINT32 uiBufLen)
{
    ALM_INT32 iRet;
    ALM_INT32 iColIdx;

    ALM_CHK_RET(pstNode != NULL && pcColName != NULL && pBuf != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_TblRsltGetColIdx((ALM_TBL_RSLT_STRU*)pstNode->pstRslt,
        pcColName, &iColIdx);
    ALM_CALL_RET(iRet, (NULL));
    ALM_CHK_RET(iColIdx >= 0, ALM_ERR_INVALID_PARA,
        ("Can not find col:\"%s\".", pcColName));

    iRet = ALM_TblRsltGetColByIdx(pstNode, iColIdx, pBuf, uiBufLen);
    ALM_CALL_RET(iRet, (NULL));

    return iRet;
}

/**
* 描述: 初始化查询包，不再使用应使用对应的函数清理。
* 参数:
* 1. pstRslt - 待初始化对象。
* 2. uiColNum - 指定列数。
* 3. pstCols - 指定要查询的列。
*/
ALM_INT32 ALM_TblRsltInitByCols(OUT ALM_TBL_RSLT_STRU* pstRslt,
    CONST ALM_TBL_COLS_STRU* pstCols)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiIdx;

    ALM_CHK_RET(pstRslt != NULL && pstCols != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(pstCols->uiColNum > 0 && pstCols->pstCols != NULL,
        ALM_ERR_INVALID_PARA, (NULL));

    (ALM_VOID)ALM_memset_s(pstRslt, sizeof(ALM_TBL_RSLT_STRU), 0,
        sizeof(ALM_TBL_RSLT_STRU));

    for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
    {
        iRet = ALM_TblRsltAddCol(pstRslt,
            pstCols->pstCols[uiIdx].acColName,
            pstCols->pstCols[uiIdx].eDataType);
        ALM_CALL_LAB(LAB_END, (NULL));
    }

LAB_END:
    if (iRet != ALM_OK)
    {
        ALM_StClsTblRslt(pstRslt);
    }

    return iRet;
}

/* 将数据行转换为查询结果 */
ALM_INT32 ALM_TblRsltAddRsltByRow(INOUT ALM_TBL_RSLT_STRU* pstRslt,
    CONST ALM_TBL_COLS_STRU* pstCols,
    CONST ALM_TBL_ROW_STRU* pstRow)
{
    ALM_TBL_DATA_STRU stTmp;
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet = ALM_OK;
    ALM_TBL_RSLT_NODE_STRU* pstNewNode = NULL;

    ALM_CHK_RET(pstRslt != NULL && pstCols != NULL
        && pstRow != NULL, ALM_ERR_NULL_PTR, (NULL));

    /* 创建新的结果行对象 */
    pstNewNode = (ALM_TBL_RSLT_NODE_STRU*)ALM_OsBaseMemAlloc(
        sizeof(ALM_TBL_RSLT_NODE_STRU));
    ALM_CHK_RET(pstNewNode != NULL, ALM_ERR_OS_MALLOC_FAIL, (NULL));
    
    pstNewNode->pData = ALM_OsBaseMemAlloc(pstRslt->uiNodeDataLen);
    ALM_CHK_LAB(!(NULL == pstNewNode->pData
        && (iRet = ALM_ERR_OS_MALLOC_FAIL) != ALM_OK), LAB_END, (NULL));

    /* 得到新的结果行 */
    for (uiIdx = 0; uiIdx < pstRslt->uiColNum; ++uiIdx)
    {
        stTmp.eType = pstRslt->pstCols[uiIdx].eDataType;
        stTmp.pData = (ALM_CHAR*)pstNewNode->pData
            + pstRslt->pstCols[uiIdx].uiOffSet;
        stTmp.uiDataSize = pstRslt->pstCols[uiIdx].uiDataSize;

        iRet = ALM_TblRowGetColVal(pstRow,
            pstCols,
            pstRslt->pstCols[uiIdx].pcColName,
            &stTmp);
        ALM_CALL_LAB(LAB_END, (NULL));
    }

    /* 添加新的结果 */
    pstNewNode->pstRslt = pstRslt;
    pstNewNode->pstNext = pstRslt->stEmptHead.pstNext;
    pstRslt->stEmptHead.pstNext = pstNewNode;
    ++pstRslt->uiRsltCount;

LAB_END:
    if (iRet != ALM_OK && pstNewNode != NULL)
    {
        ALM_OsBaseMemFree(pstNewNode->pData);
        ALM_OsBaseMemFree(pstNewNode);
    }

    return iRet;
}


/* 将列按照列名由小到大排序，并同时排序列内容，若需要会转换字节序 */
ALM_INT32 ALM_TblRsltSort(INOUT ALM_TBL_RSLT_STRU* pstRslt,
    ALM_BOOL bChangeEndin)
{
    ALM_UINT32 uiIdxCol;
    ALM_UINT32 uiIdxCol2;
    ALM_CHAR* pcNodeTmp;
    ALM_TBL_RSLT_COL_UNIT_STRU stColTmp;
    ALM_INT32 iCmp;
    ALM_BOOL bChange = FALSE;
    ALM_UINT32 uiOffset;
    ALM_TBL_RSLT_NODE_STRU* pstNode;

    ALM_CHK_RET(pstRslt != NULL, ALM_ERR_NULL_PTR, (NULL));

    /* 排序列，按名称从小到大排列 */
    for (uiIdxCol = 0; uiIdxCol < pstRslt->uiColNum; ++uiIdxCol)
    {
        for (uiIdxCol2 = 0; uiIdxCol2 < uiIdxCol; ++uiIdxCol2)
        {
            iCmp = strcmp(pstRslt->pstCols[uiIdxCol].pcColName,
                pstRslt->pstCols[uiIdxCol2].pcColName);
            if (iCmp < 0)
            {
                (ALM_VOID)ALM_memcpy_s(&stColTmp,
                    sizeof(stColTmp),
                    &pstRslt->pstCols[uiIdxCol2],
                    sizeof(pstRslt->pstCols[uiIdxCol2]));

                (ALM_VOID)ALM_memcpy_s(&pstRslt->pstCols[uiIdxCol2],
                    sizeof(pstRslt->pstCols[uiIdxCol2]),
                    &pstRslt->pstCols[uiIdxCol],
                    sizeof(pstRslt->pstCols[uiIdxCol]));

                (ALM_VOID)ALM_memcpy_s(&pstRslt->pstCols[uiIdxCol],
                    sizeof(pstRslt->pstCols[uiIdxCol]),
                    &stColTmp,
                    sizeof(stColTmp));

                bChange = TRUE;
            }
        }
    }

    /* 重新整理列内容 */
    if ((bChange || bChangeEndin) && NULL != pstRslt->stEmptHead.pstNext)
    {
        ALM_TBL_RSLT_COL_UNIT_STRU* pstCol;

        pcNodeTmp = (ALM_CHAR*)ALM_OsBaseMemAlloc(pstRslt->uiNodeDataLen);
        ALM_CHK_RET(pcNodeTmp != NULL, ALM_ERR_OS_MALLOC_FAIL, (NULL));

        pstNode = pstRslt->stEmptHead.pstNext;
        while (pstNode != NULL)
        {
            uiOffset = 0;
            for (uiIdxCol = 0; uiIdxCol < pstRslt->uiColNum; ++uiIdxCol)
            {
                pstCol = &pstRslt->pstCols[uiIdxCol];

                (ALM_VOID)ALM_memcpy_s(pcNodeTmp + uiOffset,
                    pstCol->uiDataSize,
                    (ALM_CHAR*)pstNode->pData + pstCol->uiOffSet,
                    pstCol->uiDataSize);

                if (bChangeEndin && !ALMTblDataCheckStr(pstCol->eDataType))
                {
                    ALM_ToolChangeEndin(pcNodeTmp + uiOffset,
                        pstCol->uiDataSize);
                }
                uiOffset += pstCol->uiDataSize;
            }

            (ALM_VOID)ALM_memcpy_s(pstNode->pData,
                pstRslt->uiNodeDataLen,
                pcNodeTmp,
                pstRslt->uiNodeDataLen);

            pstNode = pstNode->pstNext;
        }

        ALM_OsBaseMemFree(pcNodeTmp);
    }

    /* 刷新列offset */
    for (uiIdxCol = 0; uiIdxCol < pstRslt->uiColNum; ++uiIdxCol)
    {
        if (uiIdxCol != 0)
        {
            pstRslt->pstCols[uiIdxCol].uiOffSet = 
                pstRslt->pstCols[uiIdxCol - 1].uiOffSet
                + pstRslt->pstCols[uiIdxCol - 1].uiDataSize;
        }
        else
        {
            pstRslt->pstCols[uiIdxCol].uiOffSet = 0;
        }
    }

    
    return ALM_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
