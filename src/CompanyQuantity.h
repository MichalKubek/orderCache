#pragma once
#include <unordered_map>

#include "OrderCache.h"

class CompanyQuantity
{
public:
  using CompanyQtyMap = std::unordered_map<std::string, unsigned int>;

  CompanyQuantity ():
    totalQty (0)
  {
  };
  CompanyQuantity (const Order &);
  void addCompanyQty (const std::string &company, unsigned int qty);
  void removeCompanyQty (const std::string &company, unsigned int qty);
  unsigned int
  getTotalQty () const
  {
    return totalQty;
  }
  unsigned int getCompanyQty (const std::string &company) const;
  unsigned int getMatchingQtyForCompany (const std::string &,
                                         unsigned int qty) const;
  const CompanyQtyMap &
  getCompanyQtyMap () const
  {
    return qtyByCompany;
  };
  unsigned int
  total () const
  {
    return totalQty;
  };
  std::size_t
  size () const
  {
    return qtyByCompany.size ();
  };
  bool empty () const
  {
    return qtyByCompany.empty ();
  };
  unsigned int getMatchingQty (const CompanyQuantity &company_quantity) const;

private:
  unsigned int totalQty = 0;
  CompanyQtyMap qtyByCompany;
};
