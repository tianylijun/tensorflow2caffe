#!/bin/bash
flatc -c flatbuffer_protocols/feather_simple.fbs
protoc --cpp_out=. ./tensorflow/core/framework/*.proto

g++ rokid_convert_tf.cc \
tensorflow/core/framework/allocation_description.pb.cc \
tensorflow/core/framework/api_def.pb.cc \
tensorflow/core/framework/attr_value.pb.cc \
tensorflow/core/framework/cost_graph.pb.cc \
tensorflow/core/framework/device_attributes.pb.cc \
tensorflow/core/framework/function.pb.cc \
tensorflow/core/framework/graph.pb.cc \
tensorflow/core/framework/graph_transfer_info.pb.cc \
tensorflow/core/framework/iterator.pb.cc \
tensorflow/core/framework/kernel_def.pb.cc \
tensorflow/core/framework/log_memory.pb.cc \
tensorflow/core/framework/node_def.pb.cc \
tensorflow/core/framework/op_def.pb.cc \
tensorflow/core/framework/reader_base.pb.cc \
tensorflow/core/framework/remote_fused_graph_execute_info.pb.cc \
tensorflow/core/framework/resource_handle.pb.cc \
tensorflow/core/framework/step_stats.pb.cc \
tensorflow/core/framework/summary.pb.cc \
tensorflow/core/framework/tensor_description.pb.cc \
tensorflow/core/framework/tensor.pb.cc \
tensorflow/core/framework/tensor_shape.pb.cc \
tensorflow/core/framework/tensor_slice.pb.cc \
tensorflow/core/framework/types.pb.cc \
tensorflow/core/framework/variable.pb.cc \
tensorflow/core/framework/versions.pb.cc \
 -I./ -I/usr/include `pkg-config --cflags --libs protobuf` -o rokid_convert_tf -std=c++11