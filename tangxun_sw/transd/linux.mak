#
#  @file linux.mak
#
#  @brief The makefile for Tangxun transaction daemon.
#
#  @author Min Zhang
#
#  @note
#
#  

#---------------------------------------------------------------------------------------------------

# Name of the executable file
EXE = tx_transd

# Source files
SOURCES = datransd.c main.c

# Jiutai source files
JIUTAI_SRCS = stocklist.c envvar.c datastat.c stocktrade.c

# For code complile
EXTRA_INC_DIR = -I../jtk/inc
EXTRA_CFLAGS = -DENT

# For executable file build 
EXTRA_LDFLAGS = 
EXTRA_OBJECTS = ../jtk/inc/jf_mutex.o ../jtk/inc/jf_sem.o ../jtk/inc/jf_thread.o
EXTRA_LIB_DIR = -L../jtk/lib 
EXTRA_LIBS = -ljf_logger -ljf_files -ljf_string -ljf_ifmgmt -ljf_network \
    -ljf_jiukun -ljf_httpparser -ljf_persistency -ljf_clieng -ljf_webclient -ltx_parsedata \
    -lsqlite3 -lm

include $(TOPDIR)/mak/lnxexe.mak

#---------------------------------------------------------------------------------------------------

