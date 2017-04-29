//@	{
//@  "targets":[{"name":"container.hpp","type":"include"}]
//@	}

#ifndef ANJA_CONTAINER_HPP
#define ANJA_CONTAINER_HPP

namespace Anja
	{
	class Container
		{
		public:
			virtual ~Container()=default;
			virtual Container& add(void* handle)=0;
			virtual Container& show()=0;
			virtual Container& sensitive(bool val)=0;
			virtual void* toplevel() const=0;
		};
	}

#endif // ANJA_RANGEVIEW_HPP
