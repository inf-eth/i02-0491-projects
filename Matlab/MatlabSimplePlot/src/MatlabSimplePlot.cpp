#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include <fstream>
#include <vector>
#include <cmath>
#include "MatlabSimplePlot.h"
#include <chrono>
using std::cout;
using std::endl;

struct msClock
{
	typedef std::chrono::high_resolution_clock clock;
	std::chrono::time_point<clock> t1, t2;
	void Start() { t1 = std::chrono::high_resolution_clock::now(); }
	void Stop() { t2 = std::chrono::high_resolution_clock::now(); }
	double ElapsedTime()
	{
		std::chrono::duration<double, std::milli> ms_doubleC = t2 - t1;
		return ms_doubleC.count();
	}
}
Clock;

int main()
{
	bool MatlabPlot = true;
	std::vector<int> MatSize({128,256,512,1024});
	std::vector<double> MultiplicationTime;

	for (int i = 0; i < MatSize.size(); i++)
	{
		Clock.Start();
		int RowsA = MatSize[i];
		int ColsA = MatSize[i];
		int RowsB = MatSize[i];
		int ColsB = MatSize[i];

		double* A = new double[RowsA*ColsA];
		double* B = new double[RowsB*ColsB];
		double* C = new double[RowsA*ColsB];
		
		for (int i=0; i<RowsA; i++)
			for (int j=0; j<ColsB; j++)
				for (int k=0; k<ColsA; k++)
					C[j+ColsA*i] += A[k+ColsA*i]*B[j+ColsB*k];
		Clock.Stop();
		MultiplicationTime.push_back(Clock.ElapsedTime());
	}

	// ================= Matlab Plot =================
	if (!MatlabPlot)
		return 0;
	using namespace matlab::engine;
	using namespace matlab::data;

	// Start MATLAB engine
	std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();
	ArrayFactory factory;
	
	// Convert to MATLAB arrays
	TypedArray<int> MatSizeDM = factory.createArray({1, MatSize.size()},MatSize.begin(),MatSize.end());
	TypedArray<double> MultiplicationTimesDM = factory.createArray({1, MultiplicationTime.size()},MultiplicationTime.begin(),MultiplicationTime.end());
	
	// Send variables to MATLAB
	matlabPtr->setVariable(u"MatSize", std::move(MatSizeDM));
	matlabPtr->setVariable(u"MultiplicationTimes",std::move(MultiplicationTimesDM));

	// From C++ row-major to Matlab column-major array storage
	matlabPtr->eval(u"disp(MatSize)");
	matlabPtr->eval(u"disp(MultiplicationTimes)");

	// Plot in MATLAB
	matlabPtr->eval(u"figure(1); clf; plot(MatSize,MultiplicationTimes,'LineWidth',2); "
		"title('Matrix multiplication times'); "
		"xlabel('Square matrix size'); ylabel('time (ms)'); grid on;");

	std::cout << "Press Enter to exit..." << std::endl;
	std::cin.get();
	// ===============================================
/*
	ArrayList arrayTest;
	LinkedList linkedListTest;
	int insertions = 10000;
	int retrievals = 10000;
	int deletions = 5000;
	//cout << "x%0" << 5%0 << endl;
	Clock.Start();
	for (int i=0; i<insertions; i++)
		arrayTest.Insert(rand_pos(arrayTest.size()),rand() % insertions);
	Clock.Stop();
	cout << insertions << " insertions array: " << Clock.ElapsedTime() << " ms." << endl;

	Clock.Start();
	for (int i=0; i<retrievals; i++)
		arrayTest.Retrieve(rand_pos(arrayTest.size()));
	Clock.Stop();
	cout << retrievals << " retrievals array: " << Clock.ElapsedTime() << " ms." << endl;

	Clock.Start();
	for (int i=0; i<deletions; i++)
		arrayTest.Delete(rand_pos(arrayTest.size()));
	Clock.Stop();
	cout << deletions << " deletions array: " << Clock.ElapsedTime() << " ms." << endl;

	Clock.Start();
	for (int i=0; i<insertions; i++)
		linkedListTest.Insert(rand_pos(linkedListTest.size()),rand() % insertions);
	Clock.Stop();
	cout << insertions << " insertions linked-list: " << Clock.ElapsedTime() << " ms." << endl;

	Clock.Start();
	for (int i=0; i<retrievals; i++)
		linkedListTest.Retrieve(rand_pos(linkedListTest.size()));
	Clock.Stop();
	cout << retrievals << " retrievals linked-list: " << Clock.ElapsedTime() << " ms." << endl;

	Clock.Start();
	for (int i=0; i<deletions; i++)
		linkedListTest.Delete(rand_pos(linkedListTest.size()));
	Clock.Stop();
	cout << deletions << " deletions linked-list: " << Clock.ElapsedTime() << " ms." << endl;
*/
	return 0;
}
