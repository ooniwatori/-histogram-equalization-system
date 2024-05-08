#include"ImageProcessing.h"

ImageProcessing::ImageProcessing() {}
ImageProcessing::ImageProcessing(string path) : inputPath(path) {
    ifstream file_input;
    string buffer;
    file_input.open(path);
    // ignore form and comment here
    getline(file_input, buffer);
    getline(file_input, buffer);
    // read properties of image
    file_input >> column >> row >> scale;
    // invalid property
    if (column <= 0 || row <= 0 || scale <= 0) {
        cout << "Fail to load image, form of image is invalid.\n";
    }
    else{
        // read image into matrix
        image = vector<vector<int>>(row);
        for (int i = 0; i < row; ++i) {
            image[i] = vector<int>(column);
            for (int j = 0; j < column; ++j) {
                file_input >> image[i][j];
            }
        }
        file_input.close();
        // statistic number of each gray level in image
        intensity = vector<int>(scale+1, 0);
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < column; ++j) {
                intensity[image[i][j]]++;
            }
        }
        processed = false;
        load = true;
    }
}
void ImageProcessing::loadImage(string path) {
    image.clear();
    ifstream file_input;
    string buffer;
    file_input.open(path);
    // ignore form and comment here
    getline(file_input, buffer);
    getline(file_input, buffer);
    // read properties of image
    file_input >> column >> row >> scale;
    // invalid property
    if (column <= 0 || row <= 0 || scale <= 0) {
        cout << "Fail to load image, form of image is invalid.\n";
        system("PAUSE");
        exit(1);
    }
    // read image into matrix
    image = vector<vector<int>>(row);
    for (int i = 0; i < row; ++i) {
        image[i] = vector<int>(column);
        for (int j = 0; j < column; ++j) {
            file_input >> image[i][j];
        }
    }
    file_input.close();
    processed = false;
}
void ImageProcessing::outputImage(string path, int form) {
    outputPath = path;
    string output_form;
    // P2 or P5
    switch (form) {
    case 0:
        output_form = string("P2\n");
        break;
    case 1:
        output_form = string("P5\n");
        break;
    default:
        output_form = string("P2\n");
    }
    // no processed be done
    if (!processed) exit(1);
    ofstream file_output;
    file_output.open(path);
    file_output << output_form << comment << column << " " << row << endl << scale_processed << endl;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            //file_output << image_processed[i][j] / 255.0 * scale << " "; // rescale image
            file_output << image_processed[i][j] << " ";
        }
        file_output << endl;
    }
    file_output.close();
}

void ImageProcessing::testOutput(string path) {
    string output_form = "P2"; // P2
    ofstream file_output;
    file_output.open(path);
    // output original image
    file_output << output_form << comment << column << " " << row << endl << scale << endl;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            file_output << image[i][j] << " ";
        }
        file_output << endl;
    }
}

