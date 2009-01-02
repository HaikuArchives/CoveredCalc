# CoveredCalc
#
# Copyright (c) 2004-2008 CoveredCalc Project Contributors
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

ifeq ($(MAKECMDGOALS), release)
RELEASE = true
NODEPENDS = true
endif

ifeq ($(MAKECMDGOALS), clean)
NODEPENDS = true
endif

# ------
NAME= CoveredCalc

CPPFLAGS = -Wall -Wno-multichar -Wno-ctor-dtor-privacy -Woverloaded-virtual
LDFLAGS = -Xlinker -soname=_APP_

ifneq ($(RELEASE), true)
DEBUG = true
endif

ifeq ($(DEBUG), true)
OBJ_DIR = obj.debug
CPPFLAGS += -O0 -g -DDEBUG
LDFLAGS += -g
else
OBJ_DIR = obj.release
CPPFLAGS += -O1
LDFLAGS += -Xlinker -s -Xlinker -x
endif

#%{
# @src->@ 

SRCS= \
	../Application/AppSettings.cpp \
	../Application/BeCoveredCalcApp.cpp \
	../Application/CommandLineParam.cpp \
	../Application/Copyright.cpp \
	../Application/CoveredCalcAppBase.cpp \
	../Application/DialogID.cpp \
	../Application/KeyMappings.cpp \
	../Application/LangFileInfo.cpp \
	../Application/SettingValue.cpp \
	../Application/StringID.cpp \
	../Application/XMLLangFile.cpp \
	../Bitmap/BMPFileLoader.cpp \
	../Bitmap/BMPFileStore.cpp \
	../Bitmap/DIBColorLookup.cpp \
	../Bitmap/DIBitmap.cpp \
	../Engine/CalcCore.cpp \
	../General/MBCString.cpp \
	../General/Exception.cpp \
	../General/MemoryException.cpp \
	../Skin/ColorCodedSkin.cpp \
	../Skin/CoverButtonInfo.cpp \
	../Skin/CoverDef.cpp \
	../Skin/CoverDefParseException.cpp \
	../Skin/CoverDefParser.cpp \
	../Skin/CoverDefValidateException.cpp \
	../Skin/CoverDisplayInfo.cpp \
	../Skin/CoverFontInfo.cpp \
	../Skin/CoverInfo.cpp \
	../Skin/CoverListManager.cpp \
	../Skin/CoverMainWindowInfo.cpp \
	../Skin/CoverManager.cpp \
	../Skin/CoverToolTipInfo.cpp \
	../Skin/CoverWindowInfo.cpp \
	../Storage/BeFile.cpp \
	../Storage/FileException.cpp \
	../Storage/Path.cpp \
	../UI/BeCCSAppearance.cpp \
	../UI/BeCoverBrowser.cpp \
	../UI/BeDeskbarPseudoWindow.cpp \
	../UI/BeKeyEventFilter.cpp \
	../UI/BeMainView.cpp \
	../UI/BeMainWindow.cpp \
	../UI/BeMessageBoxProvider.cpp \
	../UI/BeSkinView.cpp \
	../UI/BeSkinWindow.cpp \
	../UI/ButtonUITask.cpp \
	../UI/CoverBrowser.cpp \
	../UI/KeyMappingManager.cpp \
	../UI/KeyNameDB.cpp \
	../UI/MainUIManager.cpp \
	../UI/MainWindowKeyFunc.cpp \
	../UI/MouseHoverUITask.cpp \
	../UI/MovingUITask.cpp \
	../UI/UIManager.cpp \
	../UI/UITaskManager.cpp \
	../UI/Message/ExceptionMessageGenerator.cpp \
	../UI/Control/BeBitmapView.cpp \
	../UI/Control/BeCCColumnListView.cpp \
	../UI/Control/BeKeyInputEdit.cpp \
	../UI/Control/BeToolTipWindow.cpp \
	../UI/Dialog/AboutCurrentCoverDlg.cpp \
	../UI/Dialog/BeAboutCurrentCoverDialog.cpp \
	../UI/Dialog/BeAboutDialog.cpp \
	../UI/Dialog/BeDialog.cpp \
	../UI/Dialog/BeEditKeymapDlg.cpp \
	../UI/Dialog/BePreferencesDlg.cpp \
	../UI/Dialog/EditKeymapDlg.cpp \
	../UI/Dialog/PreferencesDlg.cpp \
	../UI/DialogUtility/BeDialogControlHelper.cpp \
	../UI/DialogUtility/BeDialogManager.cpp \
	../UI/DialogUtility/DialogLayout.cpp \
	../UI/UIComponent/BeControlAdapter.cpp \
	../UI/UIComponent/BeKeyInputAdapter.cpp \
	../UI/UIComponent/BeLabeledControlAdapter.cpp \
	../UI/UIComponent/BeListViewAdapter.cpp \
	../UI/UIComponent/BeTextControlAdapter.cpp \
	../UI/UIComponent/BeTextViewAdapter.cpp \
	../UI/UIComponent/BeViewAdapter.cpp \
	../Utility/ExceptionMessageUtils.cpp \
	../Utility/MessageFormatter.cpp \
	../Utility/PathUtils.cpp \
	../Utility/StorageUtils.cpp \
	../Utility/UTF8ConvBe.cpp \
	../XML/DOMParseEventHandler.cpp \
	../XML/DOMUtils.cpp \
	../XML/XMLParseException.cpp \
	../XML/XMLParser.cpp \
	../XML/NanchatteDOM/NCDElementImpl.cpp \
	../XML/NanchatteDOM/NCDAttrImpl.cpp \
	../XML/NanchatteDOM/NCDDocumentImpl.cpp \
	../XML/NanchatteDOM/NCDAttrNodeMapImpl.cpp \
	../XML/NanchatteDOM/NCDCommentImpl.cpp \
	../XML/NanchatteDOM/NCDTreeNode.cpp \
	../XML/NanchatteDOM/NCDTextImpl.cpp \
	../XML/NanchatteDOM/NCDDocumentFactory.cpp \
	../../Libs/ColumnListViewBe/ColumnTypes.cpp \
	../../Libs/ColumnListViewBe/ColumnListView.cpp \
	../../Libs/ColumnListViewBe/ColorTools.cpp \


