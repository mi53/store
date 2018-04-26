/*******************************************************************************
Description    : 本文件中定义全部表格。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLS_H__
#define __ALM_TBLS_H__

#include "alm_table_store_itf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */


/* 定义全部表格索引 */
typedef enum
{
    /* 存储产品注册的BBOM及其状态 */
    ALM_TBL_IDX_PRD_BBOMS,

    /* 存储注册的产品及其状态 */
    ALM_TBL_IDX_PRD_INFO,

    /* 存储License通用信息 */
    ALM_TBL_IDX_LIC,

    /* 存储License Esn信息 */
    ALM_TBL_IDX_LIC_ESN,

    /* 存储License Bbom信息 */
    ALM_TBL_IDX_LIC_BBOM,

    /* 存储License Sbom信息 */
    ALM_TBL_IDX_LIC_SBOM,

    /* 存储License S2Bbom信息 */
    ALM_TBL_IDX_LIC_S2BBOM,

    /* 存储License附加信息 */
    ALM_TBL_IDX_LIC_ADDTION,

    /* 存储License状态信息 */
    ALM_TBL_IDX_LIC_STATE,

    /* 存储License Bbom状态信息 */
    ALM_TBL_IDX_LIC_BBOM_STATE,

    /* 存储状态校验规则 */
    ALM_TBL_IDX_STATE_RULE,

    /* 持久化存储产品状态信息 */
    ALM_TBL_IDX_PS_PRD,

    /* 持久化存储License信息 */
    ALM_TBL_IDX_PS_LIC,

    /* 计数 */
    ALM_TBL_IDX_NUM,
}ALM_TBL_IDX_ENUM;


