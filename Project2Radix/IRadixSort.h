
#ifndef IRADIXSORT_H
#define IRADIXSORT_H


class IRadixSort
{
public:
	virtual void setup(int arrSize) = 0;
	virtual int* sort(int* array, int arrSize) = 0;
};

#endif
