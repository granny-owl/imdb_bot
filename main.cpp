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

static void search_for_links(GumboNode* node) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    GumboAttribute* href;
    if (node->v.element.tag == GUMBO_TAG_A &&
    (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
        std::cout << href->value << std::endl;
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_for_links(static_cast<GumboNode*>(children->data[i]));
    }
}


int main() {
    FILE* fp;
    rapidjson::Document document;
    rapidjson::ParseResult ok;

    try {
        fp = fopen("config.json", "r");
        char readbuffer[10000];
        rapidjson::FileReadStream is(fp, readbuffer, sizeof(readbuffer) * sizeof(char));
        ok = document.ParseStream(is);
    } catch(...) {
        return 1;
    }

    if (!ok) {
        std::cout << "No ConfigFile" << std::endl;
        return 1;
    }

    TgBot::Bot bot(document["BotSettings"]["Token"].GetString());

	curlpp::Cleanup myCleanup;
	curlpp::Easy myRequest;
	myRequest.setOpt<curlpp::options::Url>("https://pubmed.ncbi.nlm.nih.gov/"); // check other setOpt options
	myRequest.perform();

    std::stringstream ss; 
    ss << myRequest; // works fine

    std::string contents = ss.str();
    GumboOutput* output = gumbo_parse(contents.c_str());
    search_for_links(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    
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