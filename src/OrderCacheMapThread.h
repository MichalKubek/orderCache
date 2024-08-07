#pragma once
#include <shared_mutex>
#include <thread>
#include <vector>
#include <atomic>

#include "OrderCache.h"
#include "OrderCacheMap.h"
#include "TaskQueue.h"


class OrderCacheMapThread : public OrderCacheInterface
{
public:
  typedef std::shared_mutex Lock;
  typedef std::unique_lock<Lock> WriteLock;
  typedef std::shared_lock<Lock> ReadLock;

  OrderCacheMapThread ();
  OrderCacheMapThread (const Order &);
  ~OrderCacheMapThread ();
  void addOrder (Order) override;
  void cancelOrder (const std::string &) override;
  std::vector<Order> getAllOrders () const override;
  void cancelOrdersForUser (const std::string &) override;
  void cancelOrdersForSecIdWithMinimumQty (const std::string &,
                                           unsigned int) override;
  unsigned int getMatchingSizeForSecurity (const std::string &) override;

protected:
  void startWorker ();

  void _addOrder (Order);
  void _cancelOrder (const std::string &);
  std::vector<Order> _getAllOrders () const;
  void _cancelOrdersForUser (const std::string &);
  void _cancelOrdersForSecIdWithMinimumQty (const std::string &, unsigned int);
  unsigned int _getMatchingSizeForSecurity (const std::string &);

private:
  mutable Lock m_lock;
  mutable TaskQueue m_taskQueue;
  std::thread m_workerThread;
  std::atomic<bool> m_stop{ false };
  OrderCacheMap m_orderCacheMap;
};
