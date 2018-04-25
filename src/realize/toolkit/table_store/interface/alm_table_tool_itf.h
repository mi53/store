/*******************************************************************************
Description    : 定义table相关工具性质的一些结构体及对应的操作。
FileName       : alm_talbe_tool_itf.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_TOOL_ITF_H__
#define __ALM_TBLE_TOOL_ITF_H__

#include "alm_intf_constant.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/************************************************************************/
/*                    ALM_TBL_DATA_STRU                                 */
/************************************************************************/

/* 表格列数据类型，不支持变长类型。 */
typedef enum 
{
    ALM_TBL_DATA_TYPE_UCHAR,
    ALM_TBL_DATA_TYPE_UINT32,
    ALM_TBL_DATA_TYPE_UINT64,
    ALM_TBL_DATA_TYPE_CHAR32,
    ALM_TBL_DATA_TYPE_CHAR64,
    ALM_TBL_DATA_TYPE_CHAR128,
    ALM_TBL_DATA_TYPE_CHAR256,
    ALM_TBL_DATA_TYPE_CHAR512
}ALM_TBL_DATA_TYPE_ENUM;

/* 存储指定数据类型的数据。 */
typedef struct
{
    ALM_TBL_DATA_TYPE_ENUM eType;
    ALM_VOID* pData;
    ALM_UINT32 uiDataSize;
}ALM_TBL_DATA_STRU;

/* 获取指定数据类型大小 */
ALM_INT32 ALM_TblDataSize(ALM_TBL_DATA_TYPE_ENUM eType,
    OUT ALM_UINT32* puiSize);

/* 比较数据大小，数据类型必须一致 */
ALM_INT32 ALM_TblDataCmp(CONST ALM_TBL_DATA_STRU* pstLeft,
    CONST ALM_TBL_DATA_STRU* pstRight,
    ALM_INT32* piRlst);

/************************************************************************/
/*                    ALM_TBL_COL_DIM_STRU: 表格定义                     */
/************************************************************************/

/**
* 描述: 定义一个简单的描述表格列的数据结构，用于方便表格定义。
* 成员:
* 1. pcColName - 列名。
* 2. eDataType - 该列存储的数据对应的数据类型。
* 3. pcDfltVal - 列默认值，插入数据行若未指定列值使用默认值。可以为NULL。
*/
typedef struct
{
    CONST ALM_CHAR* pcColName;
    ALM_TBL_DATA_TYPE_ENUM eDataType;
    CONST ALM_CHAR* pcDfltVal;
}ALM_TBL_COL_DIM_STRU;
   

/************************************************************************/
/*              ALM_TBL_SROW_STRU：查询结果、条件指定。                 */
/************************************************************************/
/**
* 描述: 工具性质的数据结构，用于指定列及对应的值。
* 成员:
* 1. pcColName - 列名。
* 2. stVal - 指定值。
*/
typedef struct
{
    ALM_CHAR acColName[ALM_CONST_NAME_MAX_LEN + 1];
    ALM_TBL_DATA_STRU stVal;
}ALM_TBL_COL_MAP_STRU;

/* 
* 描述: 使用数组包装多个ALM_TBL_COL_MAP_STRU。
* 成员:
* 1. uiMapsNum - 数组pstMaps中当前已保存的实际MAP数目。
* 2. uiMapsCap - 数组pstMaps中总计可存储的MAP数目。
* 3. pstMaps - 用于存储COL MAP的数组。
*/
typedef struct 
{
    ALM_UINT32 uiMapsNum;
    ALM_UINT32 uiMapsCap;
    ALM_TBL_COL_MAP_STRU* pstMaps;
}ALM_TBL_SROW_STRU;

/* 初始化单个列条件，不再使用应使用函数ALM_StClsTblSRow清理 */
ALM_INT32 ALM_StInitTblSRows(OUT ALM_TBL_SROW_STRU* pstSRows,
    ALM_UINT32 uiInitCap);

/* 清理已初始的列，清理后应重新初始化才能使用 */
ALM_VOID ALM_StClsTblSRow(INOUT ALM_TBL_SROW_STRU* pstSRows);

/* 得到对应列的值 */
ALM_INT32 ALM_TblSRowsGet(IN CONST ALM_TBL_SROW_STRU* pstSRows,
    CONST ALM_CHAR* pcColName,
    OUT CONST ALM_TBL_DATA_STRU** ppstVal);

/* 增加列-值对，并得到添加后所在索引 */
/* piIdx - 若为NULL，不输出索引 */
ALM_INT32 ALM_TblSRowsAdd(INOUT ALM_TBL_SROW_STRU* pstSRows,
    CONST ALM_CHAR* pcColName,
    CONST ALM_TBL_DATA_STRU* pstVal,
    OUT ALM_INT32* piIdx);

