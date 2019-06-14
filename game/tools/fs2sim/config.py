#! /bin/python

import os
import shutil
import utils
#---------------------------------------------------------------------------------------

WORK_DIR					=	os.path.dirname(os.path.abspath(__file__))

PRJ_DIR						=	os.path.abspath(WORK_DIR + '/../../')
DATA_DIR					=	os.path.abspath(PRJ_DIR + '/data')
BLOB_DIR					=	os.path.abspath(PRJ_DIR + '/blob')
TEMP_DIR					=	os.path.abspath(PRJ_DIR + '/blob/.temp')
TOOL_DIR					=	os.path.abspath(PRJ_DIR + '/tools')

EXE_7Z						=	os.path.abspath(TOOL_DIR + '/7z.exe')
EXE_PVRTEX					=   os.path.abspath(TOOL_DIR + '/PVRTexToolCLI.exe')
EXE_SQ  					=	os.path.abspath(TOOL_DIR + '/sq.exe')
EXE_MIP2SIM					= 	os.path.abspath(TOOL_DIR + '/mip2sim.exe')

EXE_MCPP                    = 	os.path.abspath(TOOL_DIR + '/mcpp.exe')

def clonedDataDir():
	if not os.path.exists(BLOB_DIR):
		shutil.os.mkdir(BLOB_DIR)
	if not os.path.exists(TEMP_DIR):
		shutil.os.mkdir(TEMP_DIR)

	return utils.cloneTree(DATA_DIR, TEMP_DIR)
#---------------------------------------------------------------------------------------
