#include <precice/SolverInterface.hpp>

#include <algorithm>
#include <benchmark/benchmark.h>
#include <iostream>
#include <random>
#include <vector>

static void shuffle(std::vector<int> &v) {
  static std::random_device rd;
  static std::mt19937 g(rd());
  std::shuffle(v.begin(), v.end(), g);
}

std::vector<int> SetupMesh(precice::SolverInterface &p, int n) {
  std::vector<int> vids(n);
  std::vector<double> pos(n * 3);
  std::generate(pos.begin(), pos.end(), []() -> double {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 100.0);
    return dis(gen);
  });
  p.setMeshVertices(p.getMeshID("MeshA"), n, pos.data(), vids.data());
  return vids;
}

#define SETUP_DATA(DIM, NAME)                                                  \
  const int nv = 1000000;                                                      \
  static precice::SolverInterface p("A", "precice.xml", 0, 1);                 \
  static std::vector<int> vids;                                                \
  static bool skip = false;                                                    \
  if (!skip) {                                                                 \
    skip = true;                                                               \
    vids = SetupMesh(p, nv);                                                   \
  }                                                                            \
  std::vector<double> data(nv *DIM,                                            \
                           3.14159); // enough for scalar and vector data\
  // shuffle(vids); // for random access;

static void BM_writeBlockVector(benchmark::State &state) {

  SETUP_DATA(3, "Vector")
  const auto mid = p.getMeshID("MeshA");
  const auto did = p.getDataID("Vector", mid);

  for (auto _ : state) {
    // for(unsigned int i=0;i<nv;++i)
    // p.writeVectorData(did, vids[i], data.data());
    p.writeBlockVectorData(did, nv, vids.data(), data.data());
  }
}

static void BM_writeBlockVectorString(benchmark::State &state) {

  SETUP_DATA(3, "Vector")

  for (auto _ : state) {
    const auto mid = p.getMeshID("MeshA");
    const auto did = p.getDataID("Vector", mid);
    p.writeBlockVectorData(did, nv, vids.data(), data.data());
  }
}

static void BM_writeVectorData(benchmark::State &state) {

  SETUP_DATA(3, "Vector")

  const auto mid = p.getMeshID("MeshA");
  const auto did = p.getDataID("Vector", mid);

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i)
      p.writeVectorData(did, vids[i], data.data());
  }
}

static void BM_writeVectorDataString(benchmark::State &state) {

  SETUP_DATA(3, "Vector")

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i) {
      const auto mid = p.getMeshID("MeshA");
      const auto did = p.getDataID("Vector", mid);
      p.writeVectorData(did, vids[i], data.data());
    }
  }
}

static void BM_writeBlockScalar(benchmark::State &state) {

  SETUP_DATA(1, "Scalar")

  const auto mid = p.getMeshID("MeshA");
  const auto did = p.getDataID("Scalar", mid);

  for (auto _ : state) {
    // for(unsigned int i=0;i<nv;++i)
    // p.writeVectorData(did, vids[i], data.data());
    p.writeBlockScalarData(did, nv, vids.data(), data.data());
  }
}

static void BM_writeBlockScalarString(benchmark::State &state) {

  SETUP_DATA(1, "Scalar")

  for (auto _ : state) {
    {
      const auto mid = p.getMeshID("MeshA");
      const auto did = p.getDataID("Scalar", mid);
      p.writeBlockScalarData(did, nv, vids.data(), data.data());
    }
  }
}

static void BM_writeScalarData(benchmark::State &state) {

  SETUP_DATA(1, "Scalar")

  const auto mid = p.getMeshID("MeshA");
  const auto did = p.getDataID("Scalar", mid);

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i)
      p.writeScalarData(did, vids[i], data[i]);
  }
}

static void BM_writeScalarDataString(benchmark::State &state) {

  SETUP_DATA(1, "Scalar")

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i) {
      const auto mid = p.getMeshID("MeshA");
      const auto did = p.getDataID("Scalar", mid);
      p.writeScalarData(did, vids[i], data[i]);
    }
  }
}

static void BM_writeScalarDataSingleFunction(benchmark::State &state) {

  SETUP_DATA(1, "Scalar")

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i) {
      p.writeScalarDataString("MeshA", "Scalar", vids[i], data[i]);
    }
  }
}

// Register the function as a benchmark
// BENCHMARK(BM_writeBlockVector);
// BENCHMARK(BM_writeBlockVectorString);
// BENCHMARK(BM_writeVectorData);
// BENCHMARK(BM_writeVectorDataString);
// BENCHMARK(BM_writeBlockScalar);
// BENCHMARK(BM_writeBlockScalarString);
BENCHMARK(BM_writeScalarData);
BENCHMARK(BM_writeScalarDataString);
BENCHMARK(BM_writeScalarDataSingleFunction);