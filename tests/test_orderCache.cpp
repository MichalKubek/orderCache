#include "../src/OrderCache.h"
#include "../src/OrderCacheMap.h"
#include "../src/OrderCacheSimple.h"
#include "../src/OrderCacheMapThread.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <sstream>

using OrderCachePtr = std::unique_ptr<OrderCacheInterface>;

template <typename T> class OrderCacheTest : public ::testing::Test {
};

TYPED_TEST_SUITE_P(OrderCacheTest);

TYPED_TEST_P(OrderCacheTest, getMatchingSizeForSecurity_example1) {
  std::vector<Order> orders = {
      Order("OrdId1", "SecId1", SIDE_BUY, 1000, "User1", "CompanyA"),
      Order("OrdId2", "SecId2", SIDE_SELL, 3000, "User2", "CompanyB"),
      Order("OrdId3", "SecId1", SIDE_SELL, 500, "User3", "CompanyA"),
      Order("OrdId4", "SecId2", SIDE_BUY, 600, "User4", "CompanyC"),
      Order("OrdId5", "SecId2", SIDE_BUY, 100, "User5", "CompanyB"),
      Order("OrdId6", "SecId3", SIDE_BUY, 1000, "User6", "CompanyD"),
      Order("OrdId7", "SecId2", SIDE_BUY, 2000, "User7", "CompanyE"),
      Order("OrdId8", "SecId2", SIDE_SELL, 5000, "User8", "CompanyE")};
  auto cache = TypeParam();
  for (auto order : orders) {
    cache.addOrder(order);
  }
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId2"), 2700);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId1"), 0);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId3"), 0);
}

TYPED_TEST_P(OrderCacheTest, getMatchingSizeForSecurity_example2) {
  std::vector<Order> orders = {
      Order("OrdId1", "SecId1", SIDE_SELL, 100, "User10", "Company2"),
      Order("OrdId2", "SecId3", SIDE_SELL, 200, "User8", "Company2"),
      Order("OrdId3", "SecId1", SIDE_BUY, 300, "User13", "Company2"),
      Order("OrdId4", "SecId2", SIDE_SELL, 400, "User12", "Company2"),
      Order("OrdId5", "SecId3", SIDE_SELL, 500, "User7", "Company2"),
      Order("OrdId6", "SecId3", SIDE_BUY, 600, "User3", "Company1"),
      Order("OrdId7", "SecId1", SIDE_SELL, 700, "User10", "Company2"),
      Order("OrdId8", "SecId1", SIDE_SELL, 800, "User2", "Company1"),
      Order("OrdId9", "SecId2", SIDE_BUY, 900, "User6", "Company2"),
      Order("OrdId10", "SecId2", SIDE_SELL, 1000, "User5", "Company1"),
      Order("OrdId11", "SecId1", SIDE_SELL, 1100, "User13", "Company2"),
      Order("OrdId12", "SecId2", SIDE_BUY, 1200, "User9", "Company2"),
      Order("OrdId13", "SecId1", SIDE_SELL, 1300, "User1", "Company2"),
  };
  auto cache = TypeParam();
  for (auto order : orders) {
    cache.addOrder(order);
  }
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId1"), 300);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId2"), 1000);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId3"), 600);
}

TYPED_TEST_P(OrderCacheTest, getMatchingSizeForSecurity_example3) {
  std::vector<Order> orders = {
      Order("OrdId1", "SecId3", SIDE_SELL, 100, "User1", "Company1"),
      Order("OrdId2", "SecId3", SIDE_SELL, 200, "User3", "Company2"),
      Order("OrdId3", "SecId1", SIDE_BUY, 300, "User2", "Company1"),
      Order("OrdId4", "SecId3", SIDE_SELL, 400, "User5", "Company2"),
      Order("OrdId5", "SecId2", SIDE_SELL, 500, "User2", "Company1"),
      Order("OrdId6", "SecId2", SIDE_BUY, 600, "User3", "Company2"),
      Order("OrdId7", "SecId2", SIDE_SELL, 700, "User1", "Company1"),
      Order("OrdId8", "SecId1", SIDE_SELL, 800, "User2", "Company1"),
      Order("OrdId9", "SecId1", SIDE_BUY, 900, "User5", "Company2"),
      Order("OrdId10", "SecId1", SIDE_SELL, 1000, "User1", "Company1"),
      Order("OrdId11", "SecId2", SIDE_SELL, 1100, "User6", "Company2")};
  OrderCacheSimple cache;
  for (auto order : orders) {
    cache.addOrder(order);
  }
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId1"), 900);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId2"), 600);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId3"), 0);
}

