#ifdef __WAND__
target[name[slider.h] type[include]]
dependency[slider.o]
#endif

#ifndef SLIDER_H
#define SLIDER_H

class GuiContainer;

class Slider
	{
	public:
		class EventHandler
			{
			public:
				virtual const char* textGet(double value)=0;
				virtual double valueGet(const char* text)=0;
			};

		static Slider* create(GuiContainer& parent,EventHandler& handler)
			{return new Slider(parent,handler);}

		void destroy()
			{delete this;}

		void valueSet(double value);

		double valueGet() const
			{return m_value;}

	private:
		Slider(GuiContainer& parent,EventHandler& handler);
		~Slider();

		double m_value;
		class Impl;
		Impl* m_impl;
	};

#endif

