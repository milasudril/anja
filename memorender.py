#@	{"dependencies_extra":[{"ref":"blender","rel":"tool"}]}

import sys
import os

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

def copyOrRender(level,frame,scene,target_dir,in_dir,target_name):
	import bpy
	temp=os.path.splitext(target_name)
	name_img=temp[0]
	if level!=0:
		name_img=name_img + '_' + str(level)
	name_img=name_img + temp[1]
	name_img_full=target_dir+'/'+in_dir+'/'+name_img
	if name_img in bpy.data.images:
		print(' r | Using prerendered result for %s'%name_img)
		sys.stdout.flush()
		bpy.data.images[name_img].filepath_raw=name_img_full
		bpy.data.images[name_img].save();
	else:
		try:
			bpy.context.user_preferences.addons['cycles'].preferences.compute_device_type = 'CUDA'
			bpy.context.user_preferences.addons['cycles'].preferences.devices[0].use = True
		except:
			print('  r | Could not set select CUDA as compute device. Rendering on the CPU')
		scene.render.filepath=name_img_full
		scene.frame_set(frame)
		scene.render.resolution_percentage=int( 100*pow(2,-level) )
		print(' r | Rendering %s'%name_img)
		sys.stdout.flush()
		bpy.ops.render.render(write_still=True)
		img=bpy.data.images.load(scene.render.filepath)
		img.name=name_img
		img.use_fake_user=True
		img.pack(as_png=True)
		bpy.ops.wm.save_mainfile()
		bpy.data.images[name_img].filepath_raw=name_img_full
		bpy.data.images[name_img].save()

def main():
	import bpy
	try:
		argv=sys.argv
		if "--" not in argv:
			argv = []  # as if no args are passed
		else:
			argv = argv[argv.index("--") + 1:] # get all args after "--"

		target_dir=argv[0]
		in_dir=argv[1]
		levels=int(argv[2])
		frames=argv[3:]
		for k,v in enumerate(frames):
			for x in range(0,levels):
				copyOrRender(x,k+1,bpy.context.scene,target_dir,in_dir,v)

	except Exception:
		print('r | %s:%d: error: %s'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
		sys.stdout.flush()
		raise


if __name__ == "__main__":
	main()

def render(mipmap_levels,source,in_dir,target_dir,*target_name):
	import subprocess
	argv=['blender','--python-exit-code','255','-b',in_dir+'/'+source \
		,'--python',__file__,'--',target_dir,in_dir,str(mipmap_levels)]
	argv.extend(target_name)
	print('# Starting blender with command line ',argv)
	blender=subprocess.Popen(argv,stdout=subprocess.PIPE)
	for lines in blender.stdout:
		progress=lines.decode('utf8').rstrip().split('|')
		print('# Blender: %s'%(progress[-1].strip()))
		sys.stdout.flush()
	blender.wait()
	if blender.returncode!=0:
		raise Exception('Blender failed')
