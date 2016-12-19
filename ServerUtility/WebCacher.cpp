
#include "WebCacher.hpp"

WebCacher::WebCacher(std::function<std::string()>  generator_f,
	int ttl_millis):ttl(ttl_millis),generator(generator_f){
	lastUpdate = 0;
}

std::string WebCacher::getResponse(){
    std::lock_guard<std::mutex> guard(response_mutex);

	int millis = TimeUtility::getCurrentMillis();
	if (millis - lastUpdate > ttl){
		response = generator();
		lastUpdate = TimeUtility::getCurrentMillis();
	}
	return response;
}