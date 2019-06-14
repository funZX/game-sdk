#! /bin/python

import sys
import os
import time
import glob

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))

import config
import utils
#---------------------------------------------------------------------------------------

def main(dirlist):

	StartTime = time.clock()
	
	content = {}

	for dir in dirlist:
		src_dir = dir['src'] + '/skybox'
		dst_dir = dir['dst'] + '/skybox'
		
		content_file = src_dir + '/content.json'
		
		if os.path.exists(content_file):
			if os.path.exists(src_dir):
			
				if not os.path.exists(dst_dir):
					os.makedirs(dst_dir)	
				
				utils.updateFile(content_file, dst_dir + '/content.json')
				utils.updateFiles(src_dir, dst_dir, src_dir, 'pvr')
			
				content['id'] 	= dir['id']
				content['name'] = 'skybox'
				content['file'] = 'skybox/content.json'	
	
	ElapsedTime = time.clock() - StartTime
	print ('\nElapsed Time: %0.3fs' % (ElapsedTime))

	return content

#---------------------------------------------------------------------------------------

if __name__ == "__main__":

	dirlist = config.clonedDataDir();	
	ret = main(dirlist)
	
	for dir in dirlist:	
		dst_dir = dir['dst'] + '/'
		
		sim 	= '../../' + dir['id'] + '.7z'
		command = config.EXE_7Z + ' u -t7z -ms=1m ' + sim + ' *'

		os.chdir(dst_dir)
		utils.spawnProcess(command)
		
	sys.exit(ret)

#---------------------------------------------------------------------------------------
