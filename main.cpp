#include <iostream>
#include <boost/bind/bind.hpp>
#include <tgbot/tgbot.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <sstream>
#include <string>
#include <gumbo.h>

// using namespace curlpp::options;

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

    /*
        That's all that is needed to do cleanup of the used resources (RAII style).
        Our request to be sent.
        Set the URL.
        Send request and get a result.
	    By default the result goes to standard output.
    */

	curlpp::Cleanup myCleanup;
	curlpp::Easy myRequest;
	myRequest.setOpt<curlpp::options::Url>("https://pubmed.ncbi.nlm.nih.gov/"); // check other setOpt options
	myRequest.perform();

    std::stringstream ss; 
    ss << myRequest; // works fine
    
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