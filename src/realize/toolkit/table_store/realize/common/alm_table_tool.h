/*******************************************************************************
Description    : 扩展并完善工具对象。
FileName       : alm_table_tool.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_TOOL_H__
#define __ALM_TBLE_TOOL_H__

#include "alm_table_tool_itf.h"
#include "alm_table_row.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/************************************************************************/
/*         ALM_TBL_DATA_STRU: 注意正确处理未字节对齐的整形数据。               */
/************************************************************************/
#define ALM_TBL_DATA_TYPE_UCHAR_LEN     sizeof(ALM_UCHAR)
#define ALM_TBL_DATA_TYPE_UINT32_LEN    sizeof(ALM_UINT32)
#define ALM_TBL_DATA_TYPE_UINT64_LEN    sizeof(ALM_UINT64)
#define ALM_TBL_DATA_TYPE_CHAR32_LEN    32
#define ALM_TBL_DATA_TYPE_CHAR64_LEN    64
#define ALM_TBL_DATA_TYPE_CHAR128_LEN   128
#define ALM_TBL_DATA_TYPE_CHAR256_LEN   256
#define ALM_TBL_DATA_TYPE_CHAR512_LEN   512
#define ALM_TBL_DATA_TYPE_MAX_LEN       1024

#define ALM_TABL_DATA_MAX_INT (0x7FFFFFFFL)

/* 确定数据类型的有效性 */
ALM_BOOL ALM_TblDataTypeCheck(ALM_TBL_DATA_TYPE_ENUM eType);

/* 清理data对象 */
ALM_VOID ALM_StClsTblData(INOUT ALM_TBL_DATA_STRU* pstData);

/* 初始化数据项 */
ALM_INT32 ALM_TblDataInit(OUT ALM_TBL_DATA_STRU* pstData,
    ALM_TBL_DATA_TYPE_ENUM eType);

/* 依据字符串及指定数据类型，得到对应的数据对象 */
ALM_INT32 ALM_TblDataInitByStrVal(OUT ALM_TBL_DATA_STRU* pstData,
    ALM_TBL_DATA_TYPE_ENUM eType,
    CONST ALM_CHAR* pcVal);

/* 写入数据，不会重新分配空间，非字符串类型数据大小必须一致，字符串类型容量必须足够 */
ALM_INT32 ALM_TblDataWriteByPtr(INOUT ALM_TBL_DATA_STRU* pstData,
    ALM_TBL_DATA_TYPE_ENUM eType,
    CONST ALM_VOID* pData,
    ALM_UINT32 uiDataSize);

/* 改变数据字节序*/
ALM_INT32 ALM_TblDataChangeEndin(INOUT ALM_TBL_DATA_STRU* pstData);

/* 获取数据内容，数值数据类型，buf长度必须与数据类型大小保持一致，string类型只需能存储便可 */
ALM_INT32 ALM_TblDataGet(CONST ALM_TBL_DATA_STRU* pstData,
    ALM_TBL_DATA_TYPE_ENUM eDataType,
    OUT ALM_VOID* pBuf,
    ALM_UINT32 uiBufLen);

/* 将字符串转换为data对应的数据类型，并写入数据对象 */
static ALM_INT32 ALMTblDataWrite(INOUT ALM_TBL_DATA_STRU* pstData,
    CONST ALM_CHAR* pcVal);

/* 判断是否为字符串类型 */
static ALM_BOOL ALMTblDataCheckStr(ALM_TBL_DATA_TYPE_ENUM eType);

/************************************************************************/
/*               ALM_TBL_SROW_STRU                                      */
/************************************************************************/

/* 更新一个数据行，只更新指定列 */
ALM_INT32 ALM_TblSRowsToUpdataRow(CONST ALM_TBL_SROW_STRU* pstSRows,
    CONST ALM_TBL_COLS_STRU* pstCols,
    INOUT ALM_TBL_ROW_STRU* pstRow);

/* 得到某一列的索引，若不存在指定列得到的索引为负值 */
ALM_INT32 ALM_TblSRowsGetIdx(CONST ALM_TBL_SROW_STRU* pstSRows,
    CONST ALM_CHAR* pcColName,
    OUT ALM_INT32* piIdx);

/* 扩展COL MAPS存储空间，保证总容量与当前数目的差值大于等于指定的扩展数目 */
static ALM_INT32 ALMTblSRowsExpand(INOUT ALM_TBL_SROW_STRU* pstSRows,
    ALM_UINT32 uiExpand);

/* 初始化单个列条件，不再使用应使用函数ALMTblQueryColCdtCls清理 */
static ALM_INT32 ALMStInitTblSRowsCol(OUT ALM_TBL_COL_MAP_STRU* pstColMap,
    CONST ALM_CHAR* pcColName,
    CONST ALM_TBL_DATA_STRU* pstVal);

/* 清理已初始的列，清理后应重新初始化才能使用 */
static ALM_VOID ALMStClsTblSRowsCol(INOUT ALM_TBL_COL_MAP_STRU* pstColMap);

/************************************************************************/
/*              ALM_TBL_CDT_STRU                                        */
/************************************************************************/

/* 增加列条件 */
ALM_INT32 ALM_TblCdtAddColCdts(INOUT ALM_TBL_CDT_STRU* pstCdt,
    CONST ALM_TBL_SROW_STRU* pstColCdts);

/************************************************************************/
/*                         ALM_TBL_RSLT_STRU                            */
/************************************************************************/

/**
* 描述: 初始化查询包，不再使用应使用对应的函数清理。
* 参数:
* 1. pstRslt - 待初始化对象。
* 2. uiColNum - 指定列数。
* 3. pstCols - 指定要查询的列。
*/
ALM_INT32 ALM_TblRsltInitByCols(OUT ALM_TBL_RSLT_STRU* pstRslt,
    CONST ALM_TBL_COLS_STRU* pstCols);

/* 将数据行转换为查询结果 */
ALM_INT32 ALM_TblRsltAddRsltByRow(INOUT ALM_TBL_RSLT_STRU* pstRslt,
    CONST ALM_TBL_COLS_STRU* pstCols,
    CONST ALM_TBL_ROW_STRU* pstRow);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_TOOL_H__ */
