//@	{"targets":[{"name":"portselector.hpp","type":"include"}]}

#ifndef ANJA_PORTSELECTOR_HPP
#define ANJA_PORTSELECTOR_HPP

#include "buttonlist.hpp"

namespace Anja
	{

	class PortSelector
		{
		public:
			PortSelector(Container& cnt):m_box(cnt,true)
				,m_label(m_box,"Select or unselect ports")
				,m_ports(m_box.insertMode({2,Box::EXPAND|Box::FILL}),true)
				{}

			~PortSelector()
				{}

			PortSelector& portAppend(const char* port_name)
				{
				m_ports.append(port_name);
				return *this;
				}

			template<class Callback>
			const PortSelector& state(Callback&& cb) const noexcept
				{
				std::for_each(m_ports.begin(),m_ports.end()
					,[&cb](const Button& btn)
						{cb(btn.label(),btn.state());});
				return *this;
				}

		private:
			Box m_box;
				Label m_label;
				ButtonList m_ports;
		};

	}

#endif // ANJA_PORTSELECTOR_HPP
