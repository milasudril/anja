import sys

def copyOrRender(frame,scene,target_dir,in_dir,target_name):
	import bpy
	name_img=target_name
	if name_img in bpy.data.images:
		bpy.data.images[name_img].filepath_raw=target_dir+'/'+in_dir+'/'+name_img
		bpy.data.images[name_img].save();
	else:
		try:
			bpy.context.user_preferences.addons['cycles'].preferences.compute_device_type = 'CUDA'
			bpy.context.user_preferences.addons['cycles'].preferences.devices[0].use = True
		except:
			print('Could not set select CUDA as compute device. Rendering on the CPU')
		scene.render.filepath=target_dir+'/'+in_dir+'/'+target_name
		scene.frame_set(frame)
		bpy.ops.render.render(write_still=True)
		img=bpy.data.images.load(scene.render.filepath)
		img.name=target_name
		img.use_fake_user=True
		img.pack(as_png=True)
		bpy.ops.wm.save_mainfile()
		bpy.data.images[name_img].filepath_raw=target_dir+'/'+in_dir+'/'+name_img
		bpy.data.images[name_img].save()

def main():
	import bpy
	argv=sys.argv
	if "--" not in argv:
		argv = []  # as if no args are passed
	else:
		argv = argv[argv.index("--") + 1:] # get all args after "--"

	target_dir=argv[0]
	in_dir=argv[1]
	frames=argv[2:]
	for k,v in enumerate(frames):
		copyOrRender(k+1,bpy.context.scene,target_dir,in_dir,v)

if __name__ == "__main__":
	main()

def render(source,in_dir,target_dir,*target_name):
	import subprocess
	argv=['blender','-b',in_dir+'/'+source \
		,'--python',__file__,'--',target_dir,in_dir]
	argv.extend(target_name)
	print('# Starting blender with command line ',argv)
	blender=subprocess.Popen(argv,stdout=subprocess.PIPE)
	for lines in blender.stdout:
		progress=lines.decode('utf8').rstrip().split('|')
		print('# Blender: %s'%(progress[-1].strip()))
