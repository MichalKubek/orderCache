#include "OrderCacheMap.h"

#include <csignal>
#include <stdexcept>

#include "OrderCache.h"

void
OrderCacheMap::addOrder (Order order)
{
  const size_t order_size = m_orders.size ();
  if (m_orders.find (order.orderId ()) != m_orders.end ())
    throw std::runtime_error ("Order alredy exist");

  m_userOrders[order.user ()].insert (order.orderId ());

  if (order.side () == SIDE_BUY)
    insertOrder (m_securityQtyBuy, order);
  else
    insertOrder (m_securityQtySell, order);
  std::string ord_id(order.orderId());
  m_orders[order.orderId()] = std::move(order);
  if(m_orders.size() != order_size + 1)
  {
    throw std::runtime_error("Order not added");
  }
  
  if( m_orders.find(ord_id) == m_orders.end())
  {
    throw std::runtime_error("Order not found after append");
  }
}

void
OrderCacheMap::insertOrder (SecurityQtyMap &security_map, const Order &order)
{
  SecurityQtyMap::iterator sec_it = security_map.find (order.securityId ());
  if (sec_it != security_map.end ())
    {
      sec_it->second.addCompanyQty (order.company (), order.qty ());
    }
  else
    {
      security_map[order.securityId ()] = CompanyQuantity (order);
    }
}

void
OrderCacheMap::cancelOrder (const std::string &orderId)
{
  // if we don't have order error occurs
  auto it = m_orders.find (orderId);
  if (it == m_orders.end ())
  {
    std::string msg = "Order " + orderId + " doesn't exist\n";
    for(auto order : m_orders)
    {
      msg += order.first + " ";
    }
    msg += "\n";
    msg += "m_orders Size: " + std::to_string(m_orders.size());
    msg += "\n";
    msg += "User size: " + std::to_string(m_userOrders.size());
    msg += "\n";
    msg += "Buy size: " + std::to_string(m_securityQtyBuy.size());
    msg += "\n";
    msg += "Sell size: " + std::to_string(m_securityQtySell.size());

    throw std::runtime_error (msg);
  }

  if( auto user_it = m_userOrders.find(it->second.user()); user_it != m_userOrders.end())
  {
    user_it->second.erase(it->second.orderId());
    if(user_it->second.size() == 0)
    {
      m_userOrders.erase(user_it);
    }
  }
  else
    throw std::runtime_error("User doesn't exist");

  removeCompanyQty (it->second.side () == SIDE_BUY ? m_securityQtyBuy
                                                   : m_securityQtySell,
                    it->second);
  m_orders.erase (it);
}

void
OrderCacheMap::removeCompanyQty (SecurityQtyMap &security_map,
                                 const Order &order)
{
  SecurityQtyMap::iterator sec_it = security_map.find (order.securityId ());
  if (sec_it != security_map.end ())
  {
    sec_it->second.removeCompanyQty (order.company (), order.qty ());
    if(sec_it->second.empty())
    {
      security_map.erase(sec_it);
    }
  }
  else
    throw std::runtime_error ("Security doesn't exist");
}

void
OrderCacheMap::cancelOrdersForUser (const std::string &user)
{
  auto user_it = m_userOrders.find (user);
  if (user_it == m_userOrders.end ())
    throw std::runtime_error ("User doesn't exist");

  std::vector<std::string> orders_to_cancel (user_it->second.begin (),
                                             user_it->second.end ());
  for (auto order_id : orders_to_cancel)
    cancelOrder (order_id);
}

void
OrderCacheMap::cancelOrdersForSecIdWithMinimumQty (
    const std::string &securityId, unsigned int minQty)
{
  std::vector<std::string> orders_to_cancel;
  for (auto order : m_orders)
    {
      if (order.second.securityId () == securityId
          && order.second.qty () <= minQty)
        orders_to_cancel.push_back (order.first);
    }
  for (auto order_id : orders_to_cancel)
    cancelOrder (order_id);
}

std::vector<Order>
OrderCacheMap::getAllOrders () const
{
  std::vector<Order> data;
  for (auto i = m_orders.begin (); i != m_orders.end (); i++)
    data.push_back (i->second);
  return data;
}

unsigned int
OrderCacheMap::getMatchingSizeForSecurity (const std::string &securityId)
{
  SecurityQtyMap::const_iterator sec_buy = m_securityQtyBuy.find (securityId);
  if (sec_buy == m_securityQtyBuy.end ())
    return 0;

  SecurityQtyMap::const_iterator sec_sell
      = m_securityQtySell.find (securityId);
  if (sec_sell == m_securityQtySell.end ())
    return 0;

  return sec_sell->second.getMatchingQty (sec_buy->second);
}
