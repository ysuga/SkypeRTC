#pragma once

#include <string>
#include <exception>
#include "skype-embedded_2.h"

class AccountEx : public Account {
 private:
  bool m_LoggedIn;
  bool m_LoggedOut;

 public:

  /**
   * @brief return true if logged in
   */
  bool isLoggedIn() {
    if(m_LoggedIn && (!m_LoggedOut)) return true;
    return false;
  }
	   
 public:
  typedef DRef<AccountEx, Account> Ref;
  typedef DRefs<AccountEx, Account> Refs;

 public:
  AccountEx(unsigned int oid, SERootObject* root);
  ~AccountEx();

  void OnChange(int prop);
};
