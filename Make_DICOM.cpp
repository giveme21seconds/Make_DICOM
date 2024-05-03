// DICOMTEST.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#include <iostream>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/libi2d/i2d.h"
#include "dcmtk/dcmdata/libi2d/i2djpgs.h"
#include "dcmtk/dcmdata/libi2d/i2dplsc.h"
#include <dcmtk/dcmimgle/dcmimage.h>
#include "dcmtk/dcmdata/dctk.h"
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

namespace fs = std::filesystem;

using namespace cv;
#include <iostream>
#include <filesystem>
#include <string>

#include <cstdlib>


//void createDicomImage(const std::string& jpegFileName) {
//    char uid[100];
//    I2DImgSource* inputPlug = new I2DJpegSource();
//    I2DOutputPlug* outPlug = new I2DOutputPlugSC();
//    Image2Dcm i2d;
//    E_TransferSyntax writeXfer;
//    DcmDataset* resultDset = NULL;
//
//    inputPlug->setImageFile(jpegFileName.c_str());
//    i2d.convert(inputPlug, outPlug, resultDset, writeXfer);
//    resultDset->putAndInsertString(DCM_PatientName, "Brandon tiff");
//    resultDset->putAndInsertString(DCM_SOPClassUID, UID_SecondaryCaptureImageStorage);
//    resultDset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT));
//    std::string dicomFileName = fs::path(jpegFileName).stem().string() + ".dcm";
//    DcmFileFormat dcmff(resultDset);
//    dcmff.saveFile(dicomFileName.c_str(), writeXfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);
//}
void createDicomImage(const fs::path& jpegPath, const fs::path& outputDicomPath) {
    char uid[100];
    I2DImgSource* inputPlug = new I2DJpegSource();
    I2DOutputPlug* outPlug = new I2DOutputPlugSC();
    Image2Dcm i2d;
    E_TransferSyntax writeXfer = EXS_LittleEndianExplicit;
    DcmDataset* resultDset = NULL;

    inputPlug->setImageFile(jpegPath.string().c_str());

    i2d.convert(inputPlug, outPlug, resultDset, writeXfer);
    resultDset->putAndInsertString(DCM_PatientName, "Brandon tiff");
    resultDset->putAndInsertString(DCM_SOPClassUID, UID_SecondaryCaptureImageStorage);
    resultDset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT));

    fs::path outputDicomFile = outputDicomPath / (jpegPath.stem().string() + ".dcm");
    DcmFileFormat dcmff(resultDset);
    dcmff.saveFile(outputDicomFile.string().c_str(), writeXfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFFalse, OFstatic_cast(Uint32, 0), EWM_fileformat);

    delete inputPlug;
    delete outPlug;
    delete resultDset;
}
void loadDicom() {
    DcmFileFormat fileformat;
    OFCondition status = fileformat.loadFile("test.dcm");
    if (status.good())
    {
        OFString patientName;
        if (fileformat.getDataset()->findAndGetOFString(DCM_PatientName, patientName).good())
        {
            cout << "Patient's Name: " << patientName << endl;
        }
        else
            cerr << "Error: cannot access Patient's Name!" << endl;
    }
    else
        cerr << "Error: cannot read DICOM file (" << status.text() << ")" << endl;
}
bool tiffToJpeg(const std::string& inputFilename, const std::string& outputFilename, int jpegQuality = 75) {
    // TIFF-Bild laden
    cv::Mat tiffImage = cv::imread(inputFilename, cv::IMREAD_UNCHANGED);
    if (tiffImage.empty()) {
        std::cerr << "Fehler beim Laden des TIFF-Bildes." << std::endl;
        return false;
    }

    // JPEG-Bild speichern
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(jpegQuality);
    bool result = cv::imwrite(outputFilename, tiffImage, compression_params);
    if (!result) {
        std::cerr << "Fehler beim Speichern des JPEG-Bildes." << std::endl;
        return false;
    }

    std::cout << "Konvertierung von TIFF zu JPEG erfolgreich abgeschlossen." << std::endl;
    return true;
}
bool convertTiffToPng(const std::string& tiffFilename, const std::string& pngFilename) {
    // TIFF-Bild laden
    cv::Mat tiffImage = cv::imread(tiffFilename, cv::IMREAD_UNCHANGED);
    if (tiffImage.empty()) {
        std::cerr << "Fehler beim Laden des TIFF-Bildes." << std::endl;
        return false;
    }

    // PNG-Bild speichern
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0); // 0 bedeutet verlustfreie Kompression
    bool result = cv::imwrite(pngFilename, tiffImage, compression_params);
    if (!result) {
        std::cerr << "Fehler beim Speichern des PNG-Bildes." << std::endl;
        return false;
    }

    std::cout << "Konvertierung von TIFF zu PNG erfolgreich abgeschlossen." << std::endl;
    return true;
}
void tiffToPng(const std::string& tiffFilePath, const std::string& pngFilePath) {
    // Read the TIFF image
    cv::Mat tiff_image = cv::imread(tiffFilePath, cv::IMREAD_UNCHANGED);

    // Check if the image was loaded successfully
    if (tiff_image.empty()) {
        std::cout << "Could not open or find the image: " << tiffFilePath << std::endl;
        return;
    }

    // Convert the TIFF image to PNG
    cv::Mat png_image;
    cv::cvtColor(tiff_image, png_image, cv::COLOR_BGR2RGB);

    // Save the PNG image
    cv::imwrite(pngFilePath, png_image);
}
bool convertTiffToDicom(const std::string& tiffFilename, const std::string& dicomFilename) {
    // TIFF-Bild laden
    cv::Mat tiffImage = cv::imread(tiffFilename, cv::IMREAD_UNCHANGED);
    if (tiffImage.empty()) {
        std::cerr << "Fehler beim Laden des TIFF-Bildes." << std::endl;
        return false;
    }

    // DICOM-Dataset erstellen
    DcmFileFormat dcmFileFormat;
    DcmDataset* dcmDataset = dcmFileFormat.getDataset();

    // Größe des Bildes setzen
    dcmDataset->putAndInsertUint16(DCM_Rows, tiffImage.rows);
    dcmDataset->putAndInsertUint16(DCM_Columns, tiffImage.cols);

    // Pixelwerte setzen
    DcmElement* pixelData = nullptr;
    dcmDataset->findAndGetElement(DCM_PixelData, pixelData);
    if (!pixelData) {
        pixelData = new DcmPixelData(DCM_PixelData);
        dcmDataset->insert(pixelData, true);
    }
    pixelData->putUint16Array((Uint16*)tiffImage.data, tiffImage.rows * tiffImage.cols);

    // Zusätzliche Metadaten hinzufügen (Beispiel)
    dcmDataset->putAndInsertString(DCM_PatientName, "John Doe");
    dcmDataset->putAndInsertString(DCM_PatientID, "12345");

    // DICOM-Bild speichern
    OFCondition status = dcmFileFormat.saveFile(dicomFilename.c_str(), EXS_LittleEndianExplicit);
    if (status.good()) {
        std::cout << "Konvertierung von TIFF zu DICOM erfolgreich abgeschlossen." << std::endl;
        return true;
    }
    else {
        std::cerr << "Fehler beim Speichern des DICOM-Bildes: " << status.text() << std::endl;
        return false;
    }
}
void tiffToBmp(const string& inputPath, const string& outputPath) {
    // Lade das TIFF-Bild
    Mat image = imread(inputPath, IMREAD_UNCHANGED);

    // Überprüfen, ob das Bild erfolgreich geladen wurde
    if (image.empty()) {
        cerr << "Fehler beim Laden der Eingabedatei: " << inputPath << endl;
        return;
    }

    // Speichere das Bild als BMP
    imwrite(outputPath, image);

    cout << "Erfolgreich konvertiert: " << inputPath << " -> " << outputPath << endl;
}
void bmpToDicom(const string& inputPath, const string& outputPath) {
    // Lade das BMP-Bild
    Mat image = imread(inputPath, IMREAD_UNCHANGED);

    // Überprüfen, ob das Bild erfolgreich geladen wurde
    if (image.empty()) {
        cerr << "Fehler beim Laden der Eingabedatei: " << inputPath << endl;
        return;
    }

    // DICOM-Datei erstellen
    DcmFileFormat dcmFileFormat;

    // DICOM-Dataset holen
    DcmDataset* dcmDataset = dcmFileFormat.getDataset();

    // Einstellungen für das Pixel-Format
    dcmDataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
    dcmDataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
    dcmDataset->putAndInsertUint16(DCM_BitsAllocated, 8);
    dcmDataset->putAndInsertUint16(DCM_BitsStored, 8);
    dcmDataset->putAndInsertUint16(DCM_HighBit, 7);
    dcmDataset->putAndInsertUint16(DCM_PixelRepresentation, 0); // unsigned integer

    // Bilddaten in das DICOM-Dataset einfügen
    Uint8* pixelArray = new Uint8[image.rows * image.cols];
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            pixelArray[i * image.cols + j] = image.at<uchar>(i, j);
        }
    }

    // Größe des Pixel-Elements setzen
    dcmDataset->putAndInsertUint32(DCM_Rows, image.rows);
    dcmDataset->putAndInsertUint32(DCM_Columns, image.cols);
    dcmDataset->putAndInsertUint32(DCM_PixelData, image.rows * image.cols);

    // Bilddaten ins DICOM-Dataset einfügen
    if (dcmDataset->putAndInsertUint8Array(DCM_PixelData, pixelArray, image.rows * image.cols).bad()) {
        cerr << "Fehler beim Einfügen der Bilddaten in das DICOM-Dataset." << endl;
        delete[] pixelArray;
        return;
    }

    delete[] pixelArray;

    // Speichern Sie das DICOM-Dataset in einer Datei
    if (dcmFileFormat.saveFile(outputPath.c_str(), EXS_LittleEndianExplicit).bad()) {
        cerr << "Fehler beim Speichern der DICOM-Datei: " << outputPath << endl;
        return;
    }

    cout << "Erfolgreich konvertiert: " << inputPath << " -> " << outputPath << endl;
}
// Function to convert PNG images to DICOM file
//bool pngToDicom(const vector<string>& pngFiles, const string& outputDicomFile) {
//    // Initialize DCMTK
//    DcmFileFormat fileformat;
//    OFCondition status;
//
//    // Iterate through PNG files
//    for (const auto& pngFile : pngFiles) {
//        // Load PNG image
//        Mat img = imread(pngFile, IMREAD_GRAYSCALE);
//        if (img.empty()) {
//            cerr << "Error: Couldn't load image " << pngFile << endl;
//            return false;
//        }
//
//        // Convert Mat image to DICOM image
//        DicomImage dicomImg(img.data, img.cols, img.rows, 8 /* bits per pixel */);
//        if (!dicomImg.isMonochrome()) {
//            cerr << "Error: Image is not monochrome" << endl;
//            return false;
//        }
//
//        // Add DICOM image to DICOM file
//        DcmDataset* dataset = fileformat.getDataset();
//        status = dicomImg.writeDataset(*dataset);
//        if (!status.good()) {
//            cerr << "Error: Failed to write DICOM dataset" << endl;
//            return false;
//        }
//    }
//
//    // Save DICOM file
//    status = fileformat.saveFile(outputDicomFile.c_str());
//    if (status.good()) {
//        cout << "DICOM file saved successfully." << endl;
//        return true;
//    }
//    else {
//        cerr << "Error: Failed to save DICOM file" << endl;
//        return false;
//    }
//}
//
void mergeDicomFiles(const string& inputDir, const string& outputFile) {
    vector<string> inputFiles;

    // Iterate through files in the input directory
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.is_regular_file()) {
            inputFiles.push_back(entry.path().string());
        }
    }

    if (inputFiles.empty()) {
        cerr << "Error: No DICOM files found in directory " << inputDir << endl;
        return;
    }

    DcmFileFormat fileFormat;
    OFCondition status;

    // Read the first DICOM file
    status = fileFormat.loadFile(inputFiles[0].c_str());
    if (!status.good()) {
        cerr << "Error: Unable to read DICOM file " << inputFiles[0] << endl;
        return;
    }

    // Get study and series instance UID
    OFString studyInstanceUID;
    OFString seriesInstanceUID;
    fileFormat.getDataset()->findAndGetOFString(DCM_StudyInstanceUID, studyInstanceUID);
    fileFormat.getDataset()->findAndGetOFString(DCM_SeriesInstanceUID, seriesInstanceUID);

    // Loop through input files (except the first one)
    for (size_t i = 1; i < inputFiles.size(); ++i) {
        DcmFileFormat tempFormat;
        status = tempFormat.loadFile(inputFiles[i].c_str());
        if (!status.good()) {
            cerr << "Error: Unable to read DICOM file " << inputFiles[i] << endl;
            continue;
        }

        // Ensure study and series instance UID match
        OFString tempStudyInstanceUID;
        OFString tempSeriesInstanceUID;
        tempFormat.getDataset()->findAndGetOFString(DCM_StudyInstanceUID, tempStudyInstanceUID);
        tempFormat.getDataset()->findAndGetOFString(DCM_SeriesInstanceUID, tempSeriesInstanceUID);
        if (studyInstanceUID != tempStudyInstanceUID || seriesInstanceUID != tempSeriesInstanceUID) {
            cerr << "Error: Study or series instance UID mismatch among DICOM files" << endl;
            continue;
        }

        // Merge datasets
        DcmDataset* dataset = tempFormat.getDataset();
        status = fileFormat.getDataset()->copyFrom(*dataset);
        if (!status.good()) {
            cerr << "Error: Failed to merge DICOM datasets" << endl;
            continue;
        }
    }

    // Save merged DICOM file
    status = fileFormat.saveFile(outputFile.c_str());
    if (!status.good()) {
        cerr << "Error: Failed to save merged DICOM file " << outputFile << endl;
        return;
    }

    cout << "DICOM files merged successfully. Merged file saved as " << outputFile << endl;
}
struct Vox1996AHeader {
    int width;
    int height;
    int depth;
};

