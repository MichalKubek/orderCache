#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>


class TaskQueue
{
public:
  typedef std::function<void ()> Taks;
  typedef std::unique_lock<std::mutex> Lock;

  void push (Taks &&task);
  Taks pop ();

  size_t size () const;
  bool empty () const;

private:
  std::queue<Taks> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_condition;
};