/************************************************************************/
/*              ALM_TBL_CDT_STRU                                        */
/************************************************************************/

/* 表格符合要求数据行，应满足所有指定列的要求。对以未指定的列，默认符合要求。*/
typedef struct 
{
    ALM_TBL_SROW_STRU stCols;
}ALM_TBL_CDT_STRU;

/* 清理已初始化的查询条件 */
ALM_VOID ALM_StClsTblCdt(INOUT ALM_TBL_CDT_STRU* pstCdt);

/* 增加列条件 */
ALM_INT32 ALM_TblCdtAdd(INOUT ALM_TBL_CDT_STRU* pstCdt,
    CONST ALM_CHAR* pcColName,
    CONST ALM_TBL_DATA_STRU* pstVal);

/************************************************************************/
/*                         ALM_TBL_RSLT_STRU                            */
/************************************************************************/

/**
* 描述: 查询结果中包含的列信息。
* 成员:
* pcColName - 列名。
* uiOffSet - 数据起始位。
* uiDataSize - 数据长度。
* eDataType - 数据类型。
*/
typedef struct
{
    ALM_CHAR* pcColName;
    ALM_UINT32 uiOffSet;
    ALM_UINT32 uiDataSize;
    ALM_TBL_DATA_TYPE_ENUM eDataType;
}ALM_TBL_RSLT_COL_UNIT_STRU;

/**
* 描述: 使用链表存储表格数据行。本结构体定义单个数据结点。
* 成员:
* pstRslt - 结果对象。
* pData - 单个数据行。
* pstNext - 迭代下一个结点。
*/
typedef struct TAG_ALM_TBL_RSLT_NODE_STRU
{
    CONST ALM_VOID* pstRslt;
    ALM_VOID* pData;
    struct TAG_ALM_TBL_RSLT_NODE_STRU* pstNext;
}ALM_TBL_RSLT_NODE_STRU;

/**
* 描述: 用于简单临时存储一些数据行。
* 成员:
* pstCols - 列信息，可以使用该信息获取一行中某一列的值。
* uiColNum - 指定查询结果中应包含的列的数目。
* uiColCap - 指定查询结果中应包含的列的数目。
* uiNodeDataLen - 存储单个结果行的内存大小。
* uiRsltCount - 结果条目数目。
* stEmptHead - 使用链表存储全部全部查询结果，头结点不包含数据。
*/
typedef struct 
{
    ALM_TBL_RSLT_COL_UNIT_STRU* pstCols;
    ALM_UINT32 uiColNum;
    ALM_UINT32 uiColCap;
    ALM_UINT32 uiNodeDataLen;
    ALM_UINT32 uiRsltCount;
    ALM_TBL_RSLT_NODE_STRU stEmptHead;
}ALM_TBL_RSLT_STRU;

/* 清理已初始化的查询包。 */
ALM_VOID ALM_StClsTblRslt(INOUT ALM_TBL_RSLT_STRU* pstRslt);

/* 获取列在结果集中的索引，若不存在得到负数 */
ALM_INT32 ALM_TblRsltGetColIdx(CONST ALM_TBL_RSLT_STRU* pstRslt,
    CONST ALM_CHAR* pcColName,
    OUT ALM_INT32* piColIdx);

/* 依据列索引得到列的值 */
ALM_INT32 ALM_TblRsltGetColByIdx(CONST ALM_TBL_RSLT_NODE_STRU* pstNode,
    ALM_INT32 iColIdx,
    OUT void* pBuf,
    ALM_UINT32 uiBufLen);

/* 获取列在结果集中的指定列的值 */
ALM_INT32 ALM_TblRsltGetCol(CONST ALM_TBL_RSLT_NODE_STRU* pstNode,
    CONST ALM_CHAR* pcColName,
    OUT void* pBuf,
    ALM_UINT32 uiBufLen);

/* 添加待查询列 */
ALM_INT32 ALM_TblRsltAddCol(INOUT ALM_TBL_RSLT_STRU* pstRslt,
    CONST ALM_CHAR* pstCol,
    ALM_TBL_DATA_TYPE_ENUM eDataType);

/* 将列按照列名由小到大排序，并同时排序列内容，若需要会转换字节序 */
ALM_INT32 ALM_TblRsltSort(INOUT ALM_TBL_RSLT_STRU* pstRslt,
    ALM_BOOL bChangeEndin);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_TOOL_ITF_H__ */
