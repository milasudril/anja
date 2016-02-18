#ifdef __WAND__
target[
	name[filenamepicker.o]
	type[object]
	platform[;Windows]
	]
#endif

#include "filenamepicker.h"
#include "widget.h"
#include "guihandle.h"
#include <windows.h>
#include <commdlg.h>

class FilenamePickerGtk:public FilenamePicker
	{
	public:
		FilenamePickerGtk(const Widget&owner
			,const char* filename_in,uint32_t mode);

		~FilenamePickerGtk();

		const char* filenameGet() const
			{return m_ok?m_buffer:nullptr;}

	private:
		char m_buffer[4096];
		OPENFILENAMEA  m_ofn;
		bool m_ok;
	};

FilenamePickerGtk::FilenamePickerGtk(
	 const Widget& owner
	,const char* filename_in,uint32_t mode)
	{
	const char* text_ok=mode==MODE_OPEN? "Open" : "Save";
	const char* text_title=mode==MODE_OPEN? "Select file to open" : "Choose filename";

	strncpy(m_buffer,filename_in,4096);
	buffer[4095]=0;

	memset(&m_ofn,0,sizeof(m_ofn));
	m_ofn.lStructSize=sizeof(m_ofn);
	m_ofn.hwndOwner=owner.handleNativeGet();
	m_ofn.hInstance=GetModuleHandle(NULL);
	m_ofn.lpstrFilter="All files\0*.*\0\0";
	m_ofn.lpstrFile=buffer;
	m_ofn.nMaxFile=4096;
	m_ofn.lpstrTitle=text_title;
	m_ofn.Flags=OFN_ENABLESIZING
		|(mode==MODE_OPEN?OFN_FILEMUSTEXIST:OFN_OVERWRITEPROMPT);

	if(mode==MODE_OPEN)
		{
		m_ok=GetOpenFileName(&ofn);
		}
	else
		{
		m_ok=GetSaveFileName(&ofn);
		}
	}

FilenamePickerGtk::~FilenamePickerGtk()
	{
	}

FilenamePicker* FilenamePicker::instanceCreate(const Widget& owner
	,const char* filename_in
	,uint32_t mode)
	{return new FilenamePickerGtk(owner,filename_in,mode);}

void FilenamePicker::destroy(FilenamePicker* picker)
	{
	delete dynamic_cast<FilenamePickerGtk*>(picker);
	}
