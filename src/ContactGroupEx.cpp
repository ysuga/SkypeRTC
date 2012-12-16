#include "SkypeEx.h"
#include "ContactGroupEx.h"

#include <iostream>
ContactGroupEx::ContactGroupEx(unsigned int oid, SERootObject* root):
  ContactGroup(oid, root)
{
}

ContactGroupEx::~ContactGroupEx()
{
}

void ContactGroupEx::OnChange(const ContactRef& contact)
{
  std::cout << __FILE__ << ": OnChange" << std::endl;
  SkypeEx* pSkypeEx = (SkypeEx*)this->root;
  pSkypeEx->onContactGroupChange(*this);
}
