#ifdef __WAND__
target[name[anja] type[application]]
#endif

//#include "waveformdataview.h"
//#include "waveformdata.h"
//#include "waveformrangetrimmer.h"

#include "sessionfilerecordimpl.h"
#include "sessionfilereader.h"

//#include "framework/window.h"
//#include "framework/boxvertical.h"
//#include "framework/keyboardlayout.h"
//#include "framework/keyboardview.h"

class PropsEnumerator:public SessionFileRecord::PropertyEnumerator
	{
	public:
		void propertyVisit(const ArrayDynamicShort<char>& name
			,const ArrayDynamicShort<char>& value)
			{
			printf("%s=%s\n",name.begin(),value.begin());
			}
	};

int main()
	{
	try
		{
		auto reader=SessionFileReader::create("testbank/testbank.txt");
		SessionFileRecordImpl record;
		PropsEnumerator enumerator;
		while(reader->recordNextGet(record))
			{
			printf("\n%s\n",record.titleGet().begin());
			if(record.sectionLevelGet()==0)
				{printf("====================================\n");}
			else
				{printf("------------------------------------\n");}
			record.propertiesEnum(enumerator);
			}

/*	KeyboardLayout layout;
	WaveformRangeTrimmer trimmer;

	auto event_loop=EventLoop::create();
	auto mainwin=Window::create(*event_loop);
	mainwin->titleSet("Anja - New session");

	auto box=BoxVertical::create(*mainwin);
	box->insertModeSet(BoxVertical::INSERTMODE_EXPAND
		|BoxVertical::INSERTMODE_FILL);
	auto keyview=KeyboardView::create(*box,layout);


	box->insertModeSet(BoxVertical::INSERTMODE_END);
	auto waveformdata=WaveformDataView::create(*box,trimmer);*/
		}
	catch(const char* msg)
		{printf("Error: %s\n",msg);}
	return 0;
	}
