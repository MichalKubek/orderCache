#include "../src/OrderCacheMap.h"
#include "../src/OrderCacheMapThread.h"
#include "../src/OrderCacheSimple.h"
#include <benchmark/benchmark.h>
#include <stdexcept>

template <typename T>
static void BM_getMatchingSizeForSecurity_match(benchmark::State &state) {
  T cache;

  std::vector<Order> orders = {
      Order("OrdId1", "SecId1", "Buy", 1000, "User1", "CompanyA"),
      Order("OrdId2", "SecId2", "Sell", 3000, "User2", "CompanyB"),
      Order("OrdId3", "SecId1", "Sell", 500, "User3", "CompanyA"),
      Order("OrdId4", "SecId2", "Buy", 600, "User4", "CompanyC"),
      Order("OrdId5", "SecId2", "Buy", 100, "User5", "CompanyB"),
      Order("OrdId6", "SecId3", "Buy", 1000, "User6", "CompanyD"),
      Order("OrdId7", "SecId2", "Buy", 2000, "User7", "CompanyE"),
      Order("OrdId8", "SecId2", "Sell", 5000, "User8", "CompanyE")};
  for (auto order : orders) {
    cache.addOrder(order);
  }
  for (auto _ : state) {
    if (cache.getMatchingSizeForSecurity("SecId2") != 2700 ||
        cache.getMatchingSizeForSecurity("SecId1") != 0 ||
        cache.getMatchingSizeForSecurity("SecId3") != 0)
      throw std::runtime_error("Error");
  }
}

template <typename T>
static void BM_getMatchingSizeForSecurity_add(benchmark::State &state) {
  std::vector<Order> orders = {
      Order("OrdId1", "SecId1", "Buy", 1000, "User1", "CompanyA"),
      Order("OrdId2", "SecId2", "Sell", 3000, "User2", "CompanyB"),
      Order("OrdId3", "SecId1", "Sell", 500, "User3", "CompanyA"),
      Order("OrdId4", "SecId2", "Buy", 600, "User4", "CompanyC"),
      Order("OrdId5", "SecId2", "Buy", 100, "User5", "CompanyB"),
      Order("OrdId6", "SecId3", "Buy", 1000, "User6", "CompanyD"),
      Order("OrdId7", "SecId2", "Buy", 2000, "User7", "CompanyE"),
      Order("OrdId8", "SecId2", "Sell", 5000, "User8", "CompanyE")};
  for (auto _ : state) {
    T cache;

    for (auto order : orders) {
      cache.addOrder(order);
    }
  }
}

#define BENCHMARK_TEMPLATE_CACHE(test_name)          \
  BENCHMARK_TEMPLATE(test_name, OrderCacheSimple);   \
  BENCHMARK_TEMPLATE(test_name, OrderCacheMap);      \
  BENCHMARK_TEMPLATE(test_name, OrderCacheMapThread)

BENCHMARK_TEMPLATE_CACHE(BM_getMatchingSizeForSecurity_match);
BENCHMARK_TEMPLATE_CACHE(BM_getMatchingSizeForSecurity_add);
