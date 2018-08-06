import sys
import os
import re
import shutil
import subprocess
import shlex
import glob
import time
import fnmatch

#---------------------------------------------------------------------------------------

def LOG(str):
	print('\n' + str)
	sys.stdout.flush()

#---------------------------------------------------------------------------------------

def failed(exc):
	raise exc

#---------------------------------------------------------------------------------------

def getWinPath(path):
    str = path.replace('/c/', 'c:/')
    str = str.replace('/d/', 'd:/')
    str = str.replace('/e/', 'e:/')

    return str

#---------------------------------------------------------------------------------------

def getListOfDirectories(path):
	list = []
	temp = []
	
	if os.path.isdir(path):
		temp = os.listdir(path)
		
	for l in temp:
		if os.path.isdir(path + '/' + l):
			list.append(l)

	return list

#---------------------------------------------------------------------------------------

def getFileExtension(filename):
	return os.path.splitext(filename)[1][1:].strip().lower()

#---------------------------------------------------------------------------------------

def getListOfFiles(path, ext):
	list = []

	for dir, dirs, files in os.walk(path, topdown=True, onerror=failed):
		for file in files:
			e = getFileExtension(file)
			if e == ext:
				list.append({'dir':dir, 'file': file})

	return list

#---------------------------------------------------------------------------------------

def removeFiles(path):

	fileList = glob.glob(path)
	
	for file in fileList:
		os.remove(file)

#---------------------------------------------------------------------------------------
	
def newerFile(src, dst):
	if not os.path.exists(dst):
		return True
	
	if time.ctime(os.path.getmtime(src)) > time.ctime(os.path.getmtime(dst)):
		return True
		
	return False

#---------------------------------------------------------------------------------------

def moveFile(src, dst):
	if newerFile(src, dst):
		shutil.move(src, dst)
		os.chmod(dst, 0664)

#---------------------------------------------------------------------------------------
	
def updateFile(src, dst):
	if newerFile(src, dst):
		shutil.copy2(src, dst)
		os.chmod(dst, 0664)
		LOG ('UPDATED: ' + dst )

#---------------------------------------------------------------------------------------
	
def updateFiles(srcDir, dstDir, root, ext):

	for dir in getListOfDirectories(srcDir):
		updateFiles(dir, dstDir, root, ext)
	
		list = cloneTree(srcDir, dstDir)
	
		for l in list:

			files = getListOfFiles(l['src'], ext)
		
			for file in files:
				d = file['dir']
				n = file['file']
				
				f = (d.split(root, 1)[1] + '/' + n)
				updateFile(root +f, dstDir + f)

#---------------------------------------------------------------------------------------
	
def isDirEmpty(path):

	return not os.listdir(path)
	
#---------------------------------------------------------------------------------------
	
def cloneTree(srcDir, dstDir):

	list = getListOfDirectories(srcDir)
	dirs = []
	
	for l in list:
		d = dstDir + '/' + l
		s = srcDir + '/' + l
		if not isDirEmpty(s):
			dirs.append({'src': s, 'dst':d, 'id':l})
			if not os.path.exists(d):
				shutil.os.mkdir(d)
	
	return dirs
	

#---------------------------------------------------------------------------------------

def spawnProcess(command):

	LOG(command)

	args = shlex.split(command)
	
	return subprocess.check_call(args)

#---------------------------------------------------------------------------------------