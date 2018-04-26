/*******************************************************************************
Description    : table store实现。
FileName       : alm_table_store.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/

#ifndef __ALM_TBLE_STORE_H__
#define __ALM_TBLE_STORE_H__

#include "alm_table_store_itf.h"
#include "alm_table_obj.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */


/* 保存临时表格对象，必须指定名称 */
static ALM_INT32 ALMTblStoreSave(ALM_TBL_OBJ_STRU* pstTbl);

/* 得到已创建表格索引，若不存在得到负值。 */
static ALM_INT32 ALMTblStoreGet(CONST ALM_CHAR* pcTblName, ALM_INT32* piIdx);

/* 得到已创建表格 */
static ALM_INT32 ALMTblStoreDel(CONST ALM_CHAR* pcTblName);


/* 得到已创建的表格 */
CONST ALM_TBL_OBJ_STRU* ALM_TblStoreGet(CONST ALM_CHAR* pcTblName);

/* 校验条件有效性 */
static ALM_BOOL ALMTblCheckCdt(CONST ALM_TBL_OBJ_STRU* pstTblObj,
    CONST ALM_TBL_CDT_STRU* pstCdt);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_STORE_H__ */
