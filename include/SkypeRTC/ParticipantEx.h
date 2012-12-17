#pragma once


#include <string>
#include "skype-embedded_2.h"
#include "VideoEx.h"

class ParticipantEx : public Participant
{
 private:

  SERootObject* m_pOwner;
 public:
  typedef DRef<ParticipantEx, Participant> Ref;
  typedef DRefs<ParticipantEx, Participant> Refs;

 private:
  bool m_Myself;
  bool m_AutoTake;
 private:
  VideoEx::Ref m_Video;


 public:
  bool isMyself() {return m_Myself;}
  bool isAutoTake() {return m_AutoTake;}
 public:
  ParticipantEx(unsigned int oid, SERootObject* root);
  ~ParticipantEx();


  void OnChange(int prop);
  void GetVideoIfAvailable();
};
