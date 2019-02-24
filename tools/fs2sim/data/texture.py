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
def print_error_parse_texture_name(file_name):
	print '\n TEXTURE NAME: ' + file_name + ' invalid.'
	print '\n TEXTURE NAME FORMAT: name.mipmaps_count.output_format.texture_wrap.texture_filter.extension'
	print '\n mipmaps_count of integer type'
	print '\n output_format: tga, pvr, mip'
	print '\n texture_wrap: clamp, repeat'
	print '\n texture_filter: nearest, linear, bilinear, trilinear, quadlinear'
	print '\n extension: mip, pvr, tga, png, jpg, jpeg'
#---------------------------------------------------------------------------------------
def parse_texture_name(file_name):
	file_format = file_name.split('.')

	is_valid = len(file_format) == 6
	if not is_valid:
	    print_error_parse_texture_name(file_name)
	    return None

	out_format = file_format[2]
	in_format = file_format[5]

	is_valid = out_format == 'tga' or out_format == 'mip' or out_format == 'pvr'

	if not is_valid:
	    print_error_parse_texture_name(file_name)
	    return None

	if out_format == 'mip' and not (in_format == 'tga' or in_format == 'mip'):
	    print '\n Texture error for: ' + file_name + '. Only tga images can be converted to mip.'
	    return None

	if out_format == 'tga' and in_format != 'tga':
	    print '\n Texture error for: ' + file_name + '. Only tga images can be converted to tga.'
	    return None

	return file_format

#---------------------------------------------------------------------------------------
def main(dirlist):

	StartTime = time.clock()

	content = {}
	
	for dir in dirlist:
		textures = []
		src_dir = dir['src'] + '/texture'
		dst_dir = dir['dst'] + '/texture'
		
		files = []
		
		if os.path.exists(src_dir):

			files = utils.getListOfFiles(src_dir, 'mip')
			files += utils.getListOfFiles(src_dir, 'pvr')
			files += utils.getListOfFiles(src_dir, 'tga')
			files += utils.getListOfFiles(src_dir, 'png')
			files += utils.getListOfFiles(src_dir, 'jpg')
			files += utils.getListOfFiles(src_dir, 'jpeg')
			
			if files and not os.path.exists(dst_dir):
				os.makedirs(dst_dir)
			
		for file in files:
			d = file['dir']
			n = file['file']

			src_subdir = d.split(src_dir, 1)[1]
			dst_subdir = dst_dir + src_subdir
			
			if not os.path.exists(dst_subdir):
				os.makedirs(dst_subdir)

			name = n
			name = name.split('.', 1)[0]
			
			in_file     = d + '/' + n
			out_file 	= dst_dir + '/' + n

			file_format = parse_texture_name(os.path.basename(in_file))
			if file_format == None:
			    continue
			
			mipmaps_count = max(16, min(1, int(file_format[1])))
			out_format = file_format[2]
			texture_wrap = file_format[3]
			texture_filter = file_format[4]
			in_format = file_format[5]

			if utils.newerFile(in_file, out_file):
			    in_temp = os.path.basename(in_file)
			    out_temp = os.path.basename(out_file) + '.temp'

			    if out_format == 'mip':
			        command = config.EXE_MIP2SIM + ' -f RGBA4 -k lanczos -w ' + texture_wrap + ' ' + in_temp + ' ' + out_temp
			    elif out_format=='pvr':
			        command = config.EXE_PVRTEX + ' ' + in_temp + ' ' + out_temp
			    elif out_format=='tga':
			        command = config.EXE_MIP2SIM + ' ' + in_temp + ' ' + out_temp
			        
			    os.chdir(d)
			    utils.spawnProcess(command)
			    utils.updateFile(out_temp, out_file)
			    os.remove(out_temp)
			
			textures.append({'name' : n, 'file': ('texture/' + n), 'wrap' : texture_wrap, 'filter' : texture_filter})

		if textures:
			with open(dst_dir + '/content.json', 'wb') as f:
				json.dump(textures, f)
				
			content['id'] = dir['id']
			content['name'] = 'texture'
			content['file'] = 'texture/content.json'
	
	ElapsedTime = time.clock() - StartTime
	print '\nElapsed Time: %0.3fs' % (ElapsedTime)

	return content

#---------------------------------------------------------------------------------------
if __name__ == "__main__":

	dirlist = config.clonedDataDir()	
	ret = main(dirlist)
	
	for dir in dirlist:	
		dst_dir = dir['dst'] + '/'
		
		sim = '../../' + dir['id'] + '.7z'
		command = config.EXE_7Z + ' u -t7z -ms=1m ' + sim + ' *'

		os.chdir(dst_dir)
		utils.spawnProcess(command)
		
	sys.exit(ret)

#---------------------------------------------------------------------------------------
