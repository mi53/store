/*******************************************************************************
Description    : 定义表格列信息。
FileName       : alm_talbe_cols.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_COLS_H__
#define __ALM_TBLE_COLS_H__

#include "alm_table_tool_itf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */


/**
* 描述: 存储表格单列。
* 成员:
* 1. acColName - 列名。
* 2. eDataType - 该列存储的数据对应的数据类型。
* 3. bExistDflt- 是否存在默认值，ALM_TRUE: 存在。标记成员stDfltVal的有效性。
* 4. stDfltVal - 列默认值，插入数据行若未指定列值使用默认值。可以为NULL。
*/
typedef struct 
{
    ALM_CHAR acColName[ALM_CONST_NAME_MAX_LEN + 1];
    ALM_TBL_DATA_TYPE_ENUM eDataType;
    ALM_BOOL bExistDflt;
    ALM_TBL_DATA_STRU stDfltVal;
}ALM_TBL_COL_STRU;

/* 描述: 初始化单个列，不再使用应使用函数ALMStClsTblCol清理。可以不指定默认值。*/
static ALM_INT32 ALMTblColInit(OUT ALM_TBL_COL_STRU* pstCol,
    CONST ALM_TBL_COL_DIM_STRU* pstColsDim);

/* 清理已初始的列，清理后应重新初始化才能使用 */
static ALM_VOID ALMStClsTblCol(INOUT ALM_TBL_COL_STRU* pstCol);

/**
* 描述: 定义表格全部列。
* 成员:
* 1. uiColNum - 列数目。
* 2. pstCols - 定义的全部列。
*/
typedef struct 
{
    ALM_UINT32 uiColNum;
    ALM_TBL_COL_STRU* pstCols;
}ALM_TBL_COLS_STRU;

/* 初始化全部列定义，不再使用应使用函数ALM_TblColsCls清理。 */
ALM_INT32 ALM_TblColsInit(OUT ALM_TBL_COLS_STRU* pstCols);

/* 清理已初始的列，清理后应重新初始化才能使用 */
ALM_VOID ALMStClsTblCols(INOUT ALM_TBL_COLS_STRU* pstCols);

/* 增加列 */
ALM_INT32 ALM_TblColsAddCol(INOUT ALM_TBL_COLS_STRU* pstCols,
    ALM_UINT32 uiColNum,
    CONST ALM_TBL_COL_DIM_STRU* pstColsDim);

/**
* 描述:获取列索引。
* 参数:
* 1. pstCols - 列存储对象。
* 2. pcColName - 待获取的列面。
* 3. piColIdx - 得到的对应列的索引。列不存在得到负值。
*/
ALM_INT32 ALM_TblColsGetColIdx(CONST ALM_TBL_COLS_STRU* pstCols,
    CONST ALM_CHAR* pcColName,
    OUT ALM_INT32* piColIdx);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_COLS_H__ */
