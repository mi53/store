/*******************************************************************************
Description    : 定义表格数据行。
FileName       : alm_talbe_row.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_row.h"
#include "alm_table_tool.h"
#include "alm_tools.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */


/* 依据数据列初始化数据行 */
ALM_INT32 ALM_TblRowInit(ALM_TBL_ROW_STRU* pstRow,
    CONST ALM_TBL_COLS_STRU* pstCols)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiIdx;

    ALM_CHK_RET(pstRow != NULL && pstCols != NULL,
        ALM_ERR_NULL_PTR, (NULL));
    ALM_CHK_RET(pstCols->uiColNum > 0 && pstCols->pstCols != NULL,
        ALM_ERR_INVALID_PARA, ("no column to init row."));

    ALM_memset_s(pstRow, sizeof(ALM_TBL_ROW_STRU), 0,
        sizeof(ALM_TBL_ROW_STRU));

    /* 获取数据行大小 */
    iRet = ALMTblRowGetColsTotalSize(pstCols->pstCols,
        pstCols->uiColNum,
        &pstRow->uiSize);
    ALM_CALL_RET(iRet, ("Get cols total size fail."));
    ALM_CHK_RET(pstRow->uiSize > 0, ALM_ERR_INVALID_PARA,
        ("row size is zero."));

    /* 分配空间 */
    pstRow->pData = ALM_OsMemAllocAndFillZero(pstRow->uiSize);
    ALM_CHK_RET(pstRow->pData != NULL, ALM_ERR_OS_MALLOC_FAIL, 
        ("malloc fail."));

    /* 初始化默认值 */
    for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
    {
        if (pstCols->pstCols[uiIdx].bExistDflt)
        {
            iRet = ALM_TblRowUpdataByIdx(pstRow,
                pstCols,
                uiIdx,
                &pstCols->pstCols[uiIdx].stDfltVal);
            ALM_CALL_LAB(LAB_END, ("Updata col default value fail."));
        }
    }

LAB_END:
    if (iRet != ALM_OK)
    {
        ALM_StClsTblRow(pstRow);
        return iRet;
    }

    return iRet;
}

/* 清理已初始化的数据行，清理后应重新初始化再使用 */
ALM_VOID ALM_StClsTblRow(ALM_TBL_ROW_STRU* pstRow)
{
    if (pstRow != NULL)
    {
        if (pstRow != NULL)
        {
            ALM_OsBaseMemFree(pstRow->pData);
            (ALM_VOID)ALM_memset_s(pstRow, sizeof(ALM_TBL_ROW_STRU), 0,
                sizeof(ALM_TBL_ROW_STRU));
        }
    }
}

/**
* 描述: 从数据行中获取某一列的值。
* 参数:
* 1. pstRow - 获取数据的行。
* 2. pstCols - 与指定行对应的列信息。
* 3. pcColName - 需要获取的列。
* 4. pstData - 得到的数据拷贝。
* 返回值:
* 1. ALM_OK - 成功。
* 2. 其他 - 失败返回相应的错误码。
*/
ALM_INT32 ALM_TblRowGetColVal(CONST ALM_TBL_ROW_STRU* pstRow,
    CONST ALM_TBL_COLS_STRU* pstCols,
    CONST ALM_CHAR* pcColName,
    INOUT ALM_TBL_DATA_STRU* pstData)
{
    ALM_INT32 iRet;
    ALM_INT32 iColIdx;
    ALM_UINT32 uiSize;
    ALM_VOID* pDataStart;

    ALM_CHK_RET(pstRow != NULL && pstCols != NULL && pcColName != NULL
        && pstData != NULL, ALM_ERR_NULL_PTR, (NULL));

    /* 找到col索引，数据行中数据项索引与列索引一致 */
    iRet = ALM_TblColsGetColIdx(pstCols, pcColName, &iColIdx);
    ALM_CALL_RET(iRet, ("Can not find col: %s.", pcColName));
    ALM_CHK_RET(iColIdx >= 0, ALM_ERR_INVALID_PARA,
        ("Can not find col: %s.", pcColName));

    /* 得到数据存储内存段 */
    iRet = ALM_TblRowGetColData(pstRow, pstCols, iColIdx, &uiSize, &pDataStart);
    ALM_CALL_RET(iRet, ("Get col store fail."));

    /* 得到数据 */
    iRet = ALM_TblDataWriteByPtr(pstData,
        pstCols->pstCols[iColIdx].eDataType,
        pDataStart,
        uiSize);
    ALM_CALL_RET(iRet, ("Copy data fail."));


    return ALM_OK;
}

