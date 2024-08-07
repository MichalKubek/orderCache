#pragma once

#include <string>
#include <vector>

#include "utils.h"

#define SIDE_BUY "Buy"
#define SIDE_SELL "Sell"
class Order
{
public:
  // do not alter signature of this constructor
  Order (const std::string &ordId, const std::string &secId,
         const std::string &side, const unsigned int qty,
         const std::string &user, const std::string &company)
      : m_orderId (ordId), m_securityId (secId), m_side (side), m_qty (qty),
        m_user (user), m_company (company)
  {
  }
  Order (Order &&other):
    m_orderId(std::move(other.m_orderId)),
    m_securityId(std::move(other.m_securityId)),
    m_side(std::move(other.m_side)),
    m_qty(other.m_qty),
    m_user(std::move(other.m_user)),
    m_company(std::move(other.m_company))
  {}
  Order (const Order &other):
    m_orderId(other.m_orderId),
    m_securityId(other.m_securityId),
    m_side(other.m_side),
    m_qty(other.m_qty),
    m_user(other.m_user),
    m_company(other.m_company)
  {}
  Order () = default;

  // do not alter these accessor methods
  std::string
  orderId () const
  {
    return m_orderId;
  }
  std::string
  securityId () const
  {
    return m_securityId;
  }
  std::string
  side () const
  {
    return m_side;
  }
  std::string
  user () const
  {
    return m_user;
  }
  std::string
  company () const
  {
    return m_company;
  }
  unsigned int
  qty () const
  {
    return m_qty;
  }
  bool
  operator== (const Order &rhs) const
  {
    return m_orderId == rhs.m_orderId;
  }
  bool
  operator<(const Order &rhs) const
  {
    return m_orderId < rhs.m_orderId;
  }
  Order &operator= (const Order &rhs)
{
  m_orderId = rhs.m_orderId;
  m_securityId = rhs.m_securityId;
  m_side = rhs.m_side;
  m_qty = rhs.m_qty;
  m_user = rhs.m_user;
  m_company = rhs.m_company;
  return *this;
  }
  Order &operator= (Order &&rhs){
    m_orderId = std::move(rhs.m_orderId);
    m_securityId = std::move(rhs.m_securityId);
    m_side = std::move(rhs.m_side);
    m_qty = rhs.m_qty;
    m_user = std::move(rhs.m_user);
    m_company = std::move(rhs.m_company);
    return *this;
  }


private:
  // use the below to hold the order data
  // do not remove the these member variables
  std::string m_orderId;    // unique order id
  std::string m_securityId; // security identifier
  std::string m_side;       // side of the order, eg Buy or Sell
  unsigned int m_qty;       // qty for this order
  std::string m_user;       // user name who owns this order
  std::string m_company;    // company for user
};

// Provide an implementation for the OrderCacheInterface interface class.
// Your implementation class should hold all relevant data structures you think
// are needed.
class OrderCacheInterface
{
public:
  // implememnt the 6 methods below, do not alter signatures

  // add order to the cache
  virtual void addOrder (Order order) = 0;

  // remove order with this unique order id from the cache
  virtual void cancelOrder (const std::string &orderId) = 0;

  // remove all orders in the cache for this user
  virtual void cancelOrdersForUser (const std::string &user) = 0;

  // remove all orders in the cache for this security with qty >= minQty
  virtual void
  cancelOrdersForSecIdWithMinimumQty (const std::string &securityId,
                                      unsigned int minQty)
      = 0;

  // return the total qty that can match for the security id
  virtual unsigned int
  getMatchingSizeForSecurity (const std::string &securityId)
      = 0;

  // return all orders in cache in a vector
  virtual std::vector<Order> getAllOrders () const = 0;
};

class CompanyQuantity;
class OrderCacheMap;
class OrderCacheSimple;
