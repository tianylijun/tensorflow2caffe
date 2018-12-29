#include "tensorflow/core/framework/graph.pb.h"
#include "feather_simple_generated.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <float.h>
#include <math.h>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#if 0
#define PRINTF printf
#else
#define PRINTF
#endif

using namespace tensorflow;
using namespace flatbuffers;

using google::protobuf::io::FileInputStream;
using google::protobuf::Message;

static uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
static uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
static std::map<std::string, float> int8scaleMap;

class TfModelWeightsConvert
{
public:
    TfModelWeightsConvert(std::string pb_model, std::string output_name);
    bool Convert();
    void SaveModelWeights(uint32_t fuseBN, uint32_t fractions, float threshold, uint32_t crypto);

private :
    bool ReadNetParam();

private :
    std::string pb_model;
    std::string output_name;
    GraphDef pb_graph;
};

TfModelWeightsConvert::TfModelWeightsConvert(std::string pb_model, std::string output_name)
{
    this->pb_model = pb_model;
    this->output_name = output_name;
}

bool TfModelWeightsConvert::Convert()
{
    if (!ReadNetParam())
    {
        std::cerr << "Read net params fail!" << std::endl;
        return false;
    }

    return true;
}

bool TfModelWeightsConvert::ReadNetParam()
{
    std::ifstream in(pb_model.c_str());
    if (!in)
    {
        std::cerr << "read caffe model weights file " << pb_model  <<" fail!" << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    if (!pb_graph.ParseFromString(std::string(buffer.str())))
    {
        std::cerr << "parse weights file " << pb_model  <<" fail!" << std::endl;
        return false;
    }

    in.close();
    return true;
}

void TfModelWeightsConvert::SaveModelWeights(uint32_t fuseBN, uint32_t frac, float threshold, uint32_t crypto)
{
    printf("\nNode Num: %d\n", pb_graph.node_size());
    for (int i = 0; i < pb_graph.node_size(); ++i)
    {
        auto nodedef = pb_graph.node(i);
        printf("---------------------------------\n");
        printf("node name: %s\n", nodedef.name().c_str());        
    }
}

int main(int argc, char *argv[])
{
    const char *pSerialFile = NULL;
    const char *pInt8ScaleFile = NULL;
    uint32_t fractions = 0, crypto = 0, fuseBN = 1;
    float threshold = 0.02f;
    if (argc < 3 || argc > 9)
    {
        printf("Usage: ./tf_model_convert $1(pb) [$3(output_model_name_prefix)] [$4(fuseBN)] [$4(fractions)] [$5(threshold)] [$6(crpty)] [$7(SNFile)] [$9(Int8ScaleFile)]\n");
        return -1;
    }
    std::string output_model_name = "out";
    std::string pb_model = argv[1];
    if (argc > 2) output_model_name = (argv[2]);
    if (argc > 3) fuseBN = atoi(argv[3]);
    if (argc > 4) fractions = atoi(argv[4]);
    if (argc > 5) threshold = atof(argv[5]);
    if (argc > 6) crypto = atoi(argv[6]);
    if (argc > 7) pSerialFile = argv[7];
    if (argc > 8) pInt8ScaleFile = argv[8];

    printf("%s pb model: %s featherCNN: %s fuseBN: %d fractions:%d threshold:%.3f crypto:%d SerialFile: %s Int8ScaleFile: %s\n", 
           argv[0], pb_model.c_str(), output_model_name.c_str(), fuseBN, fractions, threshold, crypto, pSerialFile, pInt8ScaleFile);

    TfModelWeightsConvert convert(pb_model, output_model_name);
    if (false == convert.Convert())
    {
        printf("Read file failed\n");
        return -2;
    }

    convert.SaveModelWeights(fuseBN, fractions, threshold, crypto);
    return 0;
}
