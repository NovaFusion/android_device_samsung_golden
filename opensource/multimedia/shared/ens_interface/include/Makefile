# vim:syntax=make:
# -*- Mode: Makefile -*-

#-------------------------------
# Default Config
#-------------------------------
include $(MM_MAKEFILES_DIR)/SharedConfig.mk


INSTALL_HEADERDIR= . omxil 
VPATH+= omxil

# Symbian compilation
GROUP_PATH = $(MMROOT)/shared/ens_interface/include
GROUP_NAME = group
SYMBIAN_PRJNAME = st_ens_interface
shared_EXPORTDIR = . omxil
SYMBIAN_ALL = symb-exportsonly

ifeq ($(MYMMSYSTEM),symbian)
todo: install $(SYMBIAN_ALL)
else
todo: install 
endif

include $(MM_MAKEFILES_DIR)/SharedRules.mk
include $(MM_MAKEFILES_DIR)/SharedSymbianRules.mk