TYPED_TEST_P(OrderCacheTest, RemoveOrder) {
  DEBUG("Start test RemoveOrder");
  std::vector<Order> orders = {
      Order("OrdId1", "SecId3", SIDE_SELL, 100, "User1", "Company1"),
      Order("OrdId2", "SecId3", SIDE_SELL, 200, "User3", "Company2"),
      Order("OrdId3", "SecId1", SIDE_BUY, 300, "User2", "Company1"),
      Order("OrdId4", "SecId3", SIDE_SELL, 400, "User5", "Company2"),
      Order("OrdId5", "SecId2", SIDE_SELL, 500, "User2", "Company1"),
      Order("OrdId6", "SecId2", SIDE_BUY, 600, "User3", "Company2"),
      Order("OrdId7", "SecId2", SIDE_SELL, 700, "User1", "Company1"),
      Order("OrdId8", "SecId1", SIDE_SELL, 800, "User2", "Company1"),
      Order("OrdId9", "SecId1", SIDE_BUY, 900, "User5", "Company2"),
      Order("OrdId10", "SecId1", SIDE_SELL, 1000, "User1", "Company1"),
      Order("OrdId11", "SecId2", SIDE_SELL, 1100, "User6", "Company2")};
  auto cache = TypeParam();
  for (auto order : orders) {
    cache.addOrder(order);
  }
  DEBUG("Start canceling orders");
  cache.cancelOrder("OrdId1");
  cache.cancelOrder("OrdId7");
  cache.cancelOrder("OrdId9");
  DEBUG("End canceling orders");
  std::vector<Order> sub_set = {
      Order("OrdId2", "SecId3", SIDE_SELL, 200, "User3", "Company2"),
      Order("OrdId3", "SecId1", SIDE_BUY, 300, "User2", "Company1"),
      Order("OrdId4", "SecId3", SIDE_SELL, 400, "User5", "Company2"),
      Order("OrdId5", "SecId2", SIDE_SELL, 500, "User2", "Company1"),
      Order("OrdId6", "SecId2", SIDE_BUY, 600, "User3", "Company2"),
      Order("OrdId8", "SecId1", SIDE_SELL, 800, "User2", "Company1"),
      Order("OrdId10", "SecId1", SIDE_SELL, 1000, "User1", "Company1"),
      Order("OrdId11", "SecId2", SIDE_SELL, 1100, "User6", "Company2")};
  std::vector<Order> result = cache.getAllOrders();
  DEBUG("Start sorting");
  std::sort(result.begin(), result.end());
  std::sort(sub_set.begin(), sub_set.end());
  EXPECT_EQ(result, sub_set);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId1"), 0);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId2"), 500);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId3"), 0);
}

std::string GetMissingIds(const std::vector<Order>& a, const std::vector<Order>&b)
{
  std::set<std::string> a_set;
  for(auto order : a)
  {
    a_set.insert(order.orderId());
  }
  std::vector<std::string> missing;
  for(auto order : b)
  {
    if(a_set.find(order.orderId()) == a_set.end())
    {
      missing.push_back(order.orderId());
    } else {
      a_set.erase(order.orderId());
    }
  }
  std::stringstream ss;
  ss << "Missing orders in A: " << boost::algorithm::join(missing, ", ") << " missing in B " << boost::algorithm::join(a_set, ", ") << std::endl;
  return ss.str();
}

TYPED_TEST_P(OrderCacheTest, cancelOrdersForSecIdWithMinimumQty) {
  std::vector<Order> orders = {
      Order("OrdId1", "SecId1", SIDE_BUY, 1000, "User1", "CompanyA"),
      Order("OrdId2", "SecId2", SIDE_SELL, 3000, "User2", "CompanyB"),
      Order("OrdId3", "SecId1", SIDE_SELL, 500, "User3", "CompanyA"),
      Order("OrdId4", "SecId2", SIDE_BUY, 600, "User4", "CompanyC"),
      Order("OrdId5", "SecId2", SIDE_BUY, 100, "User5", "CompanyB"),
      Order("OrdId6", "SecId3", SIDE_BUY, 1000, "User6", "CompanyD"),
      Order("OrdId7", "SecId2", SIDE_BUY, 2000, "User7", "CompanyE"),
      Order("OrdId8", "SecId2", SIDE_SELL, 5000, "User8", "CompanyE")};
  auto cache = TypeParam();
  for (auto order : orders) {
    cache.addOrder(order);
  }
  cache.cancelOrdersForSecIdWithMinimumQty("SecId2", 600);
  std::vector<Order> sub_set = {
      Order("OrdId1", "SecId1", SIDE_BUY, 1000, "User1", "CompanyA"),
      Order("OrdId2", "SecId2", SIDE_SELL, 3000, "User2", "CompanyB"),
      Order("OrdId3", "SecId1", SIDE_SELL, 500, "User3", "CompanyA"),
      Order("OrdId6", "SecId3", SIDE_BUY, 1000, "User6", "CompanyD"),
      Order("OrdId7", "SecId2", SIDE_BUY, 2000, "User7", "CompanyE"),
      Order("OrdId8", "SecId2", SIDE_SELL, 5000, "User8", "CompanyE")};
  std::vector<Order> result = cache.getAllOrders();
  std::sort(result.begin(), result.end());
  std::sort(sub_set.begin(), sub_set.end());
  EXPECT_EQ(result.size(), sub_set.size());
  EXPECT_EQ(result, sub_set) << "Missing orders: " << GetMissingIds(result, sub_set);

  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId2"), 2000);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId1"), 0);
  EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId3"), 0);
}

