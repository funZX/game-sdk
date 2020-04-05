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
def print_error_parse_texture_name(file_name):
	print ('\n TEXTURE NAME: ' + file_name + ' invalid.')
	print ('\n TEXTURE NAME FORMAT: name.mipmaps_count.output_format.texture_wrap.texture_filter.extension')
	print ('\n mipmaps_count of integer type')
	print ('\n output_format: tga, pvr, mip')
	print ('\n texture_wrap: clamp, repeat')
	print ('\n texture_filter: nearest, linear, bilinear, trilinear, quadlinear')
	print ('\n extension: mip, pvr, tga, png, jpg, jpeg')
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
	    print ('\n OUTPUT FORMAT: ' + out_format + ' not supported.')
	    print_error_parse_texture_name(file_name)
	    return None

	if out_format == 'mip' and not (in_format == 'tga' or in_format == 'mip'):
	    print ('\n Texture error for: ' + file_name + '. Only tga images can be imported to mip format.')
	    return None

	if out_format == 'tga' and in_format != 'tga':
	    print ('\n Texture error for: ' + file_name + '. Only tga images can be imported to tga format.')
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

			file_format = parse_texture_name(os.path.basename(in_file))
			if file_format == None:
			    continue
			
			texture_name = file_format[0]
			mipmaps_count = max(min(int(file_format[1]), 16), 0)
			out_format = file_format[2]
			texture_wrap = file_format[3]
			texture_filter = file_format[4]
			in_format = file_format[5]

			out_name = texture_name + '.' + out_format
			out_file = dst_dir + '/' + out_name


			if utils.newerFile(in_file, out_file):
			    if out_format == 'tga':
			        utils.updateFile(in_file, out_file)
			    elif out_format == 'mip':
			        command = config.EXE_MIP2SIM 
			        if mipmaps_count > 1:
			            command += ' -m '
			        command += ' -f RGBA4 -k lanczos -w ' + texture_wrap + ' ' + utils.getWinPath(in_file) + ' ' + utils.getWinPath(out_file)
			    elif out_format=='pvr':
			        command = config.EXE_PVRTEX + ' -legacypvr'
			        if mipmaps_count > 1:
			            command += ' -m ' + str(mipmaps_count) + ' -mfilter cubic'
			        command += ' -f PVRTC1_4'
			        command += ' -i ' + utils.getWinPath(in_file) + ' -o ' + utils.getWinPath(out_file)
			    
			    if out_format!='tga':
			        utils.spawnProcess(command)
			
			textures.append({'name' : n, 'file': ('texture/' + out_name), 'wrap' : texture_wrap, 'filter': texture_filter, 'format': out_format})

		if textures:
			with open(dst_dir + '/content.json', 'w') as f:
				json.dump(textures, f)
				
			content['id'] = dir['id']
			content['name'] = 'texture'
			content['file'] = 'texture/content.json'
	
	ElapsedTime = time.clock() - StartTime
	print ('\nElapsed Time: %0.3fs' % (ElapsedTime))

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
