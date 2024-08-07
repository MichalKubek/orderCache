#include <benchmark/benchmark.h>
#include <fstream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "../src/OrderCacheMap.h"
#include "../src/OrderCacheMapThread.h"
#include "../src/OrderCacheSimple.h"



static void TestFiles(benchmark::internal::Benchmark *b) {
  b->Arg(100)->Arg(1000)->Arg(10000);
}

class FileOrderLoader {
public:
  FileOrderLoader(int fileId)
{
  std::string file_mask = SOURCE_DIR;
  file_mask += "/benchmarks/data/orders_%d.csv";
  m_fileName = (boost::str(boost::format(file_mask) % fileId));
  load();
}

  Order lineToOrder(std::string line) {
    std::string ord_id, sec_id, type, user, company;
    std::vector<std::string> order_info;
    boost::split(order_info, line, boost::is_any_of(","));

    assert(order_info.size() == 6);

    return Order(order_info[0], order_info[1], order_info[2], std::stoi(order_info[3]), order_info[4], order_info[5]);
  }

  std::vector<Order> getOrders() {
    return m_orders;
  }
  std::set<std::string> getUsers() {
    return m_users;
  }
  std::set<std::string> getCompanies() {
    return m_companies;
  }
  std::set<std::string> getSecurities() {
    return m_securities;
  }

  protected:

  void load() {
    std::ifstream file(m_fileName);
    if(!file.is_open()) {
      throw std::runtime_error("Could not open file: " + m_fileName);
    }
    DEBUG("Loading file: " + file_name);
    std::string line;
    while (std::getline(file, line)) {
      if(!line.empty() && (line[0] == 'O' || line[0] == 'o')){
        Order order = lineToOrder(line);
        m_users.insert(order.user());
        m_companies.insert(order.company());
        m_securities.insert(order.securityId());
        m_orders.push_back(lineToOrder(line));
      }
    }
  }



private:
  std::vector<Order> m_orders;
  std::set<std::string> m_users;
  std::set<std::string> m_companies;
  std::string m_fileName;
  std::set<std::string> m_securities;
};



template <typename T>
static void BM_load_file(benchmark::State &state) {
  FileOrderLoader loader(state.range(0));
  for (auto _ : state) {
    T cache;
    benchmark::DoNotOptimize(cache);
    for(auto order : loader.getOrders()) {
      cache.addOrder(order);
    }
  }
}

template <typename T>
static void BM_getMatchingSizeForSecurity(benchmark::State &state) {
  FileOrderLoader loader(state.range(0));
  T cache;
  for(auto order : loader.getOrders()) {
    cache.addOrder(order);
  }
  benchmark::DoNotOptimize(cache);
  for (auto _ : state) {
    for(auto secId : loader.getSecurities()) {
      cache.getMatchingSizeForSecurity(secId);
    }
  }
}

template <typename T>
static void BM_cancelOrdersForSecIdWithMinimumQty(benchmark::State &state) {
  FileOrderLoader loader( state.range(0));
  T cache;
  for(auto order : loader.getOrders()) {
    cache.addOrder(order);
  }
  benchmark::DoNotOptimize(cache);
  for (auto _ : state) {
    for(auto secId : loader.getSecurities()) {
      cache.cancelOrdersForSecIdWithMinimumQty(secId, 1000);
    }
  }
}

template <typename T>
static void BM_cancelOrdersForUser(benchmark::State &state) {
  FileOrderLoader loader( state.range(0));
  for (auto _ : state) {
    state.PauseTiming();

    T cache;
    benchmark::DoNotOptimize(cache);
    for(auto order : loader.getOrders()) {
      cache.addOrder(order);
    }
    state.ResumeTiming();

    for(auto user : loader.getUsers()) {
      cache.cancelOrdersForUser(user);
    }
  }
}

template <typename T>
static void BM_cancelOrder(benchmark::State &state) {
  FileOrderLoader loader( state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    T cache;
    benchmark::DoNotOptimize(cache);
    for(auto order : loader.getOrders()) {
      cache.addOrder(order);
    }
    state.ResumeTiming();
    for(auto order : loader.getOrders()) {
      cache.cancelOrder(order.orderId());
    }
  }
}

template <typename T>
static void BM_add_and_match(benchmark::State &state) {
  FileOrderLoader loader( state.range(0));
  for (auto _ : state) {
    T cache;
    benchmark::DoNotOptimize(cache);
    for(auto order : loader.getOrders()) {
      cache.addOrder(order);
      cache.getMatchingSizeForSecurity(order.securityId());
    }
  }
}


#define BENCHMARK_TEMPLATE_CACHE(test_name)          \
  BENCHMARK_TEMPLATE(test_name, OrderCacheSimple)->Apply(TestFiles);   \
  BENCHMARK_TEMPLATE(test_name, OrderCacheMap)->Apply(TestFiles);      \
  BENCHMARK_TEMPLATE(test_name, OrderCacheMapThread)->Apply(TestFiles)

BENCHMARK_TEMPLATE_CACHE(BM_load_file);
BENCHMARK_TEMPLATE_CACHE(BM_getMatchingSizeForSecurity);
BENCHMARK_TEMPLATE_CACHE(BM_cancelOrdersForSecIdWithMinimumQty);
BENCHMARK_TEMPLATE_CACHE(BM_cancelOrdersForUser);
BENCHMARK_TEMPLATE_CACHE(BM_cancelOrder);
BENCHMARK_TEMPLATE_CACHE(BM_add_and_match);
