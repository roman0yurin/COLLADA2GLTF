#pragma once

#include "COLLADA2GLTFWriter.h"
#include "gtest/gtest.h"

namespace {
  class COLLADA2GLTFWriterTest : public ::testing::Test {
  public:
	  std::shared_ptr<COLLADA2GLTF::Writer> writer;
	  COLLADA2GLTF::Options options;
	  std::shared_ptr<COLLADA2GLTF::ExtrasHandler> extrasHandler;
	  std::shared_ptr<GLTF::Asset> asset;

	  COLLADA2GLTFWriterTest();
	  ~COLLADA2GLTFWriterTest();
  };
}
