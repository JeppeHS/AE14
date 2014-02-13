#include <iostream>
#include "BinarySearch.h"
#include "LinearSearch.h"
#include "BFSBinarySearch.h"
using std::cout;

void testInord1(){
	const int N = 9;
  int linArray[N] = {1,2,4,5,6,7,8,9,10};
  BinarySearch bfsBinSearch;
  bfsBinSearch.createDataStructure(linArray, N);
  int elem = 3;
  int res =  bfsBinSearch.binSearch(elem);
  cout << "Inorder Pred("<<elem<<") = "<<res<<"\n";
}

void testInord2(){
	const int N = 9;
  int linArray[N] = {1,2,4,5,6,7,8,9,10};
  BinarySearch bfsBinSearch;
  bfsBinSearch.createDataStructure(linArray, N);
  int elem = 10;
  int res =  bfsBinSearch.binSearch(elem);
  cout << "Inorder Pred("<<elem<<") = "<<res<<"\n";
}

void testInord3(){
	const int N = 9;
  int linArray[N] = {1,2,4,5,6,7,8,9,10};
  BinarySearch bfsBinSearch;
  bfsBinSearch.createDataStructure(linArray, N);
  int elem = 11;
  int res =  bfsBinSearch.binSearch(elem);
  cout << "Inorder Pred("<<elem<<") = "<<res<<"\n";
}

void testInord4(){
	const int N = 9;
  int linArray[N] = {1,2,4,5,6,7,8,9,10};
  BinarySearch bfsBinSearch;
  bfsBinSearch.createDataStructure(linArray, N);
  int elem = 0;
  int res =  bfsBinSearch.binSearch(elem);
  cout << "Inorder Pred("<<elem<<") = "<<res<<"\n";
}
//Linear Search test
void test1();
void test2();
int main(){
	const int N = 9;
	int linArray[N] = {1,2,4,5,6,7,8,9,10};
	LinearSearch linSearch;
	linSearch.createDataStructure(linArray, N);
	int elem = 3;
	int res =  linSearch.linSearch(elem);
	cout << "Pred("<<elem<<") = "<<res<<"\n";
	test1();
	test2();
//	testInord1();
	testInord2();
	testInord3();
	testInord4();
	return 0;
}

void test1(){
	const int N = 9;
	int linArray[N] = {1,2,4,5,6,7,8,9,10};
	LinearSearch linSearch;
	linSearch.createDataStructure(linArray, N);
	int elem1 = 0;
	int res =  linSearch.linSearch(elem1);
	cout << "Pred("<<elem1<<") = "<<res<<"\n";
}

void test2(){
	const int N = 9;
	int linArray[N] = {1,2,4,5,6,7,8,9,10};
	LinearSearch linSearch;
	linSearch.createDataStructure(linArray, N);
	int elem2 = 11;
	int res =  linSearch.linSearch(elem2);
	cout << "Pred("<<elem2<<") = "<<res<<"\n";
}