bool readVox1996AHeader(const std::string& filename, Vox1996AHeader& header) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    // Read header
    file.read(reinterpret_cast<char*>(&header), sizeof(Vox1996AHeader));
    return true;
}

bool readVox1996APixels(const std::string& filename, std::vector<unsigned char>& pixels, const Vox1996AHeader& header) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    // Skip the header
    file.seekg(sizeof(Vox1996AHeader), std::ios::beg);

    // Calculate the size of the pixel data
    std::size_t pixelDataSize = header.width * header.height * header.depth * 2; // Assuming 16-bit depth

    // Read pixel data
    pixels.resize(pixelDataSize);
    file.read(reinterpret_cast<char*>(pixels.data()), pixelDataSize);

    return true;
}

// Funktion zum Lesen des Headers einer VOX1996A-Datei
Vox1996AHeader readVox1996AHeader(const std::string& voxFilename) {
    Vox1996AHeader header;
    std::ifstream file(voxFilename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open VOX1996A file " << voxFilename << std::endl;
        header.width = header.height = header.depth = 0;
        return header;
    }

    // Lesen des Headers
    file.read(reinterpret_cast<char*>(&header), sizeof(Vox1996AHeader));

    return header;
}

// Funktion zum Lesen der Pixel aus einer VOX1996A-Datei
std::vector<short> readVox1996APixelData(const std::string& voxFilename, const Vox1996AHeader& header) {
    std::ifstream file(voxFilename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open VOX1996A file " << voxFilename << std::endl;
        return {};
    }

    // Überspringe den Header
    file.seekg(sizeof(Vox1996AHeader), std::ios::beg);

    // Lese die Pixel-Daten
    std::vector<short> pixelData(header.width * header.height * header.depth);
    file.read(reinterpret_cast<char*>(pixelData.data()), pixelData.size());

    std::cout << "PixelData Size: " << pixelData.size() << std::endl;

    return pixelData;
}

