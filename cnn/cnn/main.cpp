#include "convolution.hpp"
#include "test.hpp"
#include <iostream>

#define TEST_BATCH_SIZE 100

int main(int argc, char *argv[]) {

    // Test our event pool.
    test::runEventPoolTest();

    if (argc != 3 && argc != 4) {
        std::cout << "Usage: cnn <xml> <result> [xclbin]" << std::endl;
        exit(-1);
    }

    CNN *cnn;

    std::string xmlFile(argv[1]);
    std::string testFile(argv[2]);

    if (argc == 4) {
        std::string xclbinFile(argv[3]);
        cnn = new CNN(xmlFile, true, xclbinFile);
    }
    else {
        cnn = new CNN(xmlFile, true);
    }

    cnn::vec in(cnn->getInSize());
    for (int i = 0; i < in.size(); ++i) {
        in[i] = (float)rand() / (float)RAND_MAX - 0.5f;
    }

    std::ofstream o(testFile.c_str());
    if (!o.is_open()) {
        std::cerr << "Can't open file " << testFile << std::endl;
        exit(-1);
    }
    o << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    writeXMLOpenTag(o, "results");

    cnn::vec inBatch(cnn->getInSize() * TEST_BATCH_SIZE);
    for (int i = 0; i < in.size(); ++i) {
        in[i] = (float)rand() / (float)RAND_MAX - 0.5f;
    }

    test::runFuncTest(cnn, in);
    test::runTimeTest(o, cnn, in);
    test::runTimeTestBatch(o, cnn, inBatch, TEST_BATCH_SIZE);
    delete cnn;

    // Do the same test for pipelined cnn;
    CNN *cnnPipelined;
    if (argc == 4) {
        std::string xclbinFile(argv[3]);
        cnnPipelined = new CNN(xmlFile, false, xclbinFile);
    }
    else {
        cnnPipelined = new CNN(xmlFile, false);
    }

    test::runFuncTest(cnnPipelined, in);
    test::runTimeTestPipeline(o, cnnPipelined, inBatch, TEST_BATCH_SIZE);

    delete cnnPipelined;

    //test::runTimeTestPipeline(o, cnnPipelined, inBatch, TEST_BATCH_SIZE);
    //test::runFuncTestPipelined(cnn, cnnPipelined, inBatch, TEST_BATCH_SIZE);

    writeXMLCloseTag(o, "results");
    o.close();

    return 0;
}
