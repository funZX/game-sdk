"""
Name: 'blend2sim'
Blender: 276
Group: 'User'
Tooltip: 'Export selected objects to SimEngine format'
"""

bl_info = {
    "name":         "blend2sim",
    "author":       "Adrian SIMINCIUC",
    "blender":      (2, 63, 0),
    "version":      (0, 0, 1),
    "location":     "File > Export > SimEngine",
    "description":  "Export selected objects to SimEngine format",
    "category":     "Import-Export",
}

import bpy
from .blend2sim import Blend2Sim

def menu_func(self, context):
    self.layout.operator(Blend2Sim.bl_idname, text="SimEngine")

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()