// Funktion zum Erstellen einer DICOM-Datei
void createDicomFile(const std::string& dicomFilename, const std::vector<short>& pixelData, const Vox1996AHeader& header) {
    // Initialisierung des DCM Toolkit
    DcmFileFormat fileformat;
    OFCondition status;
    E_TransferSyntax writeXfer = EXS_BigEndianExplicit;

    // Erstellen eines leeren DICOM-Bildes
    DcmDataset* dataset = fileformat.getDataset();
    char uid[100];
    // Setzen der erforderlichen DICOM-Attribute
    dataset->putAndInsertString(DCM_PatientName, "Brandon tiff");
    dataset->putAndInsertString(DCM_SOPClassUID, UID_SecondaryCaptureImageStorage);
    dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT));
    dataset->putAndInsertString(DCM_PatientID, "123456");
    dataset->putAndInsertString(DCM_PixelSpacing, "1\\1");
    dataset->putAndInsertUint16(DCM_Rows, header.height);
    dataset->putAndInsertUint16(DCM_Columns, header.width);
    dataset->putAndInsertUint16(DCM_NumberOfFrames, header.depth);
    dataset->putAndInsertUint16(DCM_BitsAllocated, 16);
    dataset->putAndInsertUint16(DCM_BitsStored, 16);
    dataset->putAndInsertUint16(DCM_HighBit, 15);
    dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);
    dataset->putAndInsertString(DCM_Modality, "CT");
    dataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
    dataset->putAndInsertUint16(DCM_PlanarConfiguration, 0);
    //dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");

    // Pixel-Daten setzen^^ 
    status = dataset->putAndInsertUint16Array(DCM_PixelData, reinterpret_cast<const Uint16*>(pixelData.data()), pixelData.size() / sizeof(Uint16));
    if (status.bad()) {
        std::cerr << "Error: Failed to set pixel data" << std::endl;
        return;
    }

    // Speichern der DICOM-Datei
    status = fileformat.saveFile(dicomFilename.c_str(), EXS_BigEndianExplicit);
    if (status.bad()) {
        std::cerr << "Error: Failed to save DICOM file" << std::endl;
        return;
    }

    std::cout << "DICOM file created: " << dicomFilename << std::endl;
}




