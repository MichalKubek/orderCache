#include "CompanyQuantity.h"

#include <algorithm>
#include <list>
#include <stdexcept>

CompanyQuantity::CompanyQuantity (const Order &order)
{
  qtyByCompany[order.company ()] = order.qty ();
  totalQty = order.qty ();
}

void
CompanyQuantity::addCompanyQty (const std::string &company, unsigned int qty)
{
  CompanyQtyMap::iterator it = qtyByCompany.find (company);
  if (it == qtyByCompany.end ())
    {
      qtyByCompany[company] = qty;
    }
  else
    {
      it->second += qty;
    }
  totalQty += qty;
};

void
CompanyQuantity::removeCompanyQty (const std::string &company,
                                   unsigned int qty)
{
  CompanyQtyMap::iterator it = qtyByCompany.find (company);
  if (it != qtyByCompany.end () && it->second >= qty)
    {
      it->second -= qty;
      totalQty -= qty;
      if (it->second == 0)
        qtyByCompany.erase (it);
    }
  else
    {
      throw std::runtime_error ("not enough quantity to remove");
    }
};

unsigned int
CompanyQuantity::getMatchingQty (const CompanyQuantity &company_quantity) const
{
  if (totalQty == 0 || company_quantity.total () == 0)
    return 0;

  if (size () > company_quantity.size ())
    return company_quantity.getMatchingQty (*this);

  unsigned int match_qty = 0;
  unsigned int available = totalQty;

  std::list<std::pair<std::string, unsigned int> > _company_quantity (
      qtyByCompany.begin (), qtyByCompany.end ());

  for (auto [company, qty] : company_quantity.getCompanyQtyMap ())
    {
      auto it = _company_quantity.begin ();
      while (it != _company_quantity.end () && available > 0 && qty > 0)
        {
          if (it->first != company)
            {
              unsigned int to_remove = std::min (qty, it->second);
              //        std::cout << "Matched company: " << company << "
              //        ("<<qty<<") and " << it->first << "(" << it->second <<
              //        ") removing: "  << to_remove << std::endl;
              it->second -= to_remove;
              match_qty += to_remove;
              qty -= to_remove;
              available -= to_remove;

              if (it->second == 0)
                {
                  it = _company_quantity.erase (it);
                  continue;
                }
            }
          it++;
        }
      if (available == 0 || _company_quantity.empty ())
        break;
    }

  return match_qty;
}
