#include "OrderCacheMapThread.h"

#include <exception>
#include <functional>
#include <future>
#include <thread>


OrderCacheMapThread::OrderCacheMapThread () { startWorker (); }

void
worker (TaskQueue &tasqQueue, std::atomic<bool> &m_stop)
{
  while (!m_stop)
    {
    auto task = tasqQueue.pop ();
      if (task == nullptr)
        {
          DEBUG ("Stopping worker");
          break;
        }
      DEBUG ("Running task");
      task ();
    }
}

void
OrderCacheMapThread::startWorker ()
{
  DEBUG ("Starting worker");
  m_workerThread = std::thread (worker, std::ref (m_taskQueue), std::ref (m_stop));
}

OrderCacheMapThread::OrderCacheMapThread (const Order &order)
{
  startWorker ();
  addOrder (order);
};

OrderCacheMapThread::~OrderCacheMapThread ()
{
  m_stop = true;
  m_taskQueue.push (nullptr);
  m_workerThread.join ();
}

void
OrderCacheMapThread::addOrder (Order order)
{
  m_taskQueue.push ([this, order] { _addOrder (order); });
}

void
OrderCacheMapThread::_addOrder (Order order)
{
  DEBUG ("Run add order");
  WriteLock lock (m_lock);
  m_orderCacheMap.addOrder (order);
}

void
OrderCacheMapThread::cancelOrder (const std::string &orderId)
{
  m_taskQueue.push ([this, orderId] { _cancelOrder (orderId); });
}
void
OrderCacheMapThread::_cancelOrder (const std::string &orderId)
{
  DEBUG ("Run cancel order");
  WriteLock lock (m_lock);
  m_orderCacheMap.cancelOrder (orderId);
}

std::vector<Order>
OrderCacheMapThread::getAllOrders () const
{
  std::promise<std::vector<Order> > promise;
  std::future<std::vector<Order> > future = promise.get_future ();
  m_taskQueue.push ([this, &promise] () {
    try
      {
        promise.set_value (_getAllOrders ());
      }
    catch (...)
      {
        promise.set_exception (std::current_exception ());
      }
  });
  return future.get ();
}

std::vector<Order>
OrderCacheMapThread::_getAllOrders () const
{
  ReadLock lock (m_lock);
  return m_orderCacheMap.getAllOrders ();
}

void
OrderCacheMapThread::cancelOrdersForUser (const std::string &user)
{
  m_taskQueue.push ([this, user] { _cancelOrdersForUser (user); });
}

void
OrderCacheMapThread::_cancelOrdersForUser (const std::string &user)
{
  WriteLock lock (m_lock);
  m_orderCacheMap.cancelOrdersForUser (user);
}

void
OrderCacheMapThread::cancelOrdersForSecIdWithMinimumQty (
    const std::string &secId, unsigned int qty)
{
  m_taskQueue.push ([this, secId, qty] {
    _cancelOrdersForSecIdWithMinimumQty (secId, qty);
  });
}

void
OrderCacheMapThread::_cancelOrdersForSecIdWithMinimumQty (
    const std::string &secId, unsigned int qty)
{
  WriteLock lock (m_lock);
  m_orderCacheMap.cancelOrdersForSecIdWithMinimumQty (secId, qty);
}
unsigned int
OrderCacheMapThread::getMatchingSizeForSecurity (const std::string &secId)
{
  std::promise<unsigned int> promise;
  std::future<unsigned int> future = promise.get_future ();
  DEBUG ("Pushing task for getMatchingSizeForSecurity");
  m_taskQueue.push ([this, &promise, secId] () {
    try
      {
        auto val = _getMatchingSizeForSecurity (secId);
        promise.set_value (val);
      }
    catch (std::exception &e)
      {
        DEBUG ("Exception in getMatchingSizeForSecurity: %s", e.what ());
        promise.set_exception (std::current_exception ());
      }
  });
  return future.get();
}
unsigned int
OrderCacheMapThread::_getMatchingSizeForSecurity (const std::string &secId)
{
  ReadLock lock (m_lock);
  return m_orderCacheMap.getMatchingSizeForSecurity (secId);
}