int main()
{

    // Beispielbildgröße
    const int width = 512;
    const int height = 512;
    const int depth = 64; // Anzahl der Slices

    std::string voxFilename = "CT_20231219_095459.VOX";
    std::string dicomFilename = "VOXtoDICOM.dcm";

    // Header aus der VOX1996A-Datei lesen
    ////Vox1996AHeader header = readVox1996AHeader(voxFilename);
    //if (header.width == 0 || header.height == 0 || header.depth == 0) {
    //    return 1;
    //}

    Vox1996AHeader header;
    header.width = width;
    header.height = height;
    header.depth = depth;

    // Pixel aus der VOX1996A-Datei lesen
    //std::vector<short> pixelData = readVox1996APixelData(voxFilename, header);
     // Beispielbild: verschiedene Grauwerte und Strukturen
    std::vector<short> pixelData(width * height * depth);
    for (int z = 0; z < depth; ++z) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Setze den Pixelwert basierend auf der x-Koordinate
                if (x < width / 2) {
                    pixelData[z * width * height + y * width + x] = 1000; // Weiß auf der linken Seite
                }
                else {
                    pixelData[z * width * height + y * width + x] = 0; // Schwarz auf der rechten Seite
                }
            }
        }
    }
    // DICOM-Datei erstellen und speichern
    createDicomFile(dicomFilename, pixelData, header);




    /*string inputPath = "test2.tiff";
    string outputPath = "output.bmp";

    string inputPath = "output.bmp";
    string outputPath = "output.dcm";*/

    //int exitCode = system("py LosslessJpegCompression.py");

    // Überprüfe, ob der Aufruf erfolgreich war
   /* if (exitCode == 0) {
        std::cout << "Python-Skript erfolgreich ausgeführt." << std::endl;
    }
    else {
        std::cerr << "Fehler beim Ausführen des Python-Skripts." << std::endl;
    }*/




    std::string folderPath = "C:/Users/Nutzer/Documents/DICOM ToolKit/DICOMTEST/jpeg_output"; // Setzen Sie den Pfad zu Ihrem Ordner mit den JPEG-Dateien hier ein
    fs::path inputFolder = "jpeg_output"; // Set your input folder here
    fs::path outputDicomFolder = "output_dicom"; // Set your output DICOM folder here

    //if (!fs::exists(outputDicomFolder))
    //    fs::create_directory(outputDicomFolder);


    //for (const auto& entry : fs::directory_iterator(inputFolder)) {
    //    if (entry.is_regular_file() && entry.path().extension() == ".jpeg") {
    //        createDicomImage(entry.path(), outputDicomFolder);
    //       
    //    }
    //}

    //loadDicom();
    //string outputFile = "merged_output.dcm";
    //string inputDir = "output_dicom";
    //mergeDicomFiles(inputDir, outputFile);

    // Konvertiere TIFF zu BMP
   // tiffToBmp(inputPath, outputPath);

    // Konvertiere BMP zu DICOM
    //bmpToDicom(inputPath, outputPath);

    return 0;

    //    std::string tiffFilename = "test2.tiff";
    //    std::string dicomFilename = "output.dcm";
    //    if (convertTiffToDicom(tiffFilename, dicomFilename)) {
    //        std::cout << "Konvertierung erfolgreich." << std::endl;
    //    }
    //    else {
    //        std::cerr << "Fehler bei der Konvertierung." << std::endl;
    //    }
    //   /* tiffToPng("test2.tiff", "image.png");
    //    createDicomImage();
    //    loadDicom();*/
    //    return 0;
    //    /*std::string tiffFilename = "test2.tif";
    //    std::string pngFilename = "output.png";
    //    if (convertTiffToPng(tiffFilename, pngFilename)) {
    //        std::cout << "Konvertierung erfolgreich." << std::endl;
    //    }
    //    else {
    //        std::cerr << "Fehler bei der Konvertierung." << std::endl;
    //    }*/
    //
    //
    //
    //
    //    int jpegQuality = 75;
    //   /* if (tiffToJpeg(inputFilename, outputFilename, jpegQuality)) {
    //        std::cout << "Konvertierung erfolgreich." << std::endl;
    //    }
    //    else {
    //        std::cerr << "Fehler bei der Konvertierung." << std::endl;
    //    }*/
    //   
    ////    // Dateinamen für das TIFF-Bild und die zu speichernde DICOM-Datei
    ////    string tiffFileName = "CT_20231219_095459.VOX";
    ////    string dicomFileName = "output_dicom.dcm";
    ////
    ////    // TIFF-Bild laden
    ////    Mat image = imread(tiffFileName, IMREAD_GRAYSCALE);
    ////    if (image.empty()) {
    ////        cerr << "Fehler beim Laden des TIFF-Bildes." << endl;
    ////        return 1;
    //    }
    //
    //    // DICOM-Datensatz erstellen
    //    DcmFileFormat fileformat;
    //    DcmDataset* dataset = fileformat.getDataset();
    //
    //    if (!dataset) {
    //        cerr << "Fehler beim Erstellen des DICOM-Datensatzes." << endl;
    //        return 1;
    //    }
    //
    //    // TIFF-Bild in Pixel-Data-Element umwandeln und in den DICOM-Datensatz einfügen
    //    vector<unsigned char> pixelData;
    //    pixelData.assign(image.data, image.data + image.total());
    //
    //    dataset->putAndInsertUint8Array(DCM_PixelData, &pixelData[0], pixelData.size());
    //
    //    // Attribute für Breite, Höhe, Bits pro Sample usw. festlegen
    //    dataset->putAndInsertUint16(DCM_Columns, image.cols);
    //    dataset->putAndInsertUint16(DCM_Rows, image.rows);
    //    dataset->putAndInsertUint16(DCM_BitsAllocated, 8);
    //    dataset->putAndInsertUint16(DCM_BitsStored, 8);
    //    dataset->putAndInsertUint16(DCM_HighBit, 7);
    //    dataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
    //    dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
    //
    //    // Patientennamen ändern
    //    string patientName = "Max Mustermann";
    //    dataset->putAndInsertString(DCM_PatientName, patientName.c_str());
    //
    //    // DICOM-Datei speichern
    //    if (fileformat.saveFile(dicomFileName.c_str(), EXS_LittleEndianExplicit).bad()) {
    //        cerr << "Fehler beim Speichern der DICOM-Datei." << endl;
    //        return 1;
    //    }
    //
    //    cout << "DICOM-Datei erfolgreich gespeichert: " << dicomFileName << endl;
    //
    //    return 0;
    ////
    //    std::cout << "Hello World!\n";
    //    char uid[100];
    //    I2DImgSource* inputPlug = new I2DJpegSource();
    //    I2DOutputPlug* outPlug = new I2DOutputPlugSC();
    //    Image2Dcm i2d;
    //    E_TransferSyntax writeXfer;
    //    DcmDataset* resultDset = NULL;
    //
    //    inputPlug->setImageFile("test2.tiff");
    //    i2d.convert(inputPlug, outPlug, resultDset, writeXfer);
    //
    //    resultDset->putAndInsertString(DCM_PatientName, "Brandon Tiff");
    //    resultDset->putAndInsertString(DCM_SOPClassUID, UID_SecondaryCaptureImageStorage);
    //    resultDset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT));
    //
    //    DcmFileFormat dcmff(resultDset);
    //dcmff.saveFile("test2.dcm", writeXfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);
    //
    //

}
//


// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
