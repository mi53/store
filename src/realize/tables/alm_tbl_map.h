/*******************************************************************************
Description    : 表格映射对象，便于操作。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBL_MAP_H__
#define __ALM_TBL_MAP_H__

#include "alm_table_store_itf.h"
#include "alm_tbls.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/* 涉及列所在对象位置 */
typedef struct 
{
    ALM_TBL_COL_IDX_ENUM eCol;
    ALM_UINT32 uiOffset;
    ALM_UINT32 uiSize;
}ALM_TBL_MAP_COL_STRU;

/* 保存多个映射列 */
typedef struct 
{
    ALM_BOOL bNeedCheck;
    CONST ALM_TBL_MAP_COL_STRU* pstArray;
    ALM_UINT32 uiNum;
}ALM_TBL_MAP_COLS_STRU;

/* 几个用于初始化MAP列的工具宏 */
#define ALM_ST_MEB_OFFSET(type, member) (ALM_UINT32)&(((type*)0)->member)
#define ALM_ST_MEB_SIZE(type, member) sizeof(((type*)0)->member)
#define ALM_TBL_MAP_COL(idx, type, member) \
    {idx, ALM_ST_MEB_OFFSET(type, member), ALM_ST_MEB_SIZE(type, member)}

/* 表格对象关联到表格 */
typedef struct
{
    /* 关联表格 */
    ALM_TBL_IDX_ENUM eTbl;

    /* 关联对象大小 */
    ALM_UINT32 uiObjSize;
    
    /* 列映射 */
    ALM_TBL_MAP_COLS_STRU* pstColMaps;

    /* 条件列，若未指定不限制条件 */
    CONST ALM_TBL_COL_IDX_ENUM* pstCdtCols;
    ALM_UINT32 uiCdtColNum;

    /* 当前涉及列，若为NULL，默认涉及全部已映射列 */
    CONST ALM_TBL_COL_IDX_ENUM* pstCols;
    ALM_UINT32 uiColNum;

}ALM_TBL_MAP_STRU;

/* 存储查询结果 */
typedef struct
{
    ALM_VOID* pObjsArray;
    ALM_UINT32 uiNum;
    ALM_UINT32 uiCap;
}ALM_TBL_MAP_RSLT_STRU;

/* 附加列映射，优先从附加列中获取数据 */
typedef struct 
{
    ALM_TBL_COL_IDX_ENUM eCol;
    CONST ALM_VOID* pData;
    ALM_UINT32 uiSize;
}ALM_TBL_MAP_COL_EX_STRU;

/* 清理ALM_TBL_MAP_RSLT_STRU对象 */
ALM_VOID ALM_StClsTblMapRslt(INOUT ALM_TBL_MAP_RSLT_STRU*);

/* 校验映射对象的有效性 */
ALM_INT32 ALM_TblMapCheck(CONST ALM_TBL_MAP_STRU* pstTblMap);

/* 使用映射对象实现添加数据 */
ALM_INT32 ALM_TblMapAdd(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObj,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum);

/* 使用映射对象实现删除数据 */
/* pObjCdt - 若为NULL，不限制条件 */
ALM_INT32 ALM_TblMapRm(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjCdt,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum);

/* 使用映射对象实现更新，附加列只作条件 */
ALM_INT32 ALM_TblMapUpdata(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjNew,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum);

/* 使用映射对象实现查询，附加列只作条件，注意清理输出 */
/* pObjCdt - 若为NULL，不限制条件 */
ALM_INT32 ALM_TblMapQuery(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjCdt,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum,
    OUT ALM_TBL_MAP_RSLT_STRU* pstRslt);

/* 使用映射对象实现更新，附加列只作条件 */
/* pbChange - 若不为NULL，将得到是否存在数据变更的信息 */
ALM_INT32 ALM_TblMapUpdataByChange(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjOld,
    CONST ALM_VOID* pObjNew,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum,
    OUT ALM_BOOL* pbChange);

/* 将obj转换为条件 */
/* pObj - 若为NULL不限制条件 */
static ALM_INT32 ALMTblMapObjToCdt(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObj,
    CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
    ALM_UINT32 uiExNum,
    INOUT ALM_TBL_CDT_STRU* pstCdt);

/* 将obj变动新转换为插入或更新数据行 */
/* pObjOld - 若为NULL，表示数据全部变动。 */
static ALM_INT32 ALMTblMapObjChangeToSRow(CONST ALM_TBL_MAP_STRU* pstTblMap,
    CONST ALM_VOID* pObjOld,
    CONST ALM_VOID* pObjNew,
    INOUT ALM_TBL_SROW_STRU* pstRow);

/* 将涉及列添加到查询结果中 */
static ALM_INT32 ALMTblMapAddColToRslt(CONST ALM_TBL_MAP_STRU* pstTblMap,
    INOUT ALM_TBL_RSLT_STRU* pstRslt);

/* 将查询结果转换为obj */
static ALM_INT32 ALMTblMapRsltToObj(CONST ALM_TBL_MAP_STRU* pstTblMap,
    OUT ALM_VOID* pObj,
    CONST ALM_TBL_RSLT_NODE_STRU* pstRslt);

/* 得到映射列 */
static CONST ALM_TBL_MAP_COL_STRU* ALMTblMapGetCol(
    CONST ALM_TBL_MAP_STRU* pstTblMap,
    ALM_TBL_COL_IDX_ENUM eCol);

/* 得到附加列 */
static CONST ALM_TBL_MAP_COL_EX_STRU* ALMTblMapExGetCol(
    CONST ALM_TBL_MAP_COL_EX_STRU* pstExs,
    ALM_UINT32 uiExNum,
    ALM_TBL_COL_IDX_ENUM eCol);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBL_MAP_H__ */
