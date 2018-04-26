/*******************************************************************************
Description    : 实现alm_table_obj.h。
FileName       : alm_table_obj.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_obj.h"
#include "alm_tools.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


/* 初始化一个表格 */
ALM_INT32 ALM_TblObjInit(OUT ALM_TBL_OBJ_STRU* pstTbl,
    CONST ALM_CHAR* pcName,
    ALM_UINT32 uiColNum,
    CONST ALM_TBL_COL_DIM_STRU* pstColsDim)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiNameLen = ALM_Strlen(pcName, ALM_CONST_NAME_MAX_LEN + 1);

    /* 参数校验 */
    ALM_CHK_RET(pstTbl != NULL && pstColsDim != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    ALM_CHK_RET(uiColNum > 0, ALM_ERR_INVALID_PARA,
        ("Table needs at least one column."));

    ALM_CHK_RET(uiNameLen < ALM_CONST_NAME_MAX_LEN + 1,
        ALM_ERR_INVALID_PARA,
        ("The table col name length is more then max value."));

    /* 初始化输出 */
    (ALM_VOID)ALM_memset_s(pstTbl, sizeof(ALM_TBL_OBJ_STRU), 0,
        sizeof(ALM_TBL_OBJ_STRU));

    /* 初始化表格名 */
    if (pcName != NULL)
    {
        (ALM_VOID)ALM_strncpy_s(pstTbl->acTblName,
            sizeof(pstTbl->acTblName) - 1,
            pcName,
            uiNameLen);
        pstTbl->acTblName[sizeof(pstTbl->acTblName) - 1] = '\0';
    }

    /* 初始化表格列 */
    iRet = ALM_TblColsInit(&pstTbl->stCols);
    ALM_CALL_RET(iRet, ("Init table cols fail."));

    iRet = ALM_TblColsAddCol(&pstTbl->stCols,
        uiColNum,
        pstColsDim);
    ALM_CALL_LAB(LAB_END, ("Add table cols fail."));

    /* 初始化表格行 */
    iRet = ALM_TblRowsInit(&pstTbl->stRows, &pstTbl->stCols);
    ALM_CALL_LAB(LAB_END, ("Init table rows fail."));


LAB_END:

    if (iRet != ALM_OK)
    {
        ALM_StClsTblObj(pstTbl);
        return iRet;
    }

    return ALM_OK;
}

/* 清理表格，清理后应重新初始化才能使用 */
ALM_VOID ALM_StClsTblObj(INOUT ALM_TBL_OBJ_STRU* pstTbl)
{
    if (NULL == pstTbl)
    {
        return;
    }

    ALMStClsTblCols(&pstTbl->stCols);
    ALM_StClsTblRows(&pstTbl->stRows);

    (ALM_VOID)ALM_memset_s(pstTbl, sizeof(ALM_TBL_OBJ_STRU), 0,
        sizeof(ALM_TBL_OBJ_STRU));
}

/**
* 描述: 向表格中插入数据。
* 参数:
* 1. pstTbl - 待插入数据的表格。
* 2. pstRows - 插入的数据项，对以不存在默认值的列必须指定数据值。
*/
ALM_INT32 ALM_TblObjInsert(INOUT ALM_TBL_OBJ_STRU* pstTbl,
    CONST ALM_TBL_SROW_STRU* pstNewRow)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_CHK_RET(pstTbl != NULL && pstNewRow != NULL,
        ALM_ERR_NULL_PTR,
        (NULL));

    ALM_CHK_RET(pstNewRow->uiMapsNum > 0 && pstNewRow->pstMaps != NULL,
        ALM_ERR_INVALID_PARA,
        ("table insert empty."));

    iRet = ALM_TblRowsInsert(&pstTbl->stRows, pstNewRow);
    ALM_CALL_RET(iRet, ("table obj insert fail."));    

    return iRet;
}

/**
* 描述: 删除表格中符合要求的数据行。
* 参数:
* 1. pstTbl - 待删除数据的表格。
* 2. pstCdt - 待删除的数据行应符合的条件，若传入NULL删除全部数据。
*/
ALM_INT32 ALM_TblObjDel(INOUT ALM_TBL_OBJ_STRU* pstTbl,
    CONST ALM_TBL_CDT_STRU* pstCdt)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_CHK_RET(pstTbl != NULL, ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_TblRowsDel(&pstTbl->stRows, pstCdt);
    ALM_CALL_RET(iRet, ("table obj delete data fail."));    

    return iRet;
}

/**
* 描述: 更新表格中数据行。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 需要更新数据的数据行应满足的条件，若传入NULL更新全部数据。
* 3. pstNewVal - 指定待更新的列及对应的新值。
*/
ALM_INT32 ALM_TblObjUpdata(INOUT ALM_TBL_OBJ_STRU* pstTbl,
    CONST ALM_TBL_CDT_STRU* pstCdt,
    CONST ALM_TBL_SROW_STRU* pstNewVal)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_CHK_RET(pstTbl != NULL && pstNewVal != NULL,
        ALM_ERR_NULL_PTR,
        (NULL));

    ALM_CHK_RET(pstNewVal->uiMapsNum > 0 && pstNewVal->pstMaps != NULL,
        ALM_ERR_INVALID_PARA, (NULL));

    iRet = ALM_TblRowsUpdata(&pstTbl->stRows, pstCdt, pstNewVal);
    ALM_CALL_RET(iRet, ("table obj updata data fail."));

    return iRet;
}


/**
* 描述: 查询表格中数据行。调用该函数后应外部清理查询结果。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 所得数据行应满足的条件。
* 3. pstRslt - 查询得到的全部数据行，会得到指定列的对应的数据值，若没指定列得到全部列的值。
*/
ALM_INT32 ALM_TblObjQuery(CONST ALM_TBL_OBJ_STRU* pstTbl,
    CONST ALM_TBL_CDT_STRU* pstCdt,
    INOUT ALM_TBL_RSLT_STRU* pstRslt)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_CHK_RET(pstTbl != NULL && pstRslt != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_TblRowsQuery(&pstTbl->stRows, pstCdt, pstRslt);
    ALM_CALL_RET(iRet, ("table obj query data fail."));

    return ALM_OK;
}

/* 得到表格中总数据行数。*/
ALM_INT32 ALM_TblObjTotal(CONST ALM_TBL_OBJ_STRU* pstTbl, ALM_UINT32* puiRowTotal)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_CHK_RET(pstTbl != NULL && puiRowTotal != NULL,
        ALM_ERR_NULL_PTR, (NULL));

    iRet = ALM_TblRowsTotal(&pstTbl->stRows, puiRowTotal);
    ALM_CALL_RET(iRet, ("get table rows count fail."));

    return ALM_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
