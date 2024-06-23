#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <iostream>
#include <wtypes.h>

#define POPPINS_MEDIUM 8;
#define POPPINS_REGULAR 9;
#define NOTIFICATION_ICON 10;

class NotificationHandler 
{
private:
    enum TYPE
    {
        TOGGLE = 0,
        DISABLE = 1,
        INFO = 2,
    };

    struct Notification 
    {
        std::string Title;
        std::string Description;

        DWORD RegisterTime;
        DWORD ExpireTime;

        int Type;

        Notification(const std::string& Title, const std::string& Description, TYPE NotificationType, DWORD Timeout) : Title(Title), Description(Description), Type((int)NotificationType), RegisterTime(GetTickCount64()), ExpireTime(GetTickCount64() + Timeout)
        {

        }
    };

    std::vector<Notification> NotificationQueue;
    std::mutex Mutex;
public:
    enum SOUND
    {
        SIGMA = 0,
        SAMSUNG = 1,
        CHINA = 3
    };

    inline static bool Toggled = true;
    inline static bool PlayNotificationSound = false;
    inline static int Sound = SAMSUNG;

    NotificationHandler() { }

    void RegisterNotification(const std::string& Title, const std::string& Description, int Type, DWORD Timeout) 
    {
        if (!Toggled) {
            return;
        }

        std::lock_guard<std::mutex> Lock(Mutex);
        NotificationQueue.emplace_back(Notification( Title, Description, (TYPE)Type, Timeout ));
        std::cout << "Registered " << Title << std::endl;
    }

    void Render();

    ~NotificationHandler()
    {
        std::lock_guard<std::mutex> Lock(Mutex);
        NotificationQueue.clear();
    }
};

extern NotificationHandler* NotificationHandlerPtr;