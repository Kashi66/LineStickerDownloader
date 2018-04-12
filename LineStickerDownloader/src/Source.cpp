#include <fstream>
#include <regex>
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <curl/curl.h>


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


std::vector<std::string> Parse()
{
	std::ifstream file;
	file.open("code.txt", std::ios::in);

	std::string buffer;
	std::vector<std::string> v;
	std::regex reg("(https?:\/\/.*sticker.png)");
	std::smatch match;
	while (std::getline(file, buffer))
	{
		if (std::regex_search(buffer, match, reg))
		{
			v.push_back(match[0]);
		}
	}
	return v;
}


void downloadPage(const char* url)
{
	CURL* curl;
	CURLcode res;
	std::fstream code;
	std::string buffer;

	curl = curl_easy_init();
	code.open("code.txt", std::ios::out | std::ios::in | std::ios::trunc);

	if (!code.is_open())
	{
		std::cout << "Nie mozna otworzyc pliku" << std::endl;

		Sleep(2000);
		_exit(-1);
	}

	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		res = curl_easy_perform(curl);
		code << buffer;
		curl_easy_cleanup(curl);
	}

	code.close();
}


int main(int argc, char * argv[])
{
	const char* url = argv[1];

	downloadPage(url);
	std::vector<std::string> links = Parse();
	std::fstream f("link.txt", std::ios::out | std::ios::in | std::ios::trunc);
	for (auto &v : links)
	{
		f << v << "\n";
	}
	f.close();

	return 0;
}