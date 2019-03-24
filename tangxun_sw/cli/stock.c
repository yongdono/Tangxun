/**
 *  @file stock.c
 *
 *  @brief The stock command
 *
 *  @author Min Zhang
 *
 *  @note
 *  
 */

/* --- standard C lib header files ----------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <math.h>
#if defined(WINDOWS)

#elif defined(LINUX)
    #include <stdlib.h>
#endif

/* --- internal header files ----------------------------------------------- */
#include "olbasic.h"
#include "ollimit.h"
#include "bases.h"
#include "clicmd.h"
#include "stringparse.h"
#include "files.h"
#include "xmalloc.h"
#include "stocklist.h"
#include "indicator.h"
#include "statarbitrage.h"
#include "jiukun.h"
#include "damethod.h"
#include "envvar.h"

/* --- private data/data structure section --------------------------------- */
static clieng_caption_t ls_ccStockInfoAdditionalVerbose[] =
{
    {"CorrelationWithIndex", CLIENG_CAP_HALF_LINE}, {"CorrelationWithSmeIndex", CLIENG_CAP_HALF_LINE},
};

static clieng_caption_t ls_ccIndustryInfoBrief[] =
{
    {"Id", 4},
    {"Desc", 30},
    {"NumOfStock", 11},
};

static clieng_caption_t ls_ccIndustryInfoVerbose[] =
{
    {"Id", CLIENG_CAP_FULL_LINE},
    {"Desc", CLIENG_CAP_FULL_LINE},
    {"NumOfStock", CLIENG_CAP_FULL_LINE},
    {"Stock", CLIENG_CAP_FULL_LINE},
};

static clieng_caption_t ls_ccStockInfoVerbose[] =
{
    {"Name", CLIENG_CAP_FULL_LINE},
    {"GeneralCapital", CLIENG_CAP_HALF_LINE}, {"TradableShare", CLIENG_CAP_HALF_LINE},
};

/* --- private routine section---------------------------------------------- */

static u32 _stockHelp(da_master_t * pdm)
{
    u32 u32Ret = OLERR_NO_ERROR;

    cliengOutputRawLine2("\
stock\n\
stock [-s stock] [-i] [-v]");
    cliengOutputRawLine2("\
  -s: list the specified stock.\n\
  -i: show industry information.");
    cliengOutputRawLine2("\
  -v: verbose.");
    cliengOutputLine("");

    return u32Ret;
}

static void _printStockInfoAdditionalVerbose(stock_info_t * info)
{
    clieng_caption_t * pcc = &ls_ccStockInfoAdditionalVerbose[0];
    olchar_t strLeft[MAX_OUTPUT_LINE_LEN], strRight[MAX_OUTPUT_LINE_LEN];

    cliengPrintDivider();

    /*CorrelationWithIndex*/
    ol_sprintf(strLeft, "%.2f", getCorrelationWithIndex(info));
    ol_sprintf(strRight, "%.2f", getCorrelationWithSmeIndex(info));
    cliengPrintTwoHalfLine(pcc, strLeft, strRight);
    pcc += 2;

    cliengOutputLine("");
}

static void _printStockInfoVerbose(stock_info_t * info)
{
    clieng_caption_t * pcc = &ls_ccStockInfoVerbose[0];
    olchar_t strLeft[MAX_OUTPUT_LINE_LEN], strRight[MAX_OUTPUT_LINE_LEN];

    cliengPrintDivider();

    /*Name*/
    ol_sprintf(strLeft, "%s", info->si_strCode);
    cliengPrintOneFullLine(pcc, strLeft);
    pcc += 1;

    /*GeneralCapital*/
    ol_sprintf(strLeft, "%lld", info->si_u64GeneralCapital);
    ol_sprintf(strRight, "%lld", info->si_u64TradableShare);
    cliengPrintTwoHalfLine(pcc, strLeft, strRight);
    pcc += 2;

    /*Industry*/
    ol_sprintf(strLeft, "%s", getStringIndustry(info->si_nIndustry));
    cliengPrintOneFullLine(pcc, strLeft);
    pcc += 1;

    cliengOutputLine("");
}

static u32 _printStockVerbose(cli_stock_param_t * pcsp)
{
    u32 u32Ret = OLERR_NO_ERROR;
    stock_info_t * info;

    u32Ret = getStockInfo(pcsp->csp_pstrStock, &info);
    if (u32Ret == OLERR_NO_ERROR)
    {
        _printStockInfoVerbose(info);
        _printStockInfoAdditionalVerbose(info);
    }

    return u32Ret;
}

static void _printOneIndustryInfoBrief(stock_indu_info_t * info)
{
    clieng_caption_t * pcc = &ls_ccIndustryInfoBrief[0];
    olchar_t strInfo[MAX_OUTPUT_LINE_LEN], strField[MAX_OUTPUT_LINE_LEN];

    strInfo[0] = '\0';

    /* Id */
    ol_sprintf(strField, "%d", info->sii_nId);
    cliengAppendBriefColumn(pcc, strInfo, strField);
    pcc++;

    /* desc */
    cliengAppendBriefColumn(pcc, strInfo, info->sii_pstrDesc);
    pcc++;

    /* NumOfStock */
    ol_sprintf(strField, "%d", info->sii_nStock);
    cliengAppendBriefColumn(pcc, strInfo, strField);
    pcc++;

    cliengOutputLine(strInfo);
}

