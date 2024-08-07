#include "OrderCacheSimple.h"

#include <algorithm>
#include <unordered_map>

#include "OrderCache.h"

void
OrderCacheSimple::addOrder (Order order)
{
  m_orders.push_back (order);
}

void
OrderCacheSimple::cancelOrder (const std::string &orderId)
{
  auto it = std::find_if (
      m_orders.begin (), m_orders.end (),
      [orderId] (const Order &order) { return order.orderId () == orderId; });
  if (it != m_orders.end ())
    m_orders.erase (it);
}

void
OrderCacheSimple::cancelOrdersForUser (const std::string &user)
{
  m_orders.erase (std::remove_if (m_orders.begin (), m_orders.end (),
                                  [user] (const Order &order) {
                                    return order.user () == user;
                                  }),
                  m_orders.end ());
}

void
OrderCacheSimple::cancelOrdersForSecIdWithMinimumQty (
    const std::string &securityId, unsigned int minQty)
{
  m_orders.erase (std::remove_if (m_orders.begin (), m_orders.end (),
                                  [securityId, minQty] (const Order &order) {
                                    return order.securityId () == securityId
                                           && order.qty () <= minQty;
                                  }),
                  m_orders.end ());
}

unsigned int
OrderCacheSimple::getMatchingSizeForSecurity (const std::string &securityId)
{
  std::unordered_map<std::string, unsigned int> securityQty_buy;
  std::unordered_map<std::string, unsigned int> securityQty_sell;
  unsigned int totalQty = 0;
  for (auto order : m_orders)
    {
      if (order.securityId () != securityId)
        continue;
      if (order.side () == SIDE_BUY)
        addQty (securityQty_buy, order);
      else
        addQty (securityQty_sell, order);
    }

  for (auto &it_buy : securityQty_buy)
    {
      for (auto &it_sell : securityQty_sell)
        {
          if (it_buy.second > 0 && it_sell.second > 0
              && it_buy.first != it_sell.first)
            {
              totalQty += std::min (it_buy.second, it_sell.second);
              it_buy.second -= std::min (it_buy.second, it_sell.second);
              it_sell.second -= std::min (it_buy.second, it_sell.second);
            }
          if (it_buy.second == 0)
            break;
        }
    }

  return totalQty;
}

void
OrderCacheSimple::addQty (
    std::unordered_map<std::string, unsigned int> &qty_map, const Order &order)
{
  auto it = qty_map.find (order.company ());
  if (it != qty_map.end ())
    it->second += order.qty ();
  else
    qty_map.insert ({ order.company (), order.qty () });
}
