#! /bin/python

import sys
import os
import time
import glob
import simplejson as json
import subprocess
import cygwin;


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
		
		print ('src: ' + src_dir)

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
				out_temp = cygwin.cygpath(out_file, 'w')
				in_temp = cygwin.cygpath(in_file, 'w')
				command = config.EXE_SQ + ' -c -d -o ' + out_temp.replace('\\','/').lower() + ' ' + in_temp.replace('\\','/').lower()
				#command = config.EXE_SQ + ' -c -o ' + out_temp + ' ' + in_temp
				print('a' + in_file + 'b' + out_file )
				utils.spawnProcess(command)

			scripts.append({'name' : name, 'file': ('script/' + n + 't')});
				
		if scripts:
			with open(dst_dir + '/content.json', 'w') as f:
				json.dump(scripts, f)
				
			content['id'] 	= dir['id']
			content['name'] = 'script'
			content['file'] = 'script/content.json'

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
