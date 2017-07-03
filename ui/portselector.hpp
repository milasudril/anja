			//@	{"targets":[{"name":"portselector.hpp","type":"include"}]}

#ifndef ANJA_PORTSELECTOR_HPP
#define ANJA_PORTSELECTOR_HPP

#include "buttonlist.hpp"
#include "filler.hpp"

namespace Anja
	{

	class PortSelector
		{
		public:
			PortSelector(Container& cnt):m_box(cnt,true)
				,m_label(m_box.insertMode({4,0}),"Select or unselect ports by click-\ning on the corresponding button")
					,m_ports_panel(m_box.insertMode({4,Box::EXPAND|Box::FILL}),false)
						,m_filler_left(m_ports_panel.insertMode({0,Box::EXPAND|Box::FILL}))
						,m_ports(m_ports_panel.insertMode({0,0}),true)
						,m_filler_right(m_ports_panel.insertMode({0,Box::EXPAND|Box::FILL}))
				{
				}

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
				Box m_ports_panel;
					Filler m_filler_left;
					ButtonList m_ports;
					Filler m_filler_right;
		};

	}

#endif // ANJA_PORTSELECTOR_HPP
