import bpy
import bmesh
import math
import mathutils
import os
import json

from .util import util

def export(context, config):

	dir = config['dir']

	root_folder = dir + "/model"
	json_folder = root_folder + "/object"

	if not os.path.exists(root_folder):
		os.makedirs(root_folder)

	if not os.path.exists(json_folder):
		os.makedirs(json_folder)

	for ob in bpy.context.selected_objects:
		if ob.type == 'MESH':

			bm = bmNew(ob)

			matYUp = mathutils.Matrix.Rotation(-math.pi/2, 4, 'X')
			bm.transform(matYUp)

			jsonData = toJson(config, ob, bm)
			save(json_folder + '/' + ob.name + '.json', jsonData)

			bm = bmDel(bm)


def bmNew(object):
	ob = bpy.context.object
	scene = bpy.context.scene

	bm = bmesh.new()
	bm.from_mesh(ob.data)
	bmesh.ops.triangulate(bm, faces=bm.faces)

	return bm

def bmDel(bm):

	bm.free()
	del bm

	return None

def toJson(config, ob, bm):
	jsonData = {}

	jsonData['name'] = ob.name

	if config['tbn']:
		jsonData['tbn'] = 1

	materialName = "default"

	if len(ob.data.materials) > 0:
		materialName = ob.data.materials[0].name

	jsonData['material'] = materialName

	jsonSource  = {}

	jsonVertices = []
	for vert in bm.verts:
		jsonVertices.append(util.flt(vert.co.x))
		jsonVertices.append(util.flt(vert.co.y))
		jsonVertices.append(util.flt(vert.co.z))

	jsonSource['vertices'] = jsonVertices

	if config['uv']:
		jsonUV  = []
		layer   = bm.loops.layers.uv.active

		if layer != None:
			for face in bm.faces:
				for loop in face.loops:
					faceUV = loop[layer].uv

					jsonUV.append(util.flt(faceUV.x))
					jsonUV.append(util.flt(faceUV.y))

		if len(jsonUV) > 0:
			jsonSource['texcoord'] = jsonUV

	if config['nrm']:
		jsonNRM = []

		for face in bm.faces:
			for loop in face.loops:
				v = loop.vert
				n = mathutils.Vector()
				n[0] = face.normal[0]
				n[1] = face.normal[1]
				n[2] = face.normal[2]

				if face.smooth and len(v.link_faces) > 1:
					for vf in v.link_faces:
						if vf.smooth and vf.index != face.index and vf.normal.magnitude > 0.0:
							if((not ob.data.use_auto_smooth) or (face.normal.magnitude == 0.0) or (vf.normal.angle(face.normal) < ob.data.auto_smooth_angle)):
								n = n + vf.normal

				n.normalize()
				jsonNRM.append(util.flt(n.x))
				jsonNRM.append(util.flt(n.y))
				jsonNRM.append(util.flt(n.z))

		if len(jsonNRM) > 0:
			jsonSource['normals'] = jsonNRM

	if config['vco']:
		jsonVCO = []
		
		layer   = bm.loops.layers.color.active

		if layer != None:
			for face in bm.faces:
				for loop in face.loops:
					color = loop[layer]

					jsonVCO.append(util.flt(color.r * 255.0))
					jsonVCO.append(util.flt(color.g * 255.0))
					jsonVCO.append(util.flt(color.b * 255.0))
					jsonVCO.append(util.flt(255.0))

		if len(jsonVCO) > 0:
			jsonSource['vcolors'] = jsonVCO

	jsonData['source'] = jsonSource

	jsonTriangles = []

	for face in bm.faces:
		verts = face.verts
		for v in verts:
			jsonTriangles.append(v.index)

	jsonData['triangles'] = jsonTriangles

	return jsonData

def save(filepath, jsonData):
	with open(filepath, 'w') as outfile:
		json.dump(jsonData, outfile)