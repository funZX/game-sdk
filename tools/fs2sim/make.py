#! /bin/python

import sys
import os
import time
import config
import utils
import shutil
import json

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'data'))

import font
import texture
import svg
import skybox
import shader
import effect
import material
import mesh
import actor
import light
import camera
import sprite
import sound
import script
import scene

#---------------------------------------------------------------------------------------

def main(argv):

	StartTime = time.clock()
	if not os.path.exists(config.BLOB_DIR):
		shutil.os.mkdir(config.BLOB_DIR)
	if not os.path.exists(config.TEMP_DIR):
		shutil.os.mkdir(config.TEMP_DIR)
	dirlist = utils.cloneTree(config.DATA_DIR, config.TEMP_DIR)
	
	content = []
	
	print('\n--------- FONTS -----\n')
	content.append(font.main(dirlist))
	
	print('\n--------- SVG -----\n')
	content.append(svg.main(dirlist) )

	print('\n--------- TEXTURES -----\n')
	content.append(texture.main(dirlist) )
	
	print('\n--------- SKYBOXES -----\n')
	content.append(skybox.main(dirlist))

	print('\n--------- SHADERS -----\n')
	content.append(shader.main(dirlist))
	
	print('\n--------- EFFECTS -----\n')
	content.append(effect.main(dirlist))
	
	print('\n--------- MATERIALS -----\n')
	content.append(material.main(dirlist))
	
	print('\n--------- MESHES -----\n')
	content.append(mesh.main(dirlist))

	print('\n--------- ACTORS -----\n')
	content.append(actor.main(dirlist))

	print('\n--------- LIGHTS -----\n')
	content.append(light.main(dirlist))
	
	print('\n--------- CAMERAS -----\n')
	content.append(camera.main(dirlist))
	
	print('\n--------- SPRITES -----\n')
	content.append(sprite.main(dirlist))
	
	print('\n--------- SOUNDS -----\n')
	content.append(sound.main(dirlist))
	
	print('\n--------- SCRIPTS -----\n')
	content.append(script.main(dirlist))
	
	print('\n--------- SCENES -----\n')
	content.append(scene.main(dirlist))

	for dir in dirlist:	
		dst_dir = dir['dst'] + '/'
		
		dir_content = []
		for c in content:
			if c and c['id'] == dir['id']:
				dir_content.append({'name':c['name'], 'file':c['file']})

		if dir_content:
			with open(dst_dir + '/content.json', 'wb') as f:
				json.dump(dir_content, f)	

		sim 	= '../../' + dir['id'] + '.7z'
		command = config.EXE_7Z + ' u -t7z -mx0 -mmt ' + sim + ' *'

		os.chdir(dst_dir)
		utils.spawnProcess(command)
	

	ElapsedTime = time.clock() - StartTime
	print '\nElapsed Time: %0.3fs' % (ElapsedTime)
	
	return 0

#---------------------------------------------------------------------------------------

if __name__ == "__main__":
	ret = main( sys.argv[ 1: ] )
	sys.exit(ret)

#---------------------------------------------------------------------------------------
