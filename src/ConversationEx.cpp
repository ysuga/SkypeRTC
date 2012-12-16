#include "ConversationEx.h"

#include <iostream>
ConversationEx::ConversationEx(unsigned int oid, SERootObject* root): Conversation(oid, root)
{
  GetParticipants(m_Participants);
  fetch(m_Participants);
}

ConversationEx::~ConversationEx()
{

}

void ConversationEx::OnParticipantListChange()
{
  std::cout << __FILE__ << ":OnParticipantListChange()" << std::endl;
  GetParticipants(m_Participants);
  fetch(m_Participants);
}
