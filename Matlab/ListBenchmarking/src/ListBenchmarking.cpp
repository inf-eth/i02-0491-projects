#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include <fstream>
#include <vector>
#include <cmath>
#include "ListBenchmarking.h"

int main()
{
	bool MatlabPlot = true;
	int insertions = 100;
	int retrievals = 100;
	int deletions = 50;
	srand(time(NULL));

	std::fstream ArrayData("ArrayData.txt", std::ios::out);
	std::fstream ListData("ListData.txt", std::ios::out);
	std::vector<double> ArrayDataD;
	std::vector<double> ListDataD;
	std::vector<int> nSizes({100,200,400,500,1000,2000,3000,4000,5000,10000,11000,12000,13000,14000,15000});
	std::vector<double> ArraySpace;
	std::vector<double> ListSpace;
	for (int n=0; n<nSizes.size(); n++)
	{
		int i=nSizes[n];
		ArrayData << i << " "; ArrayDataD.push_back((double)i);
		ListData << i << " "; ListDataD.push_back((double)i);
		Clock.Start();
		ArrayList arrayTest(i);
		Clock.Stop();
		ArrayData << Clock.ElapsedTime() << " "; ArrayDataD.push_back(Clock.ElapsedTime());
		ArrayDataD.push_back((double)MAXSIZE);

		Clock.Start();
		for (int j=0; j<insertions; j++)
			arrayTest.Insert(rand_pos(arrayTest.size()),rand_pos(range));
		Clock.Stop();
		ArrayData << Clock.ElapsedTime() << " "; ArrayDataD.push_back(Clock.ElapsedTime());

		Clock.Start();
		for (int j=0; j<retrievals; j++)
			arrayTest.Retrieve(rand_pos(arrayTest.size()));
		Clock.Stop();
		ArrayData << Clock.ElapsedTime() << " "; ArrayDataD.push_back(Clock.ElapsedTime());

		Clock.Start();
		for (int j=0; j<deletions; j++)
			arrayTest.Delete(rand_pos(arrayTest.size()));
		Clock.Stop();
		ArrayData << Clock.ElapsedTime() << endl; ArrayDataD.push_back(Clock.ElapsedTime());

		Clock.Start();
		LinkedList linkedListTest(i);
		Clock.Stop();
		ListData << Clock.ElapsedTime() << " "; ListDataD.push_back(Clock.ElapsedTime());
		ListDataD.push_back((double)(linkedListTest.size()*2));

		Clock.Start();
		for (int j=0; j<insertions; j++)
			linkedListTest.Insert(rand_pos(linkedListTest.size()),rand_pos(range));
		Clock.Stop();
		ListData << Clock.ElapsedTime() << " "; ListDataD.push_back(Clock.ElapsedTime());

		Clock.Start();
		for (int j=0; j<retrievals; j++)
			linkedListTest.Retrieve(rand_pos(linkedListTest.size()));
		Clock.Stop();
		ListData << Clock.ElapsedTime() << " "; ListDataD.push_back(Clock.ElapsedTime());

		Clock.Start();
		for (int j=0; j<deletions; j++)
			linkedListTest.Delete(rand_pos(linkedListTest.size()));
		Clock.Stop();
		ListData << Clock.ElapsedTime() << endl; ListDataD.push_back(Clock.ElapsedTime());
	}

	ArrayData.close();
	ListData.close();

	// ================= Matlab Plot =================
	if (!MatlabPlot)
		return 0;
	using namespace matlab::engine;
	using namespace matlab::data;

	// Start MATLAB engine
	std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();
	ArrayFactory factory;

	// Convert to MATLAB arrays
	TypedArray<double> ArrayDataDM = factory.createArray({1, ArrayDataD.size()}, ArrayDataD.begin(), ArrayDataD.end());
	TypedArray<double> ListDataDM = factory.createArray({1, ListDataD.size()}, ListDataD.begin(), ListDataD.end());

	// Send variables to MATLAB
	matlabPtr->setVariable(u"ListData", std::move(ListDataDM));
	matlabPtr->setVariable(u"ArrayData", std::move(ArrayDataDM));

	// From C++ row-major to Matlab column-major array storage
	matlabPtr->eval(u"disp(ListData)");
	matlabPtr->eval(u"disp(ArrayData)");
	matlabPtr->eval(u"ListData=reshape(ListData,6,[])';");
	matlabPtr->eval(u"ArrayData=reshape(ArrayData,6,[])';");
	matlabPtr->eval(u"disp(ListData)");
	matlabPtr->eval(u"disp(ArrayData)");

	// Plot in MATLAB
	matlabPtr->eval(u"figure(1); clf; plot(ListData(:,1),ListData(:,4:end),'LineWidth',2); "
		"title('List and Array Time Complexity'); "
		"xlabel('n'); ylabel('time (ms)'); grid on;");
	matlabPtr->eval(u"figure(1); hold on; plot(ArrayData(:,1),ArrayData(:,4:end),'LineWidth',2); "
		"xlabel('n'); ylabel('time (ms)'); grid on;");
	matlabPtr->eval(u"legend('Insert (list)','Retrieve (list)','Delete (list)','Insert (array)','Retrieve (array)','Delete (array)');");
	matlabPtr->eval(u"legend('location','northwest');");

	matlabPtr->eval(u"figure(2); clf; plot(ListData(:,1),ListData(:,2),'LineWidth',2); "
		"title('n Insertions');"
		"xlabel('n'); ylabel('time (ms)'); grid on;");
	matlabPtr->eval(u"figure(2); hold on; plot(ArrayData(:,1),ArrayData(:,2),'LineWidth',2); "
		"xlabel('n'); ylabel('time (ms)'); grid on;");
	matlabPtr->eval(u"legend('list','array')");
	matlabPtr->eval(u"legend('location','northwest');");

	matlabPtr->eval(u"figure(3); clf; plot(ListData(:,1),4*ListData(:,3)/1024,'LineWidth',2); "
		"title('Space Complexity');"
		"xlabel('n'); ylabel('Space (Bytes)'); grid on;");
	matlabPtr->eval(u"figure(3); hold on; plot(ArrayData(:,1),4*ArrayData(:,3)/1024,'LineWidth',2); "
		"xlabel('n'); ylabel('Space (KBytes)'); grid on;");
	matlabPtr->eval(u"legend('list','array')");
	matlabPtr->eval(u"legend('location','northwest');");

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