RDEFS= \


RSRCS= \
	AppIcons.rsrc \


ifeq ($(TARGETTYPE), dano)
	RDEFS += CoveredCalcBeDano.rdef
else
ifeq ($(TARGETTYPE), zeta)
	RDEFS += CoveredCalcZeta.rdef
else
	RDEFS += CoveredCalcBe.rdef
endif
endif

# @<-src@ 
#%}

LIBS= stdc++.r4 translation be root expat
ifeq ($(TARGETTYPE), zeta)
	LIBS += zeta
endif

LIBPATHS= ../../Libs/libexpatBe

SYSTEM_INCLUDE_PATHS = 

LOCAL_INCLUDE_PATHS = \
	. \
	../../Libs/libexpatBe \
	../../Libs/ColumnListViewBe \


DEFINES= BEOS





# ------

CC = gcc
LD = gcc
MIMESET = mimeset
RC = rc
XRES = xres

TARGET = $(OBJ_DIR)/$(NAME)

define SRCS_LIST_TO_OBJS
	$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(foreach file, $(SRCS), \
	$(basename $(notdir $(file))))))
endef

OBJS = $(SRCS_LIST_TO_OBJS)
DEPENDS = $(patsubst %.o,%.d,$(OBJS))

define RDEF_LIST_TO_RDEFRSRCS
	$(addprefix $(OBJ_DIR)/, $(addsuffix .rsrc, $(foreach file, $(RDEFS), \
	$(basename $(notdir $(file))))))
endef

RSRCS += $(RDEF_LIST_TO_RDEFRSRCS)

SRC_PATHS += $(sort $(foreach file, $(SRCS), $(dir $(file))))

VPATH :=
VPATH += $(addprefix :, $(subst  ,:, $(filter-out $($(subst, :, ,$(VPATH))), $(SRC_PATHS))))

LOC_INCLUDES = $(foreach path, $(SRC_PATHS) $(LOCAL_INCLUDE_PATHS), $(addprefix -I, $(path)))
SYS_INCLUDES += -I-
SYS_INCLUDES += $(foreach path, $(SYSTEM_INCLUDE_PATHS), $(addprefix -I, $(path)))

INCLUDES = $(LOC_INCLUDES) $(SYS_INCLUDES)

LINK_PATHS = $(foreach path, $(SRC_PATHS) $(LIBPATHS), $(addprefix -L, $(path)))

LINK_LIBS += $(foreach lib, $(LIBS), $(addprefix -l, $(lib)))

LDFLAGS += $(LINK_PATHS) $(LINK_LIBS)

CPPFLAGS += $(foreach define, $(DEFINES), $(addprefix -D, $(define)))

BUILD_LINE = $(LD) -o $@ $(OBJS) $(LDFLAGS)

ifeq ($(RSRCS), )
	DO_RSRCS :=
else
	DO_RSRCS := $(XRES) -o "$(TARGET)" $(RSRCS)
endif

# ------ targets

.PHONY: all
all : $(TARGET)

$(TARGET) : $(OBJ_DIR) $(OBJS) $(RSRCS)
	$(BUILD_LINE)
	$(DO_RSRCS)
	$(MIMESET) -f $@

$(OBJ_DIR)::
	@[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR) > /dev/null 2>&1

$(OBJ_DIR)/%.o : %.cpp
	$(CC) -c $< $(INCLUDES) $(CPPFLAGS) -o $@

$(OBJ_DIR)/%.rsrc : %.rdef
	rc -o $@ $<

$(OBJ_DIR)/%.d: %.cpp
	@echo updating dependencies for $(notdir $<)
	@$(CC) -MM $< $(INCLUDES) $(CPPFLAGS) \
	| sed 's/\($*\)\.o[ :]*/$(OBJ_DIR)\/\1.o $(OBJ_DIR)\/\1.d : /g' > $@; \
	[ -s $@ ] || rm -f $@

ifneq ($(NODEPENDS), true)
-include $(DEPENDS)
endif

.PHONY: release
release : clean all


.PHONY: clean
clean :
	-rm -rf $(OBJ_DIR)/*
