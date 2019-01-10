#ifndef __NOTIFY_HPP
#define __NOTIFY_HPP
#include <cstdint>
#include <string>
#include <libnotify/notify.h>

class PC2Notifier {
    public:
        PC2Notifier();
        NotifyNotification *volume_notification = nullptr;
        void notify_volume(uint8_t volume);
        void notify_source(std::string source);
};
#endif
