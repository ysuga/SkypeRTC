#pragma once

#include <string>
#include "skype-embedded_2.h"

#include "ParticipantEx.h"

class ConversationEx : public Conversation
{
 private:

 public:
  typedef DRef<ConversationEx, Conversation> Ref;
  typedef DRefs<ConversationEx, Conversation> Refs;
 private:
  ParticipantEx::Refs m_Participants;

 public:
  ConversationEx(unsigned int oid, SERootObject* root);
  ~ConversationEx();


  void OnParticipantListChange();
};
