#! /bin/python

import sys
import os
import time
import glob
import simplejson as json

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))

import config
import utils
import cygwin;
#---------------------------------------------------------------------------------------

def main(dirlist):
    
    StartTime = time.clock()
    
    content = {}
    
    for dir in dirlist:
        effects = []
        src_dir = dir['src'] + '/effect'
        dst_dir = dir['dst'] + '/effect'

        files 		= []

        if os.path.exists(src_dir):
            files = utils.getListOfFiles(src_dir, 'effect')

            if files and not os.path.exists(dst_dir):
                os.makedirs(dst_dir)
        
        for file in files:
            d	= file['dir']
            n 	= file['file']

            src_subdir = d.split(src_dir, 1)[1]
            dst_subdir = dst_dir + src_subdir

            if not os.path.exists(dst_subdir):
                os.makedirs(dst_subdir)
                
            in_effect = d + '/' + n
            with open(in_effect, 'r') as effect_file:
                effect = json.load(effect_file)
                
                out_effect = dst_dir + '/' + n                
                out_vsource = ('vsource.' + n).split('.effect', 1)[0]
                out_psource = ('psource.' + n).split('.effect', 1)[0]

                out_vsource_name = out_vsource
                out_psource_name = out_psource

                out_vdir = os.path.dirname(dst_subdir) + '/' + os.path.dirname(effect['vsource'])
                out_pdir = os.path.dirname(dst_subdir) + '/' + os.path.dirname(effect['psource'])
                
                if not os.path.exists(out_vdir):
                    os.makedirs(out_vdir)

                if not os.path.exists(out_pdir):
                    os.makedirs(out_pdir)

                out_vsource = out_vdir + '/' + out_vsource
                out_psource = out_pdir + '/' + out_psource

                in_vsource = os.path.dirname(d) + '/' + effect['vsource']
                in_psource = os.path.dirname(d) + '/' + effect['psource']

                if (utils.newerFile(in_effect, out_effect) or
                    utils.newerFile(in_vsource, out_vsource) or
                    utils.newerFile(in_psource, out_psource) ):
                    
                    D = ' -P -I ' + utils.getWinPath(d)
                    for define in effect['defines']:
                        D = D + ' -D' + define+'=1'

                    DV = D[:] + ' -o ' + utils.getWinPath(out_vsource) + ' ' + utils.getWinPath(in_vsource)
                    DP = D[:] + ' -o ' + utils.getWinPath(out_psource) + ' ' + utils.getWinPath(in_psource)
                    
                    command = config.EXE_MCPP + DV
                    utils.spawnProcess(command)

                    command = config.EXE_MCPP + DP
                    utils.spawnProcess(command)

                    effect['vsource'] = os.path.dirname(effect['vsource']) +'/'+out_vsource_name
                    effect['psource'] = os.path.dirname(effect['psource']) +'/'+out_psource_name
                    
                    del effect['defines']
                    with open(out_effect, 'w') as f:
                        json.dump(effect, f)

                effects.append({'name' : n.split('.effect',1)[0], 'file': ('effect/' + n)});

        if effects:
            with open(dst_dir + '/content.json', 'w') as f:
                json.dump(effects, f)
            
            content['id'] 	= dir['id']
            content['name'] = 'effect'
            content['file'] = 'effect/content.json'

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
