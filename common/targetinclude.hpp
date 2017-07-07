//@	{"targets":[{"name":"targetinclude.hpp","type":"include"}]}

#ifndef ANJA_TARGETINCLUDE_HPP
#define ANJA_TARGETINCLUDE_HPP

#define _ANJA_TARGET(targetdir,current_dir,x) _STRINGIFY(targetdir/current_dir/x)
#define _STRINGIFY(x) #x

#if defined(MAIKE_TARGET_DIRECTORY) && defined(MAIKE_CURRENT_DIRECTORY)
#define ANJA_TARGET(x) _ANJA_TARGET(MAIKE_TARGET_DIRECTORY,MAIKE_CURRENT_DIRECTORY,x)
#else
#error "MAIKE_TARGET_DIRECTORY or MAIKE_CURRENT_DIRECTORY not set"
#endif

#endif
