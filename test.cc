#include <precice/SolverInterface.hpp>

#include <algorithm>
#include <benchmark/benchmark.h>
#include <iostream>
#include <random>
#include <vector>

static void shuffle(std::vector<int> &v)
{
  static std::random_device rd;
  static std::mt19937       g(rd());
  std::shuffle(v.begin(), v.end(), g);
}

std::vector<int> SetupMesh(precice::SolverInterface &p, int n)
{
  std::vector<int>    vids(n);
  std::vector<double> pos(n * 3);
  std::generate(pos.begin(), pos.end(), []() -> double {
    static std::random_device               rd;
    static std::mt19937                     gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 100.0);
    return dis(gen);
  });
  p.setMeshVertices(p.getMeshID("MeshA"), n, pos.data(), vids.data());
  return vids;
}

#define SETUP_DATA(DIM, NAME)                                  \
  const int                       nv = 1000000;                \
  static precice::SolverInterface p("A", "precice.xml", 0, 1); \
  static std::vector<int>         vids;                        \
  static bool                     skip = false;                \
  if (!skip) {                                                 \
    skip = true;                                               \
    vids = SetupMesh(p, nv);                                   \
  }                                                            \
  std::vector<double> data(nv *DIM,                            \
                           3.14159); // enough for scalar and vector data\
  // shuffle(vids); // for random access;

static void BM_writeBlockVector(benchmark::State &state)
{

  SETUP_DATA(3, "Vector")
  const auto mid = p.getMeshID("MeshA");
  const auto did = p.getDataID("Vector", mid);

  for (auto _ : state) {
    p.writeBlockVectorData(did, nv, vids.data(), data.data());
  }
}

static void BM_writeBlockVectorStringCopy(benchmark::State &state)
{

  SETUP_DATA(3, "Vector")

  for (auto _ : state) {
    const auto mid = p.getMeshID("MeshA");
    const auto did = p.getDataID("Vector", mid);
    p.writeBlockVectorData(did, nv, vids.data(), data.data());
  }
}

static void BM_writeBlockVectorStringReference(benchmark::State &state)
{

  SETUP_DATA(3, "Vector")

  std::string mesha("MeshA");
  std::string vectorData("Vector");

  for (auto _ : state) {
    const auto mid = p.getMeshID(mesha);
    const auto did = p.getDataID(vectorData, mid);
    p.writeBlockVectorData(did, nv, vids.data(), data.data());
  }
}

static void BM_writeVectorData(benchmark::State &state)
{

  SETUP_DATA(3, "Vector")

  const auto mid = p.getMeshID("MeshA");
  const auto did = p.getDataID("Vector", mid);

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i)
      p.writeVectorData(did, vids[i], data.data());
  }
}

static void BM_writeVectorDataStringCopy(benchmark::State &state)
{

  SETUP_DATA(3, "Vector")

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i) {
      const auto mid = p.getMeshID("MeshA");
      const auto did = p.getDataID("Vector", mid);
      p.writeVectorData(did, vids[i], data.data());
    }
  }
}

static void BM_writeVectorDataStringReference(benchmark::State &state)
{

  SETUP_DATA(3, "Vector")

  std::string mesha("MeshA");
  std::string vectorData("Vector");

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i) {
      const auto mid = p.getMeshID(mesha);
      const auto did = p.getDataID(vectorData, mid);
      p.writeVectorData(did, vids[i], data.data());
    }
  }
}

static void BM_writeBlockScalar(benchmark::State &state)
{

  SETUP_DATA(1, "Scalar")

  const auto mid = p.getMeshID("MeshA");
  const auto did = p.getDataID("Scalar", mid);

  for (auto _ : state) {
    p.writeBlockScalarData(did, nv, vids.data(), data.data());
  }
}

static void BM_writeBlockScalarStringCopy(benchmark::State &state)
{

  SETUP_DATA(1, "Scalar")

  for (auto _ : state) {
    {
      const auto mid = p.getMeshID("MeshA");
      const auto did = p.getDataID("Scalar", mid);
      p.writeBlockScalarData(did, nv, vids.data(), data.data());
    }
  }
}

static void BM_writeBlockScalarStringReference(benchmark::State &state)
{

  SETUP_DATA(1, "Scalar")

  std::string mesha("MeshA");
  std::string dataScalar("Scalar");

  for (auto _ : state) {
    {
      const auto mid = p.getMeshID(mesha);
      const auto did = p.getDataID(dataScalar, mid);
      p.writeBlockScalarData(did, nv, vids.data(), data.data());
    }
  }
}

static void BM_writeScalarData(benchmark::State &state)
{

  SETUP_DATA(1, "Scalar")

  const auto mid = p.getMeshID("MeshA");
  const auto did = p.getDataID("Scalar", mid);

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i)
      p.writeScalarData(did, vids[i], data[i]);
  }
}

static void BM_writeScalarDataStringCopy(benchmark::State &state)
{

  SETUP_DATA(1, "Scalar")

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i) {
      const auto mid = p.getMeshID("MeshA");
      const auto did = p.getDataID("Scalar", mid);
      p.writeScalarData(did, vids[i], data[i]);
    }
  }
}

static void BM_writeScalarDataStringReference(benchmark::State &state)
{

  SETUP_DATA(1, "Scalar")

  std::string mesha("MeshA");
  std::string scalarData("Scalar");

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i) {
      const auto mid = p.getMeshID(mesha);
      const auto did = p.getDataID(scalarData, mid);
      p.writeScalarData(did, vids[i], data[i]);
    }
  }
}

#ifdef PRECICE_HAS_CUSTOM_API
static void BM_writeScalarDataStringCopySingleFunction(benchmark::State &state)
{

  SETUP_DATA(1, "Scalar")

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i) {
      p.writeScalarDataString("MeshA", "Scalar", vids[i], data[i]);
    }
  }
}

static void BM_writeScalarDataStringReferenceSingleFunction(benchmark::State &state)
{

  SETUP_DATA(1, "Scalar")

  std::string mesha("MeshA");
  std::string dataScalar("Scalar");

  for (auto _ : state) {
    for (unsigned int i = 0; i < nv; ++i) {
      p.writeScalarDataString(mesha, dataScalar, vids[i], data[i]);
    }
  }
}
#endif

// Register the function as a benchmark
BENCHMARK(BM_writeBlockVector);
BENCHMARK(BM_writeBlockVectorStringCopy);
BENCHMARK(BM_writeBlockVectorStringReference);
BENCHMARK(BM_writeVectorData);
BENCHMARK(BM_writeVectorDataStringCopy);
BENCHMARK(BM_writeVectorDataStringReference);
BENCHMARK(BM_writeBlockScalar);
BENCHMARK(BM_writeBlockScalarStringCopy);
BENCHMARK(BM_writeBlockScalarStringReference);
BENCHMARK(BM_writeScalarData);
BENCHMARK(BM_writeScalarDataStringCopy);
BENCHMARK(BM_writeScalarDataStringReference);

#ifdef PRECICE_HAS_CUSTOM_API
BENCHMARK(BM_writeScalarDataStringCopySingleFunction);
BENCHMARK(BM_writeScalarDataStringReferenceSingleFunction);
#endif