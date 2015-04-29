#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <plplot/plplot.h>
#include <vector>


using namespace std;
using namespace cv;
#define BINS 256
#define QLEVELS 8
#define EPOCH 1e7

void printT(vector<double> boundPoints) {
	cout << "Values of the intervals (T) are : " << endl;
	for (int i = 0; i < QLEVELS + 1; i++) {
		cout << boundPoints[i] << " ";
	}
	cout << endl << endl;
}

void printR(vector<double> repPoints) {
	cout << "Values of the centroid (R) are : " << endl;
	for (int i = 0; i < QLEVELS; i++) {
		cout << repPoints[i] << " ";
	}
	cout << endl << endl;
}
//unsigned short* getPixelFrequency(char *memblock){
//	unsigned short *P = new unsigned short[256];
//	//Initialize the count values with 0
//	for(int i = 0; i < BINS; i++){
//		P[i] = 0;
//	}
//	unsigned char pixel;
//	for( int i = 0; i <)
//}
/*
 * This function does the Lyold max training given the
 * data. It sets quantization levels  (T) and the centroids
 * (R) in the global file
 */
//void train(char *memblock, int n){
//	cout<<"Started training.... "<<endl;
//	float previousMSE, currentMSE,PSNR, distortionRate = 1;
//	unsigned short* P;
//	P = getPixelFrequency(memblock);
//}

int main(int argc, char** argv){
	Mat img;

	img = imread("images/stirlbas.jpg",
			0);

	int bins[BINS] = {0};

	for( int r = 0; r < img.rows; r++ ){
		for( int c = 0; c < img.cols; c++ ){
			int intensity = img.at<uchar>(r,c);
			bins[intensity]++;
		}
	}

/*
	for_each(begin(bins), end(bins), [](int n){
		cout<<n<<endl;});
*/

	for(int i = 0; i < BINS; i++){
		cout<<"Number of pixels with"
				" intensity: "<<i<<""
						" "<<bins[i]<<endl;
	}

	/*
	 * sum of values in bins
	 */
	int sum;
	for(int i = 0; i < BINS; i++){
		sum = sum + bins[i];
	}


	/*
	 * Turn h(x) into a density function p(x)
	 */
	float density_p_x[256];

	for( int i=0; i < BINS; i++ ){
		density_p_x[i] = (float)bins[i]/sum;
	}

	for( int i = 0; i < BINS; i++){
		cout<<density_p_x[i]<<endl;
	}

	/*
	 * Initialize the boundaries boundary_points of the quantization levels
	 *
	 * a_{0} = 0; a_{v} = v. frac{256}{L}; a_{L+1} = 256
	 * a_{0} to a_{8}, i.et total of 9 boundary points.
	 */

	vector<double> boundary_points;
	for( int i = 0; i <=QLEVELS; i++)
		boundary_points.push_back(i*256/QLEVELS);


	/*
	 * The range 0-255 is divided uniformly into L intervals.
	 * values of representation points are center of
	 * each of the interval.
	 *
	 * Initialize the quantization points. total representation points are
	 * 10. b_{0} to b_{7}.
	 *
	 */

	vector<double> represen_points;
	for( int i = 0; i < QLEVELS; i++ ){
		represen_points.push_back((double)(i*256/QLEVELS)+(256/(2*QLEVELS)));
	}

	/*
	 * For a 3-bit quantizer, the initial values of 'b' are
	 */

	cout<<"For a 3-bit quantizer, "
			"the initial values of 'b' or representation "
			"or quantization points are \n "<<endl;

	auto iter = represen_points.begin();

	while( iter != represen_points.end() ) {
		cout<<*iter;
		cout<<endl;
		iter++;
	}

	/*
	 * Recompute the values of of R using the
	 * probability values
	 */

	int lower, higher;
	long int num, den;
	int iterations = 0;
	while(iterations < EPOCH){
		/*
		 * Calculate boundaries, 8 boundaries_points (represented as 'a' ) boundary_points[8]
		 * represen_points[7] (represented as 'b').
		 *
		 */

		for(int i = 1; i < QLEVELS; i++){
			boundary_points[i] =
					(double)(represen_points[i] + represen_points[i-1])/2;
	//	}



//		for(int i = 0; i < QLEVELS; i++){
			lower = ceilf(boundary_points[i]);

			if(i == QLEVELS - 1)
				higher = ceilf(boundary_points[i+1]);
			else
				higher = floorf(boundary_points[i+1]);

			num = 0; den = 0;

			for(int j = lower;j <= higher;j++){
				num += j * density_p_x[j];
				den += density_p_x[j];
			}
			if(den != 0){
				represen_points[i] = (float) num/den;
			}

		}

//		printR(represen_points);
//		printT(boundary_points);

		iterations++;

	}

	printR(represen_points);

	imshow("Image", img);

	waitKey(0);
}
