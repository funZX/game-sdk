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
	
	for dir in dirlist:
		textures = []
		src_dir = dir['src'] + '/texture'
		dst_dir = dir['dst'] + '/texture'
		
		files 		= []
		
		if os.path.exists(src_dir):

			files  = utils.getListOfFiles(src_dir, 'mip')
			files += utils.getListOfFiles(src_dir, 'pvr')
			files += utils.getListOfFiles(src_dir, 'tga')
			
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
			
			utils.updateFile(src_dir + '/' + file, dst_dir + '/' + file)
			
			name = file.split('.json', 1)[0]
			textures.append({'name' : name, 'file': ('texture/' + file), 'wrap' : 'clamp', 'filter' : 'linear'});

		if textures:
			with open(dst_dir + '/content.json', 'wb') as f:
				json.dump(textures, f)
				
			content['id'] 	= dir['id']
			content['name'] = 'texture'
			content['file'] = 'texture/content.json'
	
	ElapsedTime = time.clock() - StartTime
	print '\nElapsed Time: %0.3fs' % (ElapsedTime)

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
