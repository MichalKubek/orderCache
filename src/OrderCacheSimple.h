#pragma once
#include "OrderCache.h"
#include <unordered_map>

class OrderCacheSimple : public OrderCacheInterface
{
private:
  std::vector<Order> m_orders;

public:
  void addOrder (Order) override;
  void cancelOrder (const std::string &) override;
  void cancelOrdersForUser (const std::string &) override;
  void cancelOrdersForSecIdWithMinimumQty (const std::string &,
                                           unsigned int) override;
  unsigned int getMatchingSizeForSecurity (const std::string &) override;
  std::vector<Order>
  getAllOrders () const override
  {
    return m_orders;
  }

protected:
  void addQty (std::unordered_map<std::string, unsigned int> &, const Order &);
};
