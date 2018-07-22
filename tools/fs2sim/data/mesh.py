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
		meshes = []
		lods = []
		src_dir = dir['src'] + '/mesh'
		dst_dir = dir['dst'] + '/mesh'
		
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

			file = (d.split(src_dir, 1)[1])

			lodsfile = d + file + '.lods'

			if os.path.exists(lodsfile):
			    with open(lodsfile) as lods_data: 
			        lods = json.load(lods_data)

			file = (file.split('/', 1)[1] + '/' + n)

			name 		= file.split('.json', 1)[0]
			temp  		= name + '.sim'
			temp2       = name + '.lod.sim'
			out_file 	= dst_dir + '/' + temp
			out_file2 	= dst_dir + '/' + temp2
			
			if utils.newerFile(d + '/' + n, out_file):
				command = config.EXE_JSON2SIM + ' mesh ' + file + ' ' + temp

				os.chdir(src_dir)
				utils.spawnProcess(command)
				
				if n in lods:
					
					command = config.EXE_SIM2LOD + ' ' + temp + ' ' + temp2
					utils.spawnProcess(command)
					temp        = temp2
					out_file    = out_file2

				utils.moveFile(temp, out_file)

			meshes.append({'name' : name, 'file': ('mesh/' + temp)});
				
		if meshes:
			with open(dst_dir + '/content.json', 'wb') as f:
				json.dump(meshes, f)
				
			content['id'] 	= dir['id']
			content['name'] = 'mesh'
			content['file'] = 'mesh/content.json'

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
