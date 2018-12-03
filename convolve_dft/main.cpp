#include <QCoreApplication>
#include <QFile>
#include <QByteArray>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

void convolveDFT(InputArray A, InputArray B, OutputArray C)
{
    // reallocate the output array if needed
    C.create(abs(A.rows() - B.rows())+1, abs(A.cols() - B.cols())+1, A.type());
    Size dftSize;
    // calculate the size of DFT transform
    dftSize.width = getOptimalDFTSize(A.cols() + B.cols() - 1);
    dftSize.height = getOptimalDFTSize(A.rows() + B.rows() - 1);

    // allocate temporary buffers and initialize them with 0's
    Mat tempA(dftSize, A.type(), Scalar::all(0));
    Mat tempB(dftSize, B.type(), Scalar::all(0));

    // copy A and B to the top-left corners of tempA and tempB, respectively
    Mat roiA(tempA, Rect(0,0,A.cols(), A.rows()));
    A.copyTo(roiA);
    Mat roiB(tempB, Rect(0,0,B.cols(), B.rows()));
    B.copyTo(roiB);

    // now transform the padded A & B in-place;
    // use "nonzeroRows" hint for faster processing
    dft(tempA, tempA, 0, A.rows());
    dft(tempB, tempB, 0, B.rows());

    // multiply the spectrums;
    // the function handles packed spectrum representations well
    mulSpectrums(tempA, tempB, tempA, DFT_ROWS);

    // transform the product back from the frequency domain.
    // Even though all the result rows will be non-zero,
    // you need only the first C.rows of them, and thus you
    // pass nonzeroRows == C.rows
    dft(tempA, tempA, DFT_INVERSE + DFT_SCALE, C.rows());

    // now copy the result back to C.
    tempA(Rect(0, 0, C.cols(), C.rows())).copyTo(C);

    // all the temporary buffers will be deallocated automatically
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file(":/res/lena.jpg");
    if(!file.open(QFile::ReadOnly))
    {
        qWarning() << "Open file failed!";
        return -1;
    }

    QByteArray ba = file.readAll();
    Mat src = imdecode(vector<char>(ba.begin(), ba.end()), IMREAD_GRAYSCALE);
    imshow("Origin image", src);

    Mat kernel = Mat_<float>(3, 3, 1.0);
    Mat floatI;
    src.convertTo(floatI, CV_32F);

    Mat filteredI;
    convolveDFT(floatI, kernel, filteredI);

    normalize(filteredI, filteredI, 0, 1, NORM_MINMAX);

    Mat tmp = filteredI * 255;
    tmp.convertTo(filteredI, CV_8U);
    imshow("filtered", filteredI);

    return a.exec();
}