/* 定义表格列索引 */
typedef enum
{
    /************************************************************************
    * PRD_INFO
    ************************************************************************/
    ALM_TBL_COL_IDX_PRD_INFO_START,

    /* 基础信息 */
    ALM_TBL_COL_IDX_PRD_INFO_ID,
    ALM_TBL_COL_IDX_PRD_INFO_NAME,
    ALM_TBL_COL_IDX_PRD_INFO_VER,

    /* 状态段 */
    ALM_TBL_COL_IDX_PRD_INFO_STATE,
    ALM_TBL_COL_IDX_PRD_INFO_ENTER, 
    ALM_TBL_COL_IDX_PRD_INFO_LEFT_MINS,
    ALM_TBL_COL_IDX_PRD_INFO_PERM,
    ALM_TBL_COL_IDX_PRD_INFO_STATE_LSN,

    /* 密钥段 */
    ALM_TBL_COL_IDX_PRD_INFO_KEY00,
    ALM_TBL_COL_IDX_PRD_INFO_KEY01,
    ALM_TBL_COL_IDX_PRD_INFO_KEY02,
    ALM_TBL_COL_IDX_PRD_INFO_KEY03,
    ALM_TBL_COL_IDX_PRD_INFO_KEY04,
    ALM_TBL_COL_IDX_PRD_INFO_KEY05,
    ALM_TBL_COL_IDX_PRD_INFO_KEY06,
    ALM_TBL_COL_IDX_PRD_INFO_KEY07,
    ALM_TBL_COL_IDX_PRD_INFO_KEY08,
    ALM_TBL_COL_IDX_PRD_INFO_KEY09,
    ALM_TBL_COL_IDX_PRD_INFO_KEY10,
    ALM_TBL_COL_IDX_PRD_INFO_KEY11,
    ALM_TBL_COL_IDX_PRD_INFO_KEY12,
    ALM_TBL_COL_IDX_PRD_INFO_KEY13,
    ALM_TBL_COL_IDX_PRD_INFO_KEY14,
    ALM_TBL_COL_IDX_PRD_INFO_KEY15,

    ALM_TBL_COL_IDX_PRD_INFO_END,
    
    /************************************************************************
    * PRD_BBOMS
    ************************************************************************/
    ALM_TBL_COL_IDX_PRD_BBOMS_START,

    /* 基础信息 */
    ALM_TBL_COL_IDX_PRD_BBOMS_PRD_ID ,
    ALM_TBL_COL_IDX_PRD_BBOMS_BBOM_ID,
    ALM_TBL_COL_IDX_PRD_BBOMS_BBOM_NAME,
    ALM_TBL_COL_IDX_PRD_BBOMS_TYPE,
    ALM_TBL_COL_IDX_PRD_BBOMS_DFLT_VAL,
     
    /* 状态段 */
    ALM_TBL_COL_IDX_PRD_BBOMS_LSN,
    ALM_TBL_COL_IDX_PRD_BBOMS_STATE,
    ALM_TBL_COL_IDX_PRD_BBOMS_ENTER, 
    ALM_TBL_COL_IDX_PRD_BBOMS_LEFT_MINS,
    ALM_TBL_COL_IDX_PRD_BBOMS_IS_PERM,
    ALM_TBL_COL_IDX_PRD_BBOMS_VALUE,

    ALM_TBL_COL_IDX_PRD_BBOMS_END,
    /************************************************************************
    * LIC: 通用信息段
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_START,

    ALM_TBL_COL_IDX_LIC_LSN,
    ALM_TBL_COL_IDX_LIC_PRD_NAME,
    ALM_TBL_COL_IDX_LIC_PRD_VER,
    ALM_TBL_COL_IDX_LIC_TYPE,
    ALM_TBL_COL_IDX_LIC_GRACE_DAYS,
    ALM_TBL_COL_IDX_LIC_ALLOW_COUNT,

    ALM_TBL_COL_IDX_LIC_END,
    /************************************************************************
    * LIC_ESN
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_ESN_START,
    
    ALM_TBL_COL_IDX_LIC_ESN_LSN,
    ALM_TBL_COL_IDX_LIC_ESN_ESN,

    ALM_TBL_COL_IDX_LIC_ESN_END,    
    /************************************************************************
    * LIC_BBOM
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_BBOM_START,

    ALM_TBL_COL_IDX_LIC_BBOM_LSN,
    ALM_TBL_COL_IDX_LIC_BBOM_NAME,
    ALM_TBL_COL_IDX_LIC_BBOM_SAME_NAME_ID,
    ALM_TBL_COL_IDX_LIC_BBOM_TYPE,
    ALM_TBL_COL_IDX_LIC_BBOM_VALUE,
    ALM_TBL_COL_IDX_LIC_BBOM_GROUP,
    ALM_TBL_COL_IDX_LIC_BBOM_DEADLINE,
    ALM_TBL_COL_IDX_LIC_BBOM_ALLOW_DAYS,
    ALM_TBL_COL_IDX_LIC_BBOM_GRACE_DAYS,

    ALM_TBL_COL_IDX_LIC_BBOM_END,
    /************************************************************************
    * LIC_SBOM
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_SBOM_START,
    
    ALM_TBL_COL_IDX_LIC_SBOM_LSN,
    ALM_TBL_COL_IDX_LIC_SBOM_OFFER_NAME,
    ALM_TBL_COL_IDX_LIC_SBOM_OFFER_VER,
    ALM_TBL_COL_IDX_LIC_SBOM_NAME,
    ALM_TBL_COL_IDX_LIC_SBOM_VALUE,
    ALM_TBL_COL_IDX_LIC_SBOM_DEADLINE,
    ALM_TBL_COL_IDX_LIC_SBOM_DES_ENG_UNIT,
    ALM_TBL_COL_IDX_LIC_SBOM_DES_ENG,
    ALM_TBL_COL_IDX_LIC_SBOM_DES_CN_UNIT,
    ALM_TBL_COL_IDX_LIC_SBOM_DES_CN,

    ALM_TBL_COL_IDX_LIC_SBOM_END,
    /************************************************************************
    * LIC_S2BBOM
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_S2BBOM_START,

    ALM_TBL_COL_IDX_LIC_S2BBOM_LSN,
    ALM_TBL_COL_IDX_LIC_S2BBOM_SBOM_NAME,

    /* BBOM段 */
    ALM_TBL_COL_IDX_LIC_S2BBOM_BBOM_NAME,
    ALM_TBL_COL_IDX_LIC_S2BBOM_BBOM_SAME_NAME_ID,
    ALM_TBL_COL_IDX_LIC_S2BBOM_BBOM_TYPE,
    ALM_TBL_COL_IDX_LIC_S2BBOM_BBOM_DEADLINE,
    ALM_TBL_COL_IDX_LIC_S2BBOM_BBOM_VALUE,
    ALM_TBL_COL_IDX_LIC_S2BBOM_BBOM_GROUP,
    ALM_TBL_COL_IDX_LIC_S2BBOM_BBOM_ALLOW_DAYS,
    ALM_TBL_COL_IDX_LIC_S2BBOM_BBOM_GRACE_DAYS,
    ALM_TBL_COL_IDX_LIC_S2BBOM_BBOM_BPRIMARY,

    ALM_TBL_COL_IDX_LIC_S2BBOM_END,
    /************************************************************************
    * LIC_ADDTION
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_ADDTION_START,

    ALM_TBL_COL_IDX_LIC_ADDTION_LSN,
    ALM_TBL_COL_IDX_LIC_ADDTION_COPY_RIGHT,
    ALM_TBL_COL_IDX_LIC_ADDTION_FORMAT_VER,
    ALM_TBL_COL_IDX_LIC_ADDTION_CREATER,
    ALM_TBL_COL_IDX_LIC_ADDTION_ISSUER,
    ALM_TBL_COL_IDX_LIC_ADDTION_CREATE_TIME,
    ALM_TBL_COL_IDX_LIC_ADDTION_SW_UPDATA_TIME, 
    ALM_TBL_COL_IDX_LIC_ADDTION_COUNTRY, 
    ALM_TBL_COL_IDX_LIC_ADDTION_OFFICE,
    ALM_TBL_COL_IDX_LIC_ADDTION_CUSTOM,
    ALM_TBL_COL_IDX_LIC_ADDTION_LOCK_MODEL,
    ALM_TBL_COL_IDX_LIC_ADDTION_NODE_SEQ,
    ALM_TBL_COL_IDX_LIC_ADDTION_NODE_NAME,
    ALM_TBL_COL_IDX_LIC_ADDTION_NODE_DES,

    ALM_TBL_COL_IDX_LIC_ADDTION_END,
    /************************************************************************
    * LIC_STATE
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_STATE_START,

    ALM_TBL_COL_IDX_LIC_STATE_PRD_ID,
    ALM_TBL_COL_IDX_LIC_STATE_LSN,

    /* 使用记录 */
    ALM_TBL_COL_IDX_LIC_STATE_RECOVER_USING,
    ALM_TBL_COL_IDX_LIC_STATE_IS_USING,
    ALM_TBL_COL_IDX_LIC_STATE_USE_COUNT,

    /* 状态部分 */
    ALM_TBL_COL_IDX_LIC_STATE_STATE,
    ALM_TBL_COL_IDX_LIC_STATE_PASS_MINS,
    ALM_TBL_COL_IDX_LIC_STATE_LEFT_MINS,
    ALM_TBL_COL_IDX_LIC_STATE_IS_PERM,
    ALM_TBL_COL_IDX_LIC_STATE_ENTER_TIME,
    ALM_TBL_COL_IDX_LIC_STATE_GRACE_PASS_MINS,

    /* 校验项部分 */
    ALM_TBL_COL_IDX_LIC_STATE_ESN_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_PRD_NAME_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_PRD_VER_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_HAVE_RVKED,
    ALM_TBL_COL_IDX_LIC_STATE_VALID_OVER,
    ALM_TBL_COL_IDX_LIC_STATE_GRACE_OVER,

    /* 上一次刷新时保留的校验项及激活记录 */
    ALM_TBL_COL_IDX_LIC_STATE_LAST_IS_USING,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_ESN_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_PRD_NAME_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_PRD_VER_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_HAVE_RVKED,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_VALID_OVER,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_GRACE_OVER,

    /* 用于状态校验或激活校验的文件内容 */
    ALM_TBL_COL_IDX_LIC_STATE_LIC_TYPE,
    ALM_TBL_COL_IDX_LIC_STATE_LIC_DEADLINE,
    ALM_TBL_COL_IDX_LIC_STATE_LIC_REL_DAYS, 
    ALM_TBL_COL_IDX_LIC_STATE_LIC_GRACE_DAYS,
    ALM_TBL_COL_IDX_LIC_STATE_LIC_ALLOW_COUNT,

    ALM_TBL_COL_IDX_LIC_STATE_END,
    /************************************************************************
    * LIC_BBOM_STATE
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_START,

    ALM_TBL_COL_IDX_LIC_BBOM_STATE_PRD_ID,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_LSN,
    
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_BBOM_ID,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_SAME_NAME_ID,
 
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_STATE,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_PASS_MINS,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_LEFT_MINS,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_IS_PERM,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_ENTER_TIME,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_VALUE,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_GRACE_PASS_MINS,

    ALM_TBL_COL_IDX_LIC_BBOM_STATE_END,
    
    /************************************************************************
    * STATE_RULE
    ************************************************************************/
    ALM_TBL_COL_IDX_STATE_RULE_START,
        
    ALM_TBL_COL_IDX_STATE_RULE_PRD_ID,
    ALM_TBL_COL_IDX_STATE_RULE_ITEM,
    ALM_TBL_COL_IDX_STATE_RULE_IGNORE,
    ALM_TBL_COL_IDX_STATE_RULE_IGNORE_UP_AND_LOWER,
    ALM_TBL_COL_IDX_STATE_RULE_MIS_STATE,

    ALM_TBL_COL_IDX_STATE_RULE_END,
    /************************************************************************
    * PS_PRD
    ************************************************************************/
    ALM_TBL_COL_IDX_PS_PRD_START,

    ALM_TBL_COL_IDX_PS_PRD_TIME_ID,
    ALM_TBL_COL_IDX_PS_PRD_ID,
    ALM_TBL_COL_IDX_PS_PRD_VER,

    ALM_TBL_COL_IDX_PS_PRD_END,
    /************************************************************************
    * PS_LIC
    ************************************************************************/
    ALM_TBL_COL_IDX_PS_LIC_START,

    ALM_TBL_COL_IDX_PS_LIC_TIME_ID,
    ALM_TBL_COL_IDX_PS_LIC_PRD_ID,
    ALM_TBL_COL_IDX_PS_LIC_LSN,

    /* 使用记录 */
    ALM_TBL_COL_IDX_PS_LIC_BE_USING, 
    ALM_TBL_COL_IDX_PS_LIC_USED_COUNT,

    /* 状态部分 */
    ALM_TBL_COL_IDX_PS_LIC_STATE,
    ALM_TBL_COL_IDX_PS_LIC_ENTER_TIME,
    ALM_TBL_COL_IDX_PS_LIC_PASS_MINS,
    ALM_TBL_COL_IDX_PS_LIC_GRACE_PASS_MINS,

    /* 校验项部分 */
    ALM_TBL_COL_IDX_PS_LIC_ESN_MIS,
    ALM_TBL_COL_IDX_PS_LIC_PRD_NAME_MIS,
    ALM_TBL_COL_IDX_PS_LIC_PRD_VER_MIS,
    ALM_TBL_COL_IDX_PS_LIC_BE_RVKED,
    ALM_TBL_COL_IDX_PS_LIC_VALID_OVER,
    ALM_TBL_COL_IDX_PS_LIC_GRACE_OVER,

    ALM_TBL_COL_IDX_PS_LIC_END

}ALM_TBL_COL_IDX_ENUM;

