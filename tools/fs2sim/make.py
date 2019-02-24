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
import skybox
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

	dirlist = config.clonedDataDir();	
	content = []
	
	for dir in dirlist:
		print('\n--------- FONTS -----\n')
		content.append(font.main([dir]))
		
		print('\n--------- TEXTURES -----\n')
		content.append(texture.main([dir]) )
		
		print('\n--------- SKYBOXES -----\n')
		content.append(skybox.main([dir]))
		
		print('\n--------- EFFECTS -----\n')
		content.append(effect.main([dir]))
		
		print('\n--------- MATERIALS -----\n')
		content.append(material.main([dir]))
		
		print('\n--------- MESHES -----\n')
		content.append(mesh.main([dir]))
		
		print('\n--------- ACTORS -----\n')
		content.append(actor.main([dir]))
		
		print('\n--------- LIGHTS -----\n')
		content.append(light.main([dir]))
		
		print('\n--------- CAMERAS -----\n')
		content.append(camera.main([dir]))
		
		print('\n--------- SPRITES -----\n')
		content.append(sprite.main([dir]))
		
		print('\n--------- SOUNDS -----\n')
		content.append(sound.main([dir]))
		
		print('\n--------- SCRIPTS -----\n')
		content.append(script.main([dir]))
		
		print('\n--------- SCENES -----\n')
		content.append(scene.main([dir]))

		dst_dir = dir['dst'] + '/'
		
		dir_content = []
		for c in content:
			if c and c['id'] == dir['id']:
				dir_content.append({'name':c['name'], 'file':c['file']})
				if c['name'] == 'shader':
				    dir_content.append({'name':'effect', 'file':'effect/effects.json'})


		if dir_content:
			with open(dst_dir + '/content.json', 'wb') as f:
				json.dump(dir_content, f)	

			sim 	= '../../' + dir['id'] + '.7z'
			command = config.EXE_7Z + ' u -t7z -mtm=off -mta=off -mtc=off -mx0 -mmt ' + sim + ' *'
			
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
