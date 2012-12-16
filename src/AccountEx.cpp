#include "AccountEx.h"


AccountEx::AccountEx(unsigned int oid, SERootObject* root) : Account(oid, root),
							     m_LoggedIn(false),
							     m_LoggedOut(false)
{
}


AccountEx::~AccountEx()
{
}

void AccountEx::OnChange(int prop) {
  if (prop == Account::P_STATUS) {
    Account::STATUS loginStatus;
    this->GetPropStatus(loginStatus);
    if (loginStatus == Account::LOGGED_IN) {
      //	printf("Login complete.\n");
      m_LoggedIn = true;
    } else if (loginStatus == Account::LOGGED_OUT) {
      Account::LOGOUTREASON whyLogout;
      this->GetPropLogoutreason(whyLogout);
      printf("%s\n", (const char*)tostring(whyLogout));
      m_LoggedOut = true;
      printf("Logout complete.\n");
    }
  }
}
