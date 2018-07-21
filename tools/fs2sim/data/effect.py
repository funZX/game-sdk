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
		src_file = dir['src'] + '/effect/content.json'
		dst_file = dir['dst'] + '/effect/content.json'
	
		if os.path.exists(src_file):
			
			effects_dir = dir['dst'] + '/effect'
			
			if not os.path.exists(effects_dir):
				os.makedirs(effects_dir)	
			
			utils.updateFile(src_file, dst_file)
	
			content['id'] 	= dir['id']
			content['name'] = 'effect'
			content['file'] = 'effect/content.json'
	
	ElapsedTime = time.clock() - StartTime
	print '\nElapsed Time: %0.3fs' % (ElapsedTime)
	
	return content

#---------------------------------------------------------------------------------------

if __name__ == "__main__":

	dirlist = utils.cloneTree(config.DATA_DIR, config.TEMP_DIR)
	
	ret = main(dirlist)
	
	for dir in dirlist:	
		dst_dir = dir['dst'] + '/'
		
		sim 	= '../../' + dir['id'] + '.7z'
		command = config.EXE_7Z + ' u -t7z -ms=1m ' + sim + ' *'

		os.chdir(dst_dir)
		utils.spawnProcess(command)
		
	sys.exit(ret)

#---------------------------------------------------------------------------------------
