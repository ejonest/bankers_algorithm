#ifndef __bank_h__
#define __bank_h__

#include <iostream>
#include <mutex>
#include <sstream>
#include "customer.h"
#include "vector_.h"


class Bank {
public:
  Bank() = default;
  Bank(const vector_<int>& available) : avail(available), customers() { }

  vector_<int> get_avail() const { return avail; }
  bool is_avail(const vector_<int>& req) const { return req < avail; }

       // TODO --- correctly implement the Banker's algorithm for
       //    is_safe() and req_approved()

       // for (int i = 0; i < n; ++i) {
       //   if (req[i] > need[i]) { return true; }
       // }
  bool is_safe(int id, const vector_<int>& req) {
    //Utils::print_locked("<><><> is_safe has been called\n");
    Customer* c = customers[id];
    //vector_<int> max = c->get_max();
    //std::cout << "Max size is: " << max.size() << " and max is: " << max << std::endl;
    if(c->too_much(req)) {
      //std::cout << "Too_much has returned true and therefore you have been denied\n";
      return false;
    }
    int size = customers.size();
    std::cout << "size is " << size << std::endl;
    bool go_ahead = false;
    for(int i = 0; i < size; ++i) {
      vector_<int> max = customers[i]->get_max();
        for (int j = 0; j < max.size(); ++j) { //should not be 4 it should be size since not all the vectors are that size
          //Utils::print_locked("//////i:%d\tget_max:%d\tavail::%d\trequest:%d//////\n", i, customers[i]->get_max()[j], avail[j], req[j]);
          //Utils::print_locked("///i:%d\trequest_num:%d\tmax:%d\tavail:%d\treq:%d///\n", i, j, req, max[j], avail[j], req[j]);
          if(max[j] <= (avail[j] - req[j])) {
            go_ahead = true;;
          } else {
            return false;
          }
        }
      if (go_ahead == true) {
        return true;
      }
    }
    //Utils::print_locked("\n<><><><>is_safe is returning false<><><><>\n");
    return false;
  }
  bool req_approved(int id, const vector_<int>& req) {
    Customer* c = customers[id];
    //if(c->too_much(req)) {
      //std::cout << std::endl << "<><><><>too much is returning false<><><><>" << std::endl;
      //Utils::print_locked("\n<><><><>too much is returning false<><><><>\n");
      //return false;
    //} else {
      return is_safe(id, req);
    //}
    //return !c->too_much(req);
  }

  void add_customer(Customer* c) { customers.push_back(c); }

  void withdraw_resources(const vector_<int>& req) {
    if (!is_avail(req)) {
      Utils::print_locked("WARNING: req: %s is not available for withdrawing\n", (req.as_string().c_str()));
      return;
    }
    if (is_avail(req)) { avail -= req; }
  }
  void deposit_resources(const vector_<int>& req) { avail += req; }


  vector_<Customer*> get_customers() const { return customers; }

  void show() const {
    std::stringstream ss;
    ss << "\n+-----------------------------------------\n"
          "|   BANK   avail: [" << avail.as_string().c_str() << "]\n"
          "+-----------------------------------------\n";
    for (Customer* c : customers) {
      ss << "| P# " <<  std::setw(2) << c->get_idx() << "   "
         <<  (c->get_alloc().as_string().c_str()) << "   "
         << (c->get_max().as_string().c_str())    << "   "
         << (c->get_need().as_string().c_str())
         << "\n";
    }
    std::string s = ss.str();

    Utils::print_locked("%s"
                        "+----------------------------------------\n", (s.c_str()));
  }

  friend std::ostream& operator<<(std::ostream& os, const Bank& bank) {
    bank.show();
    return os;
  }

private:
  vector_<int> avail;
  vector_<Customer*> customers;
};

#endif /* __bank_h__ */
