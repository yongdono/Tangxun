#
#  @file Makefile linux.mak
#
#  @brief The Makefile for cli utility
#
#  @author Min Zhang
#
#  @note
#
#  

#---------------------------------------------------------------------------------------------------

# Name of the executable file
EXE = cli

# Source files
SOURCES = env.c indi.c model.c stat.c fix.c download.c rule.c \
	find.c parse.c backtest.c \
    analysis.c stock.c misc.c trade.c clicmd.c main.c

# Jiutai source files
JIUTAI_SRCS = stocklist.c parsedata.c fixdata.c datastat.c regression.c \
    downloaddata.c indicator.c statarbitrage.c damethod.c envvar.c


# For code complile
EXTRA_CFLAGS =
EXTRA_INC_DIR = -I$(TOPDIR)/jtk/inc
EXTRA_OBJECTS = $(TOPDIR)/jtk/inc/jf_mem.o $(TOPDIR)/jtk/inc/jf_time.o

# For executable file build 
EXTRA_LDFLAGS = 
EXTRA_LIB_DIR = -L../jtk/lib
EXTRA_LIBS = -ljf_logger -ljf_files -ljf_clieng -ljf_string -ljf_ifmgmt \
    -ljf_network -ljf_httpparser -ljf_jiukun -ljf_matrix -ljf_persistency \
    -ldatradehelper -ldarule -ldamodel -ldatrade_persistency -ldabacktesting \
    -lm -lsqlite3

include $(TOPDIR)/mak/lnxexe.mak

#---------------------------------------------------------------------------------------------------

