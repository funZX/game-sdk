#! /bin/python

import sys
import os
import time
import glob
import json

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))

import config
import utils
#---------------------------------------------------------------------------------------

def main(dirlist):

	StartTime = time.clock()
	
	content = {}
	scripts = []
	
	for dir in dirlist:

		src_dir = dir['src'] + '/script'
		dst_dir = dir['dst'] + '/script'
		
		files 		= []
		
		if os.path.exists(src_dir):

			files = utils.getListOfFiles(src_dir, 'nut')			
			
			if files and not os.path.exists(dst_dir):
				os.makedirs(dst_dir)
			
		
		for file in files:
			d	= file['dir']
			n 	= file['file']
			
			src_subdir = d.split(src_dir, 1)[1]
			dst_subdir = dst_dir + src_subdir
			
			if not os.path.exists(dst_subdir):
				os.makedirs(dst_subdir)

			file = (d.split(src_dir, 1)[1])
			file = (file.split('/', 1)[1] + '/' + n)
			
			name 		= file.split('.nut', 1)[0]
			temp  		= name + '.bnut'
			out_file 	= dst_dir + '/' + temp
			
			if utils.newerFile(d + '/' + n, out_file):
				command = config.EXE_SQ2SIM + ' ' + file + ' ' + temp
				os.chdir(src_dir)
				utils.spawnProcess(command)
				utils.moveFile(temp, out_file)

			scripts.append({'name' : name, 'file': ('script/' + temp)});
				
		if files:
			with open(dst_dir + '/content.json', 'wb') as f:
				json.dump(scripts, f)
				
			content['id'] 	= dir['id']
			content['name'] = 'script'
			content['file'] = 'script/content.json'

	ElapsedTime = time.clock() - StartTime
	print '\nElapsed Time: %0.3fs' % (ElapsedTime)
	
	print content
	
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
