/**
 *  @file model_lib.c
 *
 *  @brief Implementation file for model library file
 *
 *  @author Min Zhang
 *
 *  @note
 */

/* --- standard C lib header files -------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>

/* --- internal header files -------------------------------------------------------------------- */
#include "jf_basic.h"
#include "jf_limit.h"
#include "jf_mem.h"
#include "jf_dir.h"

#include "tx_model.h"

#include "model_lib.h"
#include "model_common.h"

/* --- private data/data structure section ------------------------------------------------------ */

/** The prefix string for model library file.
 */
#define MODEL_LIB_FILE_PREFIX  "libtx_model_"

/** The file extension of the model library file.
 */
#define MODEL_LIB_FILE_EXT     ".so"

/** The entry name of the library, MUST be implemented.
 */
#define MODEL_LIB_ENTRY_NAME  "tx_model_fillModel"

typedef u32 (* tx_model_fnFillModel_t)(tx_model_t * ptm);

/* --- private routine section ------------------------------------------------------------------ */

static u32 _handleModelLibFile(
    const olchar_t * pstrFullpath, jf_file_stat_t * pStat, jf_listhead_t * pjl)
{
    u32 u32Ret = JF_ERR_NO_ERROR;
    tx_model_t * ptm = NULL;
    tx_model_fnFillModel_t fnFillModel;

    u32Ret = createTxModel(&ptm);
    if (u32Ret == JF_ERR_NO_ERROR)
    {
        u32Ret = jf_dynlib_load(pstrFullpath, &ptm->tm_pjdLib);
    }

    if (u32Ret == JF_ERR_NO_ERROR)
    {
        u32Ret = jf_dynlib_getSymbolAddress(
            ptm->tm_pjdLib, MODEL_LIB_ENTRY_NAME, (void **)&fnFillModel);
    }

    if (u32Ret == JF_ERR_NO_ERROR)
    {
        u32Ret = fnFillModel(ptm);
    }

    if (u32Ret == JF_ERR_NO_ERROR)
    {
        u32Ret = checkTxModelField(ptm);
    }
    
    if (u32Ret == JF_ERR_NO_ERROR)
    {
        jf_listhead_add(pjl, &ptm->tm_jlList);
    }
    else if (ptm != NULL)
    {
        jf_logger_logErrMsg(u32Ret, "failed to load model");
        destroyTxModel(&ptm);
    }

    return u32Ret;
}

/* --- public routine section ------------------------------------------------------------------- */

boolean_t isTxModelLibFile(const olchar_t * pstrName)
{
    return jf_file_isTypedFile(pstrName, MODEL_LIB_FILE_PREFIX, MODEL_LIB_FILE_EXT);
}

u32 handleTxModelLibFile(
    const olchar_t * pstrFullpath, jf_file_stat_t * pStat, jf_listhead_t * pjl)
{
    JF_LOGGER_INFO("path: %s", pstrFullpath);

    return _handleModelLibFile(pstrFullpath, pStat, pjl);
}

/*------------------------------------------------------------------------------------------------*/

