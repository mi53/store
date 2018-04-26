/*******************************************************************************
Description    : 本文件中定义全部表格。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_tbls.h"
#include "alm_tools.h"
#include "alm_table_store_itf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/************************************************************************
* 工具宏
************************************************************************/

/* 依据表格简名得到表格定义 */
#define ALM_TBL_DIM(TBL) {\
    ALM_TBL_IDX(TBL),\
    "ALM_"#TBL, \
    s_astTblCols, \
    ALM_TBL_COL_IDX(TBL, START) + 1, \
    ALM_TBL_COL_NUM(TBL)}

/************************************************************************
* 定义表格列
************************************************************************/

static ALM_TBL_COL_DIM_EX_STRU s_astTblCols[] = {
    
    /************************************************************************
    * PRD_INFO
    ************************************************************************/
    ALM_TBL_COL_DIM_START(PRD_INFO),

    /* 基础信息 */
    ALM_TBL_COL_DIM(PRD_INFO, ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_INT32)),

    ALM_TBL_COL_DIM(PRD_INFO, NAME,
    ALM_TBL_DATA_TYPE_CHAR128, NULL, ALM_CONST_PRD_NAME_LEN_MAX + 1),

    ALM_TBL_COL_DIM(PRD_INFO, VER,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_PRD_VER_LEN_MAX + 1),

    /* 状态段 */
    ALM_TBL_COL_DIM(PRD_INFO, STATE,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(PRD_INFO, ENTER,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, LEFT_MINS,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(PRD_INFO, PERM,
    ALM_TBL_DATA_TYPE_UCHAR, NULL, sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(PRD_INFO, STATE_LSN,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_LSN + 1),

    /* 密钥段 */
    ALM_TBL_COL_DIM(PRD_INFO, KEY00,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY01,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY02,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY03,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY04,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY05,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY06,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY07,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY08,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY09,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY10,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY11,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY12,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY13,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY14,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM(PRD_INFO, KEY15,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_KEY_SEG_LEN + 1),

    ALM_TBL_COL_DIM_END(PRD_INFO),
    /************************************************************************
    * PRD_BBOMS
    ************************************************************************/
    ALM_TBL_COL_DIM_START(PRD_BBOMS),

    /* 基础信息 */
    ALM_TBL_COL_DIM(PRD_BBOMS, PRD_ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_INT32)),

    ALM_TBL_COL_DIM(PRD_BBOMS, BBOM_ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_INT32)),

    ALM_TBL_COL_DIM(PRD_BBOMS, BBOM_NAME,
    ALM_TBL_DATA_TYPE_CHAR128, NULL, ALM_CONST_LIC_BBOM_NAME_MAX + 1),

    ALM_TBL_COL_DIM(PRD_BBOMS, TYPE,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(PRD_BBOMS, DFLT_VAL,
    ALM_TBL_DATA_TYPE_UINT64, NULL, sizeof(ALM_UINT64)),
 
    /* 状态段 */
    ALM_TBL_COL_DIM(PRD_BBOMS, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, "", ALM_CONST_LIC_LSN + 1),

    ALM_TBL_COL_DIM(PRD_BBOMS, STATE,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(PRD_BBOMS, ENTER,
    ALM_TBL_DATA_TYPE_CHAR32, "", ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(PRD_BBOMS, LEFT_MINS,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(PRD_BBOMS, IS_PERM,
    ALM_TBL_DATA_TYPE_UCHAR, "1", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(PRD_BBOMS, VALUE,
    ALM_TBL_DATA_TYPE_UINT64, "0", sizeof(ALM_UINT64)),

    ALM_TBL_COL_DIM_END(PRD_BBOMS),
    /************************************************************************
    * LIC: 通用信息段
    ************************************************************************/
    ALM_TBL_COL_DIM_START(LIC),

    ALM_TBL_COL_DIM(LIC, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_LSN + 1),

    ALM_TBL_COL_DIM(LIC, PRD_NAME,
    ALM_TBL_DATA_TYPE_CHAR128, NULL, ALM_CONST_PRD_NAME_LEN_MAX + 1),

    ALM_TBL_COL_DIM(LIC, PRD_VER,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_PRD_VER_LEN_MAX + 1),

    ALM_TBL_COL_DIM(LIC, TYPE,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(LIC, GRACE_DAYS,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC, ALLOW_COUNT,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM_END(LIC),
    /************************************************************************
    * LIC_ESN
    ************************************************************************/
    ALM_TBL_COL_DIM_START(LIC_ESN),

    ALM_TBL_COL_DIM(LIC_ESN, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_LSN + 1),

    ALM_TBL_COL_DIM(LIC_ESN, ESN,
    ALM_TBL_DATA_TYPE_CHAR64, NULL, ALM_CONST_LIC_SINGLE_ESN_LEN_MAX + 1),

    ALM_TBL_COL_DIM_END(LIC_ESN),
    
    /************************************************************************
    * LIC_BBOM
    ************************************************************************/
    ALM_TBL_COL_DIM_START(LIC_BBOM),

    ALM_TBL_COL_DIM(LIC_BBOM, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_LSN + 1),

    ALM_TBL_COL_DIM(LIC_BBOM, NAME,
    ALM_TBL_DATA_TYPE_CHAR64, NULL, ALM_CONST_LIC_BBOM_NAME_MAX + 1),

    ALM_TBL_COL_DIM(LIC_BBOM, SAME_NAME_ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_BBOM, TYPE,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(LIC_BBOM, VALUE,
    ALM_TBL_DATA_TYPE_UINT64, NULL, sizeof(ALM_UINT64)),

    ALM_TBL_COL_DIM(LIC_BBOM, GROUP,
    ALM_TBL_DATA_TYPE_CHAR64, NULL, ALM_CONST_LIC_GROUP_NAME_MAX + 1),

    ALM_TBL_COL_DIM(LIC_BBOM, DEADLINE,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(LIC_BBOM, ALLOW_DAYS,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_BBOM, GRACE_DAYS,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM_END(LIC_BBOM),
    /************************************************************************
    * LIC_SBOM
    ************************************************************************/
    ALM_TBL_COL_DIM_START(LIC_SBOM),

    ALM_TBL_COL_DIM(LIC_SBOM, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_LSN + 1),

    ALM_TBL_COL_DIM(LIC_SBOM, OFFER_NAME,
    ALM_TBL_DATA_TYPE_CHAR128, NULL, ALM_CONST_LIC_FILE_OFFERING_NAME_MAX + 1),

    ALM_TBL_COL_DIM(LIC_SBOM, OFFER_VER,
    ALM_TBL_DATA_TYPE_CHAR128, NULL, ALM_CONST_LIC_FILE_OFFERING_VER_MAX + 1),
  
    ALM_TBL_COL_DIM(LIC_SBOM, NAME,
    ALM_TBL_DATA_TYPE_CHAR128, NULL, ALM_CONST_LIC_SBOM_NAME_MAX + 1),

    ALM_TBL_COL_DIM(LIC_SBOM, VALUE,
    ALM_TBL_DATA_TYPE_UINT64, NULL, sizeof(ALM_UINT64)),

    ALM_TBL_COL_DIM(LIC_SBOM, DEADLINE,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(LIC_SBOM, DES_ENG_UNIT,
    ALM_TBL_DATA_TYPE_CHAR256, NULL, ALM_CONST_LIC_SBOM_DESC_ENG_UNIT_MAX + 1),

    ALM_TBL_COL_DIM(LIC_SBOM, DES_ENG,
    ALM_TBL_DATA_TYPE_CHAR256, NULL, ALM_CONST_LIC_SBOM_DESC_ENG_MAX + 1),

    ALM_TBL_COL_DIM(LIC_SBOM, DES_CN_UNIT,
    ALM_TBL_DATA_TYPE_CHAR256, NULL, ALM_CONST_LIC_SBOM_DESC_CHS_UNIT_MAX + 1),

    ALM_TBL_COL_DIM(LIC_SBOM, DES_CN,
    ALM_TBL_DATA_TYPE_CHAR256, NULL, ALM_CONST_LIC_SBOM_DESC_CHS_MAX + 1),

    ALM_TBL_COL_DIM_END(LIC_SBOM),
    
    /************************************************************************
    * LIC_S2BBOM
    ************************************************************************/
    ALM_TBL_COL_DIM_START(LIC_S2BBOM),

    ALM_TBL_COL_DIM(LIC_S2BBOM, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_LSN + 1),

    ALM_TBL_COL_DIM(LIC_S2BBOM, SBOM_NAME,
    ALM_TBL_DATA_TYPE_CHAR128, NULL, ALM_CONST_LIC_SBOM_NAME_MAX + 1),

    /* BBOM段 */
    ALM_TBL_COL_DIM(LIC_S2BBOM, BBOM_NAME,
    ALM_TBL_DATA_TYPE_CHAR64, NULL, ALM_CONST_LIC_BBOM_NAME_MAX + 1),

    ALM_TBL_COL_DIM(LIC_S2BBOM, BBOM_SAME_NAME_ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_S2BBOM, BBOM_TYPE,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(LIC_S2BBOM, BBOM_DEADLINE,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(LIC_S2BBOM, BBOM_VALUE,
    ALM_TBL_DATA_TYPE_UINT64, NULL, sizeof(ALM_UINT64)),

    ALM_TBL_COL_DIM(LIC_S2BBOM, BBOM_GROUP,
    ALM_TBL_DATA_TYPE_CHAR128, NULL, ALM_CONST_LIC_GROUP_NAME_MAX + 1),

    ALM_TBL_COL_DIM(LIC_S2BBOM, BBOM_ALLOW_DAYS,
    ALM_TBL_DATA_TYPE_UINT32, NULL,  sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_S2BBOM, BBOM_GRACE_DAYS,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_S2BBOM, BBOM_BPRIMARY,
    ALM_TBL_DATA_TYPE_UCHAR, NULL, sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM_END(LIC_S2BBOM),
    /************************************************************************
    * LIC_ADDTION
    ************************************************************************/
    ALM_TBL_COL_DIM_START(LIC_ADDTION),

    ALM_TBL_COL_DIM(LIC_ADDTION, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_LSN + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, COPY_RIGHT,
    ALM_TBL_DATA_TYPE_CHAR256, NULL, ALM_CONST_LIC_FILE_COPY_RIGHT_MAX + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, FORMAT_VER,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_FILE_VER_LEN_MAX + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, CREATER,
    ALM_TBL_DATA_TYPE_CHAR64, NULL, ALM_CONST_LIC_FILE_CREATER_MAX + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, ISSUER,
    ALM_TBL_DATA_TYPE_CHAR256, NULL, ALM_CONST_LIC_FILE_ISSUER_MAX + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, CREATE_TIME,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, SW_UPDATA_TIME,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, COUNTRY,
    ALM_TBL_DATA_TYPE_CHAR256, NULL, ALM_CONST_LIC_FILE_COUNTRY_MAX + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, OFFICE,
    ALM_TBL_DATA_TYPE_CHAR256, NULL, ALM_CONST_LIC_FILE_OFFICE_MAX + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, CUSTOM,
    ALM_TBL_DATA_TYPE_CHAR512, NULL, ALM_CONST_LIC_FILE_CUSTOMER_MAX + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, LOCK_MODEL,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_ADDTION, NODE_SEQ,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_ADDTION, NODE_NAME,
    ALM_TBL_DATA_TYPE_CHAR64, NULL, ALM_CONST_LIC_NODE_NAME_MAX + 1),

    ALM_TBL_COL_DIM(LIC_ADDTION, NODE_DES,
    ALM_TBL_DATA_TYPE_CHAR256, NULL, ALM_CONST_LIC_NODE_DES_MAX + 1),

    ALM_TBL_COL_DIM_END(LIC_ADDTION),

    /************************************************************************
    * LIC_STATE
    ************************************************************************/
    ALM_TBL_COL_DIM_START(LIC_STATE),


    ALM_TBL_COL_DIM(LIC_STATE, PRD_ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_STATE, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_LSN + 1),

    /* 使用记录 */
    ALM_TBL_COL_DIM(LIC_STATE, RECOVER_USING,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, IS_USING,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, USE_COUNT,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    /* 状态部分 */
    ALM_TBL_COL_DIM(LIC_STATE, STATE,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(LIC_STATE, PASS_MINS,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_STATE, LEFT_MINS,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_STATE, IS_PERM,
    ALM_TBL_DATA_TYPE_UCHAR, "1", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, ENTER_TIME,
    ALM_TBL_DATA_TYPE_CHAR32, "", ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(LIC_STATE, GRACE_PASS_MINS,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    /* 校验项部分 */
    ALM_TBL_COL_DIM(LIC_STATE, ESN_MIS,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, PRD_NAME_MIS,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, PRD_VER_MIS,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, HAVE_RVKED,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, VALID_OVER,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, GRACE_OVER,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    /* 上一次刷新时保留的校验项及激活记录 */
    ALM_TBL_COL_DIM(LIC_STATE, LAST_IS_USING,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, LAST_ESN_MIS,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, LAST_PRD_NAME_MIS,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, LAST_PRD_VER_MIS,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, LAST_HAVE_RVKED,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, LAST_VALID_OVER,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_STATE, LAST_GRACE_OVER,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    /* 用于状态校验或激活校验的文件内容 */
    ALM_TBL_COL_DIM(LIC_STATE, LIC_TYPE,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(LIC_STATE, LIC_DEADLINE,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(LIC_STATE, LIC_REL_DAYS,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_STATE, LIC_GRACE_DAYS,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_STATE, LIC_ALLOW_COUNT,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),
    
    ALM_TBL_COL_DIM_END(LIC_STATE),
    
    /************************************************************************
    * LIC_BBOM_STATE
    ************************************************************************/
    ALM_TBL_COL_DIM_START(LIC_BBOM_STATE),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, PRD_ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, NULL, ALM_CONST_LIC_LSN + 1),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, BBOM_ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, SAME_NAME_ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, STATE,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, PASS_MINS,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, LEFT_MINS,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, IS_PERM,
    ALM_TBL_DATA_TYPE_UCHAR, "1", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, ENTER_TIME,
    ALM_TBL_DATA_TYPE_CHAR32, "", ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, VALUE,
    ALM_TBL_DATA_TYPE_UINT64, "0", sizeof(ALM_UINT64)),

    ALM_TBL_COL_DIM(LIC_BBOM_STATE, GRACE_PASS_MINS,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM_END(LIC_BBOM_STATE),
    
    /************************************************************************
    * STATE_RULE
    ************************************************************************/
    ALM_TBL_COL_DIM_START(STATE_RULE),

    ALM_TBL_COL_DIM(STATE_RULE, PRD_ID,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_INT32)),

    ALM_TBL_COL_DIM(STATE_RULE, ITEM,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_TBL_DATA_TYPE_ENUM)),

    ALM_TBL_COL_DIM(STATE_RULE, IGNORE,
    ALM_TBL_DATA_TYPE_UCHAR, NULL, sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(STATE_RULE, IGNORE_UP_AND_LOWER,
    ALM_TBL_DATA_TYPE_UCHAR, NULL, sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(STATE_RULE, MIS_STATE,
    ALM_TBL_DATA_TYPE_UINT32, NULL, sizeof(ALM_TBL_DATA_TYPE_ENUM)),

    ALM_TBL_COL_DIM_END(STATE_RULE),

    /************************************************************************
    * PS_PRD
    ************************************************************************/
    ALM_TBL_COL_DIM_START(PS_PRD),

    ALM_TBL_COL_DIM(PS_PRD, TIME_ID,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_CHAR)),

    ALM_TBL_COL_DIM(PS_PRD, ID,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(PS_PRD, VER,
    ALM_TBL_DATA_TYPE_CHAR32, "0", ALM_CONST_PRD_VER_LEN_MAX + 1),

    ALM_TBL_COL_DIM_END(PS_PRD),

    /************************************************************************
    * PS_LIC
    ************************************************************************/
    ALM_TBL_COL_DIM_START(PS_LIC),

    ALM_TBL_COL_DIM(PS_LIC, TIME_ID,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_CHAR)),

    ALM_TBL_COL_DIM(PS_LIC, PRD_ID,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(PS_LIC, LSN,
    ALM_TBL_DATA_TYPE_CHAR32, "0", ALM_CONST_LIC_LSN + 1),


    /* 使用记录 */
    ALM_TBL_COL_DIM(PS_LIC, BE_USING,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_CHAR)),

    ALM_TBL_COL_DIM(PS_LIC, USED_COUNT,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_CHAR)),


    /* 状态部分 */
    ALM_TBL_COL_DIM(PS_LIC, STATE,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_TBL_IDX_ENUM)),

    ALM_TBL_COL_DIM(PS_LIC, ENTER_TIME,
    ALM_TBL_DATA_TYPE_CHAR32, "0", ALM_CONST_DATE_LEN + 1),

    ALM_TBL_COL_DIM(PS_LIC, PASS_MINS,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),

    ALM_TBL_COL_DIM(PS_LIC, GRACE_PASS_MINS,
    ALM_TBL_DATA_TYPE_UINT32, "0", sizeof(ALM_UINT32)),


    /* 校验项部分 */
    ALM_TBL_COL_DIM(PS_LIC, ESN_MIS,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(PS_LIC, PRD_NAME_MIS,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(PS_LIC, PRD_VER_MIS,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(PS_LIC, BE_RVKED,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(PS_LIC, VALID_OVER,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM(PS_LIC, GRACE_OVER,
    ALM_TBL_DATA_TYPE_UCHAR, "0", sizeof(ALM_BOOL)),

    ALM_TBL_COL_DIM_END(PS_LIC),
};

/************************************************************************
* 定义表格
************************************************************************/
static ALM_TBL_DIM_STRU s_astTbls[] = {
    ALM_TBL_DIM(PRD_BBOMS),
    ALM_TBL_DIM(PRD_INFO),
    ALM_TBL_DIM(LIC),
    ALM_TBL_DIM(LIC_ESN),
    ALM_TBL_DIM(LIC_BBOM),
    ALM_TBL_DIM(LIC_SBOM),
    ALM_TBL_DIM(LIC_S2BBOM),
    ALM_TBL_DIM(LIC_ADDTION),
    ALM_TBL_DIM(LIC_STATE),
    ALM_TBL_DIM(LIC_BBOM_STATE),
    ALM_TBL_DIM(STATE_RULE),
    ALM_TBL_DIM(PS_PRD),
    ALM_TBL_DIM(PS_LIC),
};

/************************************************************************
* 函数的实现。
************************************************************************/

/* 排序表格及列定义*/
static ALM_VOID ALMTblsSortDim()
{
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdx2;
    ALM_TBL_DIM_STRU stTmp;
    ALM_TBL_COL_DIM_EX_STRU stTmpCol;

    /* 排序表格定义 */
    for (uiIdx = 1;
        uiIdx < sizeof(s_astTbls) / sizeof(ALM_TBL_DIM_STRU);
        ++uiIdx)
    {
        for (uiIdx2 = 0; uiIdx2 < uiIdx; ++uiIdx2)
        {
            if (s_astTbls[uiIdx2].eTblIdx > s_astTbls[uiIdx].eTblIdx)
            {
                (ALM_VOID)ALM_memcpy_s(&stTmp,
                    sizeof(stTmp),
                    &s_astTbls[uiIdx2],
                    sizeof(s_astTbls[uiIdx2]));

                (ALM_VOID)ALM_memcpy_s(&s_astTbls[uiIdx2],
                    sizeof(s_astTbls[uiIdx2]),
                    &s_astTbls[uiIdx],
                    sizeof(s_astTbls[uiIdx]));

                (ALM_VOID)ALM_memcpy_s(&s_astTbls[uiIdx],
                    sizeof(s_astTbls[uiIdx]),
                    &stTmp,
                    sizeof(stTmp));
            }
        }
    }

    /* 排序列定义 */
    for (uiIdx = 1;
        uiIdx < sizeof(s_astTblCols) / sizeof(ALM_TBL_COL_DIM_EX_STRU);
        ++uiIdx)
    {
        for (uiIdx2 = 0; uiIdx2 < uiIdx; ++uiIdx2)
        {
            if (s_astTblCols[uiIdx2].eIdx > s_astTblCols[uiIdx].eIdx)
            {
                (ALM_VOID)ALM_memcpy_s(&stTmpCol,
                    sizeof(stTmpCol),
                    &s_astTblCols[uiIdx2],
                    sizeof(s_astTblCols[uiIdx2]));

                (ALM_VOID)ALM_memcpy_s(&s_astTblCols[uiIdx2],
                    sizeof(s_astTblCols[uiIdx2]),
                    &s_astTblCols[uiIdx],
                    sizeof(s_astTblCols[uiIdx]));

                (ALM_VOID)ALM_memcpy_s(&s_astTblCols[uiIdx],
                    sizeof(s_astTblCols[uiIdx]),
                    &stTmpCol,
                    sizeof(stTmpCol));
            }
        }
    }
}

/* 校验表格及列定义 */
static ALM_BOOL ALMTblsCheckDim()
{
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdx2;
    ALM_UINT32 uiIdx3;
    ALM_INT32 iCmp;
    CONST ALM_TBL_COL_DIM_EX_STRU* pstCols;
    ALM_UINT32 uiSize;
    ALM_INT32 iRet;
    ALM_UINT32 uiColIdx;

    /* 校验表格。 */
    for (uiIdx = 0;
        uiIdx < sizeof(s_astTbls) / sizeof(ALM_TBL_DIM_STRU);
        ++uiIdx)
    {
        /* 索引必须与数组索引一致 */
        ALM_CHK_RET((ALM_UINT32)s_astTbls[uiIdx].eTblIdx == uiIdx, FALSE,
            ("Table index is differ with dim:%d.", uiIdx));

        /* 表格名不能重复 */
        for (uiIdx2 = 0; uiIdx2 < uiIdx; ++uiIdx2)
        {
            iCmp = strcmp(s_astTbls[uiIdx2].pcTblName,
                s_astTbls[uiIdx].pcTblName);

            ALM_CHK_RET(iCmp != 0, FALSE, ("Same table name \"%s\": %d - %d.",
                s_astTbls[uiIdx].pcTblName, uiIdx, uiIdx2));
        }

        /* 校验表列 */
        ALM_CHK_RET(s_astTbls[uiIdx].pstColDim == s_astTblCols, FALSE, (NULL));
        ALM_CHK_RET(s_astTbls[uiIdx].uiColNum > 0, FALSE, (NULL));
        ALM_CHK_RET(s_astTbls[uiIdx].uiStartIdx + s_astTbls[uiIdx].uiColNum
            < sizeof(s_astTblCols) / sizeof(ALM_TBL_COL_DIM_EX_STRU),
            FALSE, (NULL));

        /* 校验列 */
        pstCols = &(s_astTbls[uiIdx].pstColDim[s_astTbls[uiIdx].uiStartIdx]);
        for (uiIdx2 = 0; uiIdx2 < s_astTbls[uiIdx].uiColNum; ++uiIdx2)
        {
            uiColIdx = s_astTbls[uiIdx].uiStartIdx + uiIdx2;

            /* 列索引与列序号保持一致 */
            ALM_CHK_RET((ALM_UINT32)s_astTblCols[uiColIdx].eIdx == uiColIdx,
                FALSE, ("Col index is differ with dim: %d.", uiColIdx));

            /* 数据类型能容纳所需大小 */
            ALM_CHK_RET(s_astTblCols[uiColIdx].uiNeedSize > 0, FALSE,
                ("Col need size is zero: %d.", uiColIdx));

            iRet = ALM_TblDataSize(s_astTblCols[uiColIdx].stCol.eDataType,
                &uiSize);
            ALM_CALL_RET(FALSE, (NULL));

            ALM_CHK_RET(s_astTblCols[uiColIdx].uiNeedSize <= uiSize, FALSE,
                ("Col is not enough to store data: %d.", uiColIdx));

            /* 列名不能重复 */
            for (uiIdx3 = 0; uiIdx3 < uiIdx2; ++uiIdx3)
            {
                iCmp = strcmp(pstCols[uiIdx2].stCol.pcColName,
                    pstCols[uiIdx3].stCol.pcColName);

                ALM_CHK_RET(iCmp != 0, FALSE, ("Same col name \"%s: %s\".",
                    s_astTbls[uiIdx].pcTblName,
                    pstCols[uiIdx2].stCol.pcColName));
            }
        }

        /* 表格间列索引不能有交集 */
        for (uiIdx2 = 0; uiIdx2 < uiIdx; ++uiIdx2)
        {
            if (s_astTbls[uiIdx2].uiStartIdx < s_astTbls[uiIdx].uiStartIdx)
            {
                ALM_CHK_RET(
                    s_astTbls[uiIdx2].uiStartIdx + s_astTbls[uiIdx2].uiColNum
                    < s_astTbls[uiIdx].uiStartIdx,
                    FALSE, ("Table col index accoss\"%s: %s\".",
                    s_astTbls[uiIdx].pcTblName, s_astTbls[uiIdx2].pcTblName));
            }
            else
            {
                ALM_CHK_RET(
                    s_astTbls[uiIdx].uiStartIdx + s_astTbls[uiIdx].uiColNum
                    < s_astTbls[uiIdx2].uiStartIdx,
                    FALSE, ("Table col index accoss\"%s: %s\".",
                    s_astTbls[uiIdx].pcTblName, s_astTbls[uiIdx2].pcTblName));
            }
        }
    }

    return TRUE;
}

/*初始化建立所有表格*/
ALM_INT32 ALM_TblsMgrCreateAll()
{
    ALM_INT32 iRet = ALM_OK;
    ALM_TBL_COL_DIM_STRU* pstCols = NULL;
    ALM_UINT32 uiColNum = 0;
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdxCol;

    ALMTblsSortDim();

    ALM_CHK_RET(ALMTblsCheckDim(), ALM_ERR_UNKNOW, (NULL));

    for (uiIdx = 0; uiIdx < sizeof(s_astTbls) / sizeof(ALM_TBL_DIM_STRU);
        ++uiIdx)
    {
        if (s_astTbls[uiIdx].uiColNum > uiColNum)
        {
            ALM_OsBaseMemFree(pstCols);

            uiColNum = s_astTbls[uiIdx].uiColNum;
            pstCols = (ALM_TBL_COL_DIM_STRU*)ALM_OsBaseMemAlloc(
                sizeof(ALM_TBL_COL_DIM_STRU) * uiColNum);
            ALM_CHK_LAB(!(NULL == pstCols
                && (iRet = ALM_ERR_OS_MALLOC_FAIL) != ALM_OK),
                LAB_END, (NULL));
        }

        for (uiIdxCol = 0; uiIdxCol < s_astTbls[uiIdx].uiColNum;
            ++uiIdxCol)
        {
            (ALM_VOID)ALM_memcpy_s(pstCols + uiIdxCol,
                sizeof(ALM_TBL_COL_DIM_STRU),
                &s_astTbls[uiIdx].pstColDim[s_astTbls[uiIdx].uiStartIdx
                + uiIdxCol].stCol,
                sizeof(ALM_TBL_COL_DIM_STRU));
        }

        iRet = ALM_TblNew(s_astTbls[uiIdx].pcTblName,
            s_astTbls[uiIdx].uiColNum,
            pstCols);
        ALM_CALL_LAB(LAB_END, (NULL));
    }

LAB_END:
    ALM_OsBaseMemFree(pstCols);
    if (iRet != ALM_OK)
    {
         ALM_TblsMgrDropAll();
    }

    return iRet;
}

/*删除所有表格*/
ALM_VOID ALM_TblsMgrDropAll()
{
    ALM_TblDropAll();
}

/* 得到表格定义 */
CONST ALM_TBL_DIM_STRU* ALM_TblsGetDim(ALM_TBL_IDX_ENUM eTblIdx)
{
    ALM_CHK_RET(eTblIdx >= 0
        && eTblIdx < sizeof(s_astTbls) / sizeof(ALM_TBL_DIM_STRU),
        NULL, (NULL));

    return s_astTbls + eTblIdx;
}

/* 得到列定义 */
CONST ALM_TBL_COL_DIM_EX_STRU* ALM_TblsGetColDim(ALM_TBL_COL_IDX_ENUM eCol)
{
    ALM_CHK_RET(eCol >= 0
        && eCol < sizeof(s_astTblCols) / sizeof(ALM_TBL_COL_DIM_EX_STRU),
        NULL, (NULL));

    return s_astTblCols + eCol;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
