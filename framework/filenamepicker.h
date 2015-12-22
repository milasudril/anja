#ifdef __WAND__
target[name[filenamepicker.h] type[include]]
dependency[filenamepicker.o]
#endif

#ifndef FILENAMEPICKER_H
#define FILENAMEPICER_H

#include <memory>

class Widget;

class FilenamePicker
	{
	private:
		typedef void (*Destructor)(FilenamePicker* picker);
	public:
		static constexpr uint32_t MODE_OPEN=0;
		static constexpr uint32_t MODE_SAVE=1;

		static std::unique_ptr<FilenamePicker,Destructor> create(
			 const Widget& owner
			,const char* filename_in,uint32_t mode)
			{return {instanceCreate(owner,filename_in,mode),destroy};}

		virtual const char* filenameGet() const=0;

	protected:
		virtual ~FilenamePicker()=default;

	private:
		static FilenamePicker* instanceCreate(const Widget& owner
			,const char* filename_in
			,uint32_t mode);

		static void destroy(FilenamePicker* picker);
	};

#endif
