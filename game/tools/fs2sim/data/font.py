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

	StartTime = time.clock()
	
	
	
	content = {}
	
	for dir in dirlist:
		fonts 	= []
		src_dir = dir['src'] + '/font'
		dst_dir = dir['dst'] + '/font'
		
		files 		= []

		if os.path.exists(src_dir):
		
			files = utils.getListOfFiles(src_dir, 'ttf')	
			
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
			
			for k in range(0, 3):
				size = (12 + k * 4)				
				name = file.split('.ttf', 1)[0] + str(size)
				
				fonts.append({'name' : name, 'file': ('font/' + file), 'size' : size});
				
		if fonts:
			fs = dst_dir + '/content.json'
			with open(fs, 'w') as f:
				json.dump(fonts, f)
			
			content['id'] 	= dir['id']
			content['name'] = 'font'
			content['file'] = 'font/content.json'



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