/* 更新数据行中某一列的值。 */
ALM_INT32 ALM_TblRowUpdata(INOUT ALM_TBL_ROW_STRU* pstRow,
    CONST ALM_TBL_COLS_STRU* pstCols,
    CONST ALM_CHAR* pcColName,
    CONST ALM_TBL_DATA_STRU* pstData)
{
    ALM_INT32 iRet;
    ALM_INT32 iColIdx;

    ALM_CHK_RET(pstRow != NULL && pstCols != NULL
        && pcColName != NULL && pstData != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    /* 找到col索引，数据行中数据项索引与列索引一致 */
    iRet = ALM_TblColsGetColIdx(pstCols, pcColName, &iColIdx);
    ALM_CALL_RET(iRet, ("Can not find col: %s.", pcColName));
    ALM_CHK_RET(iColIdx >= 0, ALM_ERR_INVALID_PARA,
        ("Can not find col: %s.", pcColName));

    /* 更新索引列 */
    iRet = ALM_TblRowUpdataByIdx(pstRow, pstCols, iColIdx, pstData);
    ALM_CALL_RET(iRet, ("Updata col value failed."));

    return ALM_OK;
}

/* 更新数据行中某一列的值。 */
ALM_INT32 ALM_TblRowUpdataByIdx(INOUT ALM_TBL_ROW_STRU* pstRow,
    CONST ALM_TBL_COLS_STRU* pstCols,
    ALM_UINT32 uiColIdx,
    CONST ALM_TBL_DATA_STRU* pstData)
{
    ALM_INT32 iRet;
    ALM_UINT32 uiSize;
    ALM_VOID* pDataStart;

    ALM_CHK_RET(pstRow != NULL && pstCols != NULL && pstData != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(uiColIdx < pstCols->uiColNum, ALM_ERR_INVALID_PARA, (NULL));

    ALM_CHK_RET(pstData->eType == pstCols->pstCols[uiColIdx].eDataType,
        ALM_ERR_INVALID_PARA, ("Data type is diffrent with the col data type."));

    /* 得到数据存储内存段 */
    iRet = ALM_TblRowGetColData(pstRow, pstCols, uiColIdx, &uiSize, &pDataStart);
    ALM_CALL_RET(iRet, ("Get col store fail."));
    ALM_CHK_RET(pstData->uiDataSize == uiSize,
        ALM_ERR_INVALID_PARA, ("Data len is diffrent with the col data len."));

    /* 更新数据 */
    ALM_memcpy_s(pDataStart, uiSize, pstData->pData, pstData->uiDataSize);

    return ALM_OK;
}


/* 拷贝一个数据行 */
ALM_INT32 ALM_TblRowCpy(CONST ALM_TBL_ROW_STRU* pstSrcRow,
    OUT ALM_TBL_ROW_STRU* pstDestRow)
{
    ALM_CHK_RET(pstSrcRow != NULL && pstDestRow != NULL,
        ALM_ERR_NULL_PTR, (NULL));
    ALM_CHK_RET(pstSrcRow->uiSize > 0 && pstSrcRow->pData != NULL,
        ALM_ERR_INVALID_PARA, (NULL));

    pstDestRow->uiSize = pstSrcRow->uiSize;
    pstDestRow->pData = ALM_OsMemAllocAndFillZero(pstDestRow->uiSize);
    ALM_CHK_RET(pstDestRow->pData != NULL,
        ALM_ERR_OS_MALLOC_FAIL, ("malloc fail."));

    ALM_memcpy_s(pstDestRow->pData,
        pstDestRow->uiSize,
        pstSrcRow->pData,
        pstSrcRow->uiSize);

    return ALM_OK;
}

/* 使用部分数据初始化数据行 */
ALM_INT32 ALM_TblRowInitByPartData(OUT ALM_TBL_ROW_STRU* pstRow,
    CONST ALM_TBL_COLS_STRU* pstCols,
    CONST ALM_TBL_COLS_STRU* pstPartCols,
    CONST ALM_TBL_ROW_STRU* pstPartData)
{
    ALM_INT32 iRet;
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiSize;
    ALM_VOID* pDataSrc;
    ALM_VOID * pDataDest;
    ALM_INT32 iColIdx;

    ALM_CHK_RET(pstRow != NULL && pstCols != NULL
        && pstPartCols != NULL && pstPartData != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_TblRowInit(pstRow, pstCols);
    ALM_CALL_RET(iRet, ("Init row fail."));

    for (uiIdx = 0; uiIdx < pstPartCols->uiColNum; ++uiIdx)
    {
        /* 获取源内存段 */
        iRet = ALM_TblRowGetColData(pstPartData,
            pstPartCols,
            uiIdx,
            &uiSize,
            &pDataSrc);
        ALM_CALL_LAB(LAB_ERR, ("Get col data fail."));

        /* 获取目标内存段 */
        iRet = ALM_TblColsGetColIdx(pstCols,
            pstPartCols->pstCols[uiIdx].acColName,
            &iColIdx);
        ALM_CALL_LAB(LAB_ERR,
            ("Can not find col: %s.", pstPartCols->pstCols[uiIdx].acColName));

        iRet = ALM_ERR_INVALID_PARA;
        ALM_CHK_LAB(iColIdx >= 0 && (pstCols->pstCols[iColIdx].eDataType
            == pstPartCols->pstCols[uiIdx].eDataType),
            LAB_ERR, ("Col is mismatch."));

        iRet = ALM_TblRowGetColData(pstRow,
            pstCols,
            iColIdx,
            &uiSize,
            &pDataDest);
        ALM_CALL_LAB(LAB_ERR, ("Get col data fail."));

        /* 更新数据 */
        ALM_memcpy_s(pDataDest, uiSize, pDataSrc, uiSize);
    }

    return ALM_OK;

LAB_ERR:

    ALM_StClsTblRow(pstRow);
    return iRet;
}

/* 数据行中获取指定列对应的内存起始位及大小 */
ALM_INT32 ALM_TblRowGetColData(CONST ALM_TBL_ROW_STRU* pstRow,
    CONST ALM_TBL_COLS_STRU* pstCols,
    ALM_UINT32 uiColIdx,
    OUT ALM_UINT32* puiSize,
    OUT ALM_VOID** ppData)
{
    ALM_INT32 iRet;
    ALM_UINT32 uiSize;
    ALM_VOID* pDataStart;

    ALM_CHK_RET(pstRow != NULL && pstCols != NULL
        && puiSize != NULL && ppData != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(uiColIdx < pstCols->uiColNum, ALM_ERR_INVALID_PARA, (NULL));

    *puiSize = 0;
    *ppData = NULL;

    /* 得到数据存储起始位 */
    iRet = ALMTblRowGetColsTotalSize(pstCols->pstCols, uiColIdx, &uiSize);
    ALM_CALL_RET(iRet, ("Get cols total size fail."));

    pDataStart = (ALM_VOID*)((ALM_CHAR*)pstRow->pData + uiSize);

    /* 得到数据大小 */
    iRet = ALMTblRowGetColsTotalSize(pstCols->pstCols + uiColIdx,
        1,
        &uiSize);
    ALM_CALL_RET(iRet, ("Get col size fail."));
    ALM_CHK_RET(uiSize > 0, ALM_ERR_INVALID_PARA, ("col size is zero."));

    *puiSize = uiSize;
    *ppData = pDataStart;

    return ALM_OK;
}

/* 存储获取多个列所需数据大小 */
static ALM_INT32 ALMTblRowGetColsTotalSize(CONST ALM_TBL_COL_STRU* pstCols,
    ALM_UINT32 uiColNum,
    ALM_UINT32* puiTotalSize)
{
    ALM_UINT32 uiSize;
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet;

    ALM_CHK_RET(pstCols != NULL && puiTotalSize != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    *puiTotalSize = 0;
    for (uiIdx = 0; uiIdx < uiColNum; ++uiIdx)
    {
        iRet = ALM_TblDataSize(pstCols[uiIdx].eDataType, &uiSize);
        ALM_CALL_RET(iRet, ("Get col data size fail."));

        (*puiTotalSize) += uiSize;
    }

    return ALM_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
