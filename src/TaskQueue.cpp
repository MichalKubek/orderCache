#include "TaskQueue.h"
#include "utils.h"

void TaskQueue::push(Taks &&task)
  {
    DEBUG ("Q -> Pushing task in TaskQueue");
    Lock lock (m_mutex);
    m_queue.emplace (std::forward<Taks> (task));
    lock.unlock ();
    m_condition.notify_one ();
    DEBUG ("Q -> Pushed task in TaskQueue");
  };

TaskQueue::Taks TaskQueue::pop()
  {
    DEBUG ("Q <- Popping task in TaskQueue");
  TaskQueue::Lock lock (m_mutex);
    m_condition.wait (lock, [this] { return !m_queue.empty (); });
    DEBUG ("Q <- Popped task in TaskQueue");
    Taks task = std::move (m_queue.front ());
    m_queue.pop ();
    lock.unlock ();
    return std::move (task);
  };

size_t TaskQueue::size() const
  {
    return m_queue.size ();
  };

bool TaskQueue::empty() const
  {
    return m_queue.empty ();
  };
