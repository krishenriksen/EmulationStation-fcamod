#pragma once
#ifndef ES_APP_SCRAPERS_SCREEN_SCRAPER_H
#define ES_APP_SCRAPERS_SCREEN_SCRAPER_H

#include "scrapers/Scraper.h"
#include "EmulationStation.h"

namespace pugi { class xml_document; }


void screenscraper_generate_scraper_requests(const ScraperSearchParams& params, std::queue< std::unique_ptr<ScraperRequest> >& requests,
	std::vector<ScraperSearchResult>& results);

class ScreenScraperRequest : public ScraperHttpRequest
{
public:
	// ctor for a GetGameList request
	ScreenScraperRequest(std::queue< std::unique_ptr<ScraperRequest> >& requestsWrite, std::vector<ScraperSearchResult>& resultsWrite, const std::string& url) : ScraperHttpRequest(resultsWrite, url), mRequestQueue(&requestsWrite) {}
	// ctor for a GetGame request
	ScreenScraperRequest(std::vector<ScraperSearchResult>& resultsWrite, const std::string& url) : ScraperHttpRequest(resultsWrite, url), mRequestQueue(nullptr) {}

	// Settings for the scraper
	static const struct ScreenScraperConfig {
		std::string getGameSearchUrl(const std::string gameName, bool jeuRecherche=false) const;

		// Access to the API
		const std::string API_DEV_U = { 60, 10, 23, 6, 22, 69, 25, 23, 80, 21 };
		const std::string API_DEV_P = { 104, 86, 49, 16, 32, 98, 49, 60, 47, 85, 87, 47, 81, 67, 10, 58 };
		const std::string API_DEV_KEY = { 80, 101, 97, 99, 101, 32, 97, 110, 100, 32, 98, 101, 32, 119, 105, 108, 100 };

		const std::string API_URL_BASE = "https://www.screenscraper.fr/api2";
		const std::string API_SOFT_NAME = "Emulationstation " + static_cast<std::string>(PROGRAM_VERSION_STRING);

		/** Which type of image artwork we need. Possible values (not a comprehensive list):
		  - ss: in-game screenshot
		  - box-3D: 3D boxart
		  - box-2D: 2D boxart (default)
		  - screenmarque : marquee
		  - sstitle: in-game start screenshot
		  - steamgrid: Steam artwork
		  - wheel: spine
		  - support-2D: media showing the 2d boxart on the cart
		  - support-3D: media showing the 3d boxart on the cart

		  Note that no all games contain values for these, so we default to "box-2D" since it's the most common.
		**/
		//std::string media_name = "box-2D";

		// Which Region to use when selecting the artwork
		// Applies to: artwork, name of the game, date of release 
		std::string region = "US";

		// Which Language to use when selecting the textual information
		// Applies to: description, genre
		//std::string language = "EN";

		ScreenScraperConfig() {};
	} configuration;

protected:
	bool process(HttpReq* request, std::vector<ScraperSearchResult>& results) override;
	std::string ensureUrl(const std::string url);

	void processList(const pugi::xml_document& xmldoc, std::vector<ScraperSearchResult>& results);
	void processGame(const pugi::xml_document& xmldoc, std::vector<ScraperSearchResult>& results);
	bool isGameRequest() { return !mRequestQueue; }

	std::queue< std::unique_ptr<ScraperRequest> >* mRequestQueue;

private:
	std::vector<std::string>	getRipList(std::string imageSource);
	pugi::xml_node				findMedia(pugi::xml_node media_list, std::vector<std::string> mediaNames, std::string region);
	pugi::xml_node				findMedia(pugi::xml_node media_list, std::string mediaName, std::string region);
};



#endif // ES_APP_SCRAPERS_SCREEN_SCRAPER_H
