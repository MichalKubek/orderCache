#pragma once
#include <unordered_map>
#include <unordered_set>

#include "OrderCache.h"
#include "CompanyQuantity.h"

class OrderCacheMap : public OrderCacheInterface
{
public:
  void addOrder (Order) override;
  void cancelOrder (const std::string &) override;
  std::vector<Order> getAllOrders () const override;
  void cancelOrdersForUser (const std::string &) override;
  void cancelOrdersForSecIdWithMinimumQty (const std::string &,
                                           unsigned int) override;
  unsigned int getMatchingSizeForSecurity (const std::string &) override;
  bool
  empty () const
  {
    return m_orders.empty ();
  }

private:
  using SecurityQtyMap = std::unordered_map<std::string, CompanyQuantity>;
  using UserOrders
      = std::unordered_map<std::string, std::unordered_set<std::string> >;

protected:
  void insertOrder (SecurityQtyMap &, const Order &);
  void removeCompanyQty (SecurityQtyMap &, const Order &);

private:
  std::unordered_map<std::string, Order> m_orders;
  UserOrders m_userOrders;
  SecurityQtyMap m_securityQtyBuy;
  SecurityQtyMap m_securityQtySell;
};
