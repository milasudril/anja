args=argv();
target_dir=char(args(1));
in_dir=char(args(2));

ambient_in=double(imread([target_dir,'/',in_dir,'/knob-ambient-in.png']));
ambient_in=ambient_in(:,:,1)/65535;
mask_in=double(imread([target_dir,'/',in_dir,'/knob-mask-in.png']));
mask_in=mask_in(:,:,1)/65535;
diffuse_in=double(imread([target_dir,'/',in_dir,'/knob-diffuse-in.png']));
diffuse_in=diffuse_in(:,:,1)/65535;

srgb=@(X)( 12.92*double(X<=0.0031308).*X + (((1 + 0.055).*X.^(1./2.4)) - 0.055  ).*double(X>0.0031308));
ambient=ambient_in - diffuse_in;
imwrite(uint8(255*srgb(ambient)),[target_dir,'/',in_dir,'/knob-ambient.png'] ...
	,'Alpha',uint8(255*mask_in));
imwrite(uint8(255*srgb(diffuse_in)),[target_dir,'/',in_dir,'/knob-diffuse.png'] ...
	,'Alpha',uint8(255*mask_in));
imwrite(uint8(255*srgb(mask_in)),[target_dir,'/',in_dir,'/knob-mask.png'] ...
	,'Alpha',uint8(255*mask_in));

exit(0);
