import bpy
import os
from bpy.props import StringProperty, BoolProperty

from .sim import main

DEBUG = os.environ.get('BLENDER_DEBUG', False)
if DEBUG:
	import sys
	sys.path.append(os.environ['PYDEV_DEBUG_PATH'])
	import pydevd

class Blend2Sim(bpy.types.Operator):
    """Export selected objects to simarian engine format."""
    bl_idname   = "object.export_simarian"
    bl_label    = "Simarian"
    bl_options  = {'REGISTER'}

    directory   = StringProperty(name='Directory')

    uv          = BoolProperty(name="uv",
							description="Export texture coordinates",
							default=True)

    nrm         = BoolProperty(name="nrm",
							description="Export normals",
							default=True)

    tbn         = BoolProperty(name="tbn",
							description="Export tangents",
							default=False)
    
    vco         = BoolProperty(name="vco",
							description="Export vertex color",
							default=False)

    def invoke(self, context, event):
        wm = context.window_manager
        wm.fileselect_add(self)

        return {'RUNNING_MODAL'}

    def export(self, context, props):
        config = {
            "dir":  self.directory,
            "uv":   self.uv,
            "nrm":  self.nrm,
            "tbn":  self.tbn,
            "vco":  self.vco
        }
        main.export( context, config )
        return True

    def execute(self, context):
        exported = self.export(context, self.properties)
        return {'FINISHED'}