/* 定义表格列 */
typedef struct
{
    ALM_TBL_COL_IDX_ENUM eIdx;
    ALM_UINT32 uiNeedSize;
    ALM_TBL_COL_DIM_STRU stCol;
}ALM_TBL_COL_DIM_EX_STRU;


/* 定义表格 */
typedef struct
{
    ALM_TBL_IDX_ENUM eTblIdx;
    CONST ALM_CHAR* pcTblName;
    CONST ALM_TBL_COL_DIM_EX_STRU* pstColDim;
    ALM_UINT32 uiStartIdx;
    ALM_UINT32 uiColNum;
}ALM_TBL_DIM_STRU;

/* 排序表格及列定义*/
static ALM_VOID ALMTblsSortDim();

/* 校验表格及列定义 */
static ALM_BOOL ALMTblsCheckDim();

/*初始化建立所有表格*/
ALM_INT32 ALM_TblsMgrCreateAll();

/*删除所有表格*/
ALM_VOID ALM_TblsMgrDropAll();

/* 得到表格定义 */
CONST ALM_TBL_DIM_STRU* ALM_TblsGetDim(ALM_TBL_IDX_ENUM eTblIdx);

/* 得到列定义 */
CONST ALM_TBL_COL_DIM_EX_STRU* ALM_TblsGetColDim(ALM_TBL_COL_IDX_ENUM eCol);

