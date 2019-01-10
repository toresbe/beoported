#include <libnotify/notify.h>
#include <boost/format.hpp>
#include <iostream>
#include "notify.hpp"

PC2Notifier::PC2Notifier() {
    notify_init("BeoPort");
}

void PC2Notifier::notify_volume(uint8_t volume) {
    auto text = str(boost::format("Volume: %1$02d") % (unsigned int)volume);
    if (this->volume_notification == nullptr) {
        this->volume_notification = notify_notification_new ("BeoPort",
                text.c_str(),
                0);
    } else {
        notify_notification_update(this->volume_notification, "BeoPort",
                text.c_str(),
                0);
        notify_notification_show(this->volume_notification, 0);
    }

    notify_notification_set_timeout(this->volume_notification, 500);

    if (!notify_notification_show(this->volume_notification, 0)) 
    {
        std::cerr << "show has failed" << std::endl;
    }
}

void PC2Notifier::notify_source(std::string source) {
    // NOP to suppress unused variable message
    if (source == "") source = "";
}
