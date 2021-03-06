/**
 *  @file rule_st.c
 *
 *  @brief Implementation file for rules related to st.
 *
 *  @author Min Zhang
 *
 *  @note
 *  
 */

/* --- standard C lib header files -------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>

/* --- internal header files -------------------------------------------------------------------- */

#include "jf_basic.h"
#include "jf_limit.h"
#include "jf_jiukun.h"

#include "tx_rule.h"
#include "rule_st.h"

/* --- private data/data structure section ------------------------------------------------------ */


/* --- private routine section ------------------------------------------------------------------ */


/* --- public routine section ------------------------------------------------------------------- */

u32 txRuleNotStRelated(
    tx_stock_info_t * stockinfo, tx_ds_t * buffer, int total, void * pParam)
{
    u32 u32Ret = JF_ERR_NOT_MATCH;
    tx_ds_t * start = buffer;
    tx_ds_t * end = buffer + total - 1;

    while (start <= end)
    {
        if ((start->td_bSt) || (start->td_bStDelisting))
            return u32Ret;

        start ++;
    }

    return JF_ERR_NO_ERROR;
}

u32 txRuleSt(
    tx_stock_info_t * stockinfo, tx_ds_t * buffer, int total, void * pParam)
{
    u32 u32Ret = JF_ERR_NO_ERROR;
    tx_ds_t * start = buffer;
    tx_ds_t * end = buffer + total - 1;

    while (start <= end)
    {
        if (start->td_bSt)
            return u32Ret;

        start ++;
    }

    return JF_ERR_NOT_MATCH;
}

u32 txRuleStDelisting(
    tx_stock_info_t * stockinfo, tx_ds_t * buffer, int total, void * pParam)
{
    u32 u32Ret = JF_ERR_NO_ERROR;
    tx_ds_t * start = buffer;
    tx_ds_t * end = buffer + total - 1;

    while (start <= end)
    {
        if (start->td_bStDelisting)
            return u32Ret;

        start ++;
    }

    return JF_ERR_NOT_MATCH;
}

/*------------------------------------------------------------------------------------------------*/