TYPED_TEST_P(OrderCacheTest, cancelOrdersForUser) {
  std::vector<Order> orders = {
      Order("OrdId1", "SecId1", SIDE_BUY, 1000, "User1", "CompanyA"),
      Order("OrdId2", "SecId2", SIDE_SELL, 3000, "User2", "CompanyB"),
      Order("OrdId3", "SecId1", SIDE_SELL, 500, "User3", "CompanyA"),
      Order("OrdId4", "SecId2", SIDE_BUY, 600, "User4", "CompanyC"),
      Order("OrdId5", "SecId2", SIDE_BUY, 100, "User5", "CompanyB"),
      Order("OrdId6", "SecId3", SIDE_BUY, 1000, "User6", "CompanyD"),
      Order("OrdId7", "SecId2", SIDE_BUY, 2000, "User7", "CompanyE"),
      Order("OrdId8", "SecId2", SIDE_SELL, 5000, "User8", "CompanyE"),
      Order("OrdId9", "SecId2", SIDE_SELL, 30, "User8", "CompanyE"),
      Order("OrdId10", "SecId2", SIDE_SELL, 40, "User8", "CompanyE")};
  auto cache = TypeParam();
  for (auto order : orders) {
    cache.addOrder(order);
  }
  cache.cancelOrdersForUser("User8");
  std::vector<Order> sub_set = {
      Order("OrdId1", "SecId1", SIDE_BUY, 1000, "User1", "CompanyA"),
      Order("OrdId2", "SecId2", SIDE_SELL, 3000, "User2", "CompanyB"),
      Order("OrdId3", "SecId1", SIDE_SELL, 500, "User3", "CompanyA"),
      Order("OrdId4", "SecId2", SIDE_BUY, 600, "User4", "CompanyC"),
      Order("OrdId5", "SecId2", SIDE_BUY, 100, "User5", "CompanyB"),
      Order("OrdId6", "SecId3", SIDE_BUY, 1000, "User6", "CompanyD"),
      Order("OrdId7", "SecId2", SIDE_BUY, 2000, "User7", "CompanyE"),
  };
  std::vector<Order> result = cache.getAllOrders();
  std::sort(result.begin(), result.end());
  std::sort(sub_set.begin(), sub_set.end());
  EXPECT_EQ(result.size(), sub_set.size());
  EXPECT_EQ(result, sub_set) << "Missing ids" << GetMissingIds(result, sub_set);
}

TYPED_TEST_P(OrderCacheTest, cancelOrder)
{
  std::vector<Order> orders = {
      Order("OrdId1", "SecId1", SIDE_BUY, 1000, "User1", "CompanyA"),
      Order("OrdId2", "SecId2", SIDE_SELL, 3000, "User2", "CompanyB"),
      Order("OrdId3", "SecId1", SIDE_SELL, 500, "User3", "CompanyA"),
      Order("OrdId4", "SecId2", SIDE_BUY, 600, "User4", "CompanyC"),
      Order("OrdId5", "SecId2", SIDE_BUY, 100, "User5", "CompanyB"),
      Order("OrdId6", "SecId3", SIDE_BUY, 1000, "User6", "CompanyD"),
      Order("OrdId7", "SecId2", SIDE_BUY, 2000, "User7", "CompanyE"),
      Order("OrdId8", "SecId2", SIDE_SELL, 5000, "User8", "CompanyE"),
      Order("OrdId9", "SecId2", SIDE_SELL, 30, "User8", "CompanyE"),
      Order("OrdId10", "SecId2", SIDE_SELL, 40, "User8", "CompanyE")};
  auto cache = TypeParam();
  for (auto order : orders) {
    cache.addOrder(order);
  }
  for(auto order : orders)
  {
    cache.cancelOrder(order.orderId());
  }
  

}

using testing::Types;

REGISTER_TYPED_TEST_SUITE_P(
    OrderCacheTest, // The first argument is the test case name.
    // The rest of the arguments are the test names.
    getMatchingSizeForSecurity_example1, getMatchingSizeForSecurity_example2,
    getMatchingSizeForSecurity_example3, RemoveOrder,
    cancelOrdersForSecIdWithMinimumQty, cancelOrdersForUser, cancelOrder);
typedef Types<OrderCacheSimple, OrderCacheMap, OrderCacheMapThread> OrderCacheImplementations;

INSTANTIATE_TYPED_TEST_SUITE_P(OrderCache,                 // Instance name
                               OrderCacheTest,             // Test case name
                               OrderCacheImplementations); // Type list
