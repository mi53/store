/*******************************************************************************
Description    : rows中存储row需要指定的一些函数。
FileName       : alm_talbe_row_store.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/

#ifndef __ALM_TBLE_ROW_STORE_H__
#define __ALM_TBLE_ROW_STORE_H__

#include "alm_table_row.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */


/* 初始化ROW对象 */
ALM_INT32 ALM_TblRowStoreInit(OUT ALM_VOID* pRow, CONST ALM_VOID* pCols);

/* 清理ROW对象 */
ALM_VOID ALM_TblRowStoreCls(INOUT ALM_VOID* pRow, CONST ALM_VOID* pCols);

/* Row拷贝 */
ALM_INT32 ALM_TblRowStoreCpy(OUT ALM_VOID* pDestRow,
    CONST ALM_VOID* pSrcRow,
    CONST ALM_VOID* pCols);


/* 得到条件MARK，难于支持DATA CMP与MARK CMP的一致性，暂不适用排序 */
ALM_VOID* ALM_TblRowStoreMarkNew(CONST ALM_VOID* pRow, CONST ALM_VOID* pCols);

/* 释放条件MARK */
ALM_VOID ALM_TblRowStoreMarkFree(INOUT ALM_VOID* pCdt, CONST ALM_VOID* pCols);

/* 比较条件MARK与ROW大小，没有指定条件全部符合要求 */
ALM_INT32 ALM_TblRowStoreMarkCmp(CONST ALM_VOID* pCdt,
    CONST ALM_VOID* pRow,
    OUT ALM_INT32* piRslt,
    CONST ALM_VOID* pCols);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_ROW_STORE_H__ */
