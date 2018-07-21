import bpy
import os
import json

def export(context, config):

    dir = config['dir']

    root_folder = dir + "/texture"
    json_folder = root_folder + "/object"

    if not os.path.exists(root_folder):
        os.makedirs(root_folder)

    if not os.path.exists(json_folder):
        os.makedirs(json_folder)