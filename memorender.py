import sys

def __render__(scene,target_dir,in_dir,target_name):
	import bpy
	scene.render.filepath=target_dir+'/'+in_dir+'/'+target_name
	bpy.ops.render.render(write_still=True)
	img=bpy.data.images.load(scene.render.filepath)
	if 'render_result' in bpy.data.images:
		bpy.data.images.remove(bpy.data.images['render_result'],do_unlink=True)
	img.name='render_result'
	img.use_fake_user=True
	img.pack(as_png=True)
	scene['up_to_date']=True
	bpy.ops.wm.save_mainfile()

def renderUpToDate(scene):
	import bpy
	if not 'render_result' in bpy.data.images:
		return False
	if scene.get('up_to_date',False):
		return True
	return False

def main():
	import bpy
	argv=sys.argv
	if "--" not in argv:
		argv = []  # as if no args are passed
	else:
		argv = argv[argv.index("--") + 1:] # get all args after "--"

	target_dir=argv[0]
	in_dir=argv[1]
	target=argv[2]

	if renderUpToDate(bpy.context.scene):
		bpy.data.images['render_result'].save_render(target_dir+'/'+in_dir+'/'+target)
	else:
		try:
			bpy.context.user_preferences.addons['cycles'].preferences.compute_device_type = 'CUDA'
			bpy.context.user_preferences.addons['cycles'].preferences.devices[0].use = True
		except:
			print('Could not set select CUDA as compute device. Rendering on the CPU')
		__render__(bpy.context.scene,target_dir,in_dir,target)

if __name__ == "__main__":
	main()

def render(target_dir,in_dir,target_name):
	import subprocess
	blender=subprocess.Popen(['blender','-b',in_dir+'/logo.blend' \
		,'--factory-startup','--python','memorender.py','--',target_dir,in_dir,target_name] \
		,stdout=subprocess.PIPE)
	for lines in blender.stdout:
		progress=lines.decode('utf8').rstrip().split('|')
		print('# Blender: %s'%(progress[-1].strip()))
