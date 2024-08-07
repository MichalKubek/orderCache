#include <gtest/gtest.h>
#include <stdexcept>

#include "../src/CompanyQuantity.h"

TEST(CompanyQuantity, addCompanyQty) {
  CompanyQuantity company_qty;

  company_qty.addCompanyQty("CompanyA", 100);
  EXPECT_EQ(company_qty.size(), 1);
  EXPECT_EQ(company_qty.total(), 100);

  company_qty.addCompanyQty("CompanyB", 200);
  EXPECT_EQ(company_qty.size(), 2);
  EXPECT_EQ(company_qty.total(), 300);

  company_qty.addCompanyQty("CompanyA", 300);
  EXPECT_EQ(company_qty.size(), 2);
  EXPECT_EQ(company_qty.total(), 600);
}

TEST(CompanyQuantity, removeCompanyQty) {
  CompanyQuantity company_qty;

  company_qty.addCompanyQty("CompanyA", 100);
  EXPECT_EQ(company_qty.size(), 1);
  EXPECT_EQ(company_qty.total(), 100);

  company_qty.addCompanyQty("CompanyB", 200);
  EXPECT_EQ(company_qty.size(), 2);
  EXPECT_EQ(company_qty.total(), 300);

  company_qty.addCompanyQty("CompanyA", 300);
  EXPECT_EQ(company_qty.size(), 2);
  EXPECT_EQ(company_qty.total(), 600);

  company_qty.removeCompanyQty("CompanyA", 200);
  EXPECT_EQ(company_qty.size(), 2);
  EXPECT_EQ(company_qty.total(), 400);

  company_qty.removeCompanyQty("CompanyA", 200);
  EXPECT_EQ(company_qty.size(), 1);
  EXPECT_EQ(company_qty.total(), 200);

  company_qty.removeCompanyQty("CompanyB", 200);
  EXPECT_EQ(company_qty.size(), 0);
  EXPECT_EQ(company_qty.total(), 0);
}

TEST(CompanyQuantity, getMatchingQty) {
  CompanyQuantity company_qty_buy;
  CompanyQuantity company_qty_sell;

  EXPECT_EQ(company_qty_sell.getMatchingQty(company_qty_buy), 0);

  company_qty_buy.addCompanyQty("CompanyC", 600);
  company_qty_buy.addCompanyQty("CompanyB", 100);
  company_qty_buy.addCompanyQty("CompanyE", 2000);

  company_qty_sell.addCompanyQty("CompanyB", 3000);
  company_qty_sell.addCompanyQty("CompanyE", 5000);

  EXPECT_EQ(company_qty_sell.getMatchingQty(company_qty_buy), 2700);
  EXPECT_EQ(company_qty_buy.getMatchingQty(company_qty_sell), 2700);
}

TEST(CompanyQuantity, remove_company_qty) {
  CompanyQuantity company_qty_buy;
  company_qty_buy.addCompanyQty("CompanyC", 600);
  company_qty_buy.addCompanyQty("CompanyB", 100);
  company_qty_buy.addCompanyQty("CompanyE", 2000);

  CompanyQuantity company_qty_sell;
  company_qty_sell.addCompanyQty("CompanyB", 3000);
  company_qty_sell.addCompanyQty("CompanyE", 5000);

  EXPECT_EQ(company_qty_sell.getMatchingQty(company_qty_buy), 2700);
  EXPECT_EQ(company_qty_buy.getMatchingQty(company_qty_sell), 2700);
  company_qty_buy.removeCompanyQty("CompanyC", 200);

  EXPECT_EQ(company_qty_sell.getMatchingQty(company_qty_buy), 2500);
  EXPECT_EQ(company_qty_buy.getMatchingQty(company_qty_sell), 2500);
  company_qty_buy.removeCompanyQty("CompanyE", 1000);
  EXPECT_EQ(company_qty_sell.getMatchingQty(company_qty_buy), 1500);
  EXPECT_EQ(company_qty_buy.getMatchingQty(company_qty_sell), 1500);

  EXPECT_THROW(company_qty_buy.removeCompanyQty("CompanyC", 900),
               std::runtime_error);
  EXPECT_THROW(company_qty_buy.removeCompanyQty("CompanyX", 900),
               std::runtime_error);
}
