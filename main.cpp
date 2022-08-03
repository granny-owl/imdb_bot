#include <iostream>
#include <boost/bind/bind.hpp>
#include <tgbot/tgbot.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

int main() {
    FILE* fp = fopen("config.json", "r");
    char readbuffer[10000];
    rapidjson::Document document;
    rapidjson::FileReadStream is(fp, readbuffer, sizeof(readbuffer) * sizeof(char));
    rapidjson::ParseResult ok = document.ParseStream(is);

    if (!ok) {
        std::cout << "No ConfigFile" << std::endl;
        return 1;
    }

    TgBot::Bot bot(document["BotSettings"]["Token"].GetString());

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr msg_ptr) {
        std::string msg_txt = msg_ptr->text;
    });

    try {
        std::cout << "bot username: " << bot.getApi().getMe()->username << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start(); 
        }
    } catch (TgBot::TgException& e) {
        std::cout << "error: " << e.what() << std::endl;
    }

    return 0;
}