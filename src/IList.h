#ifndef ILIST_H
#define ILIST_H

#define None 0

template <typename T>
class IList{
	protected:
		unsigned _size;
	public:
		IList() : _size(None) {}
		IList(unsigned size) : _size(size) {}
		virtual ~IList() {}

		virtual T pop(int n)=0;
		virtual void delete_data()=0;
		virtual void push_tail(T& data)=0;
		virtual unsigned index(T& data)=0;
		virtual T& operator[](int n)=0;
		virtual const T& operator[](int n) const=0;
		virtual unsigned get_size()=0;
};

#endif