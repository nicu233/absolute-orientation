#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <time.h>

using namespace std;
using namespace Eigen;

void generateTestData(vector<Vector3f> & inPoints, vector<Vector3f> & outPoints,  const Matrix4f &T , int pointNum = 3 ) {

	inPoints.clear();
	outPoints.clear();

	//auto genRandomNum = []() {
		//srand((unsigned)time(NULL));
		//return rand() % 100;
	//};
	srand((unsigned)time(NULL));
	for (auto i = 0; i < pointNum; ++i) {
		Vector3f aPoint(rand() % 100, rand() % 100, rand() % 100);
		inPoints.push_back(aPoint);
		cout << "a inPoint is " << aPoint << endl;
	}


	cout << "T is " << T << endl;
	for (auto i = 0; i < pointNum; ++i) {

		Vector4f aInPoint(inPoints[i](0), inPoints[i](1), inPoints[i](2), 1);

		
		Vector4f aPoint(T* aInPoint);
		Vector3f aOutPoint(aPoint(0), aPoint(1), aPoint(2));
		outPoints.push_back(aOutPoint);

		cout << "an outpoint is " << aOutPoint << endl;
	}
	
}


void absoluteOrientation(vector<Vector3f> & inPoints, vector<Vector3f> & outPoints) {
	Matrix4f res;
	Vector3f inAverage(0,0,0);
	Vector3f outAverage(0,0,0);

	cout << "inPoints size " << inPoints.size() << ", outPoints size " << outPoints.size() << endl;

	if (inPoints.size() != outPoints.size()) exit(-1);

	for (auto i = 0; i < inPoints.size(); ++i) {
		//inAverage += inPoints[i];
		cout << "inAverage  is " << inAverage << ", inPoints[i]  " << inPoints[i] << endl;
		inAverage = inPoints[i] + inAverage;

		//inAverage(0) += inPoints[i](0);
		//inAverage(1) += inPoints[i](1);
		//inAverage(2) += inPoints[i](2);
	}

	cout << "before inAverage is " << inAverage << endl;

	inAverage /= inPoints.size();
	

	//inAverage = { inAverage(1) / inPoints.size() ,
	//			inAverage(2) / inPoints.size() ,
	//			inAverage(3) / inPoints.size() };

	cout << "inAverage is " << inAverage << endl;

	for (auto i = 0; i < outPoints.size(); ++i) {
		outAverage += outPoints[i];
		//outAverage(1) += outPoints[i](1);
		//outAverage(2) += outPoints[i](2);
		//outAverage(3) += outPoints[i](3);
	}

	cout << "before outAverage is " << outAverage << endl;

	//outAverage = { outAverage(1) / outPoints.size() ,
	//outAverage(2) / outPoints.size() ,
	//outAverage(3) / outPoints.size() };

	outAverage /= outPoints.size();
	
	cout << "outAverage is " << outAverage << endl;

	//pi' = pi - p_average
	vector<Vector3f> inpp;
	//qi' = qi - q_average
	vector<Vector3f> outpp;

	for (auto i = 0; i < inPoints.size(); ++i) {
		Vector3f ipp = inPoints[i] - inAverage;
		Vector3f opp = outPoints[i] - outAverage;
		inpp.push_back(ipp);
		outpp.push_back(opp);
	}


	//Matrix3f M = Matrix3f::Zero();
	Matrix3f M = Matrix3f::Zero();
	cout << "M" << M << endl;
	for (auto i = 0; i < inPoints.size(); ++i) {
		M += outpp[i] * inpp[i].transpose();
	}

	cout << "sum M" << M << endl;

	
	JacobiSVD<Matrix3f> svd(M, ComputeThinU | ComputeThinV);

	Matrix3f U = svd.matrixU();

	Matrix3f V = svd.matrixV();


	Matrix3f R = V * U.transpose();

	Vector3f T =  outAverage - R * inAverage ;

	cout << "R : \n" << R << endl;
	cout << "T : \n" << T << endl;

}


int main()
{

	vector<Vector3f> p;
	vector<Vector3f> q;

	Matrix4f groundTruth;
	groundTruth << \
		1.0, 0.0, 0.0, 10.0, \
		0.0, 0.0, 1.0, 30.0, \
		0.0, 1.0, 0.0, 50.0, \
		0.0, 0.0, 0.0, 1.0 ;

	cout << "ground truth : \n" << groundTruth << endl;

	generateTestData(p, q, groundTruth, 10);

	absoluteOrientation(p, q);
}