/************************************************************************
* 工具宏，表格整理完成移至.C文件中
************************************************************************/

/* KEY拆成8段存储，每段512字节 */
#define ALM_CONST_KEY_SEG_LEN 511

/* 依据表格简名、列简名得到列索引 */
#define ALM_TBL_COL_IDX(TBL, COL) ALM_TBL_COL_IDX_##TBL##_##COL

/* 定义表格列的工具宏 */
#define ALM_TBL_COL_DIM(TBL, COL, type, dflt, size) \
{ALM_TBL_COL_IDX(TBL, COL), size, {#COL, type, dflt}}

#define ALM_TBL_COL_DIM_START(TBL) \
    ALM_TBL_COL_DIM(TBL, START, ALM_TBL_DATA_TYPE_UCHAR, NULL, 0)

#define ALM_TBL_COL_DIM_END(TBL) \
    ALM_TBL_COL_DIM(TBL, END, ALM_TBL_DATA_TYPE_UCHAR, NULL, 0)

/* 将表格名简写转换为索引的工具宏 */
#define ALM_TBL_IDX(TBL) ALM_TBL_IDX_##TBL

/* 依据表格简名得到列数， 暂时放置此处 */
#define ALM_TBL_COL_NUM(TBL) \
    (ALM_TBL_COL_IDX(TBL, END) - ALM_TBL_COL_IDX(TBL, START) - 1)

#define ALM_TBL_NAME(TBL) "ALM_"#TBL

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLS_H__ */
