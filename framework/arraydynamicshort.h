#ifdef __WAND__
target[name[arraydynamicshort.h] type[include]]
dependency[new.o]
#endif

#ifndef ARRAYDYNAMICSHORT_H
#define ARRAYDYNAMICSHORT_H

#include <iterator>
#include <cstdint>
#include <algorithm>
#include <cassert>

template<class T>
class alignas(16) ArrayDynamicShort
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		ArrayDynamicShort(const T* source);

		ArrayDynamicShort(const T* source,uint32_t N):
			m_data(new T[N]),m_length(0),m_capacity(N)
			{
			if(source!=nullptr && N!=0)
				{append(source,N);}
			}

		ArrayDynamicShort():m_data(new T[1]),m_length(0),m_capacity(1)
			{}

		ArrayDynamicShort(const ArrayDynamicShort& obj):
			m_data(new T[obj.length()]),m_length(0),m_capacity(obj.length())
			{
			assert(&obj!=this);
			append(obj.begin(),obj.length());
			}

		ArrayDynamicShort(ArrayDynamicShort&& obj):
			m_data(obj.m_data),m_length(obj.m_length),m_capacity(obj.m_capacity)
			{
			obj.m_data=nullptr;
			obj.m_length=0;
			obj.m_capacity=0;
			}

		ArrayDynamicShort& operator=(const ArrayDynamicShort& obj)
			{
			assert(&obj!=this);
			clear();
			append(obj.begin(),obj.length());
			return *this;
			}

		ArrayDynamicShort& operator=(ArrayDynamicShort&& obj)
			{
			std::swap(obj.m_data,m_data);
			std::swap(obj.m_length,m_length);
			std::swap(obj.m_capacity,m_capacity);
			return *this;
			}

		~ArrayDynamicShort()
			{
			if(m_data!=nullptr)
				{delete[] m_data;}
			}

		uint32_t length() const
			{return m_length;}



		const T& operator[](uint32_t position) const
			{return m_data[position];}

		T& operator[](uint32_t position)
			{return m_data[position];}

		T& front()
			{return *m_data;}

		const T& front() const
			{return *m_data;}

		T& back()
			{return *(m_data+m_length-1);}

		const T& back() const
			{return *(m_data+m_length-1);}




		const T* begin() const
			{return m_data;}

		T* begin()
			{return m_data;}

		const T* data() const
			{return begin();}

		T* data()
			{return begin();}

		const T* end() const
			{return m_data+m_length;}

		T* end()
			{return m_data+m_length;}



		const_reverse_iterator rbegin() const
			{return const_reverse_iterator{end()};}

		reverse_iterator rbegin()
			{return reverse_iterator{end()};}

		const_reverse_iterator rend() const
			{return const_reverse_iterator{begin()};}

		reverse_iterator rend()
			{return reverse_iterator{begin()};}



		ArrayDynamicShort& append(const T& obj)
			{
			if(m_length==m_capacity)
				{capacityIncrease();}
			m_data[m_length]=obj;
			++m_length;
			return *this;
			}

		ArrayDynamicShort& append(T&& obj)
			{
			if(m_length==m_capacity)
				{capacityIncrease();}
			m_data[m_length]=std::move(obj);
			++m_length;
			return *this;
			}

		ArrayDynamicShort& append(const T* buffer,uint32_t N);

		ArrayDynamicShort& append(T* buffer,uint32_t N);

		ArrayDynamicShort& append(const ArrayDynamicShort& source)
			{return append(source.begin(),source.length());}

		ArrayDynamicShort& append(ArrayDynamicShort&& source)
			{return append(source.begin(),source.length());}

 		ArrayDynamicShort& erase(T* location);

		void capacitySet(uint32_t capacity_new);

		void clear()
			{m_length=0;}

		ArrayDynamicShort& truncate()
			{
			--m_length;
			return *this;
			}

	private:
		T* m_data;
		uint32_t m_length;
		uint32_t m_capacity;

		void capacityIncrease()
			{
			capacitySet(1+m_capacity);
			}


	};

template<class T>
ArrayDynamicShort<T>::ArrayDynamicShort(const T* source)
	:m_data(new T[1]),m_length(0),m_capacity(1)
	{
	T nullobj(0);
	while(*source!=nullobj)
		{
		append(*source);
		++source;
		}
	append(std::move(nullobj));
	}


template<class T>
ArrayDynamicShort<T>& ArrayDynamicShort<T>::erase(T* location)
	{
	auto ptr_end=end()-1;
	while(location!=ptr_end)
		{
		*location=std::move(*(location+1));
		++location;
		}
	--m_length;
	return *this;
	}

template<class T>
void ArrayDynamicShort<T>::capacitySet(uint32_t capacity)
	{
	if(capacity>m_capacity) // Do not reallocate buffer if it already is large enougth
		{
		uint64_t capacity_new=std::max(uint64_t(capacity),uint64_t(m_capacity) << 1);

		if(capacity_new>0xffffffffllu)
			{throw "Internal array capacity exceeded";}

		m_capacity=capacity_new;
		auto data_new=new T[capacity_new];
		std::move(begin(),end(),data_new);
		delete[] m_data;
		m_data=data_new;
		}
	}

template<class T>
ArrayDynamicShort<T>& ArrayDynamicShort<T>::append(const T* buffer,uint32_t N)
	{
	if(m_length+N>m_capacity)
		{capacitySet(m_length+N);}
	std::copy(buffer,buffer+N,end());
	m_length+=N;
	return *this;
	}

template<class T>
ArrayDynamicShort<T>& ArrayDynamicShort<T>::append(T* buffer,uint32_t N)
	{
	if(m_length+N>m_capacity)
		{capacitySet(m_length+N);}
	std::move(buffer,buffer+N,end());
	m_length+=N;
	return *this;
	}

#endif