void ImageProcessing::Equalization() {
    // new gray level
    scale_processed = GRAY_LEVEL;
    // statistic number of each gray level in image
    vector<double> intensity_pdf = vector<double>(scale+1, 0);
    // computing pdf of gray leve
    double area = row * column;
    for (int i = 0; i < scale; ++i) {
        intensity_pdf[i] = (double)intensity[i]/area;
    }
    // gray level transfer function
    intensity_s = vector<double>(GRAY_LEVEL+1, 0);
    for (int i = 0; i <= GRAY_LEVEL; ++i) {
        for (int k = 0; k <= i && k < scale; ++k) {
            intensity_s[i] += intensity_pdf[k];
        }
        intensity_s[i] *= GRAY_LEVEL; // rescale the gray level to specified value
        intensity_s[i] = floor(intensity_s[i] + 0.5); // round off to the nearest integer
        if(intensity_s[i] > GRAY_LEVEL) intensity_s[i] = GRAY_LEVEL;
    }
    // rescale each pixel's gray level
    image_processed = vector<vector<int>>(row);
    for (int i = 0; i < row; ++i) {
        image_processed[i] = vector<int>(column);
        for (int j = 0; j < column; ++j) {
            image_processed[i][j] = intensity_s[image[i][j]];
        }
    }
    // statistic number of each gray level
    intensity_processed = vector<int>(GRAY_LEVEL+1, 0);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < column; ++j){
            double p = image_processed[i][j];
            if(p < 1) {continue;}
            intensity_processed[p] += 1;
        }
    }
    processed = true;
}
// output original intensity graph
void ImageProcessing::OriginalIntensityGraph() const{
    ofstream file_output;
    string original_intensity_path = string(inputPath.substr(0, inputPath.size()-4) + "_intensity.pgm");
    vector<vector<int>> original_intensity_graph = vector<vector<int>>(h);
    for(int i = 0; i < h; ++i){
        original_intensity_graph[i] = vector<int>(w, 1);
    }
    double max = *max_element(intensity.begin(), intensity.end());
    for(int i = 0; i < scale; ++i){
        for(int j = 0; j < intensity[i]/max*h; ++j){
            original_intensity_graph[h-j-1][4*i] = 4;
            original_intensity_graph[h-j-1][4*i+1] = 4;
        }
    }
    file_output.open(original_intensity_path);
    file_output << "P2\n" << comment << w << " " << h << endl << 7 << endl;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            file_output << original_intensity_graph[i][j] << " ";
        }
        file_output << endl;
    }
    file_output.close();
}

// output processed intensity graph
void ImageProcessing::ProcessedIntensityGraph() const{
    ofstream file_output;
    string processed_intensity_path = string(outputPath.substr(0, outputPath.size()-4) + "_intensity.pgm");
    vector<vector<int>> processed_intensity_graph = vector<vector<int>>(h);
    for(int i = 0; i < h; ++i){
        processed_intensity_graph[i] = vector<int>(w, 1);
    }
    double max = *max_element(intensity_processed.begin(), intensity_processed.end());
    for(int i = 0; i < scale_processed; ++i){
        for(int j = 0; j < intensity_processed[i]/max*h; ++j){
            processed_intensity_graph[h-j-1][4*i] = 4;
            processed_intensity_graph[h-j-1][4*i+1] = 4;
        }
    }
    file_output.open(processed_intensity_path);
    file_output << "P2\n" << comment << w << " " << h << endl << 7 << endl;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            file_output << processed_intensity_graph[i][j] << " ";
        }
        file_output << endl;
    }
    file_output.close();
}
// output intensity trasfer function graph
void ImageProcessing::IntensityFuncGraph() const{
    ofstream file_output;
    string functionPath = string(outputPath.substr(0, outputPath.size()-4) + "_intensity_func.pgm");
    vector<vector<int>> intensity_function_graph = vector<vector<int>>(h);
    for(int i = 0; i < h; ++i){
        intensity_function_graph[i] = vector<int>(w,7);
    }
    for(int i = 0; i < scale_processed; ++i){
        int j = (double)intensity_s[i]/scale_processed*h - 1;
        if(j < 0) j = 0;
        intensity_function_graph[h-j-1][4*i] = 0;
        intensity_function_graph[h-j-1][4*i+1] = 0;
        intensity_function_graph[h-j-1][4*i+2] = 0;
        intensity_function_graph[h-j-1][4*i+3] = 0;
    }
    file_output.open(functionPath);
    file_output << "P2\n" << comment << w << " " << h << endl << 7 << endl;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            file_output << intensity_function_graph[i][j] << " ";
        }
        file_output << endl;
    }
    file_output.close();
}

void ImageProcessing::setCol(int col) {
    if (col <= 0) {
        cout << "Invalid value of column.\n";
    }
    else {
        column = col;
    }
}
void ImageProcessing::setRow(int row) {
    if (row <= 0) {
        cout << "Invalid value of row.\n";
    }
    else {
        this->row = row;
    }
}
void ImageProcessing::setScale(int scale) {
    if (scale <= 0 || scale > 255) {
        cout << "Invalid value of scale.\n";
    }
    else {
        this->scale = scale;
    }
}

// define P2 P5
int ImageProcessing::P2 = 0;
int ImageProcessing::P5 = 1;