static void _printOneIndustryInfoVerbose(stock_indu_info_t * info)
{
    clieng_caption_t * pcc = &ls_ccIndustryInfoVerbose[0];
    olchar_t strLeft[MAX_OUTPUT_LINE_LEN]; //, strRight[MAX_OUTPUT_LINE_LEN];
    olint_t n, j, r, len;
    olchar_t line[80];

    cliengPrintDivider();

    /*Id*/
    ol_sprintf(strLeft, "%d", info->sii_nId);
    cliengPrintOneFullLine(pcc, strLeft);
    pcc += 1;

    /*Desc*/
    cliengPrintOneFullLine(pcc, info->sii_pstrDesc);
    pcc += 1;

    /*NumOfStock*/
    ol_sprintf(strLeft, "%d", info->sii_nStock);
    cliengPrintOneFullLine(pcc, strLeft);
    pcc += 1;

    /*Stocks*/
    ol_sprintf(strLeft, "%s", " ");
    cliengPrintOneFullLine(pcc, strLeft);
    pcc += 1;

    len = ol_strlen(info->sii_pstrStocks);
    n = (len + 72 - 1) / 72;
    for (j = 0; j < n; j ++)
    {
        r = len - j * 72;
        if (r > 72)
            r = 72;

        memset(line, 0, sizeof(line));
        ol_strncpy(line, info->sii_pstrStocks + 72 * j, r);
        cliengOutputLine("%s", line);
    }

    cliengOutputLine("");
}

static u32 _printIndustryInfo(cli_stock_param_t * pcsp)
{
    u32 u32Ret = OLERR_NO_ERROR;
    stock_indu_info_t * info;
    olint_t total = 0;

    if (! pcsp->csp_bVerbose)
    {
        cliengPrintDivider();
        cliengPrintHeader(
            ls_ccIndustryInfoBrief,
            sizeof(ls_ccIndustryInfoBrief) / sizeof(clieng_caption_t));
    }

    info = getFirstIndustryInfo();
    while (info != NULL)
    {
        if (pcsp->csp_bVerbose)
            _printOneIndustryInfoVerbose(info);
        else
            _printOneIndustryInfoBrief(info);

        total += info->sii_nStock;

        info = getNextIndustryInfo(info);
    }

    cliengOutputLine("");
    cliengOutputLine("Total %d stocks\n", total);


    return u32Ret;
}

/* --- public routine section ---------------------------------------------- */
u32 processStock(void * pMaster, void * pParam)
{
    u32 u32Ret = OLERR_NO_ERROR;
    cli_stock_param_t * pcsp = (cli_stock_param_t *)pParam;
    da_master_t * pdm = (da_master_t *)pMaster;

    if (pcsp->csp_u8Action == CLI_ACTION_SHOW_HELP)
        u32Ret = _stockHelp(pdm);
    else if (pcsp->csp_u8Action == CLI_ACTION_LIST_INDUSTRY)
        u32Ret = _printIndustryInfo(pcsp);
    else if (isNullEnvVarDataPath())
    {
        cliengOutputLine("Data path is not set.");
        u32Ret = OLERR_NOT_READY;
    }
    else if (pcsp->csp_u8Action == CLI_ACTION_LIST_STOCK)
        u32Ret = _printStockVerbose(pcsp);
    else
        u32Ret = OLERR_MISSING_PARAM;

    return u32Ret;
}

u32 setDefaultParamStock(void * pMaster, void * pParam)
{
    u32 u32Ret = OLERR_NO_ERROR;
    cli_stock_param_t * pcsp = (cli_stock_param_t *)pParam;

    memset(pcsp, 0, sizeof(*pcsp));

    pcsp->csp_u8Action = CLI_ACTION_LIST_STOCK;

    return u32Ret;
}

u32 parseStock(void * pMaster, olint_t argc, olchar_t ** argv, void * pParam)
{
    u32 u32Ret = OLERR_NO_ERROR;
    cli_stock_param_t * pcsp = (cli_stock_param_t *)pParam;
//    jiufeng_cli_master_t * pocm = (jiufeng_cli_master_t *)pMaster;
    olint_t nOpt;

    optind = 0;  /* initialize the opt index */

    while (((nOpt = getopt(argc, argv,
        "s:ivh?")) != -1) && (u32Ret == OLERR_NO_ERROR))
    {
        switch (nOpt)
        {
        case 's':
            pcsp->csp_u8Action = CLI_ACTION_LIST_STOCK;
            pcsp->csp_pstrStock = (olchar_t *)optarg;
            break;
        case 'i':
            pcsp->csp_u8Action = CLI_ACTION_LIST_INDUSTRY;
            break;
        case 'v':
            pcsp->csp_bVerbose = TRUE;
            break;
        case ':':
            u32Ret = OLERR_MISSING_PARAM;
            break;
        case '?':
        case 'h':
            pcsp->csp_u8Action = CLI_ACTION_SHOW_HELP;
            break;
        default:
            u32Ret = cliengReportNotApplicableOpt(nOpt);
        }
    }

    return u32Ret;
}

/*---------------------------------------------------------------------------*/


