#! /bin/python

import os

#---------------------------------------------------------------------------------------

WORK_DIR					=	os.path.dirname(os.path.abspath(__file__))

PRJ_DIR						=	os.path.abspath(WORK_DIR + '/../../game/')
DATA_DIR					=	os.path.abspath(PRJ_DIR + '/data')
BLOB_DIR					=	os.path.abspath(PRJ_DIR + '/blob')
TEMP_DIR					=	os.path.abspath(PRJ_DIR + '/blob/.temp')
TOOL_DIR					=	os.path.abspath(PRJ_DIR + '/tools')

EXE_7Z						=	os.path.abspath(TOOL_DIR + '/7z.exe')
EXE_PVRTEX					=   os.path.abspath(TOOL_DIR + '/pvr.exe')
EXE_JSON2SIM				=	os.path.abspath(TOOL_DIR + '/json2sim.exe')
EXE_SQ2SIM					=	os.path.abspath(TOOL_DIR + '/sq2sim.exe')

EXE_SIM2MIP					= 	os.path.abspath(TOOL_DIR + '/sim2mip.exe')
EXE_SIM2LOD				    =	os.path.abspath(TOOL_DIR + '/sim2lod.exe')

#---------------------------------------------------------------------------------------
