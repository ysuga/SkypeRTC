#pragma once


#include <string>
#include <exception>
#include "skype-embedded_2.h"


class ContactGroupEx : public ContactGroup {
 private:


 public:
  

 public:
  typedef DRef<ContactGroupEx, ContactGroup> Ref;
  typedef DRefs<ContactGroupEx, ContactGroup> Refs;


 public:
  ContactGroupEx(unsigned int oid, SERootObject* root);
  ~ContactGroupEx();
  void OnChange(const ContactRef& contact);

};
