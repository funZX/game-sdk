#! /bin/python

import sys
import os
import time
import glob
import simplejson as json

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))

import config
import utils
#---------------------------------------------------------------------------------------

def main(dirlist):

	StartTime = time.monotonic()
	
	content = {}

	for dir in dirlist:
		materials = []
		src_dir = dir['src'] + '/material'
		dst_dir = dir['dst'] + '/material'
		
		files 		= []
		
		if os.path.exists(src_dir):

			files = utils.getListOfFiles(src_dir, 'json')			
			
			if files and not os.path.exists(dst_dir):
				os.makedirs(dst_dir)
			
		
		for file in files:
			d	= file['dir']
			n 	= file['file']
			
			src_subdir = d.split(src_dir, 1)[1]
			dst_subdir = dst_dir + src_subdir
			
			if not os.path.exists(dst_subdir):
				os.makedirs(dst_subdir)

			utils.updateFile(src_dir + '/' + n, dst_dir + '/' + n)
			
			name = n
			name = name.split('.', 1)[0]
			materials.append({'name' : name, 'file': ('material/' + n)});

		if materials:
			with open(dst_dir + '/content.json', 'w') as f:
				json.dump(materials, f)
				
			content['id'] 	= dir['id']
			content['name'] = 'material'
			content['file'] = 'material/content.json'


	ElapsedTime = time.monotonic() - StartTime
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
