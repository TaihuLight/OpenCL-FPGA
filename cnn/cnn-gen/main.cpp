#include <random>
#include "util.hpp"

using namespace cnn;

void genConvolutionLayer(
    std::ofstream &o,
    const std::string &name,
    const std::string &kernelName,
    size_t iWidth,
    size_t iHeight,
    size_t iDepth,
    size_t oDepth,
    size_t kernelSize
    ) {

    writeXMLOpenTag(o, "ConvolutionalLayer");
    writeXMLTag(o, "name", name);
    writeXMLTag(o, "kernelName", kernelName);
    writeXMLTag(o, "iWidth", iWidth);
    writeXMLTag(o, "iHeight", iHeight);
    writeXMLTag(o, "iDepth", iDepth);
    writeXMLTag(o, "kernelSize", kernelSize);
    writeXMLTag(o, "oDepth", oDepth);

    // Randomly write the weight.
    writeXMLOpenTag(o, "weight");
    for (int i = 0; i < oDepth; ++i) {
        // For each output feature map.
        writeXMLOpenTag(o, "oFeatureMap");
        for (int j = 0; j < iDepth; ++j) {
            writeXMLOpenTag(o, "iFeatureMap");
            for (int k = 0; k < kernelSize; ++k) {
                writeXMLOpenTag(o, "line");
                for (int k = 0; k < kernelSize; ++k) {
                    writeXMLTag(o, "item", static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
                }
                writeXMLCloseTag(o, "line");
            }
            writeXMLCloseTag(o, "iFeatureMap");
        }
        writeXMLCloseTag(o, "oFeatureMap");
    }
    writeXMLCloseTag(o, "weight");

    // Randomly write the offset.
    writeXMLOpenTag(o, "offset");
    for (int i = 0; i < oDepth; ++i) {
        writeXMLTag(o, "item", static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    }
    writeXMLCloseTag(o, "offset");

    writeXMLCloseTag(o, "ConvolutionalLayer");
}


int main(int argc, char *argv[]) {

    std::string base = "../cnn/";
    std::string names[] = {
        "convolution1",
        "convolution2"
    };

    std::string kernelNames[] = {
        "convolution",
        "convolution"
    };

    std::ofstream o(base + "convolution.xml");
    if (!o.is_open()) {
        std::cerr << "Can't open file " << base + "convolution.xml" << std::endl;
        exit(-1);
    }

    o << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    writeXMLOpenTag(o, "layers");
    genConvolutionLayer(o, names[0], kernelNames[0], 32, 32, 1, 6, 5);
    genConvolutionLayer(o, names[1], kernelNames[1], 14, 14, 6, 16, 5);
    writeXMLCloseTag(o, "layers");
    o.close();

    return 0;
}