#pragma once
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include <algorithm>
using namespace std;
class ImageProcessing {
public:
    ImageProcessing();
    ImageProcessing(string path);
    // Reloading image
    void loadImage(string path);
    // Output image
    void outputImage(string path, int form);
    // test function
    void testOutput(string path);
    // histogram equalization
    void Equalization();
    // output original intensity graph
    void OriginalIntensityGraph() const;
    // output processed intensity graph
    void ProcessedIntensityGraph() const;
    // output intensity trasfer function graph
    void IntensityFuncGraph() const;
    // Intensity vector getter
    vector<int> getOriginalIntensity() const { return intensity; }
    vector<int> getProcessedIntensity() const { return intensity_processed; }
    // form
    static int P2;
    static int P5;
    int getCol() const { return column; }
    int getRow() const { return row; }
    int getScale() const { return scale; }
    bool isLoad() const { return load; }
    string getOriginalIntensityPath() { return string(inputPath.substr(0, inputPath.size()-4) + "_intensity.pgm"); }
    string getProcessedIntensityPath() { return string(outputPath.substr(0, outputPath.size()-4) + "_intensity.pgm"); }
    string getFunctionPath(){ return string(outputPath.substr(0, outputPath.size()-4) + "_intensity_func.pgm"); }
    void setCol(int col);
    void setRow(int row);
    void setScale(int scale);
    void setProcessed() { processed = true; }
private:
    // image width, height
    int column = 0;
    int row = 0;
    // image max intensity
    int scale = 0;
    // processed image width, height
    int column_processed = 0;
    int row_processed = 0;
    // processed image max intensity
    int scale_processed = 0;
    // boolean check process done, image input done
    bool processed = false;
    bool load = false;
    // original image matrix
    vector<vector<int>> image;
    // processed image matrix
    vector<vector<int>> image_processed;
    // intensity vector
    vector<int> intensity;
    vector<int> intensity_processed;
    // intensity transformation
    vector<double> intensity_s;
    // comment string
    const string comment = "# nothing\n";
    // i/o put path string
    string outputPath;
    string inputPath;
    // intensity image size
    int h = 512;
    int w = 1024;
    int const GRAY_LEVEL = 255;
};
#endif
