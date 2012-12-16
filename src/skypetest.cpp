#include "skypetest.h"
#include <iostream>


int main(void) {
  std::string skypeName = "suga.yuki2";
  std::string passwd = "yggvb47v";
  std::cout << "Skype Test" << std::endl;
  try {
    SkypeEx skype;
    skype.setAutoTakeUserName("ysuga0731");
    skype.loadKeyPair("../key.pem");

    skype.login(skypeName, passwd);
    
    skype.getContacts();

    getchar();
    if(skype.startPreview()) {
      std::cout << "Preview OK" << std::endl;
    } else {
      std::cout << "Preview Failed." << std::endl;
    }
    getchar();

    skype.test();


    getchar();
    
    
    skype.logout();
  
    getchar();
  } catch (std::exception & ex) {
    std::cerr << "Exception: " << ex.what() << std::endl;
  }
  return 0;
}
