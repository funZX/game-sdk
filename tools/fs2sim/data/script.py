#! /bin/python

import sys
import os
import time
import glob
import json
import subprocess

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))

import config
import utils
#---------------------------------------------------------------------------------------

def main(dirlist):

	StartTime = time.clock()
	
	content = {}
	
	for dir in dirlist:
		scripts = []
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
			
			if not os.path.exists(dst_dir):
				os.makedirs(dst_dir)

			name = n
			name = name.split('.', 1)[0]
			
			in_file     = d + '/' + n
			out_file 	= dst_dir + '/' + n + 't'

			if utils.newerFile(in_file, out_file):

				in_temp = os.path.basename(in_file)
				out_temp = os.path.basename(out_file)
				command = config.EXE_SQ2SIM + ' ' + in_temp + ' ' + out_temp

				os.chdir(d)
				utils.spawnProcess(command)
				utils.updateFile(out_temp, out_file)
				os.remove(out_temp)

			scripts.append({'name' : name, 'file': ('script/' + n + 't')});
				
		if scripts:
